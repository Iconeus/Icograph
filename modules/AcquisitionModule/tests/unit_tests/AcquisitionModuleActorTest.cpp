#include <caf/actor_from_state.hpp>
#include <caf/test/caf_test_main.hpp>
#include <caf/test/test.hpp>
#include "caf/test/fixture/deterministic.hpp"

#include <iostream>

#include "AcquisitionModuleActor.hpp"

/**
 * \struct Definition of the mock-up to simulate the recepting actor
//  */
// struct mock_actor_state
// {
// 	/**
// 	 * @brief: Defines the callbacks upon message reception
// 	 */
// 	domain_model::domain_model_actor::behavior_type make_behavior()
// 	{
// 		return {[this](caf::publish_atom, int x) { _dataReceived.push_back(x); }};
// 	};

// 	std::vector<int> getReceivedData() { return _dataReceived; };

// private:
// 	std::vector<int> _dataReceived;
// };

TEST("a simple test")
{
	check_eq(1, 1);
	// auto mockDomainModelActor = sys.spawn(caf::actor_from_state<mock_actor_state>);
	// auto mockEchoViewerActor = sys.spawn(caf::actor_from_state<mock_actor_state>);

	// // Spawn acquisition module actor.
	// auto acquisitionModuleActorHandle =
	//     sys->spawn(acq_module::acquisition_actor_behavior);
}

CAF_TEST_MAIN()
