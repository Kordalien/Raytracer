#ifndef MATERIAL
#define MATERIAL
#include "mathLib.h"
class material{
 public:
  bool reflective;
  bool refractive;
  //lit the control 
  double glossiness;
  vec3d diffuse;
  vec3d specular;
  vec3d radiant;
    vec3d mirror;
  double specularHardness;
    material(){
        reflective=false;
        refractive=false;
        glossiness=0;
        diffuse=vec3d(0);
        specular=vec3d(0);
        mirror=vec3d(0);
        specularHardness=10;
    }
};

#endif
