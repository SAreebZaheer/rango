#include <iostream>
using namespace std;
#include "pets.hpp"
using namespace InteractablesAndObjects;

class activity : public pet
{
    private:
        string act;
    public:

    virtual void weight_lift()
    {
        level = level + 1;
        health = health + 20;
        cout << "The pet has done weightlifting and has gained 20 health points. Congratulations! " << endl;
    }

    virtual void jog()
    {
        health = health + 5;
        cout << "The pet has done weightlifting and has gained 5 health points. Congratulations! " << endl;
    }

    virtual void run()
    {
        level = level + 1;
        health = health + 10;
        cout << "The pet has done running and has gained 10 health points & increased 1 level. Congratulations! " << endl;
    }

    virtual void dressup()
    {
        cout << "The pet has dressed up and has swag. Have fun! " << endl;
    }

    virtual void feed()
    {
        level = level + 1;
        health = health + 5;
        cout << "The pet has eaten its food and has gained 5 health points along with 1 level. Congratulations! " << endl;
    }

    virtual void checkup()
    {
        health = health + 10;
        cout << "The pet has had its checkup and its health is : " << health << endl;
    }

    activity()
    {
        int opt;
            cout << "The following are some actvities you can take part in with your pet. Each activity increases various traits." << endl;
            cout << "Please choose any one actvity." << endl;
            cout << "1. Weight lifting" << endl << "2. Jog" << endl << "3. Run" << endl << "4. Dress Up" << endl;
            cout << "5. Feed" << endl << "6. Checkup" << endl;
            cin >> opt;
            //all activities have effects on health, level, strength etc
                if (opt == 1)
                {
                    this->weight_lift();
                }
                else if (opt == 2)
                {
                    this->jog();
                }
                else if (opt == 3)
                {
                    this->run();
                }
                else if (opt == 4)
                {
                    this->dressup();
                }
                else if (opt == 5)
                {
                   this->feed();
                }
                else if (opt == 6)
                {
                    this->checkup();
                }
    }
};