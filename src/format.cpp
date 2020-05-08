#include <string>
#include<iostream>
#include "format.h"

using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hour=seconds/3600;
    long minutes=seconds%3600/60;
    long sec= seconds%3600%60;
    string h= (hour < 10 ? "0" : "")+to_string(hour);
    string m= (minutes < 10 ? "0" : "")+ to_string(minutes);
    string s= (sec < 10 ? "0" : "")+to_string(sec);
    string output=h+":"+m+":"+s;
    return h+":"+m+":"+s;
    
}