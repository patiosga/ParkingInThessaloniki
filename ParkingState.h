#ifndef PARKINGSTATE_H
#define PARKINGSTATE_H
#define WIDTH 4
#define HEIGHT 5
#include <string>
#include <sstream>
#include <vector>
#include "Car.h"

using namespace std;

class ParkingState {
private:
    char objectMap[WIDTH][HEIGHT]; //0 τίποτα, 1 εμπόδιο, 2 Β/Ν αυτοκίνητο, 3 Α/Δ αυτοκίνητο
    string actionName;
    ParkingState *prev;
    vector<Car*> cars;

public:
    ParkingState();
    ParkingState(ParkingState &n);
    void removeObject(int i, int j);
    void addObject(int i, int j, char value);
    bool hasObject(int x, int y);
    bool addCar(Car *a);

    void setPrevious(ParkingState *p) {prev = p;}
    ParkingState *getPrevious() {return prev;}

    void setActionName(string s) {actionName=s;}
    string getActionName() {return actionName;}

    int getDepth();
    unsigned long getKey() const;

    bool CarGoUp(ParkingState &n, Car *car, int carPos);
    bool CarGoDown(ParkingState &n, Car *car, int carPos);
    bool CarGoLeft(ParkingState &n, Car *car, int carPos);
    bool CarGoRight(ParkingState &n, Car *car, int carPos);
    vector <ParkingState *> expand();

   void printSolution();
   void screenshot();
};


#endif 
