#pragma once
#include <string>
#include <iostream>

// namespaces are useful when organising large projects and preventing naming conflicts
namespace InteractablesAndObjects{
    class pet
    {
        private:
            std::string name;
            int level;
            int health;
            std::string type;

        public:
            pet(std::string n, int l, int h, std::string t) : name(n), level(l), health(h), type(t)
            {}

            pet()
            {
                std::cout << "Please enter the name of the pet: ";
                std::cin >> name;
                std::cout << "Please enter the type of the pet (cat,dog,bird etc.): ";
                std::cin >> type;
                level = 0;
                health = 50;
            }
    };
}