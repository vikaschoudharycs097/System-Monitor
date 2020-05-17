#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process 
{
public:
    Process(int pid, std::string user_, std::string command_);
    int processId();                               
    std::string processUser();                      
    std::string processCommand();                   
    float cpuUtilization();                  
    std::string processRam();                       
    long int processUpTime();                       
    bool operator<(Process const& a) const;  
    void cpuUtilization(long active_jiffies, long uptime);
    void processRam(std::string);


// Declare any necessary private members
private:
    int pid_;
    std::string user_;
    std::string command_;
    std::string ram_;
    long prev_active_jiffies_;
    long prev_uptime_;
    float util_;
};

#endif