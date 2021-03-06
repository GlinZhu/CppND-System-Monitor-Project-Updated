#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include<sstream>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version>>kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  //long int Memtotal, MemFree, Buffers, cached;
  std::ifstream memfile(kProcDirectory + kMeminfoFilename);
  string line;
  string Mem_name;
  long int value;
  long int arr[5]={0,0,0,0,0};
  int count=0;
  if(memfile.is_open()){
    while(std::getline(memfile, line)){
      std::istringstream lineparser(line);
      lineparser>>Mem_name>>value;
      arr[count]=value;
      count++;
      if(count>4){
        break;
      }
      //std::cout<<"I'm in the loop for "<<count<<"\n";
    }
  }
  long int MemUsed=arr[0]-arr[1]-(arr[3]+arr[4]);
  long int MemTotal=arr[0];
  float MemUsed_meg=MemUsed/(1024*1024);
  float MemTotal_meg=MemTotal/(1024*1024);
  return MemUsed_meg/MemTotal_meg; 
  
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream myfile(kProcDirectory + kUptimeFilename);
  string line;
  long time_spent;
  long time_idle;
  if(myfile.is_open()){
    while(std::getline(myfile, line)){
      std::istringstream lineparser(line);
      lineparser>>time_spent>>time_idle;
    }
  }
  return time_spent;
  
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream myfile(kProcDirectory + kStatFilename);
  std::string line;
  std::string command;
  //long value;
  vector<long> jiffs;
  if(myfile.is_open()){
    std::getline(myfile, line);
    std::istringstream lineparser(line);
    if(lineparser>>command){
      for(long value;lineparser>>value;){
        jiffs.push_back(value);
      }
    }
      
  }
  long total_jiffies=0;
  // sum of first four jiffies (user, niced, system, idle) 
  for(int i=0;i<4;++i){
    total_jiffies+=jiffs[i];
  }
  return total_jiffies;
  
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::ifstream myfile(kProcDirectory + kStatFilename);
  std::string line;
  std::string command;
  long value1, value2, value3;
  long active_jiff;
  if(myfile.is_open()){
    std::getline(myfile, line);
    std::istringstream lineparser(line);
    if(lineparser>>command>>value1>>value2>>value3){
      active_jiff=value1+value2+value3;
    }
      
  }
  return active_jiff;
  
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::ifstream myfile(kProcDirectory + kStatFilename);
  std::string line;
  std::string command;
  long value1, value2, value3, value4;
  long idle_jiff;
  if(myfile.is_open()){
    std::getline(myfile, line);
    std::istringstream lineparser(line);
    if(lineparser>>command>>value1>>value2>>value3>>value4){
      idle_jiff=value4;
    }
      
  }
  return idle_jiff;
  
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream myfile(kProcDirectory + kStatFilename);
  std::string line;
  std::string items;
  int process{0};
  int value;
  if(myfile.is_open()){
    while(std::getline(myfile, line)){
      std::istringstream lineparser(line);
      if(lineparser>>items>>value){
        
        if(items=="processes"){
          process=value;
          //std::cout<<process<<"\n";
        }
      }
    }
  }
  return process;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream myfile(kProcDirectory + kStatFilename);
  std::string line;
  std::string items;
  int process{0};
  int value;
  if(myfile.is_open()){
    while(std::getline(myfile, line)){
      std::istringstream lineparser(line);
      if(lineparser>>items>>value){
        if(items=="procs_running"){
          process=value;
          //std::cout<<process<<"\n";
        }
      }
    }
  }
  return process;
  
}

vector<string> LinuxParser::cpu_times(int pid){
  string spid=to_string(pid);
  std::ifstream myfile(kProcDirectory + spid+"/"+kStatFilename);
  std::string line;
  std::string command;
  vector<std::string> commands;
  int i=0;
  if(myfile.is_open()){
    std::getline(myfile, line);
    std::istringstream lineparser(line);
    while(lineparser>>command){
      if(!command.empty())
        commands.push_back(command);
      else{
        commands.push_back("0");
      }
      i++;
      if(i>22){
        break;
      }
    } 
  }
  else{
    std::cout<<"No file is open "<<"\n";
  }
  /*
  std::cout<<"The current commands"<<"\n";
  for(string & s:commands){
    std::cout<<s<<" ";
  }
  std::cout<<"\n";
  std::cout<<"the 14 element ----------------------"<<stol(commands[13])<<"\n";
  */
  return commands;
}

// TODO: Read and return CPU utilization
float LinuxParser::PIDCpuUtilization(int pid) { 
  long hertz=sysconf(_SC_CLK_TCK);
  vector<string> commands=cpu_times(pid);
  long utime{0}, stime{0}, cutime{0}, cstime{0}, startime{0};
  if(commands[13].size()>0||commands[14].size()>0||commands[15].size()>0||commands[16].size()>0||commands[21].size()>0){
    utime=stol(commands[13]);
    stime=stol(commands[14]);
    cutime=stol(commands[15]);
    cstime=stol(commands[16]);
    startime=stol(commands[21]);
  }
  bool DEBUG{false};
  if(DEBUG){
    std::cout<<"arr[13]"<<commands[13]<<" "<<utime<<"\n";
    std::cout<<"arr[14]"<<commands[14]<<" "<<stime<<"\n";
    std::cout<<"arr[15]"<<commands[15]<<" "<<cutime<<"\n";
    std::cout<<"arr[16]"<<commands[16]<<" "<<cstime<<"\n";
    std::cout<<"arr[21]"<<commands[21]<<" "<<startime<<"\n";
  }
  float cpu_uti{0};
  long total_time=utime+stime+cutime+cstime;
  long uptime=UpTime();
  float seconds=uptime-(startime/hertz);
  cpu_uti=(total_time/hertz)/seconds;
  return cpu_uti;
  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string spid=to_string(pid);
  std::ifstream myfile(kProcDirectory+spid+"/"+kCmdlineFilename);
  std::string line;
  std::string command;
  
  if(myfile.is_open()){
    std::getline(myfile, line);
    std::istringstream lineparser(line);
    lineparser>>command;
    
  }
  return command;
  
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string spid=to_string(pid);
  std::ifstream myfile(kProcDirectory + spid+"/"+kStatusFilename);
  std::string line;
  std::string command;
  string value;
  string ram_size{};
  if(myfile.is_open()){
    while(std::getline(myfile, line)){
      std::istringstream lineparser(line);
      lineparser>>command>>value;
      if(command=="VmSize:")
        ram_size=value;; //return as kb string;
    }
    
  }
  return ram_size;
  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string spid=to_string(pid);
  std::ifstream myfile(kProcDirectory + spid+"/"+kStatusFilename);
  std::string line;
  std::string command;
  string value;
  string UID{};
  if(myfile.is_open()){
    while(std::getline(myfile, line)){
      std::istringstream lineparser(line);
      lineparser>>command>>value;
      if(command=="Uid:")
        UID=value;
    }
    
  }
  return UID;
  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  //string spid=to_string(pid);
  std::ifstream myfile(kPasswordPath);
  std::string line;
  std::string command;
  string value1, value2;
  string uid=Uid(pid);
  string User{};
  if(myfile.is_open()){
    while(std::getline(myfile, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineparser(line);
      lineparser>>command>>value1>>value2;
      if(value2==uid)
        User=command;
    }
    
  }
  return User;
  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long hertz=sysconf(_SC_CLK_TCK);
  vector<string> stime=cpu_times(pid);
  //std::cout<<"check commands "<<stime[0]<<" "<<stime[1]<<"\n";
  long starttime{0};
  if(!stime[21].empty())
    starttime=stol(stime[21]);
  long uptime=UpTime();
  long process_time=uptime-starttime/hertz;
  // return in seconds
  return process_time;
}
