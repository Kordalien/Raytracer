//
//  cubes.h
//  raytracer
//
//  Created by Scott Cesar on 10/29/14.
//  Copyright (c) 2014 Scott Cesar. All rights reserved.
//

#ifndef raytracer_cubes_h
#define raytracer_cubes_h

#include "object.h"
#include "cmath"


class cube : public object{
    double r;
    double ang;
public:
    
    vec3d normal(vec3d p){
        return (p-pos);
    }
    
    bool contains(vec3d p){
        vec3d loc =(p-pos).abs();
        return (loc.x<r && loc.y<r && loc.z<r);
    }
    
    //collision is determind by ||x-c||^2 =r^2
    
    bool collide(ray r, collParam &cp){
        vec3d loc = pos-r.ori;
        vec3d front = loc+this->r;
        vec3d back = loc-this->r;
        double tfx = front.x/r.dir.x;
        double tfy = front.y/r.dir.y;
        double tfz = front.z/r.dir.z;
        double tbx = back.x/r.dir.x;
        double tby = back.y/r.dir.y;
        double tbz = back.z/r.dir.z;
        
        return false;
    }
    cube(vec3d c, double r, material m){
        this->r=r;
        pos=c;
        this->m=m;
        boundingCube=r;
    }
    
    vec3d within(){
        double x = rand()/ (double)RAND_MAX *2*r-r;
        double y = rand()/ (double)RAND_MAX *2*r-r;
        double z = rand()/ (double)RAND_MAX *2*r-r;
        return pos+vec3d(x,y,z);
    }
};

#endif
