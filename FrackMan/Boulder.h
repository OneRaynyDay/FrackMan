//
//  Boulder.h
//  FrackMan
//
//  Created by hongshuhong on 2/17/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Boulder_h
#define Boulder_h
#include "GameConstants.h"
#include "FrackMan.h"
#include "Actor.h"
#include <vector>

class Boulder : public Item{
    static const int STABLE_STATE = 0;
    static const int WAITING_STATE = 1;
    static const int FALLING_STATE = 2;
    static const int SCORE_INCREASE = 500;
    static const int WAITING_HP = 30;
    static const int DECREASE_HP = 100;
public:
    Boulder(StudentWorld* w, FrackMan* f, int startX, int startY): Item(w, f, IID_BOULDER, startX, startY, down, 1.0, 1){
        /*
        1. The Boulder object must have an image ID of IID_BOULDER.
        2. A Boulder object must have its x,y location specified for it – the StudentWorld
            class can pass in this x,y location when constructing a new Boulder object.
            3. Boulders start out in a stable state (more on this below).
            4. Boulders start out facing downward.
            5. Boulders have the following graphic parameters:
            a. They have an image depth of 1 – behind actors like Protesters, but above Dirt
            b. They have a size of 1.0
         */
        setVisible(true);
        state = STABLE_STATE;
        hitpoint = WAITING_HP;
    }
    virtual ~Boulder();
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
    virtual int getSound(){ return SOUND_FALLING_ROCK; }
    virtual bool isBlock(){ return true; }
    bool getState(){ return state; }
private:
    int state;
    int hitpoint;
};

#endif /* Boulder_h */
