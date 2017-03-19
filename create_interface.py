#!/usr/bin/env pytnon3

import xml.etree.ElementTree as etree
import sys
import re

exclude_interface = ["wl_registry", "wl_resource"]

header_preamble = """// Generated by create_interface.py
#pragma once

#include <wayland-server.h>

#include "client.h"
#include "resource_ref.h"
#include "impl_interface.h"
#include "array.h"
#include "log.h"
#include "instance.h"
#include <cassert>
"""

header_body = """
namespace karuta {{
namespace protocol {{
{interface_define}
}}
}}
"""

source_preamble = """// Generated by create_interface.py
#include "{0}"
"""

source_body = """
namespace karuta {{
namespace protocol {{
{interface_instance}
}}
}}
"""

class_format = """
class {classname} : public karuta::ImplInterface {{
private:
{member_funcs}
public:
{enums}
{member_event_funcs}
{static_funcs}
{get_interface}
{get_wl_interface}
{version}
}};
"""

static_function = """
    static void {reqname}_(
        struct wl_client* client,
        struct wl_resource* resource{args}) {{

        Client client_(client);
        ResourceRef resource_(resource);{to_class}

        auto p = static_cast<{classname}*>(wl_resource_get_user_data(resource));
        p->{reqname}(client_, resource_ {call_args});
    }}"""

def usage():
    print("""
usage: create_interface.py input_file output_prefix
""")

type_map = {
    "uint": "uint32_t",
    "string": "const char *",
    "fd": "int32_t",
    "int": "int32_t",
    "new_id": "uint32_t"
}

acceptable_interface = ["wl_client", "wl_resource"]

def to_c_type(arg):
    t = type_map.get(arg.attrib['type'], None)
    if t: return t
    if (arg.attrib['type'] == 'object'):
        if arg.attrib['interface'] not in acceptable_interface:
            return 'struct wl_resource *'
        return 'struct ' + arg.attrib['interface'] + ' *'

def to_cpp_type(arg):
    t = type_map.get(arg.attrib['type'], None)
    if t: return t
    if (arg.attrib['type'] == 'object'):
        return 'ResourceRef&'

send_type_name = {
    "uint": "uint32_t",
    "string": "const char *",
    "fd": "int32_t",
    "int": "int32_t",
    "fixed": "int32_t", # FIXME: 
}

def to_send_cpp_type(arg):
    t = send_type_name.get(arg.attrib['type'], None)
    if t: return t
    if arg.attrib['type'] == 'object':
        return 'ResourceRef&'
    if arg.attrib['type'] == 'array':
        return 'ArrayRef&'
    if arg.attrib['type'] == 'new_id':
        return 'ResourceRef&'

    print("Unknown type: " + arg.attrib['type'])

def conv_arg_for_send(arg):
    t = send_type_name.get(arg.attrib['type'], None)
    if t: return arg.attrib['name']
    if arg.attrib['type'] == 'object':
        return arg.attrib['name'] + '.get_wl_resource()'
    if arg.attrib['type'] == 'array':
        return arg.attrib['name'] + '.get_wl_array()'
    if arg.attrib['type'] == 'new_id':
        return arg.attrib['name'] + '.get_wl_resource()'

    print("Unknown type: " + arg.attrib['type'])

def to_class_name(name):
    return ''.join(map(lambda s: s.title(), name.split('_')))

def to_member_funcs(ifname, reqs):
    return ''.join(['\n' + to_member_func(ifname, *req) for req in reqs])

def to_member_func(ifname, name, args):
    s = '    virtual void ' + name + '(Client& client, ResourceRef& resource'
    s += ''.join([',\n        ' + to_cpp_type(arg) + ' ' + arg.attrib['name'] for arg in args])
    s += ') {{ warn("{}.{} not implemented"); }}\n'.format(ifname, name)
    return s

def to_enum_defs(enums):
    return ''.join(['\n' + to_enum_def(*enum) for enum in enums])

def to_enum_def(name, entries):
    s = '    enum {} {{'.format(name.title())
    s += ''.join(['\n        {}_{} = {},'.format(name.title(), entry.attrib['name'], entry.attrib['value'])
                  for entry in entries])
    s += '\n    };\n'
    return s

def to_member_event_funcs(ifname, evs):
    return ''.join(['\n' + to_member_event_func(ifname, name, args)
                    for (name, args) in evs])

def to_member_event_func(ifname, name, args):
    s = '    static void send_{}(ResourceRef resource_'.format(name)
    s += ''.join([',\n        ' + to_send_cpp_type(arg) + ' ' + arg.attrib['name'] for arg in args])
    s += ') {\n'
    s += '        {}_send_{}(resource_.get_wl_resource()'.format(ifname, name)
    s += ''.join([', ' + conv_arg_for_send(arg) for arg in args])
    s += ');\n'
    s += '    }\n'
    s += '\n'
    s += '    void send_{}('.format(name)
    s += ',\n    '.join([to_send_cpp_type(arg) + ' ' + arg.attrib['name'] for arg in args])
    s += ') {\n'
    s += '        auto impl_ = dynamic_cast<InstanceBase*>(this);\n'
    s += '        assert(impl_);\n'
    s += '        send_{}(*impl_'.format(name)
    s += ''.join([', ' + arg.attrib['name'] for arg in args])
    s += ');\n'
    s += '    }\n'
    return s

def to_static_funcs(ifname, reqs):
    return ''.join(['\n' + to_static_func(ifname, *req) for req in reqs])

def to_static_func(ifname, name, args):
    classname = to_class_name(ifname)

    args_str = ''.join([',\n        ' + to_c_type(arg) + ' ' + arg.attrib['name'] for arg in args])

    to_class = ''
    for arg in args:
        if arg.attrib['type'] == 'object' and arg.attrib['interface'] not in acceptable_interface:
            to_class += '\n        ResourceRef {0}_({0});'.format(arg.attrib['name'])

    def to_arg_name(arg):
        if arg.attrib['type'] == 'object' and arg.attrib['interface'] not in acceptable_interface:
            return arg.attrib['name'] + '_'
        else:
            return arg.attrib['name']

    call_args = ''.join([' ,' + to_arg_name(arg) for arg in args])

    return static_function.format(reqname=name,
                                  args=args_str,
                                  classname=classname,
                                  to_class=to_class,
                                  call_args=call_args)

def parse_request(req):
    return (req.attrib['name'], [child for child in req if child.tag == 'arg'])

def parse_event(ev):
    return (ev.attrib['name'], [child for child in ev if child.tag == 'arg'])

def parse_enum(enum):
    return (enum.attrib['name'], [child for child in enum if child.tag == 'entry'])

def parse_interface(intf):
    name = intf.attrib['name']
    version = intf.attrib.get('version', 0)
    reqs = [parse_request(child) for child in intf if child.tag == 'request']
    evs = [parse_event(child) for child in intf
           if child.tag == 'event' and child.attrib['name'] != 'error' and child.attrib['name'] != 'delete_id']
    enums = [parse_enum(child) for child in intf if child.tag == 'enum']
    return (name, version, reqs, evs, enums)

def create_interface_instance(name, version, reqs, _evs, _enums):
    if len(reqs) == 0: return ""
    s = 'struct {ifname}_interface {ifname}_interface_instance = {{'.format(ifname=name)
    for (funcname, args) in reqs:
        s += '\n    &{classname}::{funcname}_,'.format(classname=to_class_name(name), funcname=funcname)
    s += '\n};'
    return s

def to_get_interface(name):
    return """\
    void* get_interface() override {{
        extern struct {name}_interface {name}_interface_instance;
        return static_cast<void*>(&{name}_interface_instance);
    }}""".format(name=name)

def to_get_wl_interface(name):
    return """\
    static const struct wl_interface* get_wl_interface() {{
        extern const struct wl_interface {name}_interface;
        return &::{name}_interface;
    }}""".format(name=name)

def to_version(ver):
    return """\
    static const uint32_t version = {version};
    """.format(version=ver)

def create_interface(ns, intf):
    (name, version, reqs, evs, enums) = intf
    s = ""
    s += class_format.format(classname          = to_class_name(name),
                             member_funcs       = to_member_funcs(name, reqs),
                             enums              = to_enum_defs(enums),
                             member_event_funcs = to_member_event_funcs(name, evs),
                             static_funcs       = to_static_funcs(name, reqs),
                             get_interface      = "" if len(reqs) == 0 else to_get_interface(name),
                             get_wl_interface   = to_get_wl_interface(name),
                             version            = to_version(version)
    )
    return s

def main():
    if len(sys.argv) < 2:
        usage()
        sys.exit(1)

    from os.path import basename, splitext
    tree = etree.parse(sys.argv[1])
    root = tree.getroot()

    #proto_name = root.attrib['name']
    proto_name = splitext(basename(sys.argv[1]))[0]
    header_name = proto_name + '_karuta_server.h'
    header_file = open(header_name, 'w')
    source_file = open(proto_name + '_karuta_server.cpp', 'w')

    header_str = ""
    source_str = ""

    print(header_preamble, file=header_file)
    print("#include \"" + proto_name + "-server-protocol.h\"", file=header_file)

    for child in root:
        if child.tag == 'interface' and child.attrib['name'] not in exclude_interface:
            interface = parse_interface(child)
            print(header_body.format(interface_define=create_interface(proto_name, interface)),
                  file=header_file)

    print(source_preamble.format(header_name), file=source_file)

    for child in root:
        if child.tag == 'interface' and child.attrib['name'] not in exclude_interface:
            interface = parse_interface(child)

            print(source_body.format(namespace=proto_name,
                                     interface_instance=create_interface_instance(*interface)),
                  file=source_file)

if __name__ == '__main__':
    main()
