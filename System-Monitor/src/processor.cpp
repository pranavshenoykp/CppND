#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
/*
Reference : https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/
float Processor::Utilization() { 
  activeJiffies_ = LinuxParser::ActiveJiffies();
  
  idleJiffies_ = LinuxParser::IdleJiffies();
  totalJiffies_ = activeJiffies_ + idleJiffies_;
  
  total_del_t_ = totalJiffies_ - prevTotalJiffies_;
  idle_del_t_ = idleJiffies_ - prevIdleJiffies_;
  
  float numer, denom;
  
  numer = total_del_t_ - idle_del_t_;
  denom = total_del_t_;
  
  utilization_ = numer / denom;
  
  
  prevIdleJiffies_ = idleJiffies_;
  prevTotalJiffies_ = totalJiffies_;
  
  return utilization_; 
}