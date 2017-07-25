/*
  scheduler.c - This file contains
  the implementation for the
  Dynamic Task Queue.
  (see scheduler.c for implementation)
*/

#include <stdlib.h>
#include "TCB.h"
#include "scheduler.h"

TaskQueue AllocateTaskQueue(void) {
  // 1. Allocate a new TaskQueue
  TaskQueue queue = (TaskQueue)malloc(sizeof(TQ));
  if(queue == NULL) {
    return NULL;
  }
  // 2. Assign the members of TaskQueue
  queue->head = queue->tail = NULL;
  queue->num_tasks = 0U;
  // 3. Return the newly allocated TaskQueue
  return queue;
}

void FreeTaskQueue(TaskQueue queue) {
  // 1. Set the head/tail to NULL
  queue->head = queue->tail = NULL;
  // 2. Free the queue itself
  free(queue);
  queue = NULL;
}

int AppendTCB(TaskQueue queue, TCB_Ptr node) {
  // 0. Verify queue and node is valid
  if(!queue || !node) {
    return 0;
  }
  // 1. Get the number of tasks in the queue
  unsigned int num_tasks = queue->num_tasks;
  // 2. If the queue is initially empty
  if(num_tasks == 0U) {
    queue->head = queue->tail = node;
    node->prev = node->next = NULL;
  }
  // 3. If the queue has 1+ elements
  else {
    node->next = NULL;
    node->prev = queue->tail;
    queue->tail->next = node;
    queue->tail = node;
  }
  queue->num_tasks++;
  return 1;
}

int PushTCB(TaskQueue queue, TCB_Ptr node) {
  // 0. Verify queue and node is valid
  if(!queue || !node) {
    return 0;
  }
  // 1. Get the number of tasks in the queue
  unsigned int num_tasks = queue->num_tasks;
  // 2. If the queue is initially empty
  if(num_tasks == 0U) {
    queue->head = queue->tail = node;
    node->prev = node->next = NULL;
  }
  // 3. If the queue has 1+ elements
  else {
    node->prev = NULL;
    node->next = queue->head;
    queue->head->prev = node;
    queue->head = node;
  }
  queue->num_tasks++;
  return 1;
}

TCB_Ptr RemoveTCB(TaskQueue queue, TCB_Ptr node) {
  // 0. Verify queue is valid
  unsigned int num_tasks = queue->num_tasks;
  if(queue == NULL && num_tasks == 0U) {
    return NULL;
  }
  // 1. Search for the node to delete
  TCB_Ptr curr = queue->head;
  while(curr != NULL && curr != node) {
    curr = curr->next;
  }
  // 2. If we didn't find it
  if(curr == NULL) {
    return NULL;
  }
  // 3. If the node we want to delete is the head
  if(queue->head == curr) {
    return PopTCB(queue);
  }
  // 4. If the node we want to delete is the tail
  if(queue->tail == curr) {
    return SliceTCB(queue);
  }
  // 5. If the node we want to delete is in the middle
  curr->prev->next = curr->next;
  queue->num_tasks--;
  return curr;
}

TCB_Ptr PopTCB(TaskQueue queue) {
  // 0. Verify queue is valid
  if(queue == NULL && queue->num_tasks == 0U) {
    return NULL;
  }
  // 1. Save the old head
  TCB_Ptr old_head = queue->head;
  // 2. Move the head forward.
  queue->head = queue->head->next;
  queue->num_tasks--;
  // 3. If the list is now empty
  if(queue->num_tasks == 0U) {
    queue->tail = queue->head = NULL;
  } else {
    queue->head->prev = NULL;
  }
  return old_head;
}

TCB_Ptr SliceTCB(TaskQueue queue) {
  // 0. Verify queue is valid
  if(queue == NULL && queue->num_tasks > 0) {
    return NULL;
  }
  // 1. Save the old tail
  TCB_Ptr old_tail = queue->tail;
  // 2. Move the tail back
  queue->tail = queue->tail->prev;
  queue->num_tasks--;
  // 3. If the list is now empty
  if(queue->num_tasks == 0U) {
    queue->tail = queue->head = NULL;
  } else {
    queue->tail->next = NULL;
  }
  return old_tail;
}

unsigned int NumTasksInTaskQueue(TaskQueue queue) {
  if(queue == NULL) {
    return -1;
  } else {
    return queue->num_tasks;
  }
}
