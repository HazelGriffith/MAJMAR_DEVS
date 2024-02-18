#ifndef __HELICOPTER_QUEUE_HPP__
#define __HELICOPTER_QUEUE_HPP__

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
	struct HelicopterQueueState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		vector<HeloInfo> helosWaiting;
		bool needHelo;
		bool waitingForHelo;
		bool stop;
		
		// Set the default values for the state constructor for this specific model
		HelicopterQueueState(): sigma(0), needHelo(false), waitingForHelo(true), stop(false){};
	};

	std::ostream& operator<<(std::ostream &out, const HelicopterQueueState& state) {
		if (!state.helosWaiting.empty()){
			out << "," << state.helosWaiting.size() << " Helos are waiting to land\n";
			for (HeloInfo h : state.helosWaiting){
				out << "HeloID: " << h.heloID;
			}
		} else {
			out << ",No helos are waiting to land";
		}
		out << "\n";
		return out;
	}

	// Atomic model of HelicopterQueue
	class HelicopterQueue: public Atomic<HelicopterQueueState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<HeloInfo> inHelo;
			Port<bool> inHM;

			// Output ports
			Port<HeloInfo> outHelo;
			Port<HeloInfo> outHM;

			// Declare variables for the model's behaviour

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a HelicopterQueue object and a HelicopterQueue object
			 * are created, using the same id.
			 *
			 * @param id ID of the new HelicopterQueue model object, will be used to identify results on the output file
			 */
			HelicopterQueue(const string& id): Atomic<HelicopterQueueState>(id, HelicopterQueueState()) {

				// Initialize ports for the model

				// Input Ports
				inHelo = addInPort<HeloInfo>("inHelo");
				inHM = addInPort<bool>("inHM");

				// Output Ports
				outHM = addOutPort<HeloInfo>("outHM");
				outHelo = addOutPort<HeloInfo>("outHelo");

				// Initialize variables for the model's behaviour

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
			void internalTransition(HelicopterQueueState& state) const override {
				if (!state.stop){
					if (state.needHelo){
						if (!state.waitingForHelo){
							state.needHelo = false;
							state.helosWaiting.erase(state.helosWaiting.begin());
							if (state.helosWaiting.empty()){
								state.waitingForHelo = true;
							}
						}
					}
				} else {
					state.helosWaiting.clear();
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
			void externalTransition(HelicopterQueueState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!inHM->empty()){
					vector<bool> x = inHM->getBag();
					if (x.size() > 1){
						assert(("Too many stop msgs", false));
					} else {
						if (x[0]){
							state.stop = true;
							state.needHelo = false;
							state.sigma = 0;
						} else {
							state.needHelo = true;
							state.sigma = 0;
						}
					}
				}
				
				if(!inHelo->empty()){
					for( const auto x : inHelo->getBag()){
						state.helosWaiting.push_back(x);
					}
					state.waitingForHelo = false;
					state.sigma = 0;
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
			void output(const HelicopterQueueState& state) const override {
				if (state.stop){
					if (!state.needHelo){
						for (int i = 0; i < state.helosWaiting.size(); i++){
							HeloInfo helo = state.helosWaiting[i];
							outHelo->addMessage(HeloInfo{helo.heloID, true});
						}
					}
				} else {
					if ((state.needHelo)&&(!state.helosWaiting.empty())){
						outHM->addMessage(state.helosWaiting[0]);
						outHelo->addMessage(state.helosWaiting[0]);
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
			[[nodiscard]] double timeAdvance(const HelicopterQueueState& state) const override {
				return state.sigma;
			}
	};
	#endif // __HELICOPTER_QUEUE_HPP__
}