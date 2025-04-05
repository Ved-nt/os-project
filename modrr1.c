#include "types.h"
#include "user.h"
#include "stat.h"

// Process structure to store process details
struct Process {
    int pid;             // Process ID
    int burst_time;      // Burst time (CPU time required)
    int arrival_time;    // Arrival time
    int remaining_time;  // Remaining burst time
    int wait_time;       // Waiting time
    int turnaround_time; // Turnaround time
    int start_time;      // Start time
    int scheduled_count; // Number of times scheduled
};

// Custom fmin function
int fmin(int a, int b) {
    return (a < b) ? a : b;
}

// Function to calculate the dynamic time quantum
float calculateTimeQuantum(int base_quantum, int scheduled_count, float total_execution_time) {     // if the process is scheduled more frequently, them
    if (total_execution_time == 0) {                                                                // its scheduled_count will be high and consequently, 
        return base_quantum; // Avoid division by zero                                              // it will receive a slightly higher time quantum
    }
    return base_quantum + ((float)scheduled_count / total_execution_time) * base_quantum;           // if scheduled_count is low, time quantum will be 
}                                                                                                   // close to the base time quantum initialized by user



// Function to simulate the custom scheduler
void customScheduler(struct Process processes[], int num_processes, int base_quantum) {
    int ready_queue[num_processes]; // Use a simple array as a queue
    int queue_front = 0, queue_rear = 0;
    int current_time = 0;
    int completed_processes = 0;
    float total_execution_time = 0; // Keep track of total execution time
    int process_start_times[num_processes]; // Initialize start times


    for (int i = 0; i < num_processes; i++) {
        process_start_times[i] = -1;
    }

    // Sort processes by arrival time (using a simple bubble sort for demonstration)
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Main loop to simulate the scheduler
    while (completed_processes < num_processes) {
        // Add arriving processes to the ready queue
        for (int i = 0; i < num_processes; ++i) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                int already_in_queue = 0;
                for (int j = queue_front; j < queue_rear; j++) {
                    if (ready_queue[j] == i) {
                        already_in_queue = 1;
                        break;
                    }
                }
                if (!already_in_queue) {
                    ready_queue[queue_rear++] = i;
                }
            }
        }

        // If the ready queue is empty, increment the current time and continue
        if (queue_front == queue_rear) {
            current_time++;
            continue;
        }

        // Get the process at the front of the ready queue
        int current_process_index = ready_queue[queue_front++];

        // If the process has not started, record the start time
        if (process_start_times[current_process_index] == -1) {
            process_start_times[current_process_index] = current_time;
            processes[current_process_index].start_time = current_time; // Set start time
        }
        processes[current_process_index].scheduled_count++; // Increment the scheduled count

        // Calculate the dynamic time quantum
        float dynamic_quantum = calculateTimeQuantum(base_quantum, processes[current_process_index].scheduled_count, total_execution_time);
        // Execute the process for the time quantum or remaining time, whichever is smaller
        int execution_time = fmin(dynamic_quantum, processes[current_process_index].remaining_time);
        processes[current_process_index].remaining_time -= execution_time;
        total_execution_time += execution_time; // Add to total execution time
        current_time += execution_time;

        // If the process is completed
        if (processes[current_process_index].remaining_time == 0) {
            completed_processes++;
            // Calculate turnaround and waiting times
            processes[current_process_index].turnaround_time = current_time - processes[current_process_index].arrival_time;
            processes[current_process_index].wait_time = processes[current_process_index].turnaround_time - processes[current_process_index].burst_time;
        } else {
            // Add the process back to the ready queue if it's not completed
            ready_queue[queue_rear++] = current_process_index;
        }
    }
}

// Function to display process details
void displayProcessDetails(const struct Process processes[], int num_processes) {
    printf(1, "Process ID\tBurst Time\tArrival Time\tStart Time\tWait Time\tTurnaround Time\n");
    for (int i = 0; i < num_processes; ++i) {
        printf(1,"%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
            processes[i].pid, processes[i].burst_time, processes[i].arrival_time,
            processes[i].start_time, processes[i].wait_time, processes[i].turnaround_time);     
    }
    // Calculate and display average waiting time and turnaround time
    float total_wait_time = 0;
    float total_turnaround_time = 0;
    for (int i = 0; i < num_processes; ++i) {
        total_wait_time += processes[i].wait_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    if (num_processes > 0) {
        // Print average values as integers, scaled by 100 for decimal precision
        int avg_wait_time = (int)((total_wait_time / num_processes) * 100);
        int avg_turnaround_time = (int)((total_turnaround_time / num_processes) * 100);
        
        // Print integer values with 2 decimal places
        printf(1, "\nAverage Wait Time: %d.%02d\n", avg_wait_time / 100, avg_wait_time % 100);
        printf(1, "Average Turnaround Time: %d.%02d\n", avg_turnaround_time / 100, avg_turnaround_time % 100);
    }
}


int main() {
    // Example usage:
    int num_processes;
    int base_quantum;

    printf(1, "Enter the number of processes: ");
    // Using read and converting to int via atoi
    char input[10];
    read(0, input, 10);
    num_processes = atoi(input); // Convert to integer

    struct Process processes[num_processes]; // Use VLA or dynamically allocate if needed
    for (int i = 0; i < num_processes; ++i) {
        processes[i].pid = i + 1;
        printf(1, "Enter burst time for process %d: ", i + 1);
        // Using read and converting to int via atoi
        read(0, input, 10);
        processes[i].burst_time = atoi(input);
        processes[i].remaining_time = processes[i].burst_time;
        
        printf(1, "Enter arrival time for process %d: ", i + 1);
        // Using read and converting to int via atoi
        read(0, input, 10);
        processes[i].arrival_time = atoi(input);
        
        processes[i].wait_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].scheduled_count = 0;
        processes[i].start_time = 0;
    }

    printf(1, "Enter the base time quantum: ");
    // Using read and converting to int via atoi
    read(0, input, 10);
    base_quantum = atoi(input);

    customScheduler(processes, num_processes, base_quantum);
    displayProcessDetails(processes, num_processes);

    exit();  // Exit program
}