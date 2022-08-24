#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "pstat.h"
#include "fcntl.h"


int main(int argc, char *argv[]) {

    int process_id = getpid();
    //printf(1, "enters process %d\n", process_id);
    struct pstat* all_proc_status = malloc(sizeof(struct pstat));
    int i, j;
    int x = 0;
   // int x_child = 0;
    if (argc != 2){
        exit();
    }

    int tickets = atoi(argv[1]);
    //printf(1, "tickets argv: %d\n", tickets);
    settickets(tickets);
    //printf(1, "scheduled_tichets: %d\n", scheduled_tickets);

    for (i = 0; i <= 100000; i++){
        x += i;
    }
    printf(1, "x = %d, from pid: %d\n", x, process_id);

    int count_processes = getpinfo(all_proc_status);
    for (j = 0; j < count_processes; j++){
        if (all_proc_status->pid[j] == process_id){
            printf(1, "PID %d, tickets: %d\n", all_proc_status->pid[j], all_proc_status->tickets[j]);
            printf(1, "PID %d, in use: %d\n", all_proc_status->pid[j], all_proc_status->inuse[j]);
            printf(1, "PID %d, run for %d ticks.\n", all_proc_status->pid[j], all_proc_status->ticks[j]);
        }
    }

    int new_process = fork();

    if (new_process < 0){
        printf(1, "Fork did not succeed");
        exit();
    }else if (new_process == 0){
        printf(1, "enters child process pid: %d\n", getpid());
        int child_scheduled_tickets = settickets(50);
        printf(1, "child scheduled_tichets: %d\n", child_scheduled_tickets);
        for (i = 0; i < 10000; i++){
            x_child += i;
        }
        printf(1, "x_child = %d, from pid: %d\n", x_child, process_id);
        int count_processes = getpinfo(all_proc_status);
        for (j = 0; j < count_processes; j++){
            if (all_proc_status->pid[j] == getpid()){
                printf(1, "PID %d, tickets: %d\n", all_proc_status->pid[j], all_proc_status->tickets[j]);
                printf(1, "PID %d, in use: %d\n", all_proc_status->pid[j], all_proc_status->inuse[j]);
                printf(1, "PID %d, run for %d ticks.\n", all_proc_status->pid[j], all_proc_status->ticks[j]);
            }
        }
    }else{
        wait();
        printf(1, "enters parent process pid: %d\n", getpid());
        int tickets = atoi(argv[1]);
        //printf(1, "tickets argv: %d\n", tickets);
        int scheduled_tickets = settickets(tickets);
        printf(1, "scheduled_tichets: %d\n", scheduled_tickets);

        for (i = 0; i <= 10000; i++){
            x += i;
        }
        printf(1, "x = %d, from pid: %d\n", x, process_id);

        int count_processes = getpinfo(all_proc_status);
        for (j = 0; j < count_processes; j++){
            if (all_proc_status->pid[j] == process_id){
                printf(1, "PID %d, tickets: %d\n", all_proc_status->pid[j], all_proc_status->tickets[j]);
                printf(1, "PID %d, in use: %d\n", all_proc_status->pid[j], all_proc_status->inuse[j]);
                printf(1, "PID %d, run for %d ticks.\n", all_proc_status->pid[j], all_proc_status->ticks[j]);
            }
        }
    }
    
    exit();
}