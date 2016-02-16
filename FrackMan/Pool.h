//
//  Water.h
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Water_h
#define Water_h

#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

class Pool : public Item{
static const int SCORE_INCREASE = 100;
public:
    Pool(StudentWorld* w, FrackMan* f, int startX, int startY);
    virtual ~Pool(){};
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
private:
    int hitpoints;
};

#endif /* Water_h */
