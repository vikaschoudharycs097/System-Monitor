#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

// TODO: Return the system's CPU
Processor& System::cpu() 
{ 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
std::vector<Process>& System::processes() 
{
    std::unordered_map<int, Process> temp;
    std::vector<int> pids = linux_parser::processesPids();

    for (auto &process: processes_)
    {
        temp.insert(std::make_pair(process.processId(), process));
    }
    processes_.clear();

    for (auto &pid: pids)
    {
        std::string ram = linux_parser::processRam(pid);
        if (ram == "0")    // If "VmSize:" not present in /proc/[pid]/status
        {
            continue;
        }

        long curr_active_jiffies = linux_parser::processActiveJiffies(pid);
        long curr_uptime = linux_parser::processUpTime(pid);

        auto itr = temp.find(pid);
        if (itr != temp.end())
        {
            (*itr).second.processRam(ram);
            (*itr).second.cpuUtilization(curr_active_jiffies, curr_uptime);
            processes_.push_back((*itr).second);
        }
        else 
        {
            std::string user = linux_parser::processUser(pid);
            std::string command = linux_parser::processCommand(pid);
            Process p(pid, user, command);
            p.processRam(ram);
            p.cpuUtilization(curr_active_jiffies, curr_uptime);
            processes_.push_back(p);
        }
    }
    temp.clear();

    // Sorting processes_ and then reversing for highest to lowest utility
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());

    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::kernelVersion() 
{ 
    return linux_parser::kernelVersion(); 
}

// Return the system's memory utilization
float System::memoryUtilization() 
{ 
    return linux_parser::memoryUtilization(); 
}

// Return the operating system name
std::string System::operatingSystem() 
{ 
    return linux_parser::operatingSystem(); 
}

// Return the number of processes actively running on the system
int System::runningProcesses() 
{ 
    return linux_parser::runningProcesses(); 
}

// Return the total number of processes on the system
int System::totalProcesses() 
{ 
    return linux_parser::totalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::upTime() 
{ 
    return linux_parser::upTime(); 
}