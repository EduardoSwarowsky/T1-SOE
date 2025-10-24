#include "scheduler.h"
#include "os_config.h"
#include "syscall.h"
#include "kernel.h"

#include <stdio.h>

// Declaração da fila de aptos
extern f_aptos_t readyQueue;


tcb_t *rr_scheduler()
{
    uint8_t pos_task_running = os_task_pos(readyQueue.taskRunning->task_func);
    uint8_t idle_selected = 0;
    
    do {
        pos_task_running = (pos_task_running+1) % readyQueue.readyQueueSize;
        if (readyQueue.readyQueue[pos_task_running].task_func == os_idle_task) {
            idle_selected++;
            if (idle_selected > 1) return &readyQueue.readyQueue[0];            
        }
        
    } while (readyQueue.readyQueue[pos_task_running].task_state != READY ||
             readyQueue.readyQueue[pos_task_running].task_func == os_idle_task);
    
    return &readyQueue.readyQueue[pos_task_running];
}

tcb_t *priority_scheduler()
{
    int max_priority = -1;
    int max_priority_index = 0;
    
    for (int i=1; i<readyQueue.readyQueueSize;i++){
        if (readyQueue.readyQueue[i].task_priority > max_priority && 
                readyQueue.readyQueue[i].task_state == READY){
            max_priority_index = i;
        }
    }
    
    if (max_priority > -1){
        return &readyQueue.readyQueue[max_priority_index];
    } else {
        return &readyQueue.readyQueue[0];
    }
}

void scheduler()
{
#if DEFAULT_SCHEDULER == RR_SCHEDULER
    readyQueue.taskRunning = rr_scheduler();
#elif DEFAULT_SCHEDULER == PRIORITY_SCHEDULER
    readyQueue.taskRunning = priority_scheduler();
#endif    
}

