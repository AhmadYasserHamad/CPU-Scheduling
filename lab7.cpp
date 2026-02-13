#include "lab7.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <math.h>

using namespace std;

void Scheduler::execute()
{
    for (int i = 0; i < schedulongPoliceis.size(); i++)
    {
        clearTables();
        if (schedulongPoliceis[i].first == 1)
        {
            FCFS();
            if (type == "trace")
                trace(1, -1);
            else
                stats(1, -1);
        }
        else if (schedulongPoliceis[i].first == 2)
        {
            RR(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(2, schedulongPoliceis[i].second);
            else
                stats(2, schedulongPoliceis[i].second);
        }
        else if (schedulongPoliceis[i].first == 3)
        {
            SPN();
            if (type == "trace")
                trace(3, -1);
            else
                stats(3, -1);
        }
        else if (schedulongPoliceis[i].first == 4)
        {
            SRT();
            if (type == "trace")
                trace(4, -1);
            else
                stats(4, -1);
        }
        else if (schedulongPoliceis[i].first == 5)
        {
            HRRN();
            if (type == "trace")
                trace(5, -1);
            else
                stats(5, -1);
        }
        else if (schedulongPoliceis[i].first == 6)
        {
            FB1();
            if (type == "trace")
                trace(6, -1);
            else
                stats(6, -1);
        }
        else if (schedulongPoliceis[i].first == 7)
        {
            FB2i();
            if (type == "trace")
                trace(7, -1);
            else
                stats(7, -1);
        }
        else if (schedulongPoliceis[i].first == 8)
        {
            AGE(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(8, -1);
        }
    }
}
void Scheduler::trace(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 2)
    {
        if (argument > 10)
            cout << "RR-" << argument << " ";
        else
            cout << "RR-" << argument << "  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 3)
    {
        cout << "SPN   ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 4)
    {
        cout << "SRT   ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 5)
    {
        cout << "HRRN  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 6)
    {
        cout << "FB-1  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 7)
    {
        cout << "FB-2i ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 8)
    {
        cout << "Aging ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
}
void Scheduler::stats(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 2)
    {
        cout << "RR-" << argument << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 3)
    {
        cout << "SPN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 4)
    {
        cout << "SRT" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 5)
    {
        cout << "HRRN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 6)
    {
        cout << "FB-1" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 7)
    {
        cout << "FB-2i" << endl;
        printStats();
        cout << '\n';
    }
}
void Scheduler::FCFS()
{
    // FCFS is heavily dependent on time, thus beginning with a time loop
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                readyQueue.push(processes.at(process));
            }
        }

        if (!processorBusy && !readyQueue.empty())
        {
            currentProcess = readyQueue.front();
            readyQueue.pop();
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::RR(int quantum)
{
    // Round Robin is similar to FCFS as it is also heavily dependent on time
    // , thus beginning with a time loop
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                readyQueue.push(processes.at(process));
            }
        }
        // Checks if current process has taken up the predefined time quantum
        // If so, process quantum is reset and placed in the ready queue again
        if (processorBusy)
        {
            if (processes.at(currentProcess.id).q >= quantum)
            {
                readyQueue.push(processes.at(currentProcess.id));
                processes.at(currentProcess.id).q = 0;
                processorBusy = false;
            }
        }

        if (!processorBusy && !readyQueue.empty())
        {
            currentProcess = readyQueue.front();
            readyQueue.pop();
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            processes.at(currentProcessID).q++;
            // If the process is running --> '*'
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::SPN()
{
    vector<Process> processVector;
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                processVector.push_back(processes.at(process));
            }
        }

        if (!processorBusy && !processVector.empty())
        {
            // Intialize with the first process in the vector
            int processID = 0;
            for (int i = 0; i < processVector.size(); i++)
            {
                if (processVector.at(i).serveTime < processVector.at(processID).serveTime)
                {
                    // Swap ID only if serveTime is less than intial serveTime
                    processID = i;
                }
            }
            // Remove running process from vector list
            currentProcess = processVector.at(processID);
            processVector.erase(processVector.begin() + processID);
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::SRT()
{
    vector<Process> processVector;
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                processVector.push_back(processes.at(process));
            }
        }

        if (processorBusy)
        {
            // Check if a process in the vector has a shorter remaining time than the
            // process that is currently running. If so, stop the processor.
            for (int i = 0; i < processVector.size(); i++)
            {
                if (processVector.at(i).remainingTime < processes.at(currentProcess.id).remainingTime)
                {
                    currentProcess.remainingTime = processes.at(currentProcess.id).remainingTime;
                    processVector.push_back(currentProcess);
                    processorBusy = false;
                    // If so, push only once. There could possibly be multiple processes with
                    // shorter remaining time than the one currently executing.
                    break;
                }
            }
        }

        if (!processorBusy && !processVector.empty())
        {
            // Intialize with the first process in the vector
            int processID = 0;
            for (int i = 0; i < processVector.size(); i++)
            {
                if (processVector.at(i).remainingTime < processVector.at(processID).remainingTime)
                {
                    // Swap ID only if remainingTime is less than intial remainingTime
                    processID = i;
                }
            }
            // Remove running process from vector list
            currentProcess = processVector.at(processID);
            processVector.erase(processVector.begin() + processID);
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::HRRN()
{
    // Similar to SPN as it is also nonpremptive. Simple difference with changing the selection policy.
    vector<Process> processVector;
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                processVector.push_back(processes.at(process));
            }
        }

        if (!processorBusy && !processVector.empty())
        {
            // Intialize with the first process in the vector
            int processID = 0;
            float highestHRR = -1;
            for (int i = 0; i < processVector.size(); i++)
            {
                int serveTime = processVector.at(i).serveTime;
                int waitTime = time - processVector.at(i).arrivalTime;

                float currentHRR = (float)(serveTime + waitTime) / serveTime;

                if (currentHRR > highestHRR)
                {
                    processID = i;
                    highestHRR = currentHRR;
                }
            }
            // Remove running process from vector list
            currentProcess = processVector.at(processID);
            processVector.erase(processVector.begin() + processID);
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
                // Added waiting time as it is necessary for given policy
                // processVector.at(i).waitingTime++;
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::FB1()
{
    // Similar to aging. As process executes multiple times, gets higher current priority
    // The higher the current priority, the lower the chances of getting in the queue
    vector<Process> processVector;
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                processes.at(process).currentPriority = 0;
                processVector.push_back(processes.at(process));
            }
        }

        if (processorBusy)
        {
            // Synchronize with global processes queue
            currentProcess.remainingTime = processes.at(currentProcess.id).remainingTime;
            currentProcess.q = processes.at(currentProcess.id).q;

            if (processes.at(currentProcess.id).q >= 1)
            {

                processes.at(currentProcess.id).q = 0;
                currentProcess.q = 0;

                // Looking for an alternative process in ready vector. If exists, demote previously running, else do not demote

                if (!processVector.empty())
                {
                    currentProcess.currentPriority++;
                }

                processVector.push_back(currentProcess);
                processorBusy = false;
            }
            else
            {
                // Check if a process in the vector has a current priority higher than that of the currently
                // running process. If so, stop the processor.
                for (int i = 0; i < processVector.size(); i++)
                {
                    if (processVector.at(i).currentPriority < currentProcess.currentPriority)
                    {
                        // Reset current process to intial state and synchronize
                        processes.at(currentProcess.id).q = 0;
                        currentProcess.q = 0;
                        processVector.push_back(currentProcess);
                        processorBusy = false;
                        // If so, push only once. There could possibly be multiple processes with
                        // a higher priority than the one currently executing.
                        break;
                    }
                }
            }
        }

        if (!processorBusy && !processVector.empty())
        {
            // Intialize with the first process in the vector
            int processID = 0;
            for (int i = 0; i < processVector.size(); i++)
            {
                if (processVector.at(i).currentPriority < processVector.at(processID).currentPriority)
                {
                    // Swap ID only if current priority is higher than the currently chosen process's priority
                    processID = i;
                }
            }
            // Remove running process from vector list
            currentProcess = processVector.at(processID);
            processVector.erase(processVector.begin() + processID);
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            processes.at(currentProcessID).q++;
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                {
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
                }
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::FB2i()
{
    // Similar to aging. As process executes multiple times, gets higher current priority
    // The higher the current priority, the lower the chances of getting in the queue
    vector<Process> processVector;
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                processes.at(process).currentPriority = 1;
                processVector.push_back(processes.at(process));
            }
        }

        if (processorBusy)
        {
            // Synchronize with global processes queue
            currentProcess.remainingTime = processes.at(currentProcess.id).remainingTime;
            currentProcess.q = processes.at(currentProcess.id).q;

            if (processes.at(currentProcess.id).q >= currentProcess.currentPriority)
            {

                processes.at(currentProcess.id).q = 0;
                currentProcess.q = 0;

                // Looking for an alternative process in ready vector. If exists, demote previously running, else do not demote
                currentProcess.currentPriority *= 2;

                processVector.push_back(currentProcess);
                processorBusy = false;
            }
            else
            {

                if (!processVector.empty())
                {
                    if (processVector.at(0).currentPriority < currentProcess.currentPriority)
                    {
                        // Reset current process to intial state and synchronize
                        processes.at(currentProcess.id).q = 0;
                        currentProcess.q = 0;
                        processVector.push_back(currentProcess);
                        processorBusy = false;
                    }
                }
            }
        }

        if (!processorBusy && !processVector.empty())
        {
            // Intialize with the first process in the vector
            int processID = 0;
            for (int i = 0; i < processVector.size(); i++)
            {
                if (processVector.at(i).currentPriority < processVector.at(processID).currentPriority)
                {
                    // Swap ID only if current priority is higher than the currently chosen process's priority
                    processID = i;
                }
            }
            // Remove running process from vector list
            currentProcess = processVector.at(processID);
            processVector.erase(processVector.begin() + processID);
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            processes.at(currentProcessID).q++;
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;

            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).remainingTime > 0)
                {
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
                }
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::AGE(int quantum)
{
    // Similar to SRT but the selection criteria is based on a variable priority
    vector<Process> processVector;
    for (int time = 0; time < maxSeconds; time++)
    {
        for (int process = 0; process < numberOfProcesses; process++)
        {
            // If a specific process enters at that time, it will be pushed into the ready queue
            if (processes.at(process).arrivalTime == time)
            {
                processVector.push_back(processes.at(process));
            }
        }

        // Increase the priority of all waiting (ready) processes before any intial steps to be taken
        for (int i = 0; i < processVector.size(); i++)
        {
            processVector.at(i).currentPriority++;
        }

        if (processorBusy)
        {
            processes.at(currentProcess.id).currentPriority = processes.at(currentProcess.id).priority;
            if (processes.at(currentProcess.id).q >= quantum)
            {
                processes.at(currentProcess.id).q = 0;
                processVector.push_back(processes.at(currentProcess.id));
                processorBusy = false;
            }
            else
            {
                // Check if a process in the vector has a current priority higher than that of the currently
                // running process. If so, stop the processor.
                for (int i = 0; i < processVector.size(); i++)
                {
                    if (processVector.at(i).currentPriority >= currentProcess.priority)
                    {
                        // Reset current process to intial state and synchronize
                        processes.at(currentProcess.id).q = 0;
                        processVector.push_back(processes.at(currentProcess.id));
                        processorBusy = false;
                        // If so, push only once. There could possibly be multiple processes with
                        // a higher priority than the one currently executing.
                        break;
                    }
                }
            }
        }

        if (!processorBusy && !processVector.empty())
        {
            // Intialize with the first process in the vector
            int processID = 0;
            for (int i = 0; i < processVector.size(); i++)
            {
                if (processVector.at(i).currentPriority > processVector.at(processID).currentPriority)
                {
                    // Swap ID only if current priority is higher than the currently chosen process's priority
                    processID = i;
                }
            }
            // Remove running process from vector list
            currentProcess = processVector.at(processID);
            processVector.erase(processVector.begin() + processID);
            processorBusy = true;
        }

        if (processorBusy)
        {
            // Make changes based on the currently running process ID
            int currentProcessID = currentProcess.id;
            // If the process is running --> '*'
            processes.at(currentProcessID).q++;
            *(processesPrintingArray + currentProcessID * maxSeconds + time) = '*';
            // Process has run a quantum, therefore decrease from its remaining time a quantum
            processes.at(currentProcessID).remainingTime--;
            // Loop over all admitted processes but aren't running and still have remaining time
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (i != currentProcessID && processes.at(i).arrivalTime <= time && processes.at(i).finishTime == 0)
                {
                    *(processesPrintingArray + i * maxSeconds + time) = '.';
                }
            }

            if (processes.at(currentProcessID).remainingTime == 0)
            {
                // Process has halted, make all necessary calculations
                processorBusy = false;
                processes.at(currentProcessID).finishTime = time + 1;
                processes.at(currentProcessID).turnAroundTime = processes.at(currentProcessID).finishTime - processes.at(currentProcessID).arrivalTime;
                processes.at(currentProcessID).NormTurnTime = processes.at(currentProcessID).turnAroundTime / processes.at(currentProcessID).serveTime;
            }
        }
    }
}

void Scheduler::printTracing()
{
    for (int process = 0; process < numberOfProcesses; process++)
    {
        cout << processes[process].processName << "     |";
        for (int time = 0; time < maxSeconds; time++)
            cout << *(processesPrintingArray + process * maxSeconds + time) << '|';
        cout << " \n";
    }
}
void Scheduler::printStats()
{
    float sum, mean, sum2;
    sum = 0;
    mean = 0;
    sum2 = 0;
    cout << "Process    |";
    for (int i = 0; i < numberOfProcesses; i++)
        cout << "  " << processes[i].processName << "  |";
    cout << endl;
    cout << "Arrival    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].arrivalTime << "  |";
        else
            cout << " " << processes[i].arrivalTime << "  |";
    }
    cout << endl;
    cout << "Service    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].serveTime << "  |";
        else
            cout << " " << processes[i].serveTime << "  |";
    }
    cout << " Mean|" << endl;
    cout << "Finish     |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].finishTime >= 10)
            cout << " " << processes[i].finishTime << "  |";
        else
            cout << "  " << processes[i].finishTime << "  |";
    }
    cout << "-----|" << endl;
    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcesses; i++)
    {

        if (processes[i].turnAroundTime >= 10)
            cout << " " << (int)processes[i].turnAroundTime << "  |";
        else
            cout << "  " << (int)processes[i].turnAroundTime << "  |";
        sum += processes[i].turnAroundTime;
    }
    cout << fixed;
    cout << setprecision(2);
    mean = (sum * 1.0) / numberOfProcesses;
    if (mean >= 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
    cout << "NormTurn   |";
    sum2 = 0;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].NormTurnTime > 10)
            cout << processes[i].NormTurnTime << "|";
        else
            cout << " " << processes[i].NormTurnTime << "|";
        sum2 += (processes[i].NormTurnTime * 1.0);
    }

    mean = (sum2 * 1.0) / numberOfProcesses;
    if (mean > 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
}
void Scheduler::clearTables()
{

    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < maxSeconds; j++)
            *(processesPrintingArray + i * maxSeconds + j) = ' ';
    }
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processes[i].finishTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].NormTurnTime = 0;
        processes[i].processState = ' ';
        processes[i].remainingTime = processes[i].serveTime;
        processes[i].q = 0;
    }
    processorBusy = false;
    while (!readyQueue.empty())
        readyQueue.pop();
    while (!readyPriorityQueue.empty())
        readyPriorityQueue.pop();
}
void Scheduler::splitPolicyAndParameter(string str)
{
    string w = "";
    pair<int, int> policies;
    bool parameterExists = false;
    policies.second = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            parameterExists = true;
            policies.first = stoi(w);
            w = "";
        }
        else if (str[i] == ',')
        {
            if (parameterExists)
                policies.second = stoi(w);
            else
            {
                policies.first = stoi(w);
                policies.second = -1;
            }
            w = "";
            schedulongPoliceis.push_back(policies);
            parameterExists = false;
        }
        else
            w = w + str[i];
    }
    if (parameterExists)
        policies.second = stoi(w);
    else
        policies.first = stoi(w);
    schedulongPoliceis.push_back(policies);
}
void Scheduler::splitPrcoessAndTimes(string str, int id)
{
    Process process;
    string w = "";
    process.processName = str[0];
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i] == ',')
        {
            process.arrivalTime = stoi(w);
            w = "";
        }
        else
            w = w + str[i];
    }
    processorBusy = false;
    process.processState = ' ';
    if (schedulongPoliceis[0].first == 8)
    {
        process.priority = stoi(w);
        process.currentPriority = stoi(w);
    }
    else
        process.serveTime = stoi(w);
    process.remainingTime = process.serveTime;
    process.waitingTime = 0;
    process.id = id;
    processes.push_back(process);
}
void Scheduler::readFile()
{
    processorBusy = false;
    string temp1, temp2;
    cin >> type;
    cin >> temp1;
    splitPolicyAndParameter(temp1);
    cin >> maxSeconds;
    cin >> numberOfProcesses;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cin >> temp2;
        splitPrcoessAndTimes(temp2, i);
    }
    processesPrintingArray = new char[numberOfProcesses * maxSeconds];
    currentProcess.processName = 0;
    currentProcess.q = 0;
    clearTables();
}
void Scheduler::printHeader()
{
    for (int i = 0; i < maxSeconds + 1; i++)
        cout << i % 10 << ' ';
    cout << "\n";
    printDashes();
}
void Scheduler::printDashes()
{
    cout << "------------------------------------------------";
}
int main(void)
{
    Scheduler scheduler;
    scheduler.readFile();
    scheduler.execute();
    return 0;
}