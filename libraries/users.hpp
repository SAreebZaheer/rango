#pragma once
#include <string>
#include "pets.hpp"
using namespace InteractablesAndObjects;

// namespaces are useful when organising large projects and preventing naming conflicts
namespace UserInterfaces{
    class user 
    {
        private:
            pet p;
            std::string name;
            std::string age;
            std::string email;
            std::string password;
        public:

        user()
        {
            std::cout << "Please enter the name of the user: " << std::endl;
            std::cin >> name;
            std::cout << "Please enter the age of the user: " << std::endl;
            std::cin >> age;
            std::cout << "Please enter the email of the user: " << std::endl;
            std::cin >> email;
            std::cout << "Please enter the password of the user: " << std::endl;
            std::cin >> password;
        }
        
        void shareprogress(user u1, pet p1)
        {
            std::cout << "The user " << u1.name << " with email " << u1.email << " owns the pet with the following details:" << std::endl;
            std::cout << "Pet name: " << p1.name << std::endl << "Pet level: " <<p1.level << std::endl;
            std::cout << "Pet health: " << p1.Stats.health << std::endl << "Pet Buffness: " << p1.Stats.buffness << std::endl << "Pet stamina: " << p1.Stats.stamina <<std::endl;
            std::cout << "Pet Morale: " << p1.Stats.morale << std::endl << "Pet hunger: " << p1.Stats.hunger << std::endl;
        }

        void event(user u1, pet p1)
        {
            std::cout << "WELCOME TO THE GRAND COMPETITION FOR PETS ONLY! (sponsored by Shandar Mobiles)" << std::endl << "Get ready soldier." << std::endl;
            if (p1.Stats.buffness > 2)
            {
                std::cout << "Your pet is elegible for the competition! Congratulations!" << std::endl;
            }
            if(p1.Stats.buffness > 10)
            {
                std::cout << "You are about to battle the MIGHTY SHERA! Good luck..." << std::endl;
                std::cout << "Your pet suffered a nasty blow to the nose, but DEFEATED SHERAAAAAAAA!!!!! Iss khushi mein ROTI KHOL DO SHAFIQUE AHMED!" << std::endl;
                std::cout << "Health, buffness, and stamina have all decreased by 2. " << std::endl << "Level has increase by 6!" << std::endl;
                p1.Stats.health = p1.Stats.health - 2;
                p1.Stats.buffness = p1.Stats.buffness - 2;
                p1.Stats.stamina = p1.Stats.stamina - 2;
                p1.level = p1.level + 6;
            }
            else
            {
                std::cout << "You are about to battle the MIGHTY SHERA! Good luck..." << std::endl;
                std::cout << "Your pet suffered a nasty blow to the nose, and Shera has also gotten the better of you. You have lost. " << std::endl;
                std::cout << "Health, buffness, and stamina have all decreased by 2. " << std::endl << "Level has also decreased by 2!" << std::endl;
                p1.Stats.health = p1.Stats.health - 2;
                p1.Stats.buffness = p1.Stats.buffness - 2;
                p1.Stats.stamina = p1.Stats.stamina - 2;
                p1.level = p1.level - 2;
            }
        }
    };
}