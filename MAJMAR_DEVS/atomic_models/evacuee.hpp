#ifndef __EVACUEE_HPP__
#define __EVACUEE_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include <chrono>


#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct EvacueeState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		char triage_status;
		char curr_loc;
		int heloID;
		int evacueeID;
		bool travelling;
		bool start;
		bool firstTimeOnShip;
		
		//triage_status can be {'W' = "White", 'G' = "Green", 'Y' = "Yellow", 'R' = "Red", 'B' = "Black"}
		
		//curr_loc can be {'E' = "Evacuation Site", 'H' = "Helicopter", 'F' = "FOL", 'C' = "Coast Guard Ship"}
		// Set the default values for the state constructor for this specific model
		EvacueeState(): sigma(0), evacueeID(-1), triage_status('W'), curr_loc('E'), heloID(-1), travelling(false), start(true), firstTimeOnShip(false){};
	};

	std::ostream& operator<<(std::ostream &out, const EvacueeState& state) {
		out << "Evacuee;" << state.evacueeID << ";in triage state;" << state.triage_status;
		if (state.travelling == true){
			switch(state.curr_loc){
				case('H'):
					out << ";is entering helicopter;" << state.heloID;
					break;
				case('F'):
					out << ";is entering the FOL";
					break;
				case('E'):
					out << ";is walking in circles";
					break;
				default:
					assert(("The location is not possible", false));
					break;
			}
		} else {
			switch(state.curr_loc){
				case('H'):
					out << ";is in helicopter;" << state.heloID;
					break;
				case('F'):
					out << ";is at the FOL";
					break;
				case('E'):
					out << ";is at the evacuation site";
					break;
				case('C'):
					out << ";is on board the Coast Guard Ship";
					break;
				default:
					assert(("The location is not possible", false));
					break;
			}
		}
		return out;
	}

	// Atomic model of Evacuee
	class Evacuee: public Atomic<EvacueeState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<EvacInfo> in;

			// Output ports
			Port<EvacInfo> outHelo;
			Port<EvacInfo> outFOL;
			Port<EvacInfo> outES;

			// Declare variables for the model's behaviour
			int evacueeID;
			float m_wTog = 120*60;
			float m_gToy = 48*60;
			float m_yTor = 8*60;
			float m_rTob = 1.5*60;
			float m_rToy = m_wTog;
			float m_yTog = 72*60;
			float m_gTow = m_gToy;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a Evacuee object and a Evacuee object
			 * are created, using the same id.
			 *
			 * @param id ID of the new Evacuee model object, will be used to identify results on the output file
			 */
			Evacuee(const string& id, int i_evacueeID, char i_triage_status): Atomic<EvacueeState>(id, EvacueeState()) {

				// Initialize ports for the model

				// Input Ports
				in  = addInPort<EvacInfo>("in");

				// Output Ports
				outHelo = addOutPort<EvacInfo>("outHelo");
				outFOL = addOutPort<EvacInfo>("outFOL");
				outES = addOutPort<EvacInfo>("outES");

				// Initialize variables for the model's behaviour
				state.evacueeID = i_evacueeID;
				state.triage_status = i_triage_status;

				// Set a value for sigma (so it is not 0), this determines how often the
				// internal transition occurs
				state.sigma = 0;
				
			}

			/**
			 * The transition function is invoked each time the value of
			 * state.sigma reaches 0.
			 *
			 * In this model, the value of state.lightOn is toggled.
			 *
			 * @param state reference to the current state of the model.
			 */
			void internalTransition(EvacueeState& state) const override {
				if (state.travelling){
					state.travelling = false;
				}
				if (state.curr_loc == 'E'){
					unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
					minstd_rand0 generator(seed1);
					exponential_distribution<float> wTogDistribution{float(1.0/m_wTog)};
					exponential_distribution<float> gToyDistribution{float(1.0/m_gToy)};
					exponential_distribution<float> yTorDistribution{float(1.0/m_yTor)};
					exponential_distribution<float> rTobDistribution{float(1.0/m_rTob)};
					if (state.start){
						state.start = false;
						switch(state.triage_status){
							case('W'):
								state.sigma = (double) wTogDistribution(generator);
								break;
							case('G'):
								state.sigma = (double) gToyDistribution(generator);
								break;
							case('Y'):
								state.sigma = (double) yTorDistribution(generator);
								break;
							case('R'):
								state.sigma = (double) rTobDistribution(generator);
								break;
							default:
								assert((state.triage_status == 'B'));
								state.sigma = numeric_limits<double>::infinity();
								break;
								//The status must be black
						}
					} else {
						switch(state.triage_status){
							case('W'):
								state.triage_status = 'G';
								state.sigma = (double) gToyDistribution(generator);
								break;
							case('G'):
								state.triage_status = 'Y';
								state.sigma = (double) yTorDistribution(generator);
								break;
							case('Y'):
								state.triage_status = 'R';
								state.sigma = (double) rTobDistribution(generator);
								break;
							case('R'):
								state.triage_status = 'B';
								state.sigma = numeric_limits<double>::infinity();
								break;
							default:
								assert((state.triage_status == 'B'));
								state.sigma = numeric_limits<double>::infinity();
								break;
								//The status must be black
						}
					}
				} else if (state.curr_loc == 'C'){
					unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
					minstd_rand0 generator(seed1);
					exponential_distribution<float> rToyDistribution{float(1.0/m_rToy)};
					exponential_distribution<float> yTogDistribution{float(1.0/m_yTog)};
					exponential_distribution<float> gTowDistribution{float(1.0/m_gTow)};
					if (state.firstTimeOnShip){
						state.firstTimeOnShip = false;
						switch(state.triage_status){
							case('W'):
								state.sigma = numeric_limits<double>::infinity();
								break;
							case('G'):
								state.sigma = (double) gTowDistribution(generator);
								break;
							case('Y'):
								state.sigma = (double) yTogDistribution(generator);
								break;
							case('R'):
								state.sigma = (double) rToyDistribution(generator);
								break;
							default:
								assert((state.triage_status == 'B'));
								state.sigma = numeric_limits<double>::infinity();
								break;
								//The status must be black
						}
					} else {
						switch(state.triage_status){
							case('W'):
								state.sigma = numeric_limits<double>::infinity();
								break;
							case('G'):
								state.triage_status = 'W';
								state.sigma = numeric_limits<double>::infinity();
								break;
							case('Y'):
								state.triage_status = 'G';
								state.sigma = (double) gTowDistribution(generator);
								break;
							case('R'):
								state.triage_status = 'Y';
								state.sigma = (double) yTogDistribution(generator);
								break;
							default:
								assert((state.triage_status == 'B'));
								state.sigma = numeric_limits<double>::infinity();
								break;
								//The status must be black
						}
					}
				} else {
					state.sigma = numeric_limits<double>::infinity();
				}
			}

			/**
			 * The external transition function is invoked each time external data
			 * is sent to an input port for this model.
			 *
			 * @param state reference to the current model state.
			 * @param e time elapsed since the last state transition function was triggered.
			 */
			void externalTransition(EvacueeState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!in->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( const auto x : in->getBag()){
						if (x.cgs){
							if (x.enteringOrLeaving){
								state.curr_loc = 'C';
								state.firstTimeOnShip = true;
							} else {
								state.curr_loc = 'E';
								state.start = true;
							}
							
						} else {
							if (x.enteringOrLeaving){
								state.heloID = x.heloID;
								state.curr_loc = 'H';
							} else {
								state.curr_loc = 'F';
							}
							state.travelling = true;
						}
						state.sigma = 0;
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
			void output(const EvacueeState& state) const override {
				if (state.travelling){
					switch(state.curr_loc){
						case('H'):
							outHelo->addMessage(EvacInfo{state.evacueeID, state.heloID, false, true, state.triage_status});
							break;
						case('F'):
							outFOL->addMessage(EvacInfo{state.evacueeID, state.heloID, false, false, state.triage_status});
							break;
						default:
							assert(("The curr_loc and travelling are not lining up correctly", false));
							break;
					}
				} else if ((!state.start)&&(!state.firstTimeOnShip)){
					switch(state.curr_loc){
						case('E'):
							outES->addMessage(EvacInfo{state.evacueeID, state.heloID, false, false, state.triage_status});
							break;
						case('C'):
							outES->addMessage(EvacInfo(state.evacueeID, state.heloID, true, true, state.triage_status));
							break;
						default:
							assert(("The curr_loc and travelling are not lining up correctly", false));
							break;
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
			[[nodiscard]] double timeAdvance(const EvacueeState& state) const override {
				return state.sigma;
			}
	};
	#endif // __EVACUEE_HPP__
}