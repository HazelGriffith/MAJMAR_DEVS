#ifndef __EVACUEE_MANAGER_HPP__
#define __EVACUEE_MANAGER_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <stdio.h>
#include <random>
#include <chrono>

#include "../data_structures/evacInfo.hpp"
#include "../data_structures/heloInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct EvacueeManagerState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		vector<EvacInfo> evacueesOnSite;
		vector<EvacInfo> evacueesOnShip;
		vector<EvacInfo> evacueesInWhiteOffShip;
		vector<EvacInfo> evacueesInGreenOffShip;
		vector<EvacInfo> evacueesInYellowOffShip;
		vector<EvacInfo> evacueesInRedOffShip;
		vector<EvacInfo> evacueesInWhiteOnShip;
		vector<EvacInfo> evacueesInGreenOnShip;
		vector<EvacInfo> evacueesInYellowOnShip;
		vector<EvacInfo> evacueesInRedOnShip;
		vector<EvacInfo> evacueesInBlack;
		vector<EvacInfo> evacueesToTransfer;
		int currHeloID;
		int availableShipCapacity;
		bool stop;
		bool evacueesTransferring;
		
		
		// Set the default values for the state constructor for this specific model
		EvacueeManagerState(): sigma(0), currHeloID(-1), availableShipCapacity(50), stop(false), evacueesTransferring(false){};
	};

	std::ostream& operator<<(std::ostream &out, const EvacueeManagerState& state) {
		out << "OFF SHIP: White;" << state.evacueesInWhiteOffShip.size() << ";Green;" <<  state.evacueesInGreenOffShip.size() << ";Yellow;" << state.evacueesInYellowOffShip.size() << ";Red;" << state.evacueesInRedOffShip.size() << ";Black;" << state.evacueesInBlack.size();
		out << ";ON SHIP: White;" << state.evacueesInWhiteOnShip.size() << ";Green;" <<  state.evacueesInGreenOnShip.size() << ";Yellow;" << state.evacueesInYellowOnShip.size() << ";Red;" << state.evacueesInRedOnShip.size();
		out << ";Evacuees to Transfer;";
		for (EvacInfo e : state.evacueesToTransfer){
			out << ";EvacueeID;" << e.evacueeID << ";HeloID;" << e.heloID << ";OnShip;" << e.cgs << ";Triage Status;" << e.triage_status;
		}
		return out;
	}

	// Atomic model of EvacueeManager
	class EvacueeManager: public Atomic<EvacueeManagerState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<EvacInfo> inEvac;
			Port<HeloInfo> inHM;
			Port<bool> inCGS;

			// Output ports
			Port<EvacInfo> outEvac;
			Port<bool> outHM;
			Port<bool> outCGS;

			// Declare variables for the model's behaviour
			string heloLoadingPolicy = "GF";
			string shipLoadingPolicy = "GF";
			string shipUnloadingPolicy = "WO";
			int heloCapacity = 10;
			int shipCapacity = 50;
			int with_stretcher = 3;
			int without_stretcher = 1;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * @param id ID of the new EvacueeManager model object, will be used to identify results on the output file
			 */
			EvacueeManager(const string& id, vector<EvacInfo> i_evacueesOnSite): Atomic<EvacueeManagerState>(id, EvacueeManagerState()) {

				// Initialize ports for the model

				// Input Ports
				inEvac  = addInPort<EvacInfo>("inEvac");
				inHM  = addInPort<HeloInfo>("inHM");
				inCGS = addInPort<bool>("inCGS");

				// Output Ports
				outEvac = addOutPort<EvacInfo>("outEvac");
				outHM = addOutPort<bool>("outHM");
				outCGS = addOutPort<bool>("outCGS");

				// Initialize variables for the model's behaviour
				state.evacueesOnSite = i_evacueesOnSite;
				for (int i = 0; i < state.evacueesOnSite.size(); i++){
					//cout << "The triage status is: " << state.evacueesOnSite[i].triage_status << endl;
					switch(state.evacueesOnSite[i].triage_status){
						case('W'):
							state.evacueesInWhiteOffShip.push_back(state.evacueesOnSite[i]);
							break;
						case('G'):
							state.evacueesInGreenOffShip.push_back(state.evacueesOnSite[i]);
							break;
						case('Y'):
							state.evacueesInYellowOffShip.push_back(state.evacueesOnSite[i]);
							break;
						case('R'):
							state.evacueesInRedOffShip.push_back(state.evacueesOnSite[i]);
							break;
						case('B'):
							state.evacueesInBlack.push_back(state.evacueesOnSite[i]);
							break;
						default:
							assert(("Not a valid triage status", false));
							break;
					}
				}
				state.availableShipCapacity = shipCapacity;
				// Set a value for sigma (so it is not 0), this determines how often the
				// internal transition occurs
				state.sigma = numeric_limits<double>::infinity();
				
			}

			/**
			 * The transition function is invoked each time the value of
			 * state.sigma reaches 0.
			 *
			 * In this model, the value of state.lightOn is toggled.
			 *
			 * @param state reference to the current state of the model.
			 */
			void internalTransition(EvacueeManagerState& state) const override {
				if (state.evacueesTransferring){
					state.evacueesTransferring = false;
					state.evacueesToTransfer.clear();
				}
				state.sigma = numeric_limits<double>::infinity();
			}

			/**
			 * The external transition function is invoked each time external data
			 * is sent to an input port for this model.
			 *
			 * In this model, the value of state.fastToggle is toggled each time the
			 * button connected to the "in" port is pressed.
			 *
			 * The value of state.sigma is then updated depending on the value of
			 * state.fastToggle.  Sigma is not required to be updated in this function,
			 * but we are changing it based on our desired logic for the program.
			 *
			 * @param state reference to the current model state.
			 * @param e time elapsed since the last state transition function was triggered.
			 */
			void externalTransition(EvacueeManagerState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!inEvac->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( auto x : inEvac->getBag()){
						switch(x.triage_status){
							case('W'):
								if (x.cgs){
									assert(("Shouldn't receive msg from evacuee if on CGS and with White tag", false));
								} else {
									for (int i = 0; i < state.evacueesInWhiteOffShip.size(); i++){
										if (state.evacueesInWhiteOffShip[i].evacueeID == x.evacueeID){
											state.evacueesInWhiteOffShip.erase(state.evacueesInWhiteOffShip.begin() + i);
											for (int j = 0; j < state.evacueesOnSite.size(); j++){
												if (state.evacueesOnSite[j].evacueeID == x.evacueeID){
													state.evacueesOnSite[j].triage_status = 'G';
													break;
												}
											}
											x.triage_status = 'G';
											state.evacueesInGreenOffShip.push_back(x);
											break;
										}
									}
								}
								break;
							case('G'):
								if (x.cgs){
									for (int i = 0; i < state.evacueesInGreenOnShip.size(); i++){
										if (state.evacueesInGreenOnShip[i].evacueeID == x.evacueeID){
											state.evacueesInGreenOnShip.erase(state.evacueesInGreenOnShip.begin() + i);
											for (int j = 0; j < state.evacueesOnShip.size(); j++){
												if (state.evacueesOnShip[j].evacueeID == x.evacueeID){
													state.evacueesOnShip[j].triage_status = 'W';
													break;
												}
											}
											x.triage_status = 'W';
											state.evacueesInWhiteOnShip.push_back(x);
											break;
										}
									}
								} else {
									for (int i = 0; i < state.evacueesInGreenOffShip.size(); i++){
										if (state.evacueesInGreenOffShip[i].evacueeID == x.evacueeID){
											state.evacueesInGreenOffShip.erase(state.evacueesInGreenOffShip.begin() + i);
											for (int j = 0; j < state.evacueesOnSite.size(); j++){
												if (state.evacueesOnSite[j].evacueeID == x.evacueeID){
													state.evacueesOnSite[j].triage_status = 'Y';
													break;
												}
											}
											x.triage_status = 'Y';
											state.evacueesInYellowOffShip.push_back(x);
											break;
										}
									}
								}
								break;
							case('Y'):
								if (x.cgs){
									for (int i = 0; i < state.evacueesInYellowOnShip.size(); i++){
										if (state.evacueesInYellowOnShip[i].evacueeID == x.evacueeID){
											state.evacueesInYellowOnShip.erase(state.evacueesInYellowOnShip.begin() + i);
											for (int j = 0; j < state.evacueesOnShip.size(); j++){
												if (state.evacueesOnShip[j].evacueeID == x.evacueeID){
													state.evacueesOnShip[j].triage_status = 'G';
													break;
												}
											}
											x.triage_status = 'G';
											state.evacueesInGreenOnShip.push_back(x);
											state.availableShipCapacity += with_stretcher - without_stretcher;
											assert(state.availableShipCapacity <= shipCapacity);
											break;
										}
									}
								} else {
									for (int i = 0; i < state.evacueesInYellowOffShip.size(); i++){
										if (state.evacueesInYellowOffShip[i].evacueeID == x.evacueeID){
											state.evacueesInYellowOffShip.erase(state.evacueesInYellowOffShip.begin() + i);
											for (int j = 0; j < state.evacueesOnSite.size(); j++){
												if (state.evacueesOnSite[j].evacueeID == x.evacueeID){
													state.evacueesOnSite[j].triage_status = 'R';
													break;
												}
											}
											x.triage_status = 'R';
											state.evacueesInRedOffShip.push_back(x);
											break;
										}
									}
								}
								break;
							case('R'):
								if (x.cgs){
									for (int i = 0; i < state.evacueesInRedOnShip.size(); i++){
										if (state.evacueesInRedOnShip[i].evacueeID == x.evacueeID){
											state.evacueesInRedOnShip.erase(state.evacueesInRedOnShip.begin() + i);
											for (int j = 0; j < state.evacueesOnShip.size(); j++){
												if (state.evacueesOnShip[j].evacueeID == x.evacueeID){
													state.evacueesOnShip[j].triage_status = 'Y';
													break;
												}
											}
											x.triage_status = 'Y';
											state.evacueesInYellowOnShip.push_back(x);
											break;
										}
									}
								} else {
									for (int i = 0; i < state.evacueesInRedOffShip.size(); i++){
										if (state.evacueesInRedOffShip[i].evacueeID == x.evacueeID){
											state.evacueesInRedOffShip.erase(state.evacueesInRedOffShip.begin() + i);
											for (int j = 0; j < state.evacueesOnSite.size(); j++){
												if (state.evacueesOnSite[j].evacueeID == x.evacueeID){
													state.evacueesOnSite.erase(state.evacueesOnSite.begin() + j);
													break;
												}
											}
											x.triage_status = 'B';
											state.evacueesInBlack.push_back(x);
											break;
										}
									}
								}
								break;
							default:
								assert(("Not a valid triage status", false));
								break;
						}
					}
					assert(state.evacueesOnSite.size() == (state.evacueesInRedOffShip.size() + state.evacueesInYellowOffShip.size() + state.evacueesInGreenOffShip.size() + state.evacueesInWhiteOffShip.size()));
					assert(state.evacueesOnShip.size() == (state.evacueesInRedOnShip.size() + state.evacueesInYellowOnShip.size() + state.evacueesInGreenOnShip.size() + state.evacueesInWhiteOnShip.size()));
					state.sigma = numeric_limits<double>::infinity();
				}
				
				if (!inCGS->empty()){
					vector<bool> x = inCGS->getBag();
					if (x.size() > 1){
						assert(("Too many msgs from CGS", false));
					} else {
						state.sigma = 0;
						if (x[0]){
							unloadShip(state);
							for (int i = 0; i < state.evacueesToTransfer.size(); i++){
								state.evacueesToTransfer[i].cgs = true;
								state.evacueesToTransfer[i].enteringOrLeaving = false;
								EvacInfo evacToTransfer = state.evacueesToTransfer[i];
								evacToTransfer.cgs = false;
								evacToTransfer.enteringOrLeaving = false;
								state.evacueesOnSite.push_back(evacToTransfer);
								switch(evacToTransfer.triage_status){
									case 'W':
										state.evacueesInWhiteOffShip.push_back(evacToTransfer);
										break;
									case 'G':
										state.evacueesInGreenOffShip.push_back(evacToTransfer);
										break;
									case 'Y':
										state.evacueesInYellowOffShip.push_back(evacToTransfer);
										break;
									case 'R':
										state.evacueesInRedOffShip.push_back(evacToTransfer);
										break;
									default:
										assert(("The triage status cannot be Black on board the ship", false));
										break;
								}
							}
						} else {
							state.availableShipCapacity = loadEvacuees(state, false);
							for (int i = 0; i < state.evacueesToTransfer.size(); i++){
								state.evacueesToTransfer[i].cgs = true;
								state.evacueesToTransfer[i].enteringOrLeaving = true;
								EvacInfo evacToTransfer = state.evacueesToTransfer[i];
								state.evacueesOnShip.push_back(evacToTransfer);
								switch(evacToTransfer.triage_status){
									case 'W':
										state.evacueesInWhiteOnShip.push_back(evacToTransfer);
										break;
									case 'G':
										state.evacueesInGreenOnShip.push_back(evacToTransfer);
										break;
									case 'Y':
										state.evacueesInYellowOnShip.push_back(evacToTransfer);
										break;
									case 'R':
										state.evacueesInRedOnShip.push_back(evacToTransfer);
										break;
									default:
										assert(("The triage status cannot be Black on board the ship", false));
										break;
								}
							}
						}
					}
					assert(state.evacueesOnSite.size() == (state.evacueesInRedOffShip.size() + state.evacueesInYellowOffShip.size() + state.evacueesInGreenOffShip.size() + state.evacueesInWhiteOffShip.size()));
					assert(state.evacueesOnShip.size() == (state.evacueesInRedOnShip.size() + state.evacueesInYellowOnShip.size() + state.evacueesInGreenOnShip.size() + state.evacueesInWhiteOnShip.size()));
				}
				
				if(!inHM->empty()){
					vector<HeloInfo> x = inHM->getBag();
					if (x.size() > 1){
						assert(("Too many helos arriving", false));
					} else {
						state.currHeloID = x[0].heloID;
						state.sigma = 0;
						loadEvacuees(state, true);
						for (int i = 0; i < state.evacueesToTransfer.size(); i++){
							state.evacueesToTransfer[i].cgs = false;
							state.evacueesToTransfer[i].enteringOrLeaving = true;
							state.evacueesToTransfer[i].heloID = state.currHeloID;
						}
						if ((state.evacueesOnSite.empty())&&(state.evacueesOnShip.empty())){
							state.stop = true;
						}
					}
					assert(state.evacueesOnSite.size() == (state.evacueesInRedOffShip.size() + state.evacueesInYellowOffShip.size() + state.evacueesInGreenOffShip.size() + state.evacueesInWhiteOffShip.size()));
					assert(state.evacueesOnShip.size() == (state.evacueesInRedOnShip.size() + state.evacueesInYellowOnShip.size() + state.evacueesInGreenOnShip.size() + state.evacueesInWhiteOnShip.size()));
				}
			}

			int loadEvacuees(EvacueeManagerState& state, bool heloOrShip) const{
				int capacity;
				string loadingPolicy;
				int capacityTaken = 0;
				if (heloOrShip){
					capacity = heloCapacity;
					loadingPolicy = heloLoadingPolicy;
				} else {
					capacity = state.availableShipCapacity;
					loadingPolicy = shipLoadingPolicy;
				}
				vector<char> loadOrder;
				unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
				minstd_rand0 generator(seed1);
				if (loadingPolicy.compare("GF")==0){
					vector<char> vect{'G','W','R','Y'};
					loadOrder = vect;
				} else if (loadingPolicy.compare("YF")==0){
					vector<char> vect{'Y','W','G','R'};
					loadOrder = vect;
				} else if (loadingPolicy.compare("CF")==0){
					vector<char> vect{'R','Y','G','W'};
					loadOrder = vect;
				} else if (loadingPolicy.compare("R")==0){
					loadOrder.clear();
				} else {
					assert(("Invalid loading policy", false));
				}
				int curr = 0;
				
				bool siteNotEmpty = true;
				
				while (capacityTaken < capacity){
					if (loadOrder.empty()){
						if (!state.evacueesOnSite.empty()){
							uniform_int_distribution<> evacueeDistribution{0,(int)state.evacueesOnSite.size()-1};
							int evacueeIndex = evacueeDistribution(generator);
							EvacInfo evac = state.evacueesOnSite[evacueeIndex];
							switch(evac.triage_status){
								case('W'):
									if (state.evacueesInWhiteOffShip.empty()){
										assert(("EvacueesOnSite does not match evacueesInWhiteOffShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=without_stretcher;
									}
									break;
								case('G'):
									if (state.evacueesInGreenOffShip.empty()){
										assert(("EvacueesOnSite does not match evacueesInGreenOffShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=without_stretcher;
									}
									break;
								case('Y'):
									if (state.evacueesInYellowOffShip.empty()){
										assert(("EvacueesOnSite does not match evacueesInYellowOffShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=with_stretcher;
									}
									break;
								case('R'):
									if (state.evacueesInRedOffShip.empty()){
										assert(("EvacueesOnSite does not match evacueesInRedOffShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=with_stretcher;
									}
									break;
								default:
									assert(("Cannot have triage status Black when loading", false));
									break;
							}
						} else if ((!state.evacueesOnShip.empty())&&(heloOrShip)){
							uniform_int_distribution<> evacueeDistribution{0,(int)state.evacueesOnShip.size()-1};
							int evacueeIndex = evacueeDistribution(generator);
							EvacInfo evac = state.evacueesOnShip[evacueeIndex];
							switch(evac.triage_status){
								case('W'):
									if (state.evacueesInWhiteOnShip.empty()){
										assert(("EvacueesOnShip does not match evacueesInWhiteOnShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=without_stretcher;
										state.availableShipCapacity += without_stretcher;
									}
									break;
								case('G'):
									if (state.evacueesInGreenOnShip.empty()){
										assert(("EvacueesOnShip does not match evacueesInGreenOnShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=without_stretcher;
										state.availableShipCapacity += without_stretcher;
									}
									break;
								case('Y'):
									if (state.evacueesInYellowOnShip.empty()){
										assert(("EvacueesOnShip does not match evacueesInYellowOnShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=with_stretcher;
										state.availableShipCapacity += with_stretcher;
									}
									break;
								case('R'):
									if (state.evacueesInRedOnShip.empty()){
										assert(("EvacueesOnShip does not match evacueesInRedOnShip", false));
									} else {
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=with_stretcher;
										state.availableShipCapacity += with_stretcher;
									}
									break;
								default:
									assert(("Cannot have triage status Black when loading", false));
									break;
							}
						} else {
							break;
						}
					} else {
						char loadingTag;
						if (!state.evacueesOnSite.empty()){
							if (curr < 4){
								loadingTag = loadOrder[curr];
							} else {
								loadingTag = 'D';
							}
							switch(loadingTag){
								case('W'):
									if (state.evacueesInWhiteOffShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInWhiteOffShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=without_stretcher;
									}
									break;
								case('G'):
									if (state.evacueesInGreenOffShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInGreenOffShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=without_stretcher;
									}
									break;
								case('Y'):
									if (state.evacueesInYellowOffShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInYellowOffShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=with_stretcher;
									}
									break;
								case('R'):
									if (state.evacueesInRedOffShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInRedOffShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, false);
										capacityTaken+=with_stretcher;
									}
									break;
								default:
									assert(("Loading: evacueesOnSite and evacuees off ship lists do not match", false));
									break;
							}
						} else if ((!state.evacueesOnShip.empty())&&(heloOrShip)){
							if (siteNotEmpty == true){
								siteNotEmpty = false;
								curr = 0;
							}
							if (curr < 4){
								loadingTag = loadOrder[curr];
							} else {
								loadingTag = 'D';
							}
							switch(loadingTag){
								case('W'):
									if (state.evacueesInWhiteOnShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInWhiteOnShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=without_stretcher;
										state.availableShipCapacity += without_stretcher;
									}
									break;
								case('G'):
									if (state.evacueesInGreenOnShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInGreenOnShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=without_stretcher;
										state.availableShipCapacity += without_stretcher;
									}
									break;
								case('Y'):
									if (state.evacueesInYellowOnShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInYellowOnShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=with_stretcher;
										state.availableShipCapacity += with_stretcher;
									}
									break;
								case('R'):
									if (state.evacueesInRedOnShip.empty()){
										curr++;
									} else {
										EvacInfo evac = state.evacueesInRedOnShip[0];
										state.evacueesToTransfer.push_back(evac);
										eraseEvac(state, evac, true);
										capacityTaken+=with_stretcher;
										state.availableShipCapacity += with_stretcher;
									}
									break;
								default:
									cout << "Evacs on ship: " << state.evacueesOnShip.size() << endl;
									cout << "Evacs on ship in White: " << state.evacueesInWhiteOnShip.size() << endl;
									cout << "Evacs on ship in Green: " << state.evacueesInGreenOnShip.size() << endl;
									cout << "Evacs on ship in Yellow: " << state.evacueesInYellowOnShip.size() << endl;
									cout << "Evacs on ship in Red: " << state.evacueesInRedOnShip.size() << endl;
									assert(("Loading: evacueesOnShip and evacuees on ship lists do not match", false));
									break;
							}
						} else {
							break;
						}
					}
				}
				if (capacityTaken > capacity){
					state.stop = false;
					EvacInfo evac = state.evacueesToTransfer.back();
					state.evacueesToTransfer.pop_back();
					switch(evac.triage_status){
						case('W'):
							if (evac.cgs){
								state.evacueesInWhiteOnShip.insert(state.evacueesInWhiteOnShip.begin(), evac);
								state.evacueesOnShip.push_back(evac);
								state.availableShipCapacity -= without_stretcher;
							} else {
								state.evacueesInWhiteOffShip.insert(state.evacueesInWhiteOffShip.begin(), evac);
								state.evacueesOnSite.push_back(evac);
							}
							capacityTaken -= without_stretcher;
							break;
						case('G'):
							if (evac.cgs){
								state.evacueesInGreenOnShip.insert(state.evacueesInGreenOnShip.begin(), evac);
								state.evacueesOnShip.push_back(evac);
								state.availableShipCapacity -= without_stretcher;
							} else {
								state.evacueesInGreenOffShip.insert(state.evacueesInGreenOffShip.begin(), evac);
								state.evacueesOnSite.push_back(evac);
							}
							capacityTaken -= without_stretcher;
							break;
						case('Y'):
							if (evac.cgs){
								state.evacueesInYellowOnShip.insert(state.evacueesInYellowOnShip.begin(), evac);
								state.evacueesOnShip.push_back(evac);
								state.availableShipCapacity -= with_stretcher;
							} else {
								state.evacueesInYellowOffShip.insert(state.evacueesInYellowOffShip.begin(), evac);
								state.evacueesOnSite.push_back(evac);
							}
							capacityTaken -= with_stretcher;
							break;
						case('R'):
							if (evac.cgs){
								state.evacueesInRedOnShip.insert(state.evacueesInRedOnShip.begin(), evac);
								state.evacueesOnShip.push_back(evac);
								state.availableShipCapacity -= with_stretcher;
							} else {
								state.evacueesInRedOffShip.insert(state.evacueesInRedOffShip.begin(), evac);
								state.evacueesOnSite.push_back(evac);
							}
							capacityTaken -= with_stretcher;
							break;
						default:
							assert(("An evacuee with triage status black could not have been in evacueesToTransfer", false));
							break;
					}
				}
				if (capacityTaken > 0){
					state.evacueesTransferring = true;
				} else {
					state.evacueesTransferring = false;
				}
				return capacity - capacityTaken;
			}
			
			void unloadShip(EvacueeManagerState& state) const {
				bool random;
				unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
				minstd_rand0 generator(seed1);
				if (shipUnloadingPolicy.compare("WO")==0){
					random = false;
				} else if (shipUnloadingPolicy.compare("R")==0){
					random = true;
				} else {
					assert(("Invalid ship unloading policy", false));
				}
				if (random){
					state.evacueesToTransfer.assign(state.evacueesOnShip.begin(), state.evacueesOnShip.end());
					state.evacueesOnShip.clear();
					state.evacueesInWhiteOnShip.clear();
					state.evacueesInGreenOnShip.clear();
					state.evacueesInYellowOnShip.clear();
					state.evacueesInRedOnShip.clear();
					state.availableShipCapacity = 50;
				} else {
					for (int i = 0; i < state.evacueesOnShip.size(); i++){
						EvacInfo evac = state.evacueesOnShip[i];
						if (evac.triage_status == 'W'){
							state.evacueesToTransfer.push_back(evac);
							eraseEvac(state, evac, true);
							state.availableShipCapacity += without_stretcher;
							assert(state.availableShipCapacity <= shipCapacity);
							assert(state.availableShipCapacity >= 0);
							i--;
						}
					}
				}
				if (state.evacueesToTransfer.size() > 0){
					state.evacueesTransferring = true;
				} else {
					state.evacueesTransferring = false;
				}
			}
			
			void eraseEvac(EvacueeManagerState& state, EvacInfo evac, bool onShip) const {
				if (onShip){
					for (int i = 0; i < state.evacueesOnShip.size(); i++){
						if (state.evacueesOnShip[i].evacueeID == evac.evacueeID){
							state.evacueesOnShip.erase(state.evacueesOnShip.begin() + i);
							break;
						}
					}
				} else {
					for (int i = 0; i < state.evacueesOnSite.size(); i++){
						if (state.evacueesOnSite[i].evacueeID == evac.evacueeID){
							state.evacueesOnSite.erase(state.evacueesOnSite.begin() + i);
							break;
						}
					}
				}
				switch(evac.triage_status){
					case('W'):
						if (onShip){
							for (int i = 0; i < state.evacueesInWhiteOnShip.size(); i++){
								if (state.evacueesInWhiteOnShip[i].evacueeID == evac.evacueeID){
									state.evacueesInWhiteOnShip.erase(state.evacueesInWhiteOnShip.begin() + i);
									break;
								}
							}
						} else {
							for (int i = 0; i < state.evacueesInWhiteOffShip.size(); i++){
								if (state.evacueesInWhiteOffShip[i].evacueeID == evac.evacueeID){
									state.evacueesInWhiteOffShip.erase(state.evacueesInWhiteOffShip.begin() + i);
									break;
								}
							}
						}
						break;
					case('G'):
						if (onShip){
							for (int i = 0; i < state.evacueesInGreenOnShip.size(); i++){
								if (state.evacueesInGreenOnShip[i].evacueeID == evac.evacueeID){
									state.evacueesInGreenOnShip.erase(state.evacueesInGreenOnShip.begin() + i);
									break;
								}
							}
						} else {
							for (int i = 0; i < state.evacueesInGreenOffShip.size(); i++){
								if (state.evacueesInGreenOffShip[i].evacueeID == evac.evacueeID){
									state.evacueesInGreenOffShip.erase(state.evacueesInGreenOffShip.begin() + i);
									break;
								}
							}
						}
						break;
					case('Y'):
						if (onShip){
							for (int i = 0; i < state.evacueesInYellowOnShip.size(); i++){
								if (state.evacueesInYellowOnShip[i].evacueeID == evac.evacueeID){
									state.evacueesInYellowOnShip.erase(state.evacueesInYellowOnShip.begin() + i);
									break;
								}
							}
						} else {
							for (int i = 0; i < state.evacueesInYellowOffShip.size(); i++){
								if (state.evacueesInYellowOffShip[i].evacueeID == evac.evacueeID){
									state.evacueesInYellowOffShip.erase(state.evacueesInYellowOffShip.begin() + i);
									break;
								}
							}
						}
						break;
					case('R'):
						if (onShip){
							for (int i = 0; i < state.evacueesInRedOnShip.size(); i++){
								if (state.evacueesInRedOnShip[i].evacueeID == evac.evacueeID){
									state.evacueesInRedOnShip.erase(state.evacueesInRedOnShip.begin() + i);
									break;
								}
							}
						} else {
							for (int i = 0; i < state.evacueesInRedOffShip.size(); i++){
								if (state.evacueesInRedOffShip[i].evacueeID == evac.evacueeID){
									state.evacueesInRedOffShip.erase(state.evacueesInRedOffShip.begin() + i);
									break;
								}
							}
						}
						break;
					default:
						assert(("Cannot delete evacuee with triage status Black", false));
						break;
				}
			}

			/**
			 * This function outputs any desired state values to their associated ports.
			 *
			 * In this model, the value of state.lightOn is sent via the out port.  Once
			 * the value of state.ligthOn reaches the I/O model, that model will update
			 * the status of the LED.
			 *
			 * @param state reference to the current model state.
			 */
			void output(const EvacueeManagerState& state) const override {
				
				if (state.stop){
					outHM->addMessage(true);
					outCGS->addMessage(true);
				}
				if (state.evacueesTransferring){
					for (int j = 0; j < (state.evacueesToTransfer).size(); j++){
						EvacInfo evac = state.evacueesToTransfer[j];
						outEvac->addMessage(evac);
					}
				}
			}

			/**
			 * It returns the value of state.sigma for this model.
			 *
			 * This function is the same for all models, and does not need to be changed.
			 *
			 * @param state reference to the current model state.
			 * @return the sigma value.
			 */
			[[nodiscard]] double timeAdvance(const EvacueeManagerState& state) const override {
				return state.sigma;
			}
	};
	#endif // __EVACUEE_MANAGER_HPP__
}