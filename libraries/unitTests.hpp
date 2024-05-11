#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-exception-baseclass"
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
    const std::string AssertionTypeNames[2] = {"General", "Equality"};

    struct AssertionError
    {
        std::string Message;
        AssertionType Type;

        AssertionError() : Message(), Type(AT_General) {};
        //AssertionError(const AssertionType& AT): Message(), Type(AT) {};
        //AssertionError(const std::string& Message) : Message(Message), Type(AT_General) {};
        AssertionError(std::string  Message, AssertionType AT) : Message(std::move(Message)), Type(AT) {}; // std::move changes the pointer of Message

        std::string what() const {
            return AssertionTypeNames[Type] + " Assertion Error (CODE: " + std::to_string(Type) + ") with Message: " + Message;
        }
    };

    template <typename T>
    void Assert_EQ(const T& a, const T& b, const std::string& MessageIfError = "")
    {
        if(a == b) { return; } // expected case
        
        throw AssertionError(MessageIfError, AT_Equality);
    }

}