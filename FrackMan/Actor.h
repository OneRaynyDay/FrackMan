#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/* PLEASE REFER TO PG 52 OF SPEC */

//It must be derived from our GraphObject class

// It (or its base class) must make itself visible via a call to setVisible(true); (already done via inheritance)

class Actor : public GraphObject{
public:
    // It must have a simple constructor and destructor
    // These 3 parameters are needed because of the base class
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject(imageID, startX, startY, dir, size, depth){
        // It (or its base class) must make itself visible via a call to setVisible(true);
        setVisible(true);
    }
    virtual ~Actor(){
        setVisible(false);
    };
    
    // It must have a single virtual method called doSomething() that can
    // be called by the World to get one of the game’s actors to do
    // something.
    // This is pure virtual function
    virtual void doSomething() = 0;
    
private:
    // You may add other public/private methods and private member
    // variables to this base class, as you see fit.
};


#endif // ACTOR_H_
