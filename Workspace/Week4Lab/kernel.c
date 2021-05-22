// *******************************************************
//
// kernel.c
//
// A round robin kernel implementation
//
// Tom Rizzi, Euan Robinson, Satwik Meravanage
// Last modified: 21 May 2021
//
// *******************************************************


//*****************************************************************************
// Includes
//*****************************************************************************
#include "kernel.h"
#include "timings.h"


//*****************************************************************************
// Checks the process needs to be run.
//*****************************************************************************
bool shouldRunProcess(Process process)
{
    return (process.lastRunRef == 0 || process.rate == KERNEL_MAX_RATE || shouldBeRun(process.lastRunRef, process.rate));
}


//*****************************************************************************
// Runs the main round robin loop.
// Takes the following parameters...
//
// processes: List of processes to run.
// n: Number of processes in the list of
//*****************************************************************************
void runKernel(Process processes[], int n)
{
    // Set the initial last run reference
    int i = 0;
    for (i = 0; i < n; i++) {
        processes[i].lastRunRef = 0;
    }

    // Run the main schedule
    while (1) {
        for (i = 0; i < n; i++) {

            // Check if this process should be run
            if (shouldRunProcess(processes[i])) {

                // Reset the reference time
                processes[i].lastRunRef = getCurTime();

                // Run the actual function
                processes[i].handler();
            }
        }
    }

}


