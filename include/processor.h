#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor 
{
public:
    float utilization();
    
// Declare any necessary private members
private:
    long prev_active_jiffies_{0};
    long prev_jiffies_{0};
};

#endif