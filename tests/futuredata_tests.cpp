#include "gtest/gtest.h"
#include "DataHandler.h"

TEST(FutureData, isValid) {
    FutureData pd;
    EXPECT_FALSE(pd.isReady()); // initially not valid

    int a = 5;
    pd.setValue(a);
    EXPECT_TRUE(pd.isReady()); // should be valid after setting data
}

TEST(FutureData, get) {
    FutureData pd;
    int a = 5;
    pd.setValue(a);
    
    EXPECT_EQ(*pd.getValue<int>(), 5); // should return the correct value
}
TEST(FutureData, get_invalid) {
    FutureData pd;
    EXPECT_THROW(pd.getValue<int>(), std::runtime_error); // should throw if not fulfilled
}

TEST(FutureData_Ptr, isValid) {
    FutureData pd;
    FutureDataPtr pd_ptr(&pd);
    EXPECT_FALSE(pd_ptr.isReady()); // initially not valid

    int a = 5;
    pd.setValue(a);
    EXPECT_TRUE(pd_ptr.isReady()); // should be valid after setting data
}

TEST(FutureData_Ptr, get) {
    FutureData pd;
    int a = 5;
    pd.setValue(a);
    
    FutureDataPtr pd_ptr(&pd);
    EXPECT_EQ(*pd_ptr.getValue<int>(), 5); // should return the correct value
}

TEST(FutureData_Ptr, get_invalid) {
    FutureData pd;
    FutureDataPtr pd_ptr(&pd);
    EXPECT_THROW(pd_ptr.getValue<int>(), std::runtime_error); // should throw if not fulfilled
}

TEST(FutureData, bad_cast){
    FutureData pd;
    int a = 5;
    pd.setValue(a);
    
    EXPECT_THROW(pd.getValue<double>(), std::bad_any_cast); // should throw if type is incorrect
}
