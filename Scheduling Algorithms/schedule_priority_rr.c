#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

#define QUANTUM 10

extern struct node *head;
void pickNextTask(struct node *toExecute);
void add(char *name, int priority, int burst);
void priorityOrderedList(struct node **head, Task *newTask);

void schedule(){
    struct node *current = head;
    char *firstTaskName = current->task->name; 
    float avgTurnAroundTime = 0.0; int totalTurnAroundTime = 0;
    float avgWaitingTime = 0.0; int totalWaitingTime = 0;
    float avgResponseTime = 0.0; int totalResponseTime = 0;
    int executionCompletionTime = 0; 
    int totalTasks = 0;
    while(current != NULL){
        struct node *temp = NULL;
        if(current->task->burst <= QUANTUM){
            executionCompletionTime += current->task->burst;
            current->task->turnAroundTime = executionCompletionTime;
            if(current->task->responseTime == 0 && strcmp(current->task->name, firstTaskName) != 0){
                current->task->responseTime = current->task->turnAroundTime - current->task->burst;
            }
            pickNextTask(current);
            current = current->next;
        }

        else{
            if(current->next == NULL){
                executionCompletionTime += current->task->burst;
                current->task->turnAroundTime = executionCompletionTime;
                if(current->task->responseTime == 0 && strcmp(current->task->name, firstTaskName) != 0){
                    current->task->responseTime = current->task->turnAroundTime - current->task->burst;
                }
                pickNextTask(current);
                current = current->next;
            }
           else if(current->next->task->priority < current->task->priority){
                executionCompletionTime += current->task->burst;
                current->task->turnAroundTime = executionCompletionTime;
                if(current->task->responseTime == 0 && strcmp(current->task->name, firstTaskName) != 0){
                    current->task->responseTime = current->task->turnAroundTime - current->task->burst;
                }
                pickNextTask(current);
                current = current->next;
            } 
            else{
                executionCompletionTime += QUANTUM;
                current->task->turnAroundTime = executionCompletionTime;
                if(current->task->responseTime == 0 && strcmp(current->task->name, firstTaskName) != 0){
                    current->task->responseTime = current->task->turnAroundTime - QUANTUM;
                }
                pickNextTask(current);
                current->task->burst -= QUANTUM;
                priorityOrderedList(&head, current->task);
                temp = current->next;
                delete(&head, current->task);
                current = temp;
        }
    }
    }
  
    current = head;
    while(current != NULL){
        totalTasks += 1;
        totalTurnAroundTime += current->task->turnAroundTime;

        current->task->waitTime = current->task->turnAroundTime - current->task->burstCopy;
        totalWaitingTime += current->task->waitTime;

        totalResponseTime += current->task->responseTime;

        current = current->next;
    }
    avgResponseTime = (float)totalResponseTime/(float)totalTasks;
    avgTurnAroundTime = (float)totalTurnAroundTime/(float)totalTasks;
    avgWaitingTime = (float)totalWaitingTime/(float)totalTasks;
    printf("\nAverage waiting time : %.2f", avgWaitingTime);
    printf("\nAverage turnaround time :%.2f\n", avgTurnAroundTime);
    printf("Average response time : %.2f\n", avgResponseTime);
}

void pickNextTask(struct node *taskNode){
    Task *taskToExecute;
    taskToExecute = taskNode->task;
    if(taskNode->next == NULL){
        run(taskToExecute, taskToExecute->burst);
    }
    else if(taskNode->task->priority > taskNode->next->task->priority){
        run(taskToExecute, taskToExecute->burst);
    }
    else if(taskNode->task->priority == taskNode->next->task->priority && taskNode->task->burst <= QUANTUM){
        run(taskToExecute, taskToExecute->burst);
    }
    else{
        run(taskToExecute, QUANTUM);
    }
}

void add(char *name, int priority, int burst){
   Task *taskData;
   taskData=malloc(sizeof(Task));
   taskData->name=name;
   taskData->priority=priority;
   taskData->burst=burst;
   taskData->burstCopy=taskData->burst;     // round robin
   priorityOrderedList(&head, taskData);
}

void priorityOrderedList(struct node **head, Task *newTask){
    struct node *currentNode = *head;
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = NULL;

    if(*head == NULL){
        *head = newNode;
    }
    else{
        while(currentNode != NULL){
            if(newNode->task->priority > currentNode->task->priority && currentNode == *head){
                newNode->next = currentNode;
                *head = newNode;
                break;
            }
            else if(newNode->task->priority <= currentNode->task->priority && currentNode->next == NULL){
                currentNode->next = newNode;
                break;
            }
            else if(newNode->task->priority <= currentNode->task->priority && newNode->task->priority > currentNode->next->task->priority){
                newNode->next = currentNode->next;
                currentNode->next = newNode;
                break;
            }
            else{
                currentNode = currentNode->next;
            }
        }
    }
}