#ifndef CAR_H
#define CAR_H
#include <string>
using namespace std;

class Car {
private:
    int x;
    int y;
    char direction; //0 για Β/N, 1 για A/Δ (!!!)
    bool present;
public:
    Car(int x, int y, char direction);
    Car(Car &n);
    unsigned long getKey();
    int getX();
    int getY();
    void setX(int);
    void setY(int);
    char getDirection();
    void disappear();
    bool isPresent();
    string toString ();
};


#endif
