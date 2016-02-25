//
//  HardcoreProtester.h
//  FrackMan
//
//  Created by hongshuhong on 2/24/16.
//  Copyright © 2016 David Smallberg. All rights reserved.
//

#ifndef HardcoreProtester_h
#define HardcoreProtester_h
#include "Protester.h"

class Protester;

class HardcoreProtester : public Protester{
public:
    HardcoreProtester(StudentWorld* w, int x, int y) : Protester(w, x, y, IID_HARD_CORE_PROTESTER, 20){};
private:
};

#endif /* HardcoreProtester_h */
