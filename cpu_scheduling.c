#include <stdio.h>

// Structure to hold process details
typedef struct {
    int id, arrival_time, burst_time, priority, waiting_time, turnaround_time, remaining_time;
} Process;

// Function to sort processes by arrival time
void sort_by_arrival(Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// FCFS Scheduling Algorithm
void fcfs(Process p[], int n) {
    sort_by_arrival(p, n);
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;
        p[i].waiting_time = current_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        current_time += p[i].burst_time;
    }
}

// Shortest Job First (Non-preemptive)
void sjf(Process p[], int n) {
    int completed = 0, current_time = 0;
    while (completed < n) {
        int min_index = -1, min_burst = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].burst_time < min_burst) {
                min_burst = p[i].burst_time;
                min_index = i;
            }
        }
        if (min_index == -1) {
            current_time++;
        } else {
            p[min_index].waiting_time = current_time - p[min_index].arrival_time;
            p[min_index].turnaround_time = p[min_index].waiting_time + p[min_index].burst_time;
            current_time += p[min_index].burst_time;
            p[min_index].remaining_time = 0;
            completed++;
        }
    }
}

// Round Robin Scheduling Algorithm
void round_robin(Process p[], int n, int quantum) {
    int current_time = 0, completed = 0;
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0) {
                int execution_time = (p[i].remaining_time > quantum) ? quantum : p[i].remaining_time;
                p[i].remaining_time -= execution_time;
                current_time += execution_time;
                if (p[i].remaining_time == 0) {
                    p[i].turnaround_time = current_time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                    completed++;
                }
            }
        }
    }
}

// Priority Scheduling Algorithm
void priority_scheduling(Process p[], int n) {
    int completed = 0, current_time = 0;
    while (completed < n) {
        int min_index = -1, highest_priority = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].priority < highest_priority) {
                highest_priority = p[i].priority;
                min_index = i;
            }
        }
        if (min_index == -1) {
            current_time++;
        } else {
            p[min_index].waiting_time = current_time - p[min_index].arrival_time;
            p[min_index].turnaround_time = p[min_index].waiting_time + p[min_index].burst_time;
            current_time += p[min_index].burst_time;
            p[min_index].remaining_time = 0;
            completed++;
        }
    }
}

// Function to calculate average waiting time
float calculate_avg_waiting_time(Process p[], int n) {
    float total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += p[i].waiting_time;
    }
    return total_waiting_time / n;
}

// Main function
int main() {
    Process processes[] = {
        {1, 1, 6, 3, 0, 0, 6},
        {2, 3, 8, 1, 0, 0, 8},
        {3, 5, 2, 5, 0, 0, 2},
        {4, 7, 4, 2, 0, 0, 4},
        {5, 9, 5, 4, 0, 0, 5}
    };
    int n = 5;

    // FCFS
    fcfs(processes, n);
    printf("FCFS Average Waiting Time: %.2f\n", calculate_avg_waiting_time(processes, n));

    // Reset processes
    for (int i = 0; i < n; i++) processes[i].remaining_time = processes[i].burst_time;
    sjf(processes, n);
    printf("SJF Average Waiting Time: %.2f\n", calculate_avg_waiting_time(processes, n));

    // Reset processes
    for (int i = 0; i < n; i++) processes[i].remaining_time = processes[i].burst_time;
    round_robin(processes, n, 4);
    printf("Round Robin Average Waiting Time: %.2f\n", calculate_avg_waiting_time(processes, n));

    // Reset processes
    for (int i = 0; i < n; i++) processes[i].remaining_time = processes[i].burst_time;
    priority_scheduling(processes, n);
    printf("Priority Scheduling Average Waiting Time: %.2f\n", calculate_avg_waiting_time(processes, n));
    
    return 0;
}
