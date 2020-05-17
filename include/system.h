#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include "process.h"
#include "processor.h"

class System 
{
public:
    Processor& cpu();                   
    std::vector<Process>& processes();  
    float memoryUtilization();          
    long upTime();                      
    int totalProcesses();               
    int runningProcesses();             
    std::string kernelVersion();        
    std::string operatingSystem();      

// Define any necessary private members
private:
    Processor cpu_ = {};
    std::vector<Process> processes_ = {};
};

#endif