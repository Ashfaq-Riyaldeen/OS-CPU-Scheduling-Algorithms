#include <stdio.h>
#include <stdlib.h>

// Structure to store process information
typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
    int priority;
} Process;

/////////////////////////////////////////////////////////////
// Function to calculate the average waiting time
////////////////////////////////////////////////////////////
void calculate_average_times(Process processes[], int n, float *average_waiting_time) {
    int total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    *average_waiting_time = (float)total_waiting_time / n;
}

/////////////////////////////////////////////////////////////
// First Come First Serve (FCFS) Scheduling Algorithm
////////////////////////////////////////////////////////////
void fcfs(Process processes[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        // If CPU is idle, move to process arrival time
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        // Calculate waiting time and turnaround time
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}

/////////////////////////////////////////////////////////////
// Shortest Job First (SJF) Scheduling Algorithm (Non-preemptive)
////////////////////////////////////////////////////////////
void sjf(Process processes[], int n) {
    int completed = 0, current_time = 0;
    int is_completed[n];
    for (int i = 0; i < n; i++) is_completed[i] = 0;

    while (completed != n) {
        int min_index = -1;
        for (int i = 0; i < n; i++) {
            // Find the shortest job available at the current time
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (min_index == -1 || processes[i].burst_time < processes[min_index].burst_time) {
                    min_index = i;
                }
            }
        }
        
        if (min_index != -1) {
            processes[min_index].waiting_time = current_time - processes[min_index].arrival_time;
            if (processes[min_index].waiting_time < 0) processes[min_index].waiting_time = 0;
            current_time += processes[min_index].burst_time;
            processes[min_index].turnaround_time = processes[min_index].waiting_time + processes[min_index].burst_time;
            is_completed[min_index] = 1;
            completed++;
        } else {
            // If no process is available, move forward in time
            current_time++;
        }
    }
}

/////////////////////////////////////////////////////////////
// Round Robin (RR) Scheduling Algorithm
////////////////////////////////////////////////////////////
void rr(Process processes[], int n, int quantum) {
    int current_time = 0, completed = 0;
    int remaining_time[n];
    for (int i = 0; i < n; i++) remaining_time[i] = processes[i].burst_time;

    while (completed != n) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time) {
                done = 0;
                if (remaining_time[i] > quantum) {
                    current_time += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    current_time += remaining_time[i];
                    processes[i].waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
                    if (processes[i].waiting_time < 0) processes[i].waiting_time = 0;
                    processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
                    remaining_time[i] = 0;
                    completed++;
                }
            }
        }
        if (done) current_time++;
    }
}

/////////////////////////////////////////////////////////////
// Priority Scheduling Algorithm (Non-preemptive)
////////////////////////////////////////////////////////////
void priority_scheduling(Process processes[], int n) {
    int current_time = 0, completed = 0;
    int is_completed[n];
    for (int i = 0; i < n; i++) is_completed[i] = 0;

    while (completed != n) {
        int highest_priority_index = -1;
        for (int i = 0; i < n; i++) {
            // Find the process with the highest priority (lowest value)
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (highest_priority_index == -1 || processes[i].priority < processes[highest_priority_index].priority) {
                    highest_priority_index = i;
                }
            }
        }
        
        if (highest_priority_index != -1) {
            processes[highest_priority_index].waiting_time = current_time - processes[highest_priority_index].arrival_time;
            if (processes[highest_priority_index].waiting_time < 0) processes[highest_priority_index].waiting_time = 0;
            current_time += processes[highest_priority_index].burst_time;
            processes[highest_priority_index].turnaround_time = processes[highest_priority_index].waiting_time + processes[highest_priority_index].burst_time;
            is_completed[highest_priority_index] = 1;
            completed++;
        } else {
            current_time++;
        }
    }
}

// Function to display the process scheduling results
void print_processes(Process processes[], int n) {
    printf("Process   Arrival Time   Burst Time   Priority   Waiting Time   Turnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%-10d%-15d%-13d%-11d%-15d%-16d\n", 
               processes[i].process_id, 
               processes[i].arrival_time, 
               processes[i].burst_time, 
               processes[i].priority, 
               processes[i].waiting_time, 
               processes[i].turnaround_time);
    }
}

int main() {
    // Define sample processes with (ID, Arrival Time, Burst Time, Priority)
    Process processes[] = {
        {1, 1, 6, 0, 0, 0, 3},
        {2, 3, 8, 0, 0, 0, 1},
        {3, 5, 2, 0, 0, 0, 5},
        {4, 7, 4, 0, 0, 0, 2},
        {5, 9, 5, 0, 0, 0, 4}
    };
    int n = sizeof(processes) / sizeof(processes[0]);
    float average_waiting_time;
    int quantum = 4;
    
    printf("\nFCFS Scheduling:\n");
    fcfs(processes, n);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);
    
    printf("\nSJF Scheduling:\n");
    sjf(processes, n);
    print_processes(processes, n);
    
    printf("\nRR Scheduling (Quantum = %d):\n", quantum);
    rr(processes, n, quantum);
    print_processes(processes, n);
    
    printf("\nPriority Scheduling:\n");
    priority_scheduling(processes, n);
    print_processes(processes, n);
    
    return 0;
}
