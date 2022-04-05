#ifndef _UTILS_HPP__
#define _UTILS_HPP__

#include <iostream>
using namespace std;

bool contain( string s1 , const string s2 ){
    if( s1.find( s2 ) != string::npos )
        return true;
    else
        return false;
}

#endif  // _UTILS_HPP__
