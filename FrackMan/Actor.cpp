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

bool Item::checkDiscovered(const Actor* detector){
    int dist = getWorld()->dist(getX(), getY(), detector->getX(), detector->getY());
    if(!isDiscovered()){
        if(dist <= 4){
            setDiscovered();
            return false;
        }
    }
    if(dist <= 3){
        consume();
        getWorld()->playSound(getSound());
        getPlayer()->increasePoints(getPoints());
        return true;
    }
    return false;
}
