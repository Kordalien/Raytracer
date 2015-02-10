//
//  sphere.h
//  raytracer
//
//  Created by Scott Cesar on 10/26/14.
//  Copyright (c) 2014 Scott Cesar. All rights reserved.
//
#ifndef SPHERE_
#define SPHERE_
#include "object.h"
#include "cmath"

class sphere : public object{
    double r;
public:
    
    vec3d normal(vec3d p){
        return (p-pos).normal();
    }
    
    bool contains(vec3d p){
        return (pos-p).magnitude()<r;
    }
    
    //collision is determind by ||x-c||^2 =r^2
    
    bool collide(ray r, collParam &cp){
        double b= r.dir.dot(r.ori-pos);
        double det = b*b-(r.ori-pos).dot(r.ori-pos) + this->r*this->r;
        if(det<0){
            return false;
        }
        else {
            double t = (sqrt(det)-b);
            double t1 = (-b-sqrt(det));
            
            t = (t>0)? ((t1>0)?( (t<t1)?t :t1) : t) :t1;
            if (t<=cp.t0 || t> cp.t1){
                return false;
            }
            
            cp.p=r.ori+r.dir*t;
            cp.objectId=id;
            cp.m=m;
            cp.dist=t;
            cp.norm=normal(cp.p);
            return true;
        }
        
    }
    sphere(vec3d c, double r, material m){
        this->r=r;
        pos=c;
        this->m=m;
        boundingCube=r;
    }
    
    vec3d within(){
        vec3d ret(0);
        ret.x=  rand()/ (double)RAND_MAX *2*r-r;
        double rtmp = sqrt(r*r-ret.x*ret.x);
        ret.y = rand()/ (double)RAND_MAX*2*rtmp-rtmp;
        rtmp = sqrt(rtmp*rtmp-ret.y*ret.y);
        ret.z = rand()/ (double)RAND_MAX*2*rtmp-rtmp;
        return ret+pos; // ret is a point within a sphere of size r at (0,0,0) so we shift that to location pos.
    }
};
 
#endif

