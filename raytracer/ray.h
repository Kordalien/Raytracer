//The definition of a ray object

#ifndef _RAY_
#define _RAY_

#include "mathlib.h"
#include <vector>
class ray{
public:
    int px;
    int py;
  int depth;
  vec3d dir;
  vec3d ori;
    vec3d irrad;
    double lastD;
    double ior;
  //index of refraction
    void jitterOut(){ ori+=dir*.0001;};
    void jitterIn(){ ori-=dir*.0001;};
};

#endif
