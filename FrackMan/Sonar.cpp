//
//  Sonar.cpp
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#include <stdio.h>
#include "Sonar.h"

/*
 1. The object must check to see if it is currently alive. If not, then its doSomething() method must return immediately – none of the following steps should be performed.
 2. Otherwise, if the Sonar Kit is within a radius of 3.0 (<= 3.00 units away) from the FrackMan, then the Sonar Kit will activate, and:
     a. The Sonar Kit must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
     b. The Sonar Kit must play a sound effect to indicate that the FrackMan picked up the Goodie: SOUND_GOT_GOODIE.
     c. The Sonar Kit must tell the FrackMan object that it just received a new Sonar Kit so it can update its inventory.
     d. The Sonar Kit increases the player’s score by 75 points (This increase can be performed by the FrackMan class or the Sonar Kit class).
 3. Since the Sonar Kit is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead (so that it will be removed by your StudentWorld class from the game at the end of the current tick).
 */

Sonar::Sonar(StudentWorld* w, FrackMan* f):Item(w, f, IID_SONAR, 0, DIRT_ROWS+1){
    setVisible(true);
    hitpoints = min(100, 300 - 10*w->getLevel());
}

 
Sonar::~Sonar(){
}

void Sonar::doSomething(){
    if(isDead())
        return;
    hitpoints--;
    if(hitpoints <= 0){
        consume();
    }
    if(getWorld()->checkDiscoveredFrackMan(this)){
        getPlayer()->increaseSonar(1);
    }
}

