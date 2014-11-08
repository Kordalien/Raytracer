//
//  triangle.h
//  raytracer
//
//  Created by Scott Cesar on 10/29/14.
//  Copyright (c) 2014 Scott Cesar. All rights reserved.
//

#ifndef raytracer_triangle_h
#define raytracer_triangle_h

#include "object.h"
#include "cmath"

class triangle : public object{
    vec3d p1;
    vec3d p2;
    vec3d p3;
public:
    
    vec3d normal(vec3d p){
        return (p1-p2).cross(p1-p3).normal();
    }
    
    bool contains(vec3d p){
        ray r;
        r.dir=-1*normal(p);
        r.ori=p-r.ori*1;
        collParam cp;
        cp.t0=0;
        cp.t1=3;
        return collide(r,cp);
    }
    
    //collision is determind by ||x-c||^2 =r^2
    
    bool collide(ray r, collParam &cp){
        vec3d abc = p1-p2;
        vec3d def = p1-p3;
        vec3d jkl = p1-r.ori;
        double akjb = abc.x*jkl.y-jkl.x*abc.y;
        double jcal = jkl.x*abc.z-abc.x*jkl.z;
        double blkc = abc.y*jkl.z-jkl.y*abc.z;
        double eihf = def.y*r.dir.z-r.dir.y*def.z;
        double gfdi = r.dir.x*def.z-def.x*r.dir.z;
        double dheg = def.x*r.dir.y-def.y*r.dir.x;
        
        double M=abc.x*eihf+abc.y*gfdi+abc.z*dheg;
        
        double t =-(def.z*akjb+def.y*jcal+def.x*blkc)/M;
        if(t<cp.t0 || t>cp.t1)
            return false;
        double g = (r.dir.z*akjb+r.dir.y*jcal+r.dir.x*blkc)/M;
        if(g<0||g>1)
            return false;
        double b =(jkl.x*eihf+jkl.y*gfdi+jkl.z*dheg)/M;
        if(b<0||b>1-g)
            return false;
        
            cp.p=r.ori+r.dir*t;
            cp.objectId=id;
            cp.m=m;
            cp.dist=t;
            cp.norm=normal(cp.p);
            return true;
    }
    
    triangle(vec3d p1, vec3d p2, vec3d p3, material m){
        this->p1=p1;
        this->p2=p2;
        this->p3=p3;
        this->m=m;
        boundingCube=0;
    }
    
    vec3d within(){
        vec3d d1 = (p1-p2);
        vec3d d2 = (p1-p3);
        double a=rand()/ (double)RAND_MAX;
        vec3d r = p1+a*d1+d2*(1-a);
        return r;
    }
};

#endif
