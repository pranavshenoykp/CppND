#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long int activeJiffies_ = 0;
  long int idleJiffies_ = 0;

  long int prevIdleJiffies_ = 0;
  long int totalJiffies_ = 0;
  long int prevTotalJiffies_ = 0;
  float utilization_ = 0;
  
  long int total_del_t_ = 1;
  long int idle_del_t_ = 0;
};

#endif