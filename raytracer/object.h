#ifndef _OBJECT_
#define _OBJECT_
#include "mathlib.h"
#include "ray.h"
#include "material.h"

class collParam{
public:
    vec3d p;
    double dist;
    int objectId;
    vec3d norm;
    material m;
    double t0;
    double t1;
};

class object{
public:
  material m;
  vec3d pos;
  int id;
  //the size of one edge of a cube that could contain this object
  double boundingCube;
  //returns the surface normal with respect to a given point... contractually, this point should be on the surface of the object.
    virtual vec3d normal(vec3d point){return vec3d(0);};
  //Collide is required to reutrn a number >=0; indicating a collision happing in front of the ray source.
    //implmented to be a point.
    virtual bool collide(ray r, collParam &cp){
        double a = (pos-r.ori).normal().dot(r.dir.normal());
        if(a>.9999999){
            cp.p=pos;
            cp.objectId=id;
            cp.m=m;
            cp.dist=(pos-r.ori).magnitude();
            return true;
        }
        return false;
    };
    virtual bool contains(vec3d p){return false;};
    virtual vec3d within(){return pos;};
  
};



#endif

//TODO: implment a real object that can be inserted into a scene.
