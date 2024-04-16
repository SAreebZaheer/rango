#include <iostream>
#include <string>
using namespace std;

class pet
{
    private:
        string name;
        int level;
        int health;
        string type;

    public:
        pet(){}

        pet(string n, int l, int h, string t) : name(n), level(l), health(h), type(t)
        {}

        pet()
        {
            cout << "Please enter the name of the pet: ";
            cin >> name;
            cout << "Please enter the type of the pet (cat,dog,bird etc.): ";
            cin >> type;
            level = 0;
            health = 50;
        }
};

class user
{
    private:
        string name;
        string address;
        string age;
        string email;
        string password;
        
};

int main()
{
    
}