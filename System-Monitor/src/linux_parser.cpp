#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

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
    linestream >> os >> version >> kernel;
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
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line, key, value, kb;
  float memtotal;
  float memfree;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      //std::replace(line.begin(), line.end(), ' ', '');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kb){
        if (key == "MemTotal") {
          memtotal = std::stof(value.substr(0,value.size()-2)); 
        }
        if (key == "MemFree") {
          memfree = std::stof(value.substr(0,value.size()-2)); 
        }
      }
    }
  }
  return (memtotal - memfree)/memtotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line, time1, time2;
  long uptime;
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> time1 >> time2;
    uptime = std::stol(time1);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jiffy_list = LinuxParser::CpuUtilization();
  long int activeJiffies = 0;
  for(size_t i=0; i < jiffy_list.size(); i++)
  {
    if (i != LinuxParser::kIdle_ and i != LinuxParser::kIOwait_ and i != LinuxParser::kGuest_ and i != LinuxParser::kGuestNice_){
      activeJiffies += std::stol(jiffy_list[i]);
    }
  }
  //std::cout << "util:" << activeJiffies << std::endl;
  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffy_list = LinuxParser::CpuUtilization();
  long int idleJiffies = 0;
  for(size_t i=0; i < jiffy_list.size(); i++)
  {
    if (i == LinuxParser::kIdle_ or i == LinuxParser::kIOwait_){
      idleJiffies += std::stol(jiffy_list[i]);
    }
  }
  //std::cout << "Idle util:" << idleJiffies << std::endl;
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  vector<string> jiffy_list;
  string line, key;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu")
      {
        while(linestream >> key){
          jiffy_list.push_back(key);
        }
      }
    }
  }
  
  //std::cout << " " << jiffy_list[0] << " " << jiffy_list[1] << " " << jiffy_list[2] << " " << jiffy_list[3] << " " << jiffy_list[4] << " " << jiffy_list[5] << " " << jiffy_list[6] << " " << jiffy_list[7] << " " << jiffy_list[8] << " " << jiffy_list[9] << std::endl;
  
  
  return jiffy_list; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, key, value;
  float totalproc;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          totalproc = std::stoi(value);
          return totalproc;
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, key, value;
  float runningproc;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          runningproc = std::stoi(value);
          return runningproc;
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  string line;
  if (stream.is_open()){
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line, key, value, kb, ram;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kb)
      {
        if (key == "VmSize"){
          ram = value;
        }
      }
    }
  }
  long ram_long = std::stol(ram);
  
  return std::to_string(ram_long/1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line, key, value, uid;
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid"){
          uid = value;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::ifstream stream(kPasswordPath);
  string uid, key, value1, value2, line, user;
  uid = LinuxParser::Uid(pid);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value1 >> value2)
      {
         if (value2 == uid){
           user = key;
           //return value1;
         }
      }
    }
  }
  
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  vector<string> value_stack;
  string line, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      value_stack.push_back(value);
    }
  }
  long Hertz = sysconf(_SC_CLK_TCK);
  return LinuxParser::UpTime() - (std::stol(value_stack[21]) / Hertz);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::ActiveJiffies(int pid) {
  vector<string> value_stack;
  long uptime = LinuxParser::UpTime();
  string line, value;
  float utime, stime, cutime, cstime, starttime, total_time, seconds;
  float Hertz = sysconf(_SC_CLK_TCK);
  float cpu_usage;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      value_stack.push_back(value);
    }
    utime = std::stol(value_stack[13]);
    stime = std::stol(value_stack[14]);
    cutime = std::stol(value_stack[15]);
    cstime = std::stol(value_stack[16]);
    starttime = std::stol(value_stack[21]);
    
    total_time = utime + stime;
    total_time = total_time + cutime + cstime;
    seconds = uptime - (starttime / Hertz);
    cpu_usage = ((total_time / Hertz) / seconds); // in percentage
  }
  return cpu_usage;
}

