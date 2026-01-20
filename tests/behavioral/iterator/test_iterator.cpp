#include "../../../src/behavioral/iterator/Iterator.h"
#include <gtest/gtest.h>

// 迭代器模式测试套件

// 测试IntCollection添加元素
TEST(IteratorTest, IntCollection_AddElements) {
    IntCollection coll;
    
    coll.Add(1);
    coll.Add(2);
    coll.Add(3);
    
    EXPECT_EQ(coll.Size(), 3);
}

// 测试IntCollection访问元素
TEST(IteratorTest, IntCollection_AccessElements) {
    IntCollection coll;
    coll.Add(10);
    coll.Add(20);
    coll.Add(30);
    
    EXPECT_EQ(coll.At(0), 10);
    EXPECT_EQ(coll.At(1), 20);
    EXPECT_EQ(coll.At(2), 30);
}

// 测试IntIterator HasNext
TEST(IteratorTest, IntIterator_HasNext) {
    IntCollection coll;
    coll.Add(1);
    coll.Add(2);
    
    IntIterator it(coll);
    
    EXPECT_TRUE(it.HasNext());
    it.Next();
    EXPECT_TRUE(it.HasNext());
    it.Next();
    EXPECT_FALSE(it.HasNext());
}

// 测试IntIterator Next
TEST(IteratorTest, IntIterator_Next) {
    IntCollection coll;
    coll.Add(5);
    coll.Add(10);
    coll.Add(15);
    
    IntIterator it(coll);
    
    EXPECT_EQ(it.Next(), 5);
    EXPECT_EQ(it.Next(), 10);
    EXPECT_EQ(it.Next(), 15);
}

// 测试IntIterator Current
TEST(IteratorTest, IntIterator_Current) {
    IntCollection coll;
    coll.Add(100);
    coll.Add(200);
    
    IntIterator it(coll);
    
    EXPECT_EQ(it.Current(), 100);
    it.Next();
    EXPECT_EQ(it.Current(), 200);
}

// 测试通过迭代器修改元素
TEST(IteratorTest, IntIterator_ModifyElements) {
    IntCollection coll;
    coll.Add(1);
    coll.Add(2);
    coll.Add(3);
    
    IntIterator it(coll);
    
    while (it.HasNext()) {
        int& value = it.Next();
        value += 10;
    }
    
    EXPECT_EQ(coll.At(0), 11);
    EXPECT_EQ(coll.At(1), 12);
    EXPECT_EQ(coll.At(2), 13);
}

// 测试空集合迭代
TEST(IteratorTest, IntIterator_EmptyCollection) {
    IntCollection coll;
    IntIterator it(coll);
    
    EXPECT_FALSE(it.HasNext());
}

// 测试RunIteratorDemo
TEST(IteratorTest, RunIteratorDemo) {
    EXPECT_NO_THROW(RunIteratorDemo());
}

// 测试单元素集合
TEST(IteratorTest, IntCollection_SingleElement) {
    IntCollection coll;
    coll.Add(42);
    
    EXPECT_EQ(coll.Size(), 1);
    EXPECT_EQ(coll.At(0), 42);
}

// 测试大量元素
TEST(IteratorTest, IntCollection_LargeCollection) {
    IntCollection coll;
    
    for (int i = 0; i < 100; ++i) {
        coll.Add(i);
    }
    
    EXPECT_EQ(coll.Size(), 100);
    
    IntIterator it(coll);
    int count = 0;
    while (it.HasNext()) {
        it.Next();
        ++count;
    }
    
    EXPECT_EQ(count, 100);
}
