
#include <gtest/gtest.h>
#include "utils.h"

namespace {

class ContainerOfTest : public ::testing::Test{};

TEST_F(ContainerOfTest, test1)
{
    using namespace karuta;

    struct TestSt {
        int a;
        char b;
        int c;
    } testst{};

    EXPECT_EQ(&testst, karuta_container_of(&testst.a, &TestSt::a));
    EXPECT_EQ(&testst, karuta_container_of(&testst.b, &TestSt::b));
    EXPECT_EQ(&testst, karuta_container_of(&testst.c, &TestSt::c));
}

}
