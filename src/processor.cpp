#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::utilization() 
{ 
    long active_jiffies_temp = linux_parser::activeJiffies();
    long jiffies_temp = linux_parser::totalJiffies();
    float util = static_cast<float>(active_jiffies_temp - prev_active_jiffies_) / (jiffies_temp - prev_jiffies_);

    // Storing data for next calculation
    prev_active_jiffies_ = active_jiffies_temp;
    prev_jiffies_ = jiffies_temp;

    return util; 
}