// tests/test_queue.cpp
#include "threadsafe_queue.h"
#include <gtest/gtest.h>
#include <thread>
#include <string>

TEST(QueueTest, PushPopBasic)
{
    Queue<int> q(2);
    q.Push(10);
    q.Push(20);
    EXPECT_EQ(q.Pop(), 10);
    EXPECT_EQ(q.Pop(), 20);
}

TEST(QueueTest, DropOldestWhenFull) 
{
    Queue<int> q(2);
    q.Push(1);
    q.Push(2);
    q.Push(3); // Drop 1
    EXPECT_EQ(q.Pop(), 2);
    EXPECT_EQ(q.Pop(), 3);
}

TEST(QueueTest, TimeoutPopThrows) 
{
    Queue<int> q(2);
    EXPECT_THROW(q.PopWithTimeout(100), std::runtime_error);
}

TEST(QueueTest, CountAndSize)
{
    Queue<int> q(3);
    EXPECT_EQ(q.Size(), 3);
    q.Push(5);
    q.Push(6);
    EXPECT_EQ(q.Count(), 2);
}

TEST(QueueTest, FloatPushPop) 
{
    Queue<float> q(2);
    q.Push(1.1f);
    q.Push(2.2f);
    EXPECT_FLOAT_EQ(q.Pop(), 1.1f);
    EXPECT_FLOAT_EQ(q.Pop(), 2.2f);
}

TEST(QueueTest, StringPushPop) 
{
    Queue<std::string> q(2);
    q.Push("hello");
    q.Push("world");
    EXPECT_EQ(q.Pop(), "hello");
    EXPECT_EQ(q.Pop(), "world");
}

TEST(QueueTest, StringDropOldest)
{
    Queue<std::string> q(2);
    q.Push("apple");
    q.Push("banana");
    q.Push("cherry"); // Drops "apple"
    EXPECT_EQ(q.Pop(), "banana");
    EXPECT_EQ(q.Pop(), "cherry");
}
