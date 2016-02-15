//
//  Nugget.h
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Nugget_h
#define Nugget_h

#include "GameConstants.h"
#include "FrackMan.h"
#include "Protester.h"
#include "Actor.h"
#include <vector>

class Nugget : public Item{
    static const int SCORE_INCREASE = 10;
public:
    /*1. All Gold Nuggets must have an image ID of IID_GOLD.
     2. All Gold Nuggets must have their x,y location specified for them when they are
     created.
     3. All Gold Nuggets must start off facing rightward.
     4. A Gold Nugget may either start out invisible or visible – this must be specified by
     the code that creates the Nugget, depending on the context of its creation. Nuggets buried within the Dirt of the oil field always start out invisible, whereas Nuggets dropped by the FrackMan start out visible.
     5. A Gold Nugget will either be pickup-able by the FrackMan or pickup-able by Protesters, but not both. This state must be specified by the code that creates the Gold Nugget object.
     6. A Gold Nugget will either start out in a permanent state (where they will remain in the oil field until they are picked up by the FrackMan or the level ends) or a temporary state (where they will only remain in the oil field for a limited number of ticks before disappearing or being picked up by a Protester). This state must be specified by the code that creates the Gold Nugget object.
     7. Gold Nuggets have the following graphic parameters:
     a. They have an image depth of 2 – behind actors like Protesters, but above
     Dirt
     b. They have a size of 1.0*/
    Nugget(StudentWorld* w, FrackMan* f, int startX, int startY, bool vis = false, bool pickUpFrack = true, int autoSound = SOUND_GOT_GOODIE, int hp = -1):Item(w, f, IID_GOLD, startX, startY){
        setVisible(vis);
        visible = vis;
        pickup = pickUpFrack;
        hitpoints = hp;
        sound = autoSound;
    }
    virtual ~Nugget();
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
    virtual int getSound(){ return sound; }
private:
    bool visible;
    bool pickup; // true means pick up by player, false means pick up by protester
    int sound;
    int hitpoints;
};
#endif /* Nugget_h */
