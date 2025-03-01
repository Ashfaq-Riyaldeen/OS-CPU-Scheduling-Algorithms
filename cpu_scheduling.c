#include <stdio.h>
#include <stdlib.h>

// Structure to store process details
typedef struct {
    int process_id;      // Unique ID for each process
    int arrival_time;    // When the process arrives in the system
    int burst_time;      // Time required for the process to complete execution
    int waiting_time;    // Time the process spends waiting before execution
    int turnaround_time; // Total time from arrival to completion
    int remaining_time;  // Used for algorithms like Round Robin
    int priority;        // Priority value (lower number = higher priority)
} Process;


// Function to calculate the average waiting time for a given scheduling algorithm
void calculate_average_times(Process processes[], int n, float *average_waiting_time) {
    int total_waiting_time = 0;
    
    // Sum up the waiting times of all processes
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }
    
    // Compute the average waiting time
    *average_waiting_time = (float)total_waiting_time / n;
}


// First Come First Serve (FCFS) Scheduling Algorithm
void fcfs(Process processes[], int n) {
    int current_time = 0;

    // Process each job in the order they arrive
    for (int i = 0; i < n; i++) {
        // If CPU is idle, move time forward to process arrival
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        // Waiting time is the time spent waiting before execution
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        
        // Move time forward by the burst time of the current process
        current_time += processes[i].burst_time;
        
        // Turnaround time = waiting time + execution time
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}


// Shortest Job First (SJF) Scheduling Algorithm
void sjf(Process processes[], int n) {
    int completed = 0, current_time = 0;
    int is_completed[n]; // Track completed processes

    // Initialize all processes as incomplete
    for (int i = 0; i < n; i++) is_completed[i] = 0;

    while (completed != n) {
        int min_index = -1;

        // Find the shortest job available at the current time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (min_index == -1 || processes[i].burst_time < processes[min_index].burst_time) {
                    min_index = i;
                }
            }
        }

        // If we found a process to execute
        if (min_index != -1) {
            processes[min_index].waiting_time = current_time - processes[min_index].arrival_time;
            
            // Ensure waiting time isn't negative
            if (processes[min_index].waiting_time < 0) processes[min_index].waiting_time = 0;
            
            // Execute the shortest job and update time
            current_time += processes[min_index].burst_time;
            processes[min_index].turnaround_time = processes[min_index].waiting_time + processes[min_index].burst_time;
            
            // Mark process as completed
            is_completed[min_index] = 1;
            completed++;
        } else {
            // If no process is ready, move time forward
            current_time++;
        }
    }
}


// Round Robin (RR) Scheduling Algorithm
void rr(Process processes[], int n, int quantum) {
    int current_time = 0, completed = 0;
    int remaining_time[n];

    // Initialize remaining times with burst times
    for (int i = 0; i < n; i++) remaining_time[i] = processes[i].burst_time;

    while (completed != n) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time) {
                done = 0; // At least one process is still running
                
                if (remaining_time[i] > quantum) {
                    // Execute for the quantum time
                    current_time += quantum;
                    remaining_time[i] -= quantum;
                } else {
                    // Execute the remaining time and mark process as completed
                    current_time += remaining_time[i];
                    processes[i].waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
                    
                    if (processes[i].waiting_time < 0) processes[i].waiting_time = 0;
                    
                    processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
                    remaining_time[i] = 0;
                    completed++;
                }
            }
        }

        // If no process was executed, move time forward
        if (done) current_time++;
    }
}


// Priority Scheduling Algorithm (Lower number = Higher priority)
void priority_scheduling(Process processes[], int n) {
    int current_time = 0, completed = 0;
    int is_completed[n];

    // Initialize all processes as incomplete
    for (int i = 0; i < n; i++) is_completed[i] = 0;

    while (completed != n) {
        int highest_priority_index = -1;

        // Find the highest priority process that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (highest_priority_index == -1 || processes[i].priority < processes[highest_priority_index].priority) {
                    highest_priority_index = i;
                }
            }
        }

        // If we found a process to execute
        if (highest_priority_index != -1) {
            processes[highest_priority_index].waiting_time = current_time - processes[highest_priority_index].arrival_time;

            if (processes[highest_priority_index].waiting_time < 0) processes[highest_priority_index].waiting_time = 0;

            // Execute process and update time
            current_time += processes[highest_priority_index].burst_time;
            processes[highest_priority_index].turnaround_time = processes[highest_priority_index].waiting_time + processes[highest_priority_index].burst_time;

            is_completed[highest_priority_index] = 1;
            completed++;
        } else {
            // If no process is ready, move time forward
            current_time++;
        }
    }
}

// Function to print the process details
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

    printf("====================================================================================\n");
    printf("\nFCFS Scheduling:\n");
    fcfs(processes, n);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);

    printf("====================================================================================\n");
    printf("\nSJF Scheduling:\n");
    sjf(processes, n);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);
    
    printf("====================================================================================\n");
    printf("\nRR Scheduling (Quantum = %d):\n", quantum);
    rr(processes, n, quantum);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);
    
    printf("====================================================================================\n");
    printf("\nPriority Scheduling:\n");
    priority_scheduling(processes, n);
    print_processes(processes, n);
    calculate_average_times(processes, n, &average_waiting_time);
    printf("Average Waiting Time: %.2f\n\n", average_waiting_time);

    return 0;
}
