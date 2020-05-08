#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 

    return this->pid; 
    
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    return LinuxParser::PIDCpuUtilization(this->pid); 
    
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(this->pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string ram_size=LinuxParser::Ram(this->pid);
    long int ram{0};
    if(!ram_size.empty())
        ram=stol(ram_size);
    int ramf=ram/1024; //convert to Mb from kb
    this->ram_size=ramf;
    return to_string(ramf);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(this->pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    int curr_ram=this->ram_size;
    int new_ram=a.Get_Ram();
    return curr_ram>new_ram;
}