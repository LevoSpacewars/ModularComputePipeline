#include "DataHandler.h"

#include <gtest/gtest.h>


TEST(DataHandler, addData)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_EQ(*dh.getData<int>("a"), 5);
}

TEST(DataHandler, getData_cast_exception)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_THROW(*dh.getData<double>("a"), std::bad_any_cast);
}

TEST(DataHandler, addData_missingid_exception)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_THROW(dh.getData<int>("b"), std::runtime_error);
}

TEST(DataHandler, addData_alreadydefined_exception)
{
    DataHandler dh;
    int a = 5;
    dh.addData("a", a);
    EXPECT_THROW(dh.addData("a", a), std::runtime_error);
}


TEST(DataHandler, getData_notready_exception)
{
    DataHandler dh;
    FutureDataPtr pd_ptr = dh.requestFutureData("a");
    EXPECT_FALSE(pd_ptr.isReady()); // initially not ready
    EXPECT_FALSE(dh.isDataReady("a")); // check if data is ready
    EXPECT_THROW(dh.getData<int>("a"), std::runtime_error); // should throw if not ready

    dh.addData<int>("a", 5); // set the data
    EXPECT_TRUE(dh.isDataReady("a")); // check if data is ready
    EXPECT_TRUE(pd_ptr.isReady()); // check if pointer is ready
}
