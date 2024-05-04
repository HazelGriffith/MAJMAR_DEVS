#ifndef __TOP_MODEL_HPP__
#define __TOP_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <iostream>
#include <string>
#include <cassert>

// We include any models that are directly contained within this coupled model
#include "../coupled_models/HelicopterCoupledModel.hpp"
#include "../coupled_models/EvacueeCoupledModel.hpp"
#include "../coupled_models/EvacuationSiteModel.hpp"
#include "../atomic_models/fol.hpp"

// data structures
#include "../data_structures/heloInfo.hpp"
#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class Top_model : public Coupled {
        public:
        Top_model(const std::string& id): Coupled(id){


            // Declare and initialize all controller models (non-input/output)
			double timeToLoad = 15;
			double shipArrivalTime = 300*60;
			double heloArrivalTime = 2730;
			vector<shared_ptr<HelicopterCoupled>> heloCoupledModels;
			int numOfHelos = 1;
			vector<shared_ptr<EvacueeCoupled>> evacueeCoupledModels;
			vector<EvacInfo> evacuees;
			int numOfWhite = 0;
			int numOfGreen = 475;
			int numOfYellow = 20;
			int numOfRed = 5;
			int numOfBlack = 0;
			int i = 0;
			int initial = i+1;
			for (i = initial; i < initial+numOfWhite; i++){
				string id = to_string(i);
				evacueeCoupledModels.push_back(addComponent<EvacueeCoupled>("evacueeCoupled"+id, i, 'W'));
				evacuees.push_back(EvacInfo{i,-1,false,false,'W'});
			}
			initial = i+1;
			for (i = initial; i < initial+numOfGreen; i++){
				string id = to_string(i);
				evacueeCoupledModels.push_back(addComponent<EvacueeCoupled>("evacueeCoupled"+id, i, 'G'));
				evacuees.push_back(EvacInfo{i,-1,false,false,'G'});
			}
			initial = i+1;
			for (i = initial; i < initial+numOfYellow; i++){
				string id = to_string(i);
				evacueeCoupledModels.push_back(addComponent<EvacueeCoupled>("evacueeCoupled"+id, i, 'Y'));
				evacuees.push_back(EvacInfo{i,-1,false,false,'Y'});
			}
			initial = i+1;
			for (i = initial; i < initial+numOfRed; i++){
				string id = to_string(i);
				evacueeCoupledModels.push_back(addComponent<EvacueeCoupled>("evacueeCoupled"+id, i, 'R'));
				evacuees.push_back(EvacInfo{i,-1,false,false,'R'});
			}
			initial = i+1;
			for (i = initial; i < initial+numOfBlack; i++){
				string id = to_string(i);
				evacueeCoupledModels.push_back(addComponent<EvacueeCoupled>("evacueeCoupled"+id, i, 'B'));
				evacuees.push_back(EvacInfo{i,-1,false,false,'B'});
			}
			
			for (int j = 1; j < numOfHelos+1; j++){
				string id = to_string(j);
				heloCoupledModels.push_back(addComponent<HelicopterCoupled>("helicopterCoupled"+id, j, heloArrivalTime));
				heloArrivalTime += 60;
			}

			shared_ptr<FOL> fol = addComponent<FOL>("FOL");

			shared_ptr<EvacuationSite> evacuationSite = addComponent<EvacuationSite>("EvacuationSite", timeToLoad, evacuees, shipArrivalTime);
			
            // Connect the models with coupling
			
			for (int i = 0; i < evacueeCoupledModels.size(); i++){
				for (int j = 0; j < heloCoupledModels.size(); j++){
					addCoupling(heloCoupledModels[j]->outEvac,evacueeCoupledModels[i]->in);
					addCoupling(evacueeCoupledModels[i]->outHelo,heloCoupledModels[j]->inEvac);
				}
				addCoupling(evacueeCoupledModels[i]->outFOL,fol->in);
				addCoupling(evacueeCoupledModels[i]->outES,evacuationSite->inEvac);
				addCoupling(evacuationSite->outEvac,evacueeCoupledModels[i]->in);
			}
			
			for (int i = 0; i < heloCoupledModels.size(); i++){
				addCoupling(heloCoupledModels[i]->outES,evacuationSite->inHelo);
				addCoupling(evacuationSite->outHelo,heloCoupledModels[i]->inES);
			}
			
        }
    };
} // namespace cadmium::assignment1

#endif // __TOP_MODEL_HPP__
