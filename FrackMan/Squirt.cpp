//
//  Squirt.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/16/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Squirt.h"

Squirt::~Squirt(){
}

void Squirt::doSomething(){
    /*
     1. If a Squirt is within a radius of 3.0 of one or more Protesters (up to and including a distance of 3.0 squares away), it will cause 2 points of annoyance to these Protester(s), and then immediately set its state to dead, so it can be removed from the oil field at the end of the tick.
     2. If a Squirt has traveled through its full travel distance, then it immediately sets its state to dead, so it can be removed from the oil field at the end of the current tick.
     3. Otherwise, the Squirt must check to see if it can move one square in its currently-
     facing direction. If this target location is occupied by either Dirt or a Boulder, then the Squirt immediately sets its state to dead so it can be removed from the oil field at the end of the current tick.
     4. Otherwise, the Squirt moves one square forward in its currently-facing direction, and then returns.
     */
    if(getWorld()->checkDiscoveredProtester(this)){
        int state = -1;
        if(getWorld()->attackProtestersAt(getX(), getY(), 4, 2, state)){
            hit = true;
            consume();
            if(state == 0)
                getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
            else if(state == 1)
                getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
            return;
        }
    }
    if(hitpoints == 0){
        consume();
        return;
    }
    hitpoints--;
    // move in the specified direction 1 square forward
    int x = getX(), y = getY();
    moveDelta(getWorld(), getDirection(), x, y);
    changeState(x, y, getSize()*4);
}

void Squirt::changeState(int x, int y, int size){
    bool flag = false; // stub
    if(getWorld()->existsBlock(x, y, size, size, flag)){
        x = getX();
        y = getY();
    }
    if(getX() == x && getY() == y){
        consume();
    }
    else{
        moveTo(x, y);
    }
}
