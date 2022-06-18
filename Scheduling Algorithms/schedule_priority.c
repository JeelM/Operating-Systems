#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

extern struct node *head;
void pickNextTask(struct node *taskNode);
void add(char *name, int priority, int burst);
void priorityOrderedList(struct node **head, Task *newTask);
void schedule(){
    struct node *current = head;
    float avgTurnAroundTime = 0.0; int totalTurnAroundTime = 0;
    float avgWaitingTime = 0.0; int totalWaitingTime = 0;
    float avgResponseTime = 0.0; int totalResponseTime = 0;
    int totalBurstTime = 0;
    int totalTasks = 0;
    while(current != NULL){
        totalTasks += 1;
        totalBurstTime += current->task->burst;

        current->task->turnAroundTime = totalBurstTime;
        totalTurnAroundTime += current->task->turnAroundTime;

        current->task->waitTime = current->task->turnAroundTime - current->task->burst;
        totalWaitingTime += current->task->waitTime;

        current->task->responseTime = current->task->turnAroundTime - current->task->burst;
        totalResponseTime += current->task->responseTime;

        pickNextTask(current);
        current = current->next;
    }
    avgTurnAroundTime = (float)totalTurnAroundTime/(float)totalTasks;
    avgWaitingTime = (float)totalWaitingTime/(float)totalTasks;
    avgResponseTime = (float)totalResponseTime/(float)totalTasks;

    printf("\nAverage waiting time: %.2f",avgWaitingTime);
    printf("\nAverage turnaround time: %.2f", avgTurnAroundTime);
    printf("\nAverage response time: %.2f\n\n", avgResponseTime);

}
void pickNextTask(struct node *taskNode){
    Task *taskToExecute;
    taskToExecute = taskNode->task;
    run(taskToExecute, taskToExecute->burst);
}
void add(char *name, int priority, int burst){
    Task *taskData;
    taskData=malloc(sizeof(Task));
    taskData->name=name;
    taskData->priority=priority;
    taskData->burst=burst;

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