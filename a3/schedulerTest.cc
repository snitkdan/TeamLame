/*
  schedulerTest.cc - This file contains
  unit tests for the scheduler.
*/

#include "gtest/gtest.h"
#include <iostream>
extern "C" {
    #include "./scheduler.h"
    #include "startup.h"
}

// This tests the "AppendTCB" & "SliceTCB" methods in "scheduler.c"
TEST(TaskQueueTest, Test_AppendAndSliceTCB) {
  // 1. Define test TCB && task queue
  TCB testTCB, testTCB2;
  TCB_Ptr testTCB_ptr = &testTCB;
  TCB_Ptr testTCB2_ptr = &testTCB2;
  TQ queue;
  TaskQueue q = &queue;
  // 2. Append test
  ASSERT_TRUE(AppendTCB(q, testTCB_ptr));
  ASSERT_EQ(q->head, testTCB_ptr);
  ASSERT_EQ(q->tail, testTCB_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 1U);
  ASSERT_TRUE(AppendTCB(q, testTCB2_ptr));
  ASSERT_EQ(q->tail, testTCB2_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 2U);
  // 3. Slice test
  ASSERT_EQ(SliceTCB(q), testTCB2_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 1U);
  ASSERT_EQ(SliceTCB(q), testTCB_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 0U);
  // 4. Edge cases
  ASSERT_FALSE(AppendTCB(q, NULL));
  ASSERT_FALSE(AppendTCB(NULL, testTCB_ptr));
  ASSERT_EQ(SliceTCB(q), (TCB_Ptr)NULL);
}

// This tests the "PushTCB" & "PopTCB" methods in "scheduler.c"
TEST(TaskQueueTest, Test_PushAndPopTCB) {
  // 1. Define test TCB && task queue
  TCB testTCB, testTCB2;
  TCB_Ptr testTCB_ptr = &testTCB;
  TCB_Ptr testTCB2_ptr = &testTCB2;
  TQ queue;
  TaskQueue q = &queue;
  // 2. Push test
  ASSERT_TRUE(PushTCB(q, testTCB_ptr));
  ASSERT_EQ(q->head, testTCB_ptr);
  ASSERT_EQ(q->tail, testTCB_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 1U);
  ASSERT_TRUE(PushTCB(q, testTCB2_ptr));
  ASSERT_EQ(q->head, testTCB2_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 2U);
  // 3. Pop test
  ASSERT_EQ(PopTCB(q), testTCB2_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 1U);
  ASSERT_EQ(PopTCB(q), testTCB_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 0U);
  // 4. Edge cases
  ASSERT_FALSE(PushTCB(q, NULL));
  ASSERT_FALSE(PushTCB(NULL, testTCB_ptr));
  ASSERT_EQ(PopTCB(q), (TCB_Ptr)NULL);
}

// This tests the "RemoveTCB" methods in "scheduler.c"
TEST(TaskQueueTest, Test_RemoveTCB) {
  // 1. Define test TCBs && task queue
  TCB testTCB, testTCB2, testTCB3, testTCB4;
  TCB_Ptr testTCB_ptr = &testTCB;
  TCB_Ptr testTCB2_ptr = &testTCB2;
  TCB_Ptr testTCB3_ptr = &testTCB3;
  TCB_Ptr testTCB4_ptr = &testTCB4;
  TQ queue;
  TaskQueue q = &queue;
  // 2. Add TCBs to task queue
  AppendTCB(q, testTCB_ptr);
  AppendTCB(q, testTCB2_ptr);
  AppendTCB(q, testTCB3_ptr);
  // 3. Remove test
  ASSERT_EQ(RemoveTCB(q, testTCB2_ptr), testTCB2_ptr);
  ASSERT_EQ(q->head, testTCB_ptr);
  ASSERT_EQ(q->tail, testTCB3_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 2U);
  ASSERT_EQ(RemoveTCB(q, testTCB_ptr), testTCB_ptr);
  ASSERT_EQ(NumTasksInTaskQueue(q), 1U);
  // 4. Edge cases
  ASSERT_EQ(RemoveTCB(q, testTCB4_ptr), (TCB_Ptr)NULL);
  ASSERT_EQ(RemoveTCB(q, NULL), (TCB_Ptr)NULL);
  ASSERT_EQ(RemoveTCB(NULL, testTCB3_ptr), (TCB_Ptr)NULL);
}
