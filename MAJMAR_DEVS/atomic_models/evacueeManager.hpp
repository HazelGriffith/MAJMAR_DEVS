#ifndef __EVACUEE_MANAGER_HPP__
#define __EVACUEE_MANAGER_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <stdio.h>

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
		vector<EvacInfo> evacueesInWhite;
		vector<EvacInfo> evacueesInGreen;
		vector<EvacInfo> evacueesInYellow;
		vector<EvacInfo> evacueesInRed;
		vector<EvacInfo> evacueesInBlack;
		vector<EvacInfo> evacueesToLoad;
		int currHeloID;
		int capacityTaken;
		bool stop;
		bool evacueesLoading;
		
		
		// Set the default values for the state constructor for this specific model
		EvacueeManagerState(): sigma(0), currHeloID(-1), capacityTaken(0), stop(false), evacueesLoading(false){};
	};

	std::ostream& operator<<(std::ostream &out, const EvacueeManagerState& state) {
		out << ",White: " << state.evacueesInWhite.size() << ",Green: " <<  state.evacueesInGreen.size() << ",Yellow: " << state.evacueesInYellow.size() << ",Red: " << state.evacueesInRed.size() << ",Black: " << state.evacueesInBlack.size();
		out << ",Evacuees to Load: ";
		for (EvacInfo e : state.evacueesToLoad){
			out << "EvacueeID: " << e.evacueeID << "HeloID: " << e.heloID << "Triage Status: " << e.triage_status;
			out << "\n";
		}
		out << "\n";
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

			// Output ports
			Port<EvacInfo> outEvac;
			Port<bool> outHM;

			// Declare variables for the model's behaviour
			string heloLoadingPolicy = "Green First";
			int heloCapacity = 10;
			int with_stretcher = 3;
			int without_stretcher = 1;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a EvacueeManager object and a EvacueeManager object
			 * are created, using the same id.
			 *
			 * @param id ID of the new EvacueeManager model object, will be used to identify results on the output file
			 */
			EvacueeManager(const string& id, vector<EvacInfo> i_evacueesOnSite): Atomic<EvacueeManagerState>(id, EvacueeManagerState()) {

				// Initialize ports for the model

				// Input Ports
				inEvac  = addInPort<EvacInfo>("inEvac");
				inHM  = addInPort<HeloInfo>("inHM");

				// Output Ports
				outEvac = addOutPort<EvacInfo>("outEvac");
				outHM = addOutPort<bool>("outHM");

				// Initialize variables for the model's behaviour
				state.evacueesOnSite = i_evacueesOnSite;
				for (int i = 0; i < state.evacueesOnSite.size(); i++){
					//cout << "The triage status is: " << state.evacueesOnSite[i].triage_status << endl;
					switch(state.evacueesOnSite[i].triage_status){
						case('W'):
							state.evacueesInWhite.push_back(state.evacueesOnSite[i]);
							break;
						case('G'):
							state.evacueesInGreen.push_back(state.evacueesOnSite[i]);
							break;
						case('Y'):
							state.evacueesInYellow.push_back(state.evacueesOnSite[i]);
							break;
						case('R'):
							state.evacueesInRed.push_back(state.evacueesOnSite[i]);
							break;
						case('B'):
							state.evacueesInBlack.push_back(state.evacueesOnSite[i]);
							break;
						default:
							assert(("Not a valid triage status", false));
							break;
					}
				}

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
				if (state.evacueesLoading){
					state.evacueesLoading = false;
					state.evacueesToLoad.clear();
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
								for (int i = 0; i < state.evacueesInWhite.size(); i++){
									if (state.evacueesInWhite[i].evacueeID == x.evacueeID){
										state.evacueesInWhite.erase(state.evacueesInWhite.begin() + i);
										x.triage_status = 'G';
										state.evacueesInGreen.push_back(x);
									}
								}
								break;
							case('G'):
								for (int i = 0; i < state.evacueesInGreen.size(); i++){
									if (state.evacueesInGreen[i].evacueeID == x.evacueeID){
										state.evacueesInGreen.erase(state.evacueesInGreen.begin() + i);
										x.triage_status = 'Y';
										state.evacueesInYellow.push_back(x);
									}
								}
								break;
							case('Y'):
								for (int i = 0; i < state.evacueesInYellow.size(); i++){
									if (state.evacueesInYellow[i].evacueeID == x.evacueeID){
										state.evacueesInYellow.erase(state.evacueesInYellow.begin() + i);
										x.triage_status = 'R';
										state.evacueesInRed.push_back(x);
									}
								}
								break;
							case('R'):
								for (int i = 0; i < state.evacueesInRed.size(); i++){
									if (state.evacueesInRed[i].evacueeID == x.evacueeID){
										state.evacueesInRed.erase(state.evacueesInRed.begin() + i);
										x.triage_status = 'B';
										state.evacueesInBlack.push_back(x);
									}
								}
								break;
							default:
								assert(("Not a valid triage status", false));
								break;
						}
					}
					state.sigma = numeric_limits<double>::infinity();
				}
				
				if(!inHM->empty()){
					vector<HeloInfo> x = inHM->getBag();
					if (x.size() > 1){
						assert(("Too many helos arriving", false));
					} else {
						state.currHeloID = x[0].heloID;
						state.sigma = 0;
						if (heloLoadingPolicy.compare("Green First")==0){
							while (state.capacityTaken < heloCapacity){
								if (!state.evacueesInGreen.empty()){
									EvacInfo evac = state.evacueesInGreen[0];
									state.evacueesInGreen.erase(state.evacueesInGreen.begin());
									state.evacueesToLoad.push_back(evac);
									state.capacityTaken+=without_stretcher;
								} else if (!state.evacueesInWhite.empty()){
									EvacInfo evac = state.evacueesInWhite[0];
									state.evacueesInWhite.erase(state.evacueesInWhite.begin());
									state.evacueesToLoad.push_back(evac);
									state.capacityTaken+=without_stretcher;
								} else if (!state.evacueesInRed.empty()){
									EvacInfo evac = state.evacueesInRed[0];
									state.evacueesInRed.erase(state.evacueesInRed.begin());
									state.evacueesToLoad.push_back(evac);
									state.capacityTaken+=with_stretcher;
								} else if (!state.evacueesInYellow.empty()){
									EvacInfo evac = state.evacueesInYellow[0];
									state.evacueesInYellow.erase(state.evacueesInYellow.begin());
									state.evacueesToLoad.push_back(evac);
									state.capacityTaken+=with_stretcher;
								} else {
									state.capacityTaken = 10;
									state.stop = true;
								}
							}
							if (state.capacityTaken > heloCapacity){
								state.stop = false;
								EvacInfo evac = state.evacueesToLoad.back();
								state.evacueesToLoad.pop_back();
								switch(evac.triage_status){
									case('W'):
										state.evacueesInWhite.insert(state.evacueesInWhite.begin(),evac);
										break;
									case('G'):
										state.evacueesInGreen.insert(state.evacueesInGreen.begin(),evac);
										break;
									case('Y'):
										state.evacueesInYellow.insert(state.evacueesInYellow.begin(),evac);
										break;
									case('R'):
										state.evacueesInRed.insert(state.evacueesInRed.begin(),evac);
										break;
									default:
										assert(("Not a valid triage status", false));
										break;
								}
							}
							if (state.capacityTaken > 0){
								state.evacueesLoading = true;
							} 
							state.capacityTaken = 0;
						}
					}
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
				}
				if (state.evacueesLoading){
					for (int j = 0; j < (state.evacueesToLoad).size(); j++){
						EvacInfo evac = state.evacueesToLoad[j];
						evac.heloID = state.currHeloID;
						evac.enteringOrLeaving = true;
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