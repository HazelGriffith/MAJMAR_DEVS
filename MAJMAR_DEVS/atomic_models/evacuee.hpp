#ifndef __EVACUEE_HPP__
#define __EVACUEE_HPP__

#include <core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include <chrono>
#include <unordered_map>


#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
	
	enum triage_category{
		WHITE,
		GREEN,
		YELLOW,
		RED,
		BLACK
	};
	
	inline triage_category operator++ (triage_category& tc){
		tc = static_cast<triage_category>(abs(static_cast<int>(tc) + 1) % 5);
		return tc;
	}
	
	inline triage_category operator-- (triage_category& tc){
		tc = static_cast<triage_category>(abs(static_cast<int>(tc) - 1) % 5);
		return tc;
	}
	
	struct transition_time_key{
		bool multiOrSingle;
		bool EorC;
		bool changingTS;
		triage_category TS;
	};
	
	struct KeyHash{
		size_t operator()(const transition_time_key& k) const{
			return ((((hash<bool>()(k.multiOrSingle)
						^ (hash<bool>()(k.EorC) << 1)) >> 1)
						^ (hash<bool>()(k.changingTS) << 1)) >> 1)
						^ (hash<int>()(k.TS) << 1);
		}
	};
	
	struct KeyEqual{
		bool operator ()(const transition_time_key& lhs, const transition_time_key& rhs) const {
			if (lhs.multiOrSingle != rhs.multiOrSingle){
				return false;
			} else if (lhs.EorC != rhs.EorC){
				return false;
			} else if (lhs.changingTS != rhs.changingTS){
				return false;
			} else if (lhs.TS != rhs.TS){
				return false;
			} else {
				return true;
			}
		}
	};
	
	// A class to represent the state of this specific model
	// All atomic models will have their own state
	struct EvacueeState {

		// sigma is a mandatory variable, used to advance the time of the simulation
		double sigma;

		// Declare model-specific variables
		triage_category triage_status;
		vector<char> triage_categories;
		char curr_loc;
		int heloID;
		int evacueeID;
		bool travelling;
		bool start;
		bool firstTimeOnShip;
		
		//triage_status can be {'W' = "White", 'G' = "Green", 'Y' = "Yellow", 'R' = "Red", 'B' = "Black"}
		
		//curr_loc can be {'E' = "Evacuation Site", 'H' = "Helicopter", 'F' = "FOL", 'C' = "Coast Guard Ship"}
		// Set the default values for the state constructor for this specific model
		EvacueeState(): sigma(0), evacueeID(-1), triage_status(WHITE), triage_categories({'W', 'G', 'Y', 'R', 'B'}), 
		curr_loc('E'), heloID(-1), travelling(false), start(true), firstTimeOnShip(false){};
	};

	std::ostream& operator<<(std::ostream &out, const EvacueeState& state) {
		out << "Evacuee;" << state.evacueeID << ";in triage state;" << state.triage_categories[state.triage_status];
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
			double m_wTog = 120*60;
			double m_gToy = 48*60;
			double m_yTor = 8*60;
			double m_rTob = 1.5*60;
			double m_rToy = m_wTog;
			double m_yTog = 72*60;
			double m_gTow = m_gToy;
			bool multiOrSingleState;
			
			vector<float> transition_times = {7200,2880,480,90,7200,4320,2880};
			
			unordered_map<transition_time_key, double, KeyHash, KeyEqual> transition_time_map;
			
			/**
			 * Constructor function for this atomic model, and its respective state object.
			 *
			 * For this model, both a Evacuee object and a Evacuee object
			 * are created, using the same id.
			 *
			 * @param id ID of the new Evacuee model object, will be used to identify results on the output file
			 */
			Evacuee(const string& id, int i_evacueeID, char i_triage_status, bool i_multiOrSingleState): Atomic<EvacueeState>(id, EvacueeState()) {

				// Initialize ports for the model

				// Input Ports
				in  = addInPort<EvacInfo>("in");

				// Output Ports
				outHelo = addOutPort<EvacInfo>("outHelo");
				outFOL = addOutPort<EvacInfo>("outFOL");
				outES = addOutPort<EvacInfo>("outES");

				// Initialize variables for the model's behaviour
				
				state.evacueeID = i_evacueeID;
				
				switch(i_triage_status){
					case('W'):
						state.triage_status = WHITE;
						break;
					case('G'):
						state.triage_status = GREEN;
						break;
					case('Y'):
						state.triage_status = YELLOW;
						break;
					case('R'):
						state.triage_status = RED;
						break;
					case('B'):
						state.triage_status = BLACK;
						break;
					default:
						assert(("The char is not a valid triage_status", false));
						break;
				}
				
				transition_time_map = {{{true, true, true, WHITE}, m_wTog},
									{{true, true, true, GREEN}, m_gToy},
									{{true, true, true, YELLOW}, m_yTor},
									{{true, true, true, RED}, m_rTob},
									{{true, true, true, BLACK}, numeric_limits<double>::infinity()},
									{{true, true, false, WHITE}, m_gToy},
									{{true, true, false, GREEN}, m_yTor},
									{{true, true, false, YELLOW}, m_rTob},
									{{true, true, false, RED}, numeric_limits<double>::infinity()},
									{{true, false, true, WHITE}, numeric_limits<double>::infinity()},
									{{true, false, true, GREEN}, m_gTow},
									{{true, false, true, YELLOW}, m_yTog},
									{{true, false, true, RED}, m_rToy},
									{{true, false, false, GREEN}, numeric_limits<double>::infinity()},
									{{true, false, false, YELLOW}, m_gTow},
									{{true, false, false, RED}, m_yTog}};
				//Determines whether all evacuees share same transition times or not
				multiOrSingleState = i_multiOrSingleState;
				if (!multiOrSingleState){
					
					
					//handle init here
					//Generating? Evac site? Not Changing TS?
					transition_time_map.insert({{{false, true, true, WHITE}, transition_times[0]},
									{{false, true, true, GREEN}, transition_times[1]},
									{{false, true, true, YELLOW}, transition_times[2]},
									{{false, true, true, RED}, transition_times[3]},
									{{false, true, false, WHITE}, transition_times[1]},
									{{false, true, false, GREEN}, transition_times[2]},
									{{false, true, false, YELLOW}, transition_times[3]},
									{{false, true, false, RED}, numeric_limits<double>::infinity()},
									{{false, false, true, WHITE}, numeric_limits<double>::infinity()},
									{{false, false, true, GREEN}, transition_times[6]},
									{{false, false, true, YELLOW}, transition_times[5]},
									{{false, false, true, RED}, transition_times[4]},
									{{false, false, false, GREEN}, numeric_limits<double>::infinity()},
									{{false, false, false, YELLOW}, transition_times[6]},
									{{false, false, false, RED}, transition_times[5]}});
					
					state.start = false;
					transition_time_key key = {false, true, true, state.triage_status};
					double time;
					try {
						time = transition_time_map.at(key);
					} catch(const out_of_range& e){
						assert(("Tried to access non-existent key", false));
					}
					state.sigma = time;
					
				} else {
					state.sigma = 0;
				}
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
				
				unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
				minstd_rand0 generator(seed1);
				
				if (multiOrSingleState){
					if (state.curr_loc == 'E'){
						if ((!state.start)&&(state.triage_status == BLACK)){
							assert(("Cannot change triage state past Black at ES", false));
						}
						transition_time_key key = {true, true, state.start, state.triage_status};
						double mean;
						try {
							mean = transition_time_map.at(key);
						} catch(const out_of_range& e){
							assert(("Tried to access non-existent key", false));
						}
						exponential_distribution<float> time_distribution{float(1.0/mean)};
						if (!state.start){
							++state.triage_status;
						} else {
							state.start = false;
						}
						state.sigma = (double) time_distribution(generator);
					} else if (state.curr_loc == 'C'){
						if (state.triage_status == BLACK){
							assert(("Cannot have triage status B on CGS", false));
						}
						if ((state.triage_status == WHITE)&&(!state.firstTimeOnShip)){
							assert(("Cannot improve health beyond W", false));
						}
						transition_time_key key = {true, false, state.firstTimeOnShip, state.triage_status};
						if (!state.firstTimeOnShip){
							--state.triage_status;
						} else {
							state.firstTimeOnShip = false;
						}
						double mean;
						try {
							mean = transition_time_map.at(key);
						} catch(const out_of_range& e){
							assert(("Tried to access non-existent key", false));
						}
						exponential_distribution<float> time_distribution{float(1.0/mean)};
						state.sigma = (double) time_distribution(generator);
					} else {
						state.sigma = numeric_limits<double>::infinity();
					}
				} else {
					if (state.curr_loc == 'E'){
						if (state.start){
							state.start = false;
							transition_time_key key = {true, true, state.start, state.triage_status};
							double mean;
							try {
								mean = transition_time_map.at(key);
							} catch(const out_of_range& e){
								assert(("Tried to access non-existent key", false));
							}
							exponential_distribution<float> time_distribution{float(1.0/mean)};
							state.sigma = (double) time_distribution(generator);
						} else {
							if (state.triage_status == BLACK){
								assert(("Cannot change triage state past Black at ES", false));
							}
							transition_time_key key = {false, true, state.start, state.triage_status};
							++state.triage_status;
							double time;
							try {
								time = transition_time_map.at(key);
							} catch(const out_of_range& e){
								assert(("Tried to access non-existent key", false));
							}
							state.sigma = time;
						}
					} else if (state.curr_loc == 'C'){
						if (state.triage_status == BLACK){
							assert(("Cannot have triagestatus B on CGS", false));
						}
						if ((state.triage_status == WHITE)&&(!state.firstTimeOnShip)){
							assert(("Cannot improve health beyond W", false));
						}
						transition_time_key key = {false, false, state.firstTimeOnShip, state.triage_status};
						if (!state.firstTimeOnShip){
							--state.triage_status;
						} else {
							state.firstTimeOnShip = false;
						}
						double time;
						try {
							time = transition_time_map.at(key);
						} catch(const out_of_range& e){
							assert(("Tried to access non-existent key", false));
						}
						state.sigma = time;
					} else {
						state.sigma = numeric_limits<double>::infinity();
					}
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
							outHelo->addMessage(EvacInfo{state.evacueeID, state.heloID, false, true, state.triage_categories[state.triage_status]});
							break;
						case('F'):
							outFOL->addMessage(EvacInfo{state.evacueeID, state.heloID, false, false, state.triage_categories[state.triage_status]});
							break;
						default:
							assert(("The curr_loc and travelling are not lining up correctly", false));
							break;
					}
				} else if ((!state.start)&&(!state.firstTimeOnShip)){
					switch(state.curr_loc){
						case('E'):
							outES->addMessage(EvacInfo{state.evacueeID, state.heloID, false, false, state.triage_categories[state.triage_status]});
							break;
						case('C'):
							outES->addMessage(EvacInfo(state.evacueeID, state.heloID, true, true, state.triage_categories[state.triage_status]));
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