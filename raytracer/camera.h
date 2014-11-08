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

  //the width & height of the lens to use for depth of field (as in, the area over which we move the
  //the eye to simulate a lens.
  double lensWidth;
  double lensHeight; 
  
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
    //cameraPlane is eye location, real eye calcualted by fovy
    camera(vec3d cameraPlane, double fovy, vec3d focalPoint, vec3d up, int px, int py, double x, double y, double width, double height);
  
    int getX();
    int getY();

  void sigmoidMapping();
    void linearMap();
    //cut linear map flattens high spcular values
     void cutLinearMap(double max);
  //a simple function which returns the effective focal length of the camera, ie, the distance between the eye and the lens. (basically just eyepos-focalPoint)
  double focalLength(); 

};

#endif
