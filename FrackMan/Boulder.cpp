//
//  Boulder.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/17/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Boulder.h"

void Boulder::doSomething(){
    /*Check to see if it’s still alive. If not, its doSomething() method should immediately return.
     2. If the Boulder is currently in the stable state, then it must check to see if there is any Dirt in the 4 squares immediately below it. For example, if the Boulder is at location x=20,y=30, it would check squares (20,29), (21,29), (22,29) and (23,29) for Dirt. If there is any Dirt below the Boulder, it does nothing – after all it’s a dumb rock. However, if none of the 4 squares beneath the Boulder have any Dirt, then the Boulder must transition into a waiting state:
     A. It must enter a waiting state for the next 30 ticks
     3. If the Boulder is in a waiting state and 30 ticks have elapsed, then it must
     transition into a falling state and play the sound SOUND_FALLING_ROCK.
     4. If the Boulder is in a falling state, then:
     A. It must continue to move downward one square during each tick until it either (a) hits the bottom of the oil field (i.e., it tries to move to y=-1), (b) runs into the top of another Boulder, or (c) it runs into Dirt (i.e., by moving down a square, the Boulder would overlap over one or more Dirt objects). When any of the above conditions are met the Boulder must set its state to dead so it can be removed from the game at the end of the current tick.
     B. If the Boulder comes within a radius of 3 (i.e. less than or equal to 3 squares, so 2.99 would count) of any Protester(s) or the FrackMan while falling, it must cause 100 points of annoyance to those actors (effectively completely annoying them in one fell swoop). The Boulder will continue falling down as described above, even if it annoys one or more Protesters on its way down (i.e., it won’t stop falling if it runs into a Protester). However, if the Boulder annoys the FrackMan, the player will instantly lose a life.
     */
    if(state == STABLE_STATE){
        bool dirtOrActor = false;
        
        getWorld()->existsBlock(getX(), getY()-1, 4, 1, dirtOrActor, false, this);
        if(!dirtOrActor){
            state = WAITING_STATE;
        }
    }
    else if(state == WAITING_STATE){
        hitpoint--;
        if(hitpoint == 0){
            state = FALLING_STATE;
        }
    }
    else if(state == FALLING_STATE){
        int x = getX(), y = getY();
        bool flag = false; // stubby
        moveDelta(getWorld(), getDirection(), x, y);
        if(getWorld()->existsBlock(x, y, 4, 1, flag, false, this)){
            x = getX();
            y = getY();
        }
        if(getX() == x && getY() == y){
            consume();
        }
        else{
            /*if(getWorld()->attackProtestersAt(x, y, 3, DECREASE_HP) || getWorld()->attackFrackManAt(x, y, 3, DECREASE_HP))
                std::cout << x << "\t" << y << "\t" << getPlayer()->getX() << "\t" << getPlayer()->getY() << "\t" << std::endl;*/
            getWorld()->attackProtestersAt(x, y, 3, DECREASE_HP);
            getWorld()->attackFrackManAt(x, y, 3, DECREASE_HP);
            moveTo(x, y);
        }
    }
}