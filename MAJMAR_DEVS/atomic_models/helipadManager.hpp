#ifndef __HELIPAD_MANAGER_HPP__
#define __HELIPAD_MANAGER_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <cassert>

#include "../data_structures/heloInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct HelipadManagerState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		bool stop;
		bool heloLoading;
		bool requesting;
		int currHeloID;
		
		
		// Set the default values for the state constructor for this specific model
		HelipadManagerState(): sigma(0), stop(false), heloLoading(false), requesting(true), currHeloID(-1){};
	};

	std::ostream& operator<<(std::ostream &out, const HelipadManagerState& state) {
		if (state.heloLoading){
			out << "Helo;" << state.currHeloID << ";is at the ES";
		} else {
			out << ";No helos are at ES";
		} 
		if (state.stop == true){
			out << ";Stopped evacuating people";
		}
		return out;
	}

	// Atomic model of HelipadManager
	class HelipadManager: public Atomic<HelipadManagerState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<HeloInfo> inHQ;
			Port<bool> inEM;

			// Output ports
			Port<HeloInfo> outHelo;
			Port<bool> outHQ;
			Port<HeloInfo> outEM;

			// Declare variables for the model's behaviour
			string heloLoadingPolicy = "Green First";
			double timeToLoad;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a HelipadManager object and a HelipadManager object
			 * are created, using the same id.
			 *
			 * @param id ID of the new HelipadManager model object, will be used to identify results on the output file
			 */
			HelipadManager(const string& id, double i_timeToLoad): Atomic<HelipadManagerState>(id, HelipadManagerState()) {

				// Initialize ports for the model

				// Input Ports
				inHQ = addInPort<HeloInfo>("inHQ");
				inEM = addInPort<bool>("inEM");

				// Output Ports
				outEM = addOutPort<HeloInfo>("outEM");
				outHelo = addOutPort<HeloInfo>("outHelo");
				outHQ = addOutPort<bool>("outHQ");

				// Initialize variables for the model's behaviour
				timeToLoad = i_timeToLoad;

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
			void internalTransition(HelipadManagerState& state) const override {
				if (!state.stop){
					if (!state.requesting){
						if (state.heloLoading){
							state.heloLoading = false;
							state.requesting = true;
							state.sigma = 0;
						} else {
							state.heloLoading = true;
							state.sigma = timeToLoad;
						}
					} else {
						state.requesting = false;
						state.sigma = numeric_limits<double>::infinity();
					}
				} else {
					if (state.heloLoading){
						state.heloLoading = false;
						state.sigma = numeric_limits<double>::infinity();;
					} else {
						state.heloLoading = true;
						state.sigma = timeToLoad;
					}
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
			void externalTransition(HelipadManagerState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!inEM->empty()){
					vector<bool> x = inEM->getBag();
					if (x.size() > 1){
						assert(("Too many stop msgs", false));
					} else {
						state.stop = x[0];
						state.sigma -= e;
					}
				}
				
				if(!inHQ->empty()){
					for( const auto x : inHQ->getBag()){
						if (state.heloLoading == false){
							state.currHeloID = x.heloID;
							state.sigma = 0;
						} else {
							assert(("No room for a helo", false));
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
			void output(const HelipadManagerState& state) const override {
				if (!state.stop){
					if (!state.requesting){
						if (state.heloLoading){
							outHelo->addMessage(HeloInfo{state.currHeloID, false});
						} else {
							outEM->addMessage(HeloInfo{state.currHeloID, false});
						}
					} else {
						outHQ->addMessage(false);
					}
				} else {
					if (state.heloLoading){
						outHelo->addMessage(HeloInfo{state.currHeloID, true});
					}
					outHQ->addMessage(true);
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
			[[nodiscard]] double timeAdvance(const HelipadManagerState& state) const override {
				return state.sigma;
			}
	};
	#endif // __HELIPAD_MANAGER_HPP__
}