#include <caf/test/caf_test_main.hpp>
#include <caf/test/test.hpp>

#include <iostream>

#include "WorkflowActor.hpp"

caf::behavior mock_actor_impl()
{
	return ([](caf::get_atom) { std::cout << "" << std::endl; });
}

TEST("a simple test")
{
	check_eq(1, 1);
}

CAF_TEST_MAIN()
