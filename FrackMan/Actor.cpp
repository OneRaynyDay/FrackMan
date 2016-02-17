#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::moveDelta(StudentWorld* world, Direction dir, int& xdir, int& ydir){
    int tempx = xdir, tempy = ydir;
    switch(dir){
        case left:
            tempx--;
            break;
        case right:
            tempx++;
            break;
        case up:
            tempy++;
            break;
        case down:
            tempy--;
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

