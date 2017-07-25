/*
  scheduler.h - This file contains
  the specification for the
  Dynamic Task Queue.
  (see scheduler.c for implementation)
*/

#ifndef SCHED
#define SCHED

#include "TCB.h"

// Declares a TCB structure
typedef struct taskqueue {
  TCB_Ptr head, tail;  // Head/Tail of the task queue
  unsigned int num_tasks;  // Current number of tasks in the queue
} TQ, *TaskQueue;

/*
  @returns
    a new Task Queue with a null head/tail,
    if successful, and NULL, otherwise.
*/
TaskQueue AllocateTaskQueue(void);

/*
  @param queue
    the Task Queue to free
  @modifies
    queue
  @effects
    frees the queue and all its
    elements
*/
void FreeTaskQueue(TaskQueue queue);


/*
  @param node
    node to add
  @param queue
    Task Queue to add into
  @modifies
    queue
  @effects
    adds node to the tail
    of queue
  @returns
    1 on success, and 0 on failure.
*/
int AppendTCB(TaskQueue queue, TCB_Ptr node);

/*
  @param node
    node to remove
  @param queue
    Task Queue to remove from
  @modifies
    queue
  @effects
    removes node from the queue
  @returns
    Reference to the deleted
    TCB if successful, and NULL otherwise.
*/
TCB_Ptr RemoveTCB(TaskQueue queue, TCB_Ptr node);

/*
  @modifies
    queue
  @effects
    removes the current head from queue
  @returns
    former head of queue if successful,
    and NULL otherwise.
*/
TCB_Ptr PopTCB(TaskQueue queue);

/*
  @modifies
    queue
  @effects
    removes the current tail from queue
  @returns
    former tail of queue if successful,
    and NULL otherwise. 
*/
TCB_Ptr SliceTCB(TaskQueue queue);

/*
  @returns
    the number of elements in queue
*/
unsigned int NumElementsInTaskQueue(TaskQueue queue);

#endif
