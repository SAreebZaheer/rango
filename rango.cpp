/**
 * PROJECT RANGO v0.1.0-alpha
 * 
 * This file contains the main implementation of our front-end
*/

#include "rango.hpp" // all dependencies for project rango live here

using namespace std; // we wont need to repeatedly do "std::cin, std::cout"
namespace RAISE = RangoAPIServiceEndpoint; // writing Rango API Service Endpoint is long and cumbersome 
namespace UI = UserInterfaces; // same logic as RAISE, we want to shorten namespaces
namespace IAO = InteractablesAndObjects; // same logic as RAISE, we want to shorten namespaces

/**
 * @brief 
 * 
 * @return Error Code (int) 
 */
int main()
{
    int ExecutionStatus = NO_ERRORS;
    RAISE::RunTests();

    // initialise
    cout << "Welcome to CS112 PROJECT: RANGO";

    return ExecutionStatus;
}