#include "Car.h"

Car::Car(int x, int y, char direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    present = true;
}

Car::Car(Car &n) {
    x = n.x;
    y = n.y;
    direction = n.direction;
    present = n.present;
}

unsigned long Car::getKey() {
    return 10000*x + y*100 + direction;
}

int Car::getX() {
    return x;
}

int Car::getY() {
    return y;
}

void Car::setX(int t) {
    x = t;
}

void Car::setY(int t) {
    y = t;
}

void Car::disappear() {
    present = false;
}

bool Car::isPresent() {
    return present;
}

char Car::getDirection() {
    return direction;
}



string Car::toString() {
    string output;
    string coordinates = "("+to_string(getX())+","+to_string(getY())+")";
    if (direction) output = "E/W car at ";
    else output = "N/S car at ";
    return output + coordinates;
}