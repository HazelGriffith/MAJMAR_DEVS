#ifndef __EVACUATION_SITE_TEST_MODEL_HPP__
#define __EVACUATION_SITE_TEST_MODEL_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <core/modeling/coupled.hpp>
#include <lib/iestream.hpp>
#include <string>

// We include any models that are directly contained within this coupled model
#include "../../coupled_models/EvacuationSiteModel.hpp"

// data structures
#include "../../data_structures/heloInfo.hpp"
#include "../../data_structures/evacInfo.hpp"

using namespace std;

namespace cadmium::assignment1 {
    class evacuation_site_test_model : public Coupled {
        public:
        evacuation_site_test_model(const std::string& id, std::string testNumber): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
			vector<EvacInfo> evacuees;
			int evacID = 1;
			for (int i = 0; i < 25; i++){
				evacuees.push_back(EvacInfo{evacID+i,-1,false,'W'});
				evacID++;
			}
			
			shared_ptr<EvacuationSite> evacuationSite = addComponent<EvacuationSite>("evacuationSite", 15, evacuees);
			
			std::string filepath1 = "../../input_data/evacuee_manager_inEvac_test" + testNumber + "_input.txt";
			std::string filepath2 = "../../input_data/evacuee_manager_inHelo_test" + testNumber + "_input.txt";
			
			auto evacuationSiteInputEvac = addComponent<cadmium::lib::IEStream<EvacInfo>>("EvacueeManagerInputEvac",filepath1.c_str());
			auto evacuationSiteInputHelo = addComponent<cadmium::lib::IEStream<HeloInfo>>("EvacueeManagerInputHelo",filepath2.c_str());
			
			addCoupling(evacuationSiteInputEvac->out, evacuationSite->inEvac);
			addCoupling(evacuationSiteInputHelo->out, evacuationSite->inHelo);
        }
    };
} // namespace cadmium::assignment1

#endif // __EVACUATION_SITE_TEST_MODEL_HPP__
