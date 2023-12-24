/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/singleton.h"

#include "gtest/gtest.h"

namespace bazel_template {
namespace util {

/*
// example: define a singleton class
class TestClass : public SingletonBase<TestClass>
{
    friend class SingletonBase<TestClass>;
private:
    TestClass() {}
    ~TestClass() {}
public:
    int Test() const
    {
        return 1;
    }
};
// example2: define a singleton class with alt access method
// private inherit make 'Instance' method unaccessable
class TestClass2 : private SingletonBase<TestClass2>
{
    friend class SingletonBase<TestClass2>;
private:
    TestClass() {}
    ~TestClass() {}
public:
    // using DefaultInstance to instead Instance
    static TestClass2& DefaultInstance()
    {
        return Instance();
    }
};
// example3: make a singleton for class
class TestClass3
{
};
typedef Singleton<TestClass3> SingletonTestClass3;
TestClass3& instance = SingletonTestClass3::Instance();
*/

class TestClass : public SingletonBase<TestClass> {
  friend class SingletonBase<TestClass>;

 private:
  TestClass() {}

 public:
  int Test() const { return 1; }
};

TEST(Singleton, SingletonClass) {
  TestClass &test = TestClass::Instance();
  EXPECT_EQ(1, test.Test());
}

class TestClass2 {};

TEST(Singleton, ClassSingleton) {
  TestClass2 &test = Singleton<TestClass2>::Instance();
  (void)test;
}

class TestClass3 : public SingletonBase<TestClass3> {
  friend class SingletonBase<TestClass3>;
};

TEST(Singleton, IsAlive) {
  EXPECT_FALSE(TestClass3::IsAlive());
  TestClass3::Instance();
  EXPECT_TRUE(TestClass3::IsAlive());
}

}  // namespace util
}  // namespace bazel_template
