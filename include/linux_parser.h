#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <vector>
#include <string>

namespace linux_parser 
{
    // Paths
    const std::string PROC_DIRECTORY{"/proc"};
    const std::string CMDLINE_FILENAME{"/cmdline"};
    const std::string CPUINFO_FILENAME{"/cpuinfo"};
    const std::string STATUS_FILENAME{"/status"};
    const std::string STAT_FILENAME{"/stat"};
    const std::string UPTIME_FILENAME{"/uptime"};
    const std::string MEMINFO_FILENAME{"/meminfo"};
    const std::string VERSION_FILENAME{"/version"};
    const std::string OS_PATH{"/etc/os-release"};
    const std::string PASSWORD_PATH{"/etc/passwd"};

    // System
    float memoryUtilization();
    long upTime();
    std::vector<int> processesPids();
    int totalProcesses();
    int runningProcesses();
    std::string operatingSystem();
    std::string kernelVersion();

    // CPU
    std::vector<std::string> cpuUtilization();
    long totalJiffies();
    long activeJiffies();
    long idleJiffies();

    // Processes
    long processActiveJiffies(int pid);
    std::string processCommand(int pid);
    std::string processRam(int pid);
    std::string processUid(int pid);
    std::string processUser(int pid);
    long int processUpTime(int pid);
};  // namespace linux_parser

#endif