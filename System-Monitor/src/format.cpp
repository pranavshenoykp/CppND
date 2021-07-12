#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long s, m, h;
  string SS, MM, HH, format_time;
  s = seconds % 60;
  m = (seconds % 3600) / 60;
  h = seconds / 3600;
  
  if (s < 10){
    SS = '0' + std::to_string(s);
  }
  else{
    SS = std::to_string(s);
  }
  if (m < 10){
    MM = '0' + std::to_string(m);
  }
  else{
    MM = std::to_string(m);
  }
  if (h < 10){
    HH = '0' + std::to_string(h);
  }
  else{
    HH = std::to_string(h);
  }
  
  format_time = HH + " : " + MM + " : " + SS;
  return format_time; }