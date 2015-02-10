#include "camera.h"
//TODO:Implment sigmoid color mapping
//a function which applies sigmoidal tone mapping for conversion from irradience values to pixel colors.
//Basic sigmoid equation is I
void camera::sigmoidMapping(){
  double globalMean=0.;
  
}

//scales the  irradience such that the maximum value seen is 256, 0 remains basis point.
//a mapping also loads the pixel array;
void camera::linearMap(){
    double maxInt =0;
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            if(irradience[i+x*j].max()>maxInt)
                maxInt=irradience[i+x*j].max();
        }
    }
    double ratio = 255.0/maxInt;
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            irradience[i+x*j]*=ratio;
            for(int k=0; k<3; k++){
                pixels[(i+j*x)*3+k] = (int)irradience[i+j*x][k];
                if(i==250&&j==250)
                    std::cout<<(int)pixels[(i+j*x)*3+k]<<" "<<irradience[i+j*x][k];
            }
        }
    }
}

//scales the  irradience such that the maximum value seen is 256, 0 remains basis point.
//a mapping also loads the pixel array;
void camera::cutLinearMap(double max){
    double maxInt =0;
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            for(int k=0; k<3; k++)
                if(irradience[i+x*j][k]>max)
                    irradience[i+x*j][k]=max;
            if(irradience[i+x*j].max()>maxInt)
                maxInt=irradience[i+x*j].max();
        }
    }
    double ratio = 255.0/maxInt;
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            irradience[i+x*j]*=ratio;
            for(int k=0; k<3; k++){
                pixels[(i+j*x)*3+k] = (int)irradience[i+j*x][k];
                if(i==250&&j==250)
                    std::cout<<(int)pixels[(i+j*x)*3+k]<<" "<<irradience[i+j*x][k];
            }
        }
    }
}

void camera::flatMap(double max){
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            for(int k=0; k<3; k++){
                pixels[(i+j*x)*3+k] = (int)std::max(0.0,std::min(255.,irradience[i+j*x][k]*255));
                if(i==250&&j==250)
                std::cout<<(int)pixels[(i+j*x)*3+k]<<" "<<irradience[i+j*x][k];
            }
        }
    }
}

double camera::focalLength(){
  return (eyePos-focalPoint).magnitude();
}

//TODO: Depth of field disabled... fix it.
ray camera::makeRay(vec3d imLoc){
    //this jitter needs to be moved into the Up, Right basis:
    ray r;
    r.ori=eyePos;
    r.dir= (imLoc-eyePos).normal();
    
    //To point to the actual point, solve for collision with the focal plane...
    //this collision is solved by l . n !=0 which is known, due to the way we calcualte focalPoint
    //then d = (p0-l0).n / (l.n)
    //focal point is [0--it defines the plan
    // back is normal to the plane then
    
    double d = (focalPoint-r.ori).dot(Back)/(r.dir.dot(Back));
    vec3d target(d*r.dir+r.ori);
    
    //find the amount to move the lens by... //Jitter happened here.
    vec3d lensJitter = rectAdjust(Back,confRad*2,confRad*2);
    //target is the point on the focal plane the rays need to converge through
    // the new source is given by the lensJitter + imLoc;
    r.ori=imLoc+lensJitter;
    r.dir = (target-r.ori).normal();
    
    r.depth=0;
    r.ior=1;
    return r;
}

/*
 Get ray is a method which returns a ray whose origin is in the right place which passes through a certain pixel of the pixel frame.
 When using depth of field the
 The two tasks too perform are 1, choose an origin, 2, choose a direction
 The origin lies on the imaging plane,
 
 Depth of field approximations are nominally done by:
 Find calculate eye through image plane line
 then find the claculated image plane through focal point line;
 with a fixed focal point we can then jitter on the image plane, this in turn yeilds the correct behavior.
 location on the image plane is given by: imagePlane = center point of the image plane
 imagePlane - width/2 = left edge
 imagePlane - height/2 = bottom edge
 + instead is upper edges
 then goal: convert pixels => real units:
 pixels/pixelTotal*realUnitsTotal
 */
ray camera::getRay(int px, int py){
    //find the center of the pixel we're going to be collecting light from
  double xLoc = (px+.5+linearAdjust(.5))/(double)x*width;
  double yLoc = (py+.5+linearAdjust(.5))/(double)y*height;
  vec3d imLoc = imBL+Up*yLoc+Right*xLoc;
    
    return makeRay(imLoc);
};

ray camera::getRayStrat(int px, int py, int subray, int subrayMax){
    //find the center of the pixel we're going to be collecting light from
    int subx = subray%subrayMax;
    int suby = subray/subrayMax;
    double xLoc = ((px) +(subx+.5 +linearAdjust(.5))/(double)subrayMax )/(double)x*width;
    double yLoc = ((py) +(suby+.5+linearAdjust(.5))/(double)subrayMax )/(double)y *height;
    vec3d imLoc = imBL+Up*yLoc+Right*xLoc;
    return  makeRay(imLoc);
    //this jitter needs to be moved into the Up, Right basis:
};
//takes fovy in radians.
camera::camera(vec3d cameraPlane, double fovy, vec3d focalPoint, vec3d up, int px, int py, double aspectRatio, double ccdHeight,double allowConf, double depthOfField){
    //The first parameter sets the loction of the plane where we will capture the iamge
    this->imagePlane=cameraPlane;
    //the direction headed backwards is the line from the target to the center of the camera plane
    Back = (imagePlane-focalPoint).normal();
    Right = Back.cross(up).normal();
    Up = Right.cross(Back).normal(); // make sure up is ortho to the other factors.
    //the actual eye is shifted back from the camera plane by enough to make the scene frow at the correct rate.
    eyePos=ccdHeight/2/tan(fovy/2)*Back+cameraPlane;
    this->focalPoint=focalPoint;
    this->x=px;
    this->y=py;
    //TODO: switch this up: currently defines sensor width/height, switch to defining the width/height at the image plane...
    //
    this->height=ccdHeight;
    this->width=height*aspectRatio;

    
    //the maximum circle of confusion at the lens (ie, how much a lens centered pixel needs to be moved) is calcualtable from the depth of field terms:
    //confusion size is the diameter of confusion
    //1/2allowConf = radius, + dist =triangle: tan = allow/2 / dist then tan*fd-cp = allow conf.
    this->confRad = (allowConf/2.)/depthOfField*(cameraPlane-focalPoint).magnitude();
    imBL=imagePlane-width/2*Right-height/2*Up;
    irradience = new vec3d[px*py];
    pixels = new unsigned char [px*py*3];
}

int camera::getX(){
    return x;
}

int camera::getY(){
    return y;
}