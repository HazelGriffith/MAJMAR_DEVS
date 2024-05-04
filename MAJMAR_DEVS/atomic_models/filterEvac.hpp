#ifndef __FILTER_EVAC_HPP__
#define __FILTER_EVAC_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <core/modeling/atomic.hpp>
#include <iostream>

#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct FilterEvacState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		vector<EvacInfo> msgs_passing_filter;
		
		// Set the default values for the state constructor for this specific model
		FilterEvacState(): sigma(0){};
	};

	std::ostream& operator<<(std::ostream &out, const FilterEvacState& state) {
		for (int j = 0; j < (state.msgs_passing_filter).size(); j++){
			out << ";Evacuee;" << state.msgs_passing_filter[j].evacueeID << ";in triage category;" << state.msgs_passing_filter[j].triage_status;
			if (state.msgs_passing_filter[j].enteringOrLeaving){
				out << ";is entering helicopter;" << state.msgs_passing_filter[j].heloID;
			} else {
				out << ";is leaving helicopter;" << state.msgs_passing_filter[j].heloID;
			}
		}
		return out;
	}

	// Atomic model of FilterEvac
	class FilterEvac: public Atomic<FilterEvacState> {
		private:

		public:

			// Declare ports for the model

			// Input ports
			Port<EvacInfo> in;

			// Output ports
			Port<EvacInfo> out;

			// Declare variables for the model's behaviour
			int heloID;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a FilterEvac object and a FilterEvac object
			 * are created, using the same id.
			 *
			 * @param id ID of the new FilterEvac model object, will be used to identify results on the output file
			 */
			FilterEvac(const string& id, int i_heloID): Atomic<FilterEvacState>(id, FilterEvacState()) {

				// Initialize ports for the model

				// Input Ports
				in  = addInPort<EvacInfo>("in");

				// Output Ports
				out = addOutPort<EvacInfo>("out");

				// Initialize variables for the model's behaviour
				heloID = i_heloID;

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
			void internalTransition(FilterEvacState& state) const override {
				state.msgs_passing_filter.clear();
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
			void externalTransition(FilterEvacState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!in->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( const auto x : in->getBag()){
						if ((x.heloID == heloID)&&(x.enteringOrLeaving == true)){
							state.msgs_passing_filter.push_back(x);
							state.sigma = 0;
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
			void output(const FilterEvacState& state) const override {
				for (int j = 0; j < (state.msgs_passing_filter).size(); j++){
					out->addMessage(state.msgs_passing_filter[j]);
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
			[[nodiscard]] double timeAdvance(const FilterEvacState& state) const override {
				return state.sigma;
			}
	};
	#endif // __FILTER_EVAC_HPP__
}