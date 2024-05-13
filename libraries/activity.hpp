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

    string pet::make_sound(){
        return "Base pet cannot make a sound :(";
    }
    string pet::do_trick(){
        return "Base pet cannot do tricks :(";
    }

    string bird::make_sound(){
        return "Chirp!";
    }
    string bird::do_trick(){
        return name + " flies through a hoop!";
    }

    string dog::make_sound(){
        return "Bark!";
    }
    string dog::do_trick(){
        return name + " does a backflip!";
    }

    string cat::make_sound(){
        return "Meow!";
    }
    string cat::do_trick(){
        return name + " fell asleep :(";
    }

    string fish::make_sound(){
        return "Click!";
    }
    string fish::do_trick(){
        return name + "does a spin!";
    }


    string interact(pet& A, pet& B) {
        std::cout << "Generic types do not interact :(" << std::endl;
    }
};