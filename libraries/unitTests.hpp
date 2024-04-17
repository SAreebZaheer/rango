/**
 * UNIT TESTS v0.1.0-alpha
 * 
 * Inspired by gtest
*/
#pragma once
#include <string>

namespace UnitTesting{

    enum AssertionType{
        AT_General = 0,
        AT_Equality
    };

    struct AssertionError
    {
        std::string Message;

        
    };

    template <typename T>
    void Assert_EQ(const T& a, const T& b)
    {
        if(a == b) { return; } // expected case
        
        throw new AssertionError e; 
    }

}