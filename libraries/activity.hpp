#include <iostream>
using namespace std;
#include "pets.hpp"
using namespace InteractablesAndObjects;

class activity : public pet
{
    private:
        string act;
    public:
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
                    //pet does weight lifting
                }
                else if (opt == 2)
                {
                    // pet jogs
                }
                else if (opt == 3)
                {
                    //pet runs
                }
                else if (opt == 4)
                {
                    //pet gets dressed up with collar etc
                }
                else if (opt == 5)
                {
                   //pet eats treats etc
                }
                else if (opt == 6)
                {
                    //pet gets checkup aka reveals health, level, strength etc
                }
       }
};