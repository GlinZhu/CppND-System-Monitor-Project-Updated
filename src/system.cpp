#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include<iostream>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;


// TODO: Return the system's CPU
//done
Processor& System::Cpu() { 
    return cpu_; 
    
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids=LinuxParser::Pids();
    //std::cout<<"the size of processes_ "<<pids.size()<<"\n";
    set<int> pidsets;

    for(Process &p:processes_){
        pidsets.insert(p.Pid());
    }
    for(auto i:pids){
        if(pidsets.find(i)==pidsets.end()){
            Process pro;
            pro.Int_pid(i);
            pro.Ram();
            processes_.emplace_back(pro);
        }
    }
    for(Process &p:processes_){
        p.Ram();
        p.CpuUtilization();
    }
    std::sort(processes_.begin(), processes_.end());
    
    //for(int i=0;i<15;i++){
    ///    Process proc=processes_[i];
    //    std::cout<<"the pid "<<proc.Get_pid()<<"the ram is "<<proc.Ram()<<"\n";
    //}

    return processes_; 
    
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    std::string kernel=LinuxParser::Kernel();
    //std::cout<<"the kernel name is "<<kernel<<std::endl;
    return kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 

    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
    
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
    
}