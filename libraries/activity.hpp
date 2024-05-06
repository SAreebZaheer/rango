#include <iostream>
using namespace std;
#include "pets.hpp"
namespace InteractablesAndObjects{
    void pet::weight_lift()
    {
        Stats.health += 20;
        Stats.buffness++;
    }

    void pet::jog()
    {
        Stats.health += 5;
        Stats.stamina++;
    }

    void pet::run()
    {
        Stats.health += 10;
        Stats.stamina += 2;
    }

    void pet::dressup()
    {
        Stats.morale++;
    }

    void pet::feed()
    {
        Stats.hunger = 0;
        Stats.health += 5;
    }

    void pet::checkup()
    {
        Stats.health += 10;
    }

    string bird::make_sound(){
        return "Chirp!";
    }
    string bird::do_trick(){
        return "";
    }
};