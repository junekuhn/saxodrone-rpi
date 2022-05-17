//
//  metaball.hpp
//  audiovisualApplication
//
//  Created by Justin Kuhn on 3/1/22.
//

#ifndef metaball_hpp
#define metaball_hpp
#include "ofMain.h"

#include <stdio.h>
using namespace glm;

class Metaball {
public:
    Metaball();
    void update();
    
    float size;
    vec2 vel;
    vec2 pos;
    float radius;
};

#endif /* metaball_hpp */
