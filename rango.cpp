/**
 * PROJECT RANGO v0.1.0-alpha
 * 
 * This file contains the main implementation of our front-end
*/

#include "rango.hpp" // all dependencies for project rango live here

using namespace std; // we won't need to repeatedly do "std::cin, std::cout"
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
    ExecutionStatus = RAISE::RunTests();

    // initialise
    cout << "Welcome to CS112 PROJECT: RANGO" << endl;
    cout << "----------------------------------------" << endl;
    SOCKET listeningSocket;
    try{
        listeningSocket = HTTP::InitialiseHTTPServer();
        cout << "Server is now listening for active connections on port 80" << endl;
        HTTP::HandleConnections(listeningSocket);
    }
    catch (const HTTP::ServiceUnavailableException& E)
    {
        cout << E.HTTPDetailedWhat() << endl;
        WSACleanup(); // It is a good practice to cleanup the WinSock API if we are unable to initialise the server
        return E.getCode();// If the server did not initialise, there is no point, so we should 
    }
    catch (const HTTP::GatewayTimeoutException& E)
    {
		cout << E.HTTPDetailedWhat() << endl;
	}
    /*
    Disable the catch all for testing so we can see the exact errors when they come
    catch (...)
    {
		cout << "An unknown error occurred while trying to initialise the server" << endl;
        WSACleanup(); // It is a good practice to cleanup the WinSock API if we are unable to initialise the server
		return -1;
	}
    */

    closesocket(listeningSocket);


    return ExecutionStatus;
}