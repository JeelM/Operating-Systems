#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

extern struct node *head;
void pickNextTask(struct node *toExecute);
void add(char *name, int priority, int burst);
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
        
        current->task->turnAroundTime = totalBurstTime;       //Summing up TAT of each process. Assuming we have arrival time equal to zero.
        totalTurnAroundTime += current->task->turnAroundTime;   //TAT for each process, WT+BT

        current->task->waitTime = current->task->turnAroundTime - current->task->burst; //WT for each process, TAT
        totalWaitingTime += current->task->waitTime;
        
        current->task->responseTime = current->task->turnAroundTime - current->task->burst;
        totalResponseTime += current->task->responseTime;

        pickNextTask(current);
        current = current->next;
    }
    avgTurnAroundTime = (float)totalTurnAroundTime/(float)totalTasks; 
    avgWaitingTime = (float)totalWaitingTime/(float)totalTasks;
    avgResponseTime = (float)totalResponseTime/(float)totalTasks;
   
    printf("\nAverage waiting time: %.2f\n",avgWaitingTime);
    printf("Average turnaround time: %.2f\n", avgTurnAroundTime);
    printf("Average response time: %.2f\n\n", avgResponseTime);
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

   insert(&head, taskData);
  
}