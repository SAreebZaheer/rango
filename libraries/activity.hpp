#include <iostream>
using namespace std;
#include "pets.hpp"
namespace InteractablesAndObjects{
    void pet::weight_lift()
    {
        Stats.health += 20;
        Stats.buffness++;
        cout << "The pet has done weightlifting and has gained buffness and 20 health points. Congratulations! " << endl;
    }

    void pet::jog()
    {
        Stats.health += 5;
        Stats.stamina++;
        cout << "The pet has jogged, gaining 5 health and 1 stamina. Congratulations! " << endl;
    }

    void pet::run()
    {
        Stats.health += 10;
        Stats.stamina += 2;
        cout << "The pet has done running and has gained 10 health points and gained 2 stamina. Congratulations! " << endl;
    }

    void pet::dressup()
    {
        Stats.morale++;
        cout << "The pet has dressed up and has swag, this is good for morale. Have fun! " << endl;
    }

    void pet::feed()
    {
        Stats.hunger = 0;
        Stats.health += 5;
        cout << "The pet has eaten its food and has gained 5 health points and hunger has been reset. Congratulations! " << endl;
    }

    void pet::checkup()
    {
        Stats.health += 10;
        cout << "The pet has had its checkup and its health is : " << Stats.health << endl;
    }
};