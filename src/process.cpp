#include <unistd.h>
#include <string>
#include "process.h"

// Constructor
Process::Process(int pid, std::string user, std::string command):
pid_(pid), user_(user), command_(command), prev_active_jiffies_(0), prev_uptime_(0) {}

// Return this process's ID
int Process::processId() 
{ 
    return pid_; 
}

// Return this process's CPU utilization
float Process::cpuUtilization() 
{ 
    return util_; 
}

// Return the command that generated this process
std::string Process::processCommand() 
{ 
    return command_; 
}

// Return this process's memory utilization
std::string Process::processRam() 
{ 
    return ram_; 
}

// Return the user (name) that generated this process
std::string Process::processUser() 
{ 
    return user_; 
}

// Return the age of this process (in seconds)
long int Process::processUpTime() 
{ 
    return prev_uptime_; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const 
{   
    return util_ < a.util_; 
}

// Calculating cpu utilization
void Process::cpuUtilization(long active_jiffies, long uptime)
{
    util_ = static_cast<float>(active_jiffies - prev_active_jiffies_) / 
            ((uptime - prev_uptime_) * sysconf(_SC_CLK_TCK));
    prev_active_jiffies_ = active_jiffies;
    prev_uptime_ = uptime;
}

// Setting RAM used by the process
void Process::processRam(std::string ram)
{
    ram_ = ram;
}