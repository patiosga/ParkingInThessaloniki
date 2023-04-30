#include <iostream>
#include "ParkingState.h"
#include "unordered_map"
#include "queue"
#include <chrono>

using namespace std;

ParkingState *searchBFS(ParkingState *initial, long long &examined, long long &mem) {
    queue <ParkingState *> agenda;
    unordered_map <unsigned long, ParkingState *> closed;

    agenda.push(initial);
    examined=0;
    mem=1;
    ParkingState *s; //δείκτης στην κατάσταση που βγαίνει κάθε φορά από την ουρά
    while (!agenda.empty()) {
        if (agenda.size() + closed.size() > mem)
            mem = agenda.size() + closed.size();
        s = agenda.front();
        agenda.pop();
        if (closed.count(s->getKey()) == 0) { // αν δεν έχω ξαναδεί αυτή την κατάσταση
            examined++;
            if (s->getKey() == -1) { //Έχω κενό από αυτοκίνητα parking όταν key --> -1
                return s;
            }
            pair<unsigned long, ParkingState *> k (s->getKey(),s);
            closed.insert(k);
            vector<ParkingState *> children = s->expand(); //εύρεση νέων πιθανών καταστάσεων που προκύπτουν από την τωρινή
            for (int i=0; i<children.size(); i++)
                if (closed.count(children.at(i)->getKey())==0)
                    agenda.push(children.at(i));
        }
    }
    return nullptr;
}


int main() {
    long long int mem,examined;
    int scenario = 3; //1,2,3
    ParkingState *initialParking = new ParkingState();
    if (scenario == 1) { //2x2 με 2 αυτοκίνητα και ένα εμπόδιο, WIDTH = 2, HEIGHT = 2
        initialParking->addCar(new Car(0,1,1)); //Α/Δ στο (0,1)
        initialParking->addCar(new Car(1,0,0)); //Β/Ν στο (1,0)
        initialParking->addObject(0,0,1); //εμπόδιο
    }
//    Solution depth 2
//    Time elapsed: 92500 ns
//    Examined: 7
//    Memory: 13

    else if (scenario == 2) { //4x3 --> εικόνα εκφώνησης, WIDTH = 4, HEIGHT = 3
        initialParking->addCar(new Car(2, 2, 1)); //Α/Δ στο (2,2)
        initialParking->addCar(new Car(2, 1, 0)); //Β/Ν στο (2,1)
        initialParking->addCar(new Car(1, 1, 1)); //Α/Δ στο (1,1)
        initialParking->addObject(0, 1, 1); //εμπόδιο στο (0,1)
        initialParking->addObject(2, 0, 1); ///εμπόδιο στο (2,0)
        initialParking->addObject(3, 2, 1); ///εμπόδιο στο (3,2)
    }
    //Solution depth 8
    //Time elapsed: 545000 ns
    //Examined: 39
    //Memory: 51

    else if (scenario == 3) {  // 4x5 , WIDTH = 4, HEIGHT = 5
        initialParking->addCar(new Car(2,2,1));  //Α/Δ στο (2,2)
        initialParking->addCar(new Car(2,1,0));  //Β/Ν στο (2,1)
        initialParking->addCar(new Car(1,1,1));  //Α/Δ στο (1,1)
        initialParking->addCar(new Car(1,2,0));  //Β/Ν στο (1,2)
        initialParking->addCar(new Car(2,3,0));  //Β/Ν στο (2,3)
        initialParking->addCar(new Car(0,3,0));  //Β/Ν στο (0,3)
        initialParking->addCar(new Car(0,4,0));  //Β/Ν στο (0,4)
        initialParking->addCar(new Car(1,3,1));  //Α/Δ στο (1,3)
        initialParking->addCar(new Car(1,4,1));  //Α/Δ στο (1,4)
        initialParking->addObject(0,1,1); //εμπόδιο στο (0,1)
        initialParking->addObject(2,0,1); ///εμπόδιο στο (2,0)
        initialParking->addObject(3,2,1); ///εμπόδιο στο (3,2)
        initialParking->addObject(3,4,1); ///εμπόδιο στο (3,4)
    }
    //Solution depth 22
    //Time elapsed: 1440559100 ns
    //Examined: 39315
    //Memory: 49120


    ParkingState *end;
    auto begin = std::chrono::high_resolution_clock::now();
    end = searchBFS(initialParking, examined, mem);
    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - begin);

    if (end == nullptr)
        cout<<"Unsolvable"<<endl;
    else {
        cout<<"Found solution" << endl;
        end->printSolution();
        cout<<"Solution depth "<<end->getDepth()<<endl;
    }
    cout << "Time elapsed: " << elapsed.count() << " ns" << endl;
    cout << "Examined: " << examined << endl;
    cout << "Memory: " << mem << endl;

    return 0;
}

