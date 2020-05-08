#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long curr_total_jiff, curr_active;
    float utilization{0};
    //pre_total_jiffs=LinuxParser::Jiffies();
    //pre_active_jiffs=LinuxParser::ActiveJiffies();
    //sleep(5);
    curr_total_jiff=LinuxParser::Jiffies();
    curr_active=LinuxParser::ActiveJiffies();
    utilization=static_cast<float> (curr_active-pre_active_jiffs)/(curr_total_jiff-pre_total_jiffs);
    //utilization=static_cast<float> (curr_active)/(curr_total_jiff);
    pre_total_jiffs=curr_total_jiff;
    pre_active_jiffs=curr_active;
    return utilization;
    
}