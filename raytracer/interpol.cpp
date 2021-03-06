//
//  interpol.cpp
//  raytracer
//
//  Created by Scott Cesar on 10/24/14.
//  Copyright (c) 2014 Scott Cesar. All rights reserved.
//

#include "interpol.h"
#include <random>
#define initial() if(init==false) initMe()

bool init=false;
std::default_random_engine generator;
std::uniform_real_distribution<double> uniform;
/*
 returns a randomly chosen direction which will land within a bounding rectangle at a distance d away from
 */
vec3d squareInterpol(vec3d dir, double range){
    vec3d w =dir.normal();
    vec3d t =vec3d(0,0,1);
    vec3d u = (t.cross(w)).normal();
    if(u.x==0&&u.y==0&&u.z==0){
        t =vec3d(1,0,0);
        u = (t.cross(w)).normal();
    }
    vec3d v = (w.cross(u)).normal();
    //let X be a uniformly distrubted variable over the range [0-1]
    double X1, X2;
    X1 = rand()/ (double)RAND_MAX;
    X2 = rand()/ (double)RAND_MAX;
    X1= X1*range - range/2;
    X2= X2*range - range/2;
    return (w+X1*u+X2*v).normal();
};

/*
 Returns a vector which adjusts by width/height along the plane orthagonal to a given direction.
 Given direction,
 */
vec3d rectAdjust(vec3d dir, double width, double height){
    vec3d w =dir.normal();
    vec3d t =vec3d(0,0,1);
    vec3d u = (t.cross(w)).normal();
    if(u.x==0&&u.y==0&&u.z==0){
        t =vec3d(1,0,0);
        u = (t.cross(w)).normal();
    }
    vec3d v = (w.cross(u)).normal();
    //let X be a uniformly distrubted variable over the range [0-1]
    double X1, X2;
    X1 = rand()/ (double)RAND_MAX;
    X2 = rand()/ (double)RAND_MAX;
    X1= X1*width- width/2;
    X2= X2*height - height/2;
    vec3d h1 = X1*u;
    vec3d h2 = X2*v;
    return (X1*u+X2*v);
};

void initMe(){
    uniform = std::uniform_real_distribution<double>(0.0,1.0);
    init=true;
};

//uniformly select a random point across (-amt .. amt)
double linearAdjust(double amt){
    initial();
    return uniform(generator)*amt*2-amt;
};

double lerp(double a, double b, double lerpVal){
    return lerpVal*a +(1-lerpVal)*b;
};
