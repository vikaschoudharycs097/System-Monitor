#include <cstdio>
#include <string>
#include "format.h"

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
std::string format::elapsedTime(long seconds) 
{
    char formatted[10];
    long hours;
    long minutes;

    // Convert seconds into minutes
    minutes = seconds / 60;
    seconds %= 60;

    // Convert minutes into hours
    hours = minutes / 60;
    minutes %= 60;

    // Formatting string
    sprintf(formatted, "%02ld:%02ld:%02ld", hours, minutes, seconds);

    return std::string(formatted); 
}