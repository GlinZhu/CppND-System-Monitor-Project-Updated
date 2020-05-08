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
    int hour=int(seconds/3600);
    int minutes=int((seconds%3600)/60);
    int sec=int(seconds%60);
    string h=to_string(hour);
    string m=to_string(minutes);
    string s=to_string(sec);
    //std::cout<<"the long sec is "<<seconds<<"\n";
    //std::cout<<"the seconds is "<<sec<<"\n";
    string output=h+":"+m+":"+s;
    return output; 
    
}