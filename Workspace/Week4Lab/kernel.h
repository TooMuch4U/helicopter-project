//*****************************************************************************
// EXAMPLE: Scheduling a function at 1hz
//*****************************************************************************
//
// void testFunc(void)
// {
//      updateDisplay(0, 0, 0, 0);
// }
//
// int main(void)
// {
//      Process processes[1] = {*testFunc, 1};
//      runKernel(processes, 1);
// }
//
//*****************************************************************************

#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Constants
//*****************************************************************************
#define KERNEL_MAX_RATE 0

//*****************************************************************************
// Structure to represent a process
//*****************************************************************************
typedef struct Process {
    void (*handler)(void);  // The function handler for the process.
    uint32_t rate;          // The rate in HZ to try and schedule the task at.
    uint64_t lastRunRef;    // Set automatically when the function is run.
} Process;


//*****************************************************************************
// Function declarations
//*****************************************************************************
bool shouldRunProcess(Process process);
void runKernel(Process processes[], int n);


#endif /* KERNEL_H_ */
