
#include <gtest/gtest.h>

#include "array.h"

namespace {

using namespace karuta;

class ArrayTest : public ::testing::Test{};

TEST_F(ArrayTest, test1)
{
    Array<int> array;

    for(int i=1; i<=10; i++) {
        array.add(i);
    }

    int total=0;
    for(auto val: array)
        total += val;

    EXPECT_EQ(55, total);
}

}
