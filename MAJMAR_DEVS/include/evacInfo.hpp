#ifndef __EVAC_INFO_HPP__
#define __EVAC_INFO_HPP__


#include <iostream>
#include <math.h>

using namespace std;

struct EvacInfo{
	
	EvacInfo() {}
	
	EvacInfo(int i_evacueeID, int i_heloID, bool i_cgs, bool i_enteringOrLeaving, char i_triage_status)
				:evacueeID(i_evacueeID), heloID(i_heloID), cgs(i_cgs), enteringOrLeaving(i_enteringOrLeaving), triage_status(i_triage_status) {}
				  
	
	
	int evacueeID;
	int heloID;
	bool cgs;
	bool enteringOrLeaving;
	char triage_status;
};

ostream& operator << (ostream &out, const EvacInfo &e);

istream& operator >> (istream &in, EvacInfo &e);

#endif //__EVAC_INFO_HPP__