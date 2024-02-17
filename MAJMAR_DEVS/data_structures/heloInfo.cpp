#include <math.h> 
#include <iostream>
#include <string>

#include "heloInfo.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator << (ostream &out, const HeloInfo &h){
	out << "Helicopter " << h.heloID;
	if (!h.stop){
		out << " is evacuating people" << h.heloID;
	} else {
		out << " has stopped";
	}
	
	return out;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator >> (istream &in, HeloInfo &h){
	string tempStr;
	
	in >> h.heloID;
	
	in >> tempStr;
	if (tempStr.compare("T") == 0){
		h.stop = true;
	} else {
		h.stop = false;
	}
	
	return in;
}