//
//  FrackMan.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/14/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "FrackMan.h"

void FrackMan::doSomething(){
    //The FrackMan must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
    if(isDead())
    return;
    /*If the FrackMan’s 4x4 image currently overlaps any Dirt objects within the oil field, then it will call upon the StudentWorld object to:
     a. Remove/destroy the Dirt objects from the 4x4 area occupied by the FrackMan (from x, y to x+3,y+3 inclusive)
     b. Make a digging sound by playing the SOUND_DIG sound (see the StudentWorld section of this document for details on how to play a sound).*/
    getWorld()->removeDirt(getX(), getY(), (int)getSize()*4);
    
    
    /*Otherwise, the doSomething() method must check to see if the human player pressed a key (the section below shows how to check if the user pressed a key, and if so, how to determine what key the user pressed).
        a. If the user presses the Escape key, this allows the user to abort the current level. In this case, the FrackMan object should set itself to completely annoyed (aka dead). The code in your StudentWorld class should detect that the FrackMan has died and address this appropriately (e.g., replay the level from scratch, or end the game if the player has run out of lives).
        b. If the user presses the spacebar key, then the FrackMan will fire a Squirt into the oil field, assuming they have sufficient water in their squirt gun to do so. The FrackMan will then reduce their water count by 1. To fire a Squirt, the player must create and add a new Squirt object into the oil field at a location that is four squares immediately in front of the FrackMan, facing the same direction as the FrackMan.
     If the player wants their FrackMan to turn to face a different direction then it is currently facing (by pressing one of the arrow keys), then the FrackMan’s direction should be adjusted to the indicated direction but the FrackMan must not move in that direction. For example, if the FrackMan is currently facing up, and the player hits the right arrow key, then the FrackMan should turn to face right, but not move right.*/
    int keyPressed = -1;
    if(getWorld()->getKey(keyPressed)){
        switch(keyPressed){
            case KEY_PRESS_ESCAPE:
                while(!isDead())
                    consume();
                break;
            case KEY_PRESS_SPACE:
                getWorld()->squirt(getX(), getY(), getDirection());
                break;
            case KEY_PRESS_LEFT:
            case KEY_PRESS_RIGHT:
            case KEY_PRESS_UP:
            case KEY_PRESS_DOWN:{
                Direction dir = keyToDir(keyPressed);
                changeState(dir);
                break;
            }
            case KEY_PRESS_TAB:{
                if(nugget <= 0)
                    break;
                
                Nugget* nugget = new Nugget(getWorld(), this, getX(), getY(), true, false, SOUND_PROTESTER_FOUND_GOLD, 100);
                getWorld()->addActor(nugget);
                nugget--;
                break;
            }
            case 'Z':
            case 'z':{
                if(sonar <= 0)
                    break;
                getWorld()->revealSonar(getX(), getY());
                sonar--;
            }
        }
    }
};