/*
  A file describing a basic camera
 */
#ifndef _CAMERA_
#define _CAMERA_
#include "mathlib.h"
#include "interpol.h"
#include "ray.h"
class camera{


  vec3d eyePos; //location of the eye
    vec3d imagePlane; //location of the camera plane
    vec3d imBL;
  vec3d focalPoint; //location of the focal plane.

  //the basis vectors for the camera
  vec3d Up,Right,Back;

  /*
   maximum size of the confusion region: objects at the lens can be confused by this amount.
   in other words, if you placed a point object exactly at the lens, it would cast light on all pixels inside confRad
   this will be the actual amount to permute the area of selection over... 
   */
    double confRad;
  
  //width and height of the ccd in pixels
  int x;
  int y;
  
  //size of the ccd in real units
  double width;
  double height;
    
public:
    vec3d * irradience;
    unsigned char * pixels;
  //get the ray which leaves the eye/lens and passes through the pixel px, py
  ray getRay(int px, int py);
    //generates rays using a stratified sample
  ray getRayStrat(int px, int py, int subRay, int subRayMax);
    //launch an actual ray
    ray makeRay(vec3d imLoc);
    //cameraPlane is eye location, real eye calcualted by fovy
    camera(vec3d cameraPlane, double fovy, vec3d focalPoint, vec3d up, int px, int py, double aspectRatio, double ccdHeight,double allowConf, double depthOfField);
    int getX();
    int getY();

  void sigmoidMapping();
    void linearMap();
    //cut linear map flattens high spcular values
     void cutLinearMap(double max);
    //flat map just converts 1 to 255;
     void flatMap(double max);
  //a simple function which returns the effective focal length of the camera, ie, the distance between the eye and the lens. (basically just eyepos-focalPoint)
  double focalLength(); 

};

#endif
