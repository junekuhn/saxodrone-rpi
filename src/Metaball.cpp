//
//  metaball.cpp
//  audiovisualApplication
//
//  Created by Justin Kuhn on 3/1/22.
//

#include "Metaball.hpp"

Metaball::Metaball() {
    size = std::pow(ofRandom(0.25), 2);
    vel = vec2(ofRandom(0.2), ofRandom(0.2)) * (8 * ( 1- size) +2);
    radius = 800 * size + 3;
    
    pos = vec2(ofRandom(ofGetWidth()/2), ofRandom(ofGetHeight()/2));
}

void Metaball::update() {
    pos += vel;
    
    if(pos.x < radius || pos.x > ofGetWidth() - radius) {
        vel.x *= -1;
    }
    if(pos.y < radius || pos.y > ofGetHeight() - radius) {
        vel.y *= -1;
    }
}
