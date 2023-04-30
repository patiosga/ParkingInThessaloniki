#include "ParkingState.h"
#include <iostream>
 using namespace std;

ParkingState::ParkingState() {
    for (int i=0;i<WIDTH;i++)
        for (int j=0;j<HEIGHT;j++)
            objectMap[i][j] = 0;
    setPrevious(nullptr);
    setActionName("");
}

ParkingState::ParkingState(ParkingState &ancestor) {
    for (int i=0;i<WIDTH;i++) {
        for (int j = 0; j < HEIGHT; j++) {
            objectMap[i][j] = ancestor.objectMap[i][j];
        }
    }
//    setPrevious(nullptr); //Αρχικοποιούνται μέσα στις συναρτήσεις τύπου CarGo
//    setActionName("");
    int numOfCars = ancestor.cars.size();
    for (int i=0; i<numOfCars; i++) {
        cars.push_back(ancestor.cars.at(i));
    }
    //εδώ οι δείκτες για τα αυτοκίνητα στη νέα κατάσταση δείχνουν στις
    // ίδιες θέσεις μνήμης και αλλάζει μετά μόνο ο δείκτης που δείχνει στο αυτοκίνητο που μετακινήθηκε
}


void ParkingState::removeObject(int i, int j) {
    objectMap[i][j] = 0;
}

void ParkingState::addObject(int i, int j, char value) { //value ανάλογα με τον τύπο του αντικειμένου
    objectMap[i][j] = value;
}

bool ParkingState::hasObject(int i, int j) {
    return (objectMap[i][j] != 0 );
}

bool ParkingState::addCar(Car *a) {
    int x,y;
    x = a->getX();
    y = a->getY();
    if (hasObject(x,y))
        return false;
    cars.push_back(a);
    addObject(x,y,2+a->getDirection()); // 2 αν direction = 0 (B/N) ,  3 αν direction = 1 (Α/Δ)
    return true;
}

int ParkingState::getDepth() {
    int counter =0;
    ParkingState *p = this;
    while (p->prev!=nullptr)
    {
        p=p->prev;
        counter++;
    }
    return counter;
}

unsigned long ParkingState::getKey() const {
    int numOfCars = cars.size();
    unsigned long key = -1; //-1 περίπτωση που δεν έχω κανένα αυτοκίνητο και άρα έχω λύση (!)
    for (int i=0; i<numOfCars; i++) {
        Car *temp = cars.at(i);
        if (temp->isPresent()) {
            key++; //αποφυγή περίπτωσης που μοναδικό αυτοκίνητο στο (0,0) με κατεύθυνση Β/Ν να έχει ίδιο key με τελική κατάσταση
            key += (temp->getKey()*17 + 31)*(i+1);
        }
    }
    return key;
}

vector<ParkingState *> ParkingState::expand() {
    vector <ParkingState *> children;
    ParkingState *child;
    int numOfCars = cars.size();
    for (int i=0; i<numOfCars; i++) {
        Car *tempCar = cars.at(i);
        if (tempCar->isPresent()) {
            child = new ParkingState(*this);
            if (CarGoUp(*child, tempCar, i))
                children.push_back(child);
            else
                delete child;
            child = new ParkingState(*this);
            if (CarGoDown(*child, tempCar, i))
                children.push_back(child);
            else
                delete child;
            child = new ParkingState(*this);
            if (CarGoLeft(*child, tempCar, i))
                children.push_back(child);
            else
                delete child;
            child = new ParkingState(*this);
            if (CarGoRight(*child, tempCar, i))
                children.push_back(child);
            else
                delete child;
        }
    }
    return children;
}

bool ParkingState::CarGoUp(ParkingState &child, Car *car, int carPos) {
    if (car->getDirection() == 1) //το αυτοκίνητο κοιτάει Α/Δ και δεν μπορεί να κινηθεί πάνω κάτω
        return false;
    if (car->getY()<HEIGHT-1)
        if (hasObject(car->getX(), car->getY()+1))//υπάρχει αντικείμενο πάνω από το αυτοκίνητο
            return false;
    string coordinates = "("+to_string(car->getX())+","+to_string(car->getY())+")";
    child.setActionName("Car at " + coordinates + " went UP");
    child.setPrevious(this);
    child.removeObject(car->getX(), car->getY()); //αφαιρώ αυτοκίνητο από την προηγούμενη θέση του
    Car *movedCar = new Car(*car); //αλλάζω μόνο τον δείκτη του αυτοκινήτου που μετακινείται για καλύτερη διαχείριση μνήμης
    if (car->getY()< HEIGHT - 1) {
        child.addObject(car->getX(), car->getY() + 1, 2 + car->getDirection()); //προσθήκη αυτοκινήτου ένα κουτάκι πάνω ως "εμπόδιο" αν είναι ακόμα εντός χάρτη
        movedCar->setY(car->getY() + 1);
    }
    else {
        movedCar->disappear();
    }
    child.cars.at(carPos) = movedCar;
    return true;
}

bool ParkingState::CarGoDown(ParkingState &child, Car *car, int carPos) {
    if (car->getDirection() == 1) //το αυτοκίνητο κοιτάει Α/Δ και δεν μπορεί να κινηθεί πάνω κάτω
        return false;
    if (car->getY()>0)
        if (hasObject(car->getX(), car->getY() - 1))//υπάρχει αντικείμενο πάνω από το αυτοκίνητο
            return false;
    string coordinates = "("+to_string(car->getX())+","+to_string(car->getY())+")";
    child.setActionName("Car at " + coordinates + " went DOWN");
    child.setPrevious(this);
    child.removeObject(car->getX(), car->getY()); //αφαιρώ αυτοκίνητο από την προηγούμενη θέση του
    Car *movedCar = new Car(*car); //αλλάζω μόνο τον δείκτη του αυτοκινήτου που μετακινείται για καλύτερη διαχείριση μνήμης
    if (car->getY()>0) {
        child.addObject(car->getX(), car->getY() - 1, 2 + car->getDirection()); //προσθήκη αυτοκινήτου ένα κουτάκι κάτω ως "εμπόδιο" αν είναι ακόμα εντός χάρτη
        movedCar->setY(car->getY() - 1);
    }
    else {
        movedCar->disappear();
    }
    child.cars.at(carPos) = movedCar;
    return true;
}

bool ParkingState::CarGoLeft(ParkingState &child, Car *car, int carPos) {
    if (car->getDirection() == 0) //το αυτοκίνητο κοιτάει Β/Ν και δεν μπορεί να κινηθεί αριστερά δεξιά
        return false;
    if (car->getX() > 0)
        if (hasObject(car->getX() - 1, car->getY()))//υπάρχει αντικείμενο αριστερά από το αυτοκίνητο
            return false;
    string coordinates = "("+to_string(car->getX())+","+to_string(car->getY())+")";
    child.setActionName("Car at " + coordinates + " went LEFT");
    child.setPrevious(this);
    child.removeObject(car->getX(), car->getY()); //αφαιρώ αυτοκίνητο από την προηγούμενη θέση του
    Car *movedCar = new Car(*car); //αλλάζω μόνο τον δείκτη του αυτοκινήτου που μετακινείται για καλύτερη διαχείριση μνήμης
    if (car->getX()>0) {
        child.addObject(car->getX() - 1, car->getY(), 2 + car->getDirection()); //προσθήκη αυτοκινήτου ένα κουτάκι αριστερά ως "εμπόδιο" αν είναι ακόμα εντός χάρτη
        movedCar->setX(car->getX() - 1);
    }
    else {
        movedCar->disappear();
    }
    child.cars.at(carPos) = movedCar;
    return true;
}

bool ParkingState::CarGoRight(ParkingState &child, Car *car, int carPos) {
    if (car->getDirection() == 0) //το αυτοκίνητο κοιτάει Β/Ν και δεν μπορεί να κινηθεί αριστερά δεξιά
        return false;
    if (car->getX() < WIDTH - 1)
        if (hasObject(car->getX() + 1, car->getY()))//υπάρχει αντικείμενο δεξιά από το αυτοκίνητο
            return false;
    string coordinates = "("+to_string(car->getX())+","+to_string(car->getY())+")";
    child.setActionName("Car at " + coordinates + " went RIGHT");
    child.setPrevious(this);
    child.removeObject(car->getX(), car->getY()); //αφαιρώ αυτοκίνητο από την προηγούμενη θέση του
    Car *movedCar = new Car(*car); //αλλάζω μόνο τον δείκτη του αυτοκινήτου που μετακινείται για καλύτερη διαχείριση μνήμης
    if (car->getX() < WIDTH - 1) {
        child.addObject(car->getX() + 1, car->getY(), 2 + car->getDirection()); //προσθήκη αυτοκινήτου ένα κουτάκι δεξιά ως "εμπόδιο" αν είναι ακόμα εντός χάρτη
        movedCar->setX(car->getX() + 1);
    }
    else {
        movedCar->disappear();
    }
    child.cars.at(carPos) = movedCar;
    return true;
}

void ParkingState::printSolution() {
    ParkingState * temp = this;
    vector <ParkingState *> actions;
    while (temp != nullptr) {
        actions.push_back(temp);
        temp = temp->getPrevious();
    }
    int numOfActions = actions.size();
    cout << "Initial parking state:" << endl;
    cout << actions.at(numOfActions-1)->getActionName() << endl;
    actions.at(numOfActions-1)->screenshot();
    for (int i=numOfActions-2; i>=0; i--) { //τυπώνω τις ενέργειες ξεκινώντας από τη ρίζα
        cout << "Step "<< (numOfActions - i - 1) << endl; //όπου step 0 η αρχική κατάσταση
        cout << actions.at(i)->getActionName() << endl;
        actions.at(i)->screenshot();
    }
}

void ParkingState::screenshot() {
    for (int j=HEIGHT-1; j>=0; j--) {
        for (int i=0; i<WIDTH;i++) {
            if (objectMap[i][j] == 0)
                cout<< "O  ";
            else if (objectMap[i][j] == 1)
                cout << "X  ";
            else if (objectMap[i][j] == 2)
                cout << "^  ";
            else
                cout << ">  ";
        }
        cout << endl;
    }
    cout << endl;
}







