#ifndef __COAST_GUARD_SHIP_HPP__
#define __COAST_GUARD_SHIP_HPP__

#include <core/modeling/atomic.hpp>
#include <iostream>

using namespace std;

namespace cadmium::assignment1 {
	
	// Contains state variables for coast guard ship
	struct CoastGuardShipState {

		// time advance variable sigma
		double sigma;

		//unloading is true when evacuees are being unloaded from the ship
		bool unloading;
		
		//arrived is true when the ship has arrived at the evacuation site
		bool arrived;
		
		//stop is true when there are no more evacuees at the evacuation site so the coast guard ship should stop
		bool stop;
		
		// these are the default state values
		CoastGuardShipState(): sigma(0), unloading(false), arrived(false), stop(false){};
	};

	std::ostream& operator<<(std::ostream &out, const CoastGuardShipState& state) {
		if (state.arrived){
			if (!state.stop){
				if (state.unloading){
					out << "The ship has arrived and is unloading evacuees.";
				} else {
					out << "The ship has arrived and is loading evacuees.";
				}
			} else {
				out << "There are no more evacuees.";
			}
		} else {
			out << "The ship will arrive in " << state.sigma/60 << " hours.";
		}
		return out;
	}

	// Atomic model of CoastGuardShip
	class CoastGuardShip: public Atomic<CoastGuardShipState> {
		private:

		public:

			// Input port from EvacueeManager
			Port<bool> in;

			// Output port to EvacueeManager
			Port<bool> out;

			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a CoastGuardShip object and a CoastGuardShipState object
			 * are created, using the same id.
			 *
			 * @param id ID of the new CoastGuardShip model object, will be used to identify results on the output file
			 */
			CoastGuardShip(const string& id, double startTime): Atomic<CoastGuardShipState>(id, CoastGuardShipState()) {

				// Initialize ports for the model

				// Input port from EvacueeManager
				in  = addInPort<bool>("in");

				// Output port to EvacueeManager
				out = addOutPort<bool>("out");

				// Initialize variables for the model's behaviour

				// Set a value for sigma (so it is not 0), this determines how often the
				// internal transition occurs
				state.sigma = startTime;
				
			}

			/**
			 * The transition function is invoked each time the value of
			 * state.sigma reaches 0.
			 *
			 * @param state reference to the current state of the model.
			 */
			void internalTransition(CoastGuardShipState& state) const override {
				if (!state.arrived){
					state.arrived = true;
				}
				if (!state.stop){
					if (state.unloading){
						state.sigma = 60;
					} else {
						state.sigma = 1440;
					}
					state.unloading = !state.unloading;
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
			void externalTransition(CoastGuardShipState& state, double e) const override {

				// First check if there are un-handled inputs for the "in" port
				if(!in->empty()){

					// The variable x is created to handle the external input values in sequence.
					// The getBag() function is used to get the next input value.
					for( const auto x : in->getBag()){
						state.stop = x;
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
			void output(const CoastGuardShipState& state) const override {
				if (!state.stop){
					out->addMessage(state.unloading);
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
			[[nodiscard]] double timeAdvance(const CoastGuardShipState& state) const override {
				return state.sigma;
			}
	};
	#endif // __COAST_GUARD_SHIP_HPP__
}