#include <iostream>
using namespace std;

class item : public pet
{
    private:
        string thing[4];
    public:
        item()
        {
            int opt;
            cout << "The following are some items you can use for your pet. Please choose any 4." << endl;
            cout << "1. Toy Ball" << endl << "2. Chewtoy" << endl << "3. Biscuits" << endl << "4. Scooby Snacks" << endl;
            cout << "5. Collar" << endl << "6. Blanket" << endl <<  "7. Boots" << endl << "8. Laserpointer" << endl << "9. Brush" << endl;
            for (int i = 0; i<4 ; ++i)
            {
                cin >> opt;
                if (opt == 1)
                {
                    thing[i] = "Toy Ball";
                }
                else if (opt == 2)
                {
                    thing[i] = "Chewtoy";
                }
                else if (opt == 3)
                {
                    thing[i] = "Biscuits";
                }
                else if (opt == 4)
                {
                    thing[i] = "Scooby Snacks";
                }
                else if (opt == 5)
                {
                    thing[i] = "Collar";
                }
                else if (opt == 6)
                {
                    thing[i] = "Blanket";
                }
                else if (opt == 7)
                {
                    thing[i] = "Boots";
                }
                else if (opt == 8)
                {
                    thing[i] = "Laserpointer";
                }
                else if (opt == 9)
                {
                    thing[i] = "Brush";
                }
            }
        }

        //use derived classes and suggestion given so that items chosen have an effect on pet health, level & strength

};
