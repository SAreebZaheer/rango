#include <iostream>
using namespace std;
#include "pets.hpp"
using namespace InteractablesAndObjects;

class environment : public pet
{
    private:
        string env;
        int temp;
    public:
        environment() //default constructor to set an environment and temperature relative to environment
        {
            int opt;
            do 
            {
            cout << "Please enter the type of environment you would like for your pet (ENTER A NUMBER FROM 1-6) :" << endl;
            cout << "1. Snow" << endl << "2. Desert" << endl << "3. Jungle" << endl << "4. Beach" << "5. Park"  << "6. Space" << endl;
            cin >> opt;
            } while (opt != 1 or opt != 2 or opt != 3 or opt != 4 or opt != 5 or opt != 6);
            if (opt == 1)
            {
                env = "Snow";
                temp = -10;
                level = 5;          //error on purpose
            }
            else if (opt == 2)
            {
                env = "Desert";
                temp = 55;
                level = 5;          //error on purpose
            }
            else if (opt == 3)
            {
                env = "Jungle";
                temp = 25;
                level = 2;          //error on purpose
            }
            else if (opt == 4)
            {
                env = "Beach";
                temp = 40;
                level = 4;
            }
            else if (opt == 5)
            {
                env = "Park";
                temp = 20;
                level = 2;
            }
            else if (opt == 6)
            {
                env = "Space";
                temp = -100;
                level = 8;
            }
        }

 //use derived classes so that temperature can have an effect on pet health

};