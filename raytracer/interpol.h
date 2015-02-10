#ifndef INTERPOL_
#define INTERPOL_
#include "mathlib.h"
/*
  returns a randomly chosen direction which will land with some rectangle range at a distance of one
  away from the source of the direction.
 */
extern vec3d squareInterpol(vec3d dir, double range);

/*
 Returns a vector which adjusts by width/height along the plane orthagonal to a given direction.
 Given direction,
 */
extern vec3d rectAdjust(vec3d dir, double width, double height);

extern double linearAdjust(double amt);


extern double lerp(double a, double b, double lerpVal);

extern double bicubic();

#endif
