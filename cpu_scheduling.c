#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
} Process;

/////////////////////////////////////////////////////////////
// Calculate average Time
////////////////////////////////////////////////////////////

void calculate_average_times(Process processes[], int n, float *average_waiting_time) {
    int total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    *average_waiting_time = (float)total_waiting_time / n;
}

/////////////////////////////////////////////////////////////
// FCFS
////////////////////////////////////////////////////////////

void fcfs(Process processes[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}


/////////////////////////////////////////////////////////////
// SJF
////////////////////////////////////////////////////////////

void sjf(Process processes[], int n) {
    int completed = 0, current_time = 0, min_index;
    int *is_completed = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        is_completed[i] = 0;
    }
    while (completed != n) {
        min_index = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (min_index == -1 || processes[i].burst_time < processes[min_index].burst_time) {
                    min_index = i;
                }
            }
        }
        if (min_index != -1) {
            processes[min_index].waiting_time = current_time - processes[min_index].arrival_time;
            current_time += processes[min_index].burst_time;
            processes[min_index].turnaround_time = processes[min_index].waiting_time + processes[min_index].burst_time;
            is_completed[min_index] = 1;
            completed++;
        } else {
            current_time++;
        }
    }
    free(is_completed);
}

/////////////////////////////////////////////////////////////
// Round Robin
////////////////////////////////////////////////////////////

void rr(Process processes[], int n, int quantum) {
    int current_time = 0, completed = 0;
    int *remaining_burst_time = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        remaining_burst_time[i] = processes[i].burst_time;
    }
    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && remaining_burst_time[i] > 0) {
                if (remaining_burst_time[i] > quantum) {
                    current_time += quantum;
                    remaining_burst_time[i] -= quantum;
                } else {
                    current_time += remaining_burst_time[i];
                    processes[i].waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
                    remaining_burst_time[i] = 0;
                    processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
                    completed++;
                }
            }
        }
    }
    free(remaining_burst_time);
}



void print_processes(Process processes[], int n) {
    printf("Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t\t%d\t\t\t%d\t\t\t\t%d\t\t\t\t\t%d\n",
               processes[i].process_id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }
}


/////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////

int main() {
    Process processes[] = {
        {1, 0, 10, 0, 0, 0},
        {2, 6, 8, 0, 0, 0},
        {3, 7, 4, 0, 0, 0},
        {4, 9, 5, 0, 0, 0},
    };
    int n = sizeof(processes) / sizeof(processes[0]);
    float average_waiting_time;

    printf("FCFS Scheduling:\n");
    fcfs(processes, n);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);

    printf("SJF Scheduling:\n");
    sjf(processes, n);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);

    int quantum = 2;
    printf("RR Scheduling (Quantum = %d):\n", quantum);
    rr(processes, n, quantum);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);

    return 0;
}
