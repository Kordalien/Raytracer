/*
  the basic raytracer
 */
#ifndef _RAYTRACER_
#define  _RAYTRACER_

#include <vector>

//user includes
#include "camera.h"
#include "object.h"
#include "mathLib.h"
#include "interpol.h"
using namespace std;
class raytracer{
  

  vector<camera> cameras;
  vector<object *> scene;
  vector<object *> lights;
  //animationController * anim;   //detects intersection events, triggers approaite reponses
  int maxDepth;
    int distributionRays;
    int subRays;
 public:
  raytracer(int maxDepth, int distributionRays);
    
    void addCamera(camera c);
    void addObject(object * o);
    void addLight(object * o);
    
  void castRay(ray &r);
  void getColor(collParam cp, ray &r);
  void reflect(collParam cp, ray &r);
  bool refract(collParam cp, ray &r, bool entering); //if entering, the ray is entering a new material, othersiwe, exiting one.
  void irradience(collParam cp, ray& initR);
  unsigned char * render(camera c);
};

#endif
