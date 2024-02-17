#ifndef __HELICOPTER_HPP__
#define __HELICOPTER_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>
#include <cassert>

#include "../data_structures/evacInfo.hpp"
#include "../data_structures/heloInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct HelicopterState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		vector<EvacInfo> evacueesOnBoard;
		char curr_loc;
		int capacity_taken;
		int heloID;
		bool returning;
		bool stop;
		
		// Set the default values for the state constructor for this specific model
		HelicopterState(): sigma(0), curr_loc('F'), heloID(-1), capacity_taken(0), returning(false), stop(false){};
	};

	std::ostream& operator<<(std::ostream &out, const HelicopterState& state) {
		out << ",Helicopter " << state.heloID;
		switch(state.curr_loc){
			case('F'):
				out << " is unloading people at the FOL";
				break;
			case('W'):
				out << " is waiting to land at the evacuation site";
				break;
			case('E'):
				out << " is loading people at the evacuation site";
				break;
			case('T'):
				if (state.returning){
					out << " is returning to the FOL";
				} else {
					out << " Is going to the evacuation site";
				}
				break;
			default:
				assert(("Not a valid location char", false));
				break;
		}
		out << " and has " << state.capacity_taken << " spots taken";
		out << "\n";
		return out;
	}

	// Atomic model of Helicopter
	class Helicopter: public Atomic<HelicopterState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<EvacInfo> inEvac;
			Port<HeloInfo> inES;

			// Output ports
			Port<EvacInfo> outEvac;
			Port<HeloInfo> outES;

			// Declare variables for the model's behaviour
			int heloID;
			int capacity = 10;
			double time_until_leaving = 0.25*60;
			double travel_time = 2.5*60;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a Helicopter object and a Helicopter object
			 * are created, using the same id.
			 *
			 * @param id ID of the new Helicopter model object, will be used to identify results on the output file
			 */
			Helicopter(const string& id, int i_heloID): Atomic<HelicopterState>(id, HelicopterState()) {

				// Initialize ports for the model

				// Input Ports
				inEvac  = addInPort<EvacInfo>("inEvac");
				inES = addInPort<HeloInfo>("inES");

				// Output Ports
				outEvac = addOutPort<EvacInfo>("outEvac");
				outES = addOutPort<HeloInfo>("outES");

				// Initialize variables for the model's behaviour
				state.heloID = i_heloID;

				// Set a value for sigma (so it is not 0), this determines how often the
				// internal transition occurs
				state.sigma = time_until_leaving;
				
			}

			/**
			 * The transition function is invoked each time the value of
			 * state.sigma reaches 0.
			 *
			 * In this model, the value of state.lightOn is toggled.
			 *
			 * @param state reference to the current state of the model.
			 */
			void internalTransition(HelicopterState& state) const override {
				switch(state.curr_loc){
					case('T'):
						if (state.returning){
							state.evacueesOnBoard.clear();
							state.capacity_taken = 0;
							state.curr_loc = 'F';
							state.returning = false;
							if (state.stop){
								state.sigma = numeric_limits<double>::infinity();
							} else {
								state.sigma = time_until_leaving;
							}
						} else {
							state.curr_loc = 'W';
							state.sigma = numeric_limits<double>::infinity();
						}
						break;
					case('F'):
						state.curr_loc = 'T';
						state.sigma = travel_time;
						break;
					case('E'):
						state.curr_loc = 'T';
						state.returning = true;
						state.sigma = travel_time;
						break;
					case('W'):
						if (!state.stop){
							state.curr_loc = 'E';
							state.sigma = numeric_limits<double>::infinity();
						} else {
							state.curr_loc = 'T';
							state.returning = true;
							state.sigma = travel_time;
						}
						break;
					default:
						assert(("Not a possible location", false));
						break;
				}
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
			void externalTransition(HelicopterState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!inEvac->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( const auto x : inEvac->getBag()){
						if (state.capacity_taken < capacity){
							state.evacueesOnBoard.push_back(x);
							if ((x.triage_status == 'W')||(x.triage_status == 'G')){
								state.capacity_taken++;
							} else if ((x.triage_status == 'Y')||(x.triage_status == 'R')){
								state.capacity_taken += 3;
							} else {
								assert(("Not a valid triage status", false));
							}
						} else {
							assert(("ES sent too many people", false));
						}
					}
				}
				
				if(!inES->empty()){
					vector<HeloInfo> x = inES->getBag();
					if (x.size() > 1){
						assert(("ES sent too many messages", false));
					} else {
						state.stop = x[0].stop;
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
			void output(const HelicopterState& state) const override {
				switch(state.curr_loc){
					case('F'):
						break;
					case('T'):
						if (!state.returning){
							outES->addMessage(HeloInfo{state.heloID, state.stop});
						} else {
							for (int j = 0; j < (state.evacueesOnBoard).size(); j++){
								EvacInfo currInfo = state.evacueesOnBoard[j];
								currInfo.heloID = state.heloID;
								currInfo.enteringOrLeaving = false;
								outEvac->addMessage(currInfo);
							}
						}
						break;
					case('W'):
						break;
					case('E'):
						break;
					default:
						assert(("Not a valid location char", false));
						break;
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
			[[nodiscard]] double timeAdvance(const HelicopterState& state) const override {
				return state.sigma;
			}
	};
	#endif // __HELICOPTER_HPP__
}