#ifndef MATERIAL
#define MATERIAL
#include "mathLib.h"
class collParam;

class material{
 public:
  enum type {COLOR, TEXTURE};
  bool reflective;
  bool refractive;
  bool cheapTransperancy;
  //lit the control 
  double glossiness;
    double indexOfRefraction;
  vec3d diffuse;
  vec3d specular;
  vec3d radiant;
    vec3d mirror;
    vec3d transperancy;
    vec3d debeers;
    double transperancyAmt;
  double specularHardness;
    type shading;
    bool hasfallOff;
    vec3d fallOff;
    vec3d ambient;
    //a texture function does what it says on the tin:
    //takes a collision parameters object
    //and produceds an image absed on it.
    vec3d (*texture)(collParam& cp);
    material(){
        shading=COLOR;
        reflective=false;
        refractive=false;
        glossiness=0;
        diffuse=vec3d(0);
        specular=vec3d(0);
        mirror=vec3d(0);
        transperancy=vec3d();
        specularHardness=10;
        indexOfRefraction=1;
        debeers = vec3d(0);
        transperancyAmt=0;
        hasfallOff=true;
        shading = COLOR;
        fallOff = vec3d(.25,.1,.01);
        ambient=vec3d(0,0,0);
    }
};

#endif
