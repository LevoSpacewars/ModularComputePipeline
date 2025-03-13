#include "DataHandler.h"

#include <gtest/gtest.h>


TEST(DataHandler, addData)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_EQ(*dh.getData<int>("a"), 5);
}

TEST(DataHandler, getData_exception)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_THROW(*dh.getData<float>("a"), std::bad_any_cast);
}

TEST(DataHandler, addData_null)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_EQ(dh.getData<int>("b"), nullptr);
}