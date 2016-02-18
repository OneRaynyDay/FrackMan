#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir, int steps){
    int tempx = xdir, tempy = ydir;
    switch(dir){
        case left:
            tempx-=steps;
            break;
        case right:
            tempx+=steps;
            break;
        case up:
            tempy+=steps;
            break;
        case down:
            tempy-=steps;
            break;
        default:
            return false;
    }
    if(world->inBound(tempx, tempy) && world->inBound(tempx+(getSize()*4), tempy+(getSize()*4))){
        xdir = tempx;
        ydir = tempy;
    }
    return true;
}


void Human::changeState(Direction dir){
    if(dir != getDirection()){
        setDirection(dir);
        return;
    }
    int xdelta = getX(), ydelta = getY();
    if(moveDelta(getWorld(), dir, xdelta, ydelta)){
        moveTo(xdelta, ydelta);
    }
}

