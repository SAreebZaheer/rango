#pragma once
#include <string>
#include <iostream>
#include "gaming.hpp"

// namespaces are useful when organising large projects and preventing naming conflicts
namespace InteractablesAndObjects{
    class pet
    {   
        protected:
            string                          name;
            Gamification::ProgressTracker   Progress;
            Gamification::StatTracker       Stats;

        public:
            pet(string name) : name(name), Progress(), Stats()
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

            virtual string make_sound();
            virtual string do_trick();

            friend class user;
    }; 

    class bird : public pet {
        public:
		string make_sound() override;
        string do_trick() override;
        string fly(){
            return name + " flies";
        }

        bird(string name) : pet(name) {};
	};

    class dog : public pet {
        public:
        string make_sound() override;
        string do_trick() override;
        string guard(){
            return name + " stands on guard";
        }

        dog(string name) : pet(name) {};
    };

    class cat : public pet {
        public:
        string make_sound() override;
        string do_trick() override;
        string play() {
            return name + " jumps around";
        }

        cat(string name) : pet(name) {};
    };

    class fish: public pet {
        public:
        string make_sound() override;
        string do_trick() override;
        string swim() {
            return name + " swims";
        }

        fish(string name) : pet(name) {};
    };
}