#include <math.h> 
#include <iostream>
#include <string>

#include "evacInfo.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator << (ostream &out, const EvacInfo &e){
	out << "Evacuee " << e.evacueeID << " in triage category " << e.triage_status;
	if (e.cgs){
		if (e.enteringOrLeaving){
			out << " is told to board the coast guard ship";
		} else {
			out << " is told to leave the coast guard ship";
		}
	} else {
		if (e.enteringOrLeaving){
			out << " is assigned helicopter " << e.heloID;
		} else {
			out << " is not assigned a helicopter";
		}
	}
	return out;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator >> (istream &in, EvacInfo &e){
	string tempStr;
	
	in >> e.evacueeID;
	in >> e.heloID;
	
	in >> tempStr;
	if (tempStr.compare("T") == 0){
		e.cgs = true;
	} else {
		e.cgs = false;
	}
	
	in >> tempStr;
	if (tempStr.compare("T") == 0){
		e.enteringOrLeaving = true;
	} else {
		e.enteringOrLeaving = false;
	}
	
	in >> e.triage_status;
	
	return in;
}