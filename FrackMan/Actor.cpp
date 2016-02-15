#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Human::moveDelta(Direction dir, int& xdir, int& ydir){
    switch(dir){
        case left:
            xdir--;
            break;
        case right:
            xdir++;
            break;
        case up:
            ydir++;
            break;
        case down:
            ydir--;
            break;
        default:
            return false;
    }
    return true;
}


void Human::changeState(Direction dir){
    if(dir != getDirection()){
        setDirection(dir);
        return;
    }
    int xdelta = getX(), ydelta = getY();
    if(moveDelta(dir, xdelta, ydelta) && world->inBound(xdelta, ydelta)){
        moveTo(xdelta, ydelta);
    }
}


