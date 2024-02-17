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
#include "../atomic_models/evacuee.hpp"
#include "../atomic_models/evacueeManager.hpp"
#include "../atomic_models/filterES.hpp"
#include "../atomic_models/filterEvac.hpp"
#include "../atomic_models/filterLoc.hpp"
#include "../atomic_models/fol.hpp"
#include "../atomic_models/helicopter.hpp"
#include "../atomic_models/helicopterQueue.hpp"
#include "../atomic_models/helipadManager.hpp"

// data structures
#include "../data_structures/heloInfo.hpp"
#include "../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class Top_model : public Coupled {
        public:
        Top_model(const std::string& id): Coupled(id){


            // Declare and initialize all controller models (non-input/output)
			vector<shared_ptr<Evacuee>> evacueeModels;
			vector<shared_ptr<FilterLoc>> filterLocs;
			vector<EvacInfo> evacuees;
			int i = 0;
			int initial = i+1;
			for (i = initial; i < initial+10; i++){
				string id = to_string(i);
				evacueeModels.push_back(addComponent<Evacuee>("evacuee"+id, i, 'W'));

				evacuees.push_back(EvacInfo{i,-1,false,'W'});

				filterLocs.push_back(addComponent<FilterLoc>("filterLoc"+id, i));

			}
			initial = i+1;
			cout << "FIRES" << endl;
			for (i = initial; i < initial+10; i++){
				
				string id = to_string(i);

				evacueeModels.push_back(addComponent<Evacuee>("evacuee"+id, i, 'G'));

				evacuees.push_back(EvacInfo{i,-1,false,'G'});
				
				filterLocs.push_back(addComponent<FilterLoc>("filterLoc"+id, i));
				
			}
			initial = i+1;
			cout << "FIRES" << endl;
			for (i = initial; i < initial+5; i++){
				
				string id = to_string(i);

				evacueeModels.push_back(addComponent<Evacuee>("evacuee"+id, i, 'Y'));

				evacuees.push_back(EvacInfo{i,-1,false,'Y'});

				filterLocs.push_back(addComponent<FilterLoc>("filterLoc"+id, i));


			}
			initial = i+1;
			cout << "FIRES" << endl;
			for (i = initial; i < initial+5; i++){

				string id = to_string(i);

				evacueeModels.push_back(addComponent<Evacuee>("evacuee"+id, i, 'R'));

				evacuees.push_back(EvacInfo{i,-1,false,'R'});

				filterLocs.push_back(addComponent<FilterLoc>("filterLoc"+id, i));

			}
			initial = i+1;
			cout << "FIRES" << endl;
			for (i = initial; i < initial+1; i++){
				string id = to_string(i);

				evacueeModels.push_back(addComponent<Evacuee>("evacuee"+id, i, 'B'));

				evacuees.push_back(EvacInfo{i,-1,false,'B'});

				filterLocs.push_back(addComponent<FilterLoc>("filterLoc"+id, i));

			}
			cout << "FIRES" << endl;
			shared_ptr<Helicopter> helo1 = addComponent<Helicopter>("helicopter1", 1);

			shared_ptr<Helicopter> helo2 = addComponent<Helicopter>("helicopter2", 2);

			shared_ptr<FilterES> filterES1 = addComponent<FilterES>("filterES1", 1);

			shared_ptr<FilterES> filterES2 = addComponent<FilterES>("filterES2", 2);

			shared_ptr<FilterEvac> filterEvac1 = addComponent<FilterEvac>("filterEvac1", 1);

			shared_ptr<FilterEvac> filterEvac2 = addComponent<FilterEvac>("filterEvac2", 2);

			shared_ptr<FOL> fol = addComponent<FOL>("FOL");

			shared_ptr<HelipadManager> helipadManager = addComponent<HelipadManager>("helipadManager", 15.0);

			shared_ptr<HelicopterQueue> helicopterQueue = addComponent<HelicopterQueue>("helicopterQueue");

			shared_ptr<EvacueeManager> evacueeManager = addComponent<EvacueeManager>("evacueeManager", evacuees);
			cout << "FIRES" << endl;
			
            // Declare and initialize all simulated input files (these must exist in the file system before compilation)

            // Connect the input files to the rest of the simulation with coupling
			
			for (int i = 0; i < evacueeModels.size(); i++){
				addCoupling(filterLocs[i]->out,evacueeModels[i]->in);
				addCoupling(helo1->outEvac,filterLocs[i]->in);
				addCoupling(helo2->outEvac,filterLocs[i]->in);
				addCoupling(evacueeManager->outEvac,filterLocs[i]->in);
				addCoupling(evacueeModels[i]->outHelo,filterEvac1->in);
				addCoupling(evacueeModels[i]->outHelo,filterEvac2->in);
				addCoupling(evacueeModels[i]->outFOL,fol->in);
				addCoupling(evacueeModels[i]->outES,evacueeManager->inEvac);
			}
			cout << "FIRES" << endl;
			addCoupling(helo1->outES,helicopterQueue->inHelo);
			addCoupling(helo2->outES,helicopterQueue->inHelo);
			addCoupling(filterES1->out,helo1->inES);
			addCoupling(filterES2->out,helo2->inES);
			addCoupling(filterEvac1->out,helo1->inEvac);
			addCoupling(filterEvac2->out,helo2->inEvac);
			
			addCoupling(helicopterQueue->outHelo,filterES1->in);
			addCoupling(helicopterQueue->outHelo,filterES2->in);
			addCoupling(helicopterQueue->outHM,helipadManager->inHQ);
			
			addCoupling(helipadManager->outHelo,filterES1->in);
			addCoupling(helipadManager->outHelo,filterES2->in);
			addCoupling(helipadManager->outHQ,helicopterQueue->inHM);
			addCoupling(helipadManager->outEM,evacueeManager->inHelo);
			
			addCoupling(evacueeManager->outHM,helipadManager->inEM);
			
        }
    };
} // namespace cadmium::assignment1

#endif // __TOP_MODEL_HPP__
