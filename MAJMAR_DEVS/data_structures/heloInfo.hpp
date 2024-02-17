#ifndef __HELO_INFO_HPP__
#define __HELO_INFO_HPP__


#include <iostream>
#include <math.h>

using namespace std;

struct HeloInfo{
	
	HeloInfo() {}
	
	HeloInfo(int i_heloID, bool i_stop)
				:heloID(i_heloID), stop(i_stop){}
				  
	
	
	int heloID;
	bool stop;
};

ostream& operator << (ostream &out, const HeloInfo &h);

istream& operator >> (istream &in, HeloInfo &h);

#endif //__HELO_INFO_HPP__