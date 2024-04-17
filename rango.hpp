#pragma once
#include <iostream> // need this for cin, cout
#include "libraries/pets.hpp" // our pets classes and helpers live here
#include "libraries/users.hpp" // our users classes and helpers live here
#include "libraries/unitTests.hpp" // unit testing lives here
#include "libraries/errorCodes.hpp" // useful error code macros


// namespaces are useful when organising large projects and preventing naming conflicts
namespace RangoAPIServiceEndpoint{
    /**
     * @brief Runs unit tests and returns any relevant information
     * 
     */
    int RunTests()
    {
        try
        {
            // Check if the equality assertion function works
            UnitTesting::Assert_EQ(2, 2, "Unit Test - Equality Assertion Function - Incorrectly Operating");
            UnitTesting::Assert_EQ(2, 2, "Unit Test - Equality Assertion Function - Incorrectly Operating");
            UnitTesting::Assert_EQ(2, 2, "Unit Test - Equality Assertion Function - Incorrectly Operating");
            UnitTesting::Assert_EQ(2, 2, "Unit Test - Equality Assertion Function - Incorrectly Operating");
            UnitTesting::Assert_EQ(2, 2, "Unit Test - Equality Assertion Function - Incorrectly Operating");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return GENERAL_ERROR;
        }
        catch(const UnitTesting::AssertionError& e)
        {
            std::cerr << e.what() << std::endl;
            return TESTING_ERROR;
        }
        catch(...)
        {
            std::cerr << "Unknown error" << std::endl;
            return UNKNOWN_ERROR;
        }

        return NO_ERRORS;
    }
}