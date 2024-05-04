#ifndef __FOL_HPP__
#define __FOL_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>

#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct FOLState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		vector<EvacInfo> evacuees;
		
		// Set the default values for the state constructor for this specific model
		FOLState(): sigma(0){};
	};

	std::ostream& operator<<(std::ostream &out, const FOLState& state) {
		for (int j = 0; j < (state.evacuees).size(); j++){
			out << ";EvacueeID;" << state.evacuees[j].evacueeID << ";Triage Status;" << state.evacuees[j].triage_status;
		}
		out << ";Lives Saved;" << state.evacuees.size();
		return out;
	}

	// Atomic model of FOL
	class FOL: public Atomic<FOLState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<EvacInfo> in;

			// Output ports

			// Declare variables for the model's behaviour

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a FOL object and a FOL object
			 * are created, using the same id.
			 *
			 * @param id ID of the new FOL model object, will be used to identify results on the output file
			 */
			FOL(const string& id): Atomic<FOLState>(id, FOLState()) {

				// Initialize ports for the model

				// Input Ports
				in  = addInPort<EvacInfo>("in");

				// Output Ports

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
			void internalTransition(FOLState& state) const override {
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
			void externalTransition(FOLState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!in->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( const auto x : in->getBag()){
						state.evacuees.push_back(x);
					}
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
			void output(const FOLState& state) const override {
				
			}

			/**
			 * It returns the value of state.sigma for this model.
			 *
			 * This function is the same for all models, and does not need to be changed.
			 *
			 * @param state reference to the current model state.
			 * @return the sigma value.
			 */
			[[nodiscard]] double timeAdvance(const FOLState& state) const override {
				return state.sigma;
			}
	};
	#endif // __FOL_HPP__
}