#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "linux_parser.h"

// Read and return operating system with version
std::string linux_parser::operatingSystem() 
{
    std::string line;
    std::string key;
    std::string value;
    std::ifstream filestream(OS_PATH);
    if (filestream) 
    {
        while (std::getline(filestream, line)) 
        {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            linestream >> key >> value; 
            if (key == "PRETTY_NAME") 
            {
                std::replace(value.begin(), value.end(), '_', ' ');
                break;
            }
        }

        // Closing file stream
        filestream.close();
    }

    return value;
}

// Read and return kernel version
std::string linux_parser::kernelVersion() 
{
    std::string os, version, kernel;
    std::string line;
    std::ifstream filestream(PROC_DIRECTORY + VERSION_FILENAME);
    if (filestream) 
    {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;

        // Closing file stream
        filestream.close();
    }

    return "Version " + kernel;
}

// Finding all processes pids
// Implemented using C++17 filesystem
std::vector<int> linux_parser::processesPids() 
{
    std::vector<int> pids;
    std::string filename;
    for (auto &file: std::filesystem::directory_iterator(PROC_DIRECTORY))
    {
        if (file.is_directory())
        {
            filename = file.path().filename().string();
            if (std::all_of(filename.begin(), filename.end(), isdigit))
            {
                pids.push_back(std::stoi(filename));
            }
        }
    }

    return pids;
}

// Read and return the system memory utilization
float linux_parser::memoryUtilization() 
{ 
    std::string line;
    std::string key;
    std::string value;
    std::vector<long> memory;
    std::ifstream filestream(PROC_DIRECTORY + MEMINFO_FILENAME);

    if (filestream)
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            linestream >> key >> value;
            memory.push_back(std::stol(value));
        }
    }
    // totalM - freeM - Buffers - Cached - Slab 
    // https://access.redhat.com/solutions/406773 (Red Hat Enterprise Linux 7.1 or later)
    return static_cast<float>(memory[0] - memory[1] - memory[3] - memory[4] - memory[22]) / memory[0]; 
}

// Read and return the system uptime
long linux_parser::upTime() 
{ 
    std::string line;
    std::string value;
    std::ifstream filestream(PROC_DIRECTORY + UPTIME_FILENAME);
    if (filestream)
    {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> value;

        // Closing file stream
        filestream.close();
    }

    return long(std::stof(value)); 
}

// Read and return the number of jiffies for the system
long linux_parser::totalJiffies() 
{
    long total_jiffies = 0.0;
    std::vector<std::string> cpu_uses = linux_parser::cpuUtilization();
    for (auto &jiff: cpu_uses)
    {
        total_jiffies += std::stol(jiff);
    }

    return total_jiffies; 
}

// Read and return the number of active jiffies for a PID
long linux_parser::processActiveJiffies(int pid) 
{
    long total_jiffies = 0;
    std::string value;
    std::string line;
    std::vector<std::string> values;
    std::ifstream filestream(PROC_DIRECTORY + "/" + std::to_string(pid) + STAT_FILENAME);
    if (filestream)
    {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while (linestream >> value)
        {
            values.push_back(value);
        }

        // Closing file stream
        filestream.close();
    } 
    for (int i = 13; i < 17; i++)
    {
        total_jiffies += std::stol(values[i]);
    }

    return total_jiffies; 
}

// Read and return the number of active jiffies for the system
long linux_parser::activeJiffies() 
{ 
    long total_active_jiffies = 0.0;
    std::vector<std::string> cpu_uses = linux_parser::cpuUtilization();
    for (auto &jiff: cpu_uses)
    {
        total_active_jiffies += std::stol(jiff);
    }

    // Substracting idle jiffies beacause it added and not the part of active jiffies
    total_active_jiffies -= std::stol(cpu_uses[3]);

    return total_active_jiffies;  
}

// Read and return the number of idle jiffies for the system
long linux_parser::idleJiffies() 
{ 
    std::vector<std::string> cpu_uses = linux_parser::cpuUtilization();

    // Return idle jiffies
    return std::stol(cpu_uses[3]);  
}

// Read and return CPU utilization
std::vector<std::string> linux_parser::cpuUtilization() 
{ 
    std::vector<std::string> cpu_uses;
    std::string line;
    std::string key;
    std::string value;
    std::ifstream filestream(PROC_DIRECTORY + STAT_FILENAME);
    if (filestream)
    {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> key; // Skipping "cpu" string

        while (linestream >> value)
        {
            cpu_uses.push_back(value);
        }
    }
    
    return cpu_uses; 
}

// Read and return the total number of processes
int linux_parser::totalProcesses() 
{ 
    std::string line;
    std::string key;
    std::string value;
    std::ifstream filestream(PROC_DIRECTORY + STAT_FILENAME);
    if (filestream)
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "processes")
            {
                linestream >> value;
                break;
            }
        }

        // Closing file stream
        filestream.close();
    }
    return std::stoi(value); 
}

// Read and return the number of running processes
int linux_parser::runningProcesses() 
{ 
    std::string line;
    std::string key;
    std::string value;
    std::ifstream filestream(PROC_DIRECTORY + STAT_FILENAME);
    if (filestream)
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "procs_running")
            {
                linestream >> value;
                break;
            }
        }

        // Closing file stream
        filestream.close();
    }
    return std::stoi(value);
}

// Read and return the command associated with a process
std::string linux_parser::processCommand(int pid) 
{ 
    std::string line;
    std::ifstream filestream(PROC_DIRECTORY + "/" + std::to_string(pid) + CMDLINE_FILENAME);
    if (filestream)
    {
        std::getline(filestream, line);

        // Closing file stream
        filestream.close();
    }

    return line; 
}

// Read and return the memory used by a process
std::string linux_parser::processRam(int pid) 
{ 
    std::string mem = "";
    std::string key;
    std::string value;
    std::string line;
    std::ifstream filestream(PROC_DIRECTORY + "/" + std::to_string(pid) + STATUS_FILENAME);
    if (filestream)
    {
        while(std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "VmSize:")
            {
                mem = value;
                break;
            }
        }
    }

    if (mem == "")
    {
        return "0";
    }

    return std::to_string(long(std::stof(mem) / 1000)); 
}

// Read and return the user ID associated with a process
std::string linux_parser::processUid(int pid) 
{ 
    std::string key;
    std::string value;
    std::string line;
    std::ifstream filestream(PROC_DIRECTORY + "/" + std::to_string(pid) + STATUS_FILENAME);
    if (filestream)
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "Uid:")
            {
                linestream >> value;
                break;
            }    
        }

        // Closing file stream
        filestream.close();
    }

    return value; 
}

// Read and return the user associated with a process
std::string linux_parser::processUser(int pid) 
{
    std::string key;
    std::string temp;
    std::string value;
    std::string line;
    std::string uid = linux_parser::processUid(pid);
    std::ifstream filestream(PASSWORD_PATH);
    if (filestream)
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> key >> temp >> value;
            if (value == uid)
            {
                break;
            }    
        }

        // Closing file stream
        filestream.close();
    }

    return key; 
}

// Read and return the uptime of a process
long linux_parser::processUpTime(int pid) 
{
    std::string value;
    std::string line;
    std::vector<std::string> values;
    std::ifstream filestream(PROC_DIRECTORY + "/" + std::to_string(pid) + STAT_FILENAME);
    if (filestream)
    {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while (linestream >> value)
        {
            values.push_back(value);
        }

        // Closing file stream
        filestream.close();
    }

    return linux_parser::upTime() - std::stol(values[21]) / sysconf(_SC_CLK_TCK); 
}