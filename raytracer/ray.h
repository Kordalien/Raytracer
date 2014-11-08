//The definition of a ray object

#ifndef _RAY_
#define _RAY_

#include "mathlib.h"

class ray{
public:
  int depth;
  vec3d dir;
  vec3d ori;
  vec3d irrad;
  //index of refraction
  double ior;
    void jitterOut(){ ori+=dir*.0001;};
    void jitterIn(){ ori-=dir*.0001;};
};

#endif
