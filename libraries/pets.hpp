#pragma once
#include <string>
#include <iostream>
#include "gaming.hpp"

// namespaces are useful when organising large projects and preventing naming conflicts
namespace InteractablesAndObjects{
    class pet
    {   
        protected:
            std::string                     name;
            Gamification::ProgressTracker   Progress;
            Gamification::StatTracker       Stats;

        public:


            pet(std::string name, int health) : name(name), Progress(), Stats()
            {}

            pet() : Progress()
            {
                std::cout << "Please enter the name of the pet: ";
                std::cin >> name;
                Stats.health = 50;
            };

            virtual void weight_lift();
            virtual void jog();
            virtual void run();
            virtual void dressup();
            virtual void feed();
            virtual void checkup();

            virtual void make_sound() = 0;
            virtual void do_trick() = 0;
    }; 
}