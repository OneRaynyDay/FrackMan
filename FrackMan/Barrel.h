//
//  Barrel.h
//  FrackMan
//
//  Created by hongshuhong on 2/15/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef Barrel_h
#define Barrel_h

#include "GameConstants.h"
#include "FrackMan.h"
#include "Actor.h"
#include <vector>

using namespace std;

class Barrel : public Item{
    static const int SCORE_INCREASE = 1000;
public:
    /*
     All Barrels must have an image ID of IID_BARREL.
     2. All Barrels must have their x,y location specified for them when they are created.
     3. All Barrels must start off facing rightward.
     4. All Barrels start out invisible – they must be discovered (by the FrackMan
     walking in close proximity to them) before they become visible.
     5. Barrels have the following graphic parameters:
     a. They have an image depth of 2 – behind actors like Protesters, but above Dirt
     b. They have a size of 1.0
     */
    Barrel(StudentWorld* w, FrackMan* f, int startX, int startY):Item(w, f, IID_BARREL, startX, startY){
    }
    virtual ~Barrel(){};
    virtual void doSomething();
    virtual int getPoints(){ return SCORE_INCREASE; }
    virtual int getSound(){ return SOUND_FOUND_OIL; }
private:
    
};

#endif /* Barrel_h */
