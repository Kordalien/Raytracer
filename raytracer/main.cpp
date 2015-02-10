//
//  main.cpp
//  raytracer
//
//  Created by Scott Cesar on 10/23/14.
//  Copyright (c) 2014 Scott Cesar. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "raytracer.h"
#include "sphere.h"
#include "cubes.h"
#include "triangle.h"

void writePPM(const char* filename, unsigned char* pixels, int width, int height)
{
    // try to open the file
    ofstream file(filename);
    if (!file)
    {
        cout << " Couldn't open file " << filename << "! " << endl;
        exit(1);
    }
    
    // write out the image dimensions
    file<< "P6\n"<<width<<" "<<height<<"\n255\n";
    // finish the printing of the pixels...
    unsigned char * tmp = new unsigned char[width*height*3];
    for(int i=0; i<width*3; i++){
        for (int j=0; j<height; j++){
            
            tmp[(height-j-1)*width*3+i] = pixels[j*width*3+i];
        }
    }

    for(int i=0; i<height*width*3; i++){
        file<<tmp[i];
    }
    delete [] tmp;
    file.close();
    // output some success information
    cout << " Successfully printed out " << filename << " with dimensions: "
    << width << " " << height << endl;
}

int main(int argc, const char * argv[]) {
    raytracer r(10, 16);
    vec3d eye(25,0,0);
    vec3d lookat(0,0,0);
    vec3d up(0,1,0);
    //vec3d cameraPlane, double fovy, vec3d focalPoint, vec3d up, int px, int py, double aspectRatio, double ccdHeight
    camera c(eye, M_PI/4., lookat, up, 500, 500, 1, .05, .01, 20);
    
    material red;
    red.diffuse=vec3d(1,0,0);
    red.specular=vec3d(0.5,0.5,0.5);
    material ref =red;
    ref.diffuse=vec3d(.7,.3,.1);
    ref.refractive=true;
    //ref.reflective=true;
    ref.mirror=vec3d(1,1,1);
    ref.debeers=vec3d(1,1,1);
    ref.transperancyAmt=1;
    ref.specularHardness=15;
    ref.transperancy = vec3d(1,1,1);
    ref.indexOfRefraction=1.52;
    ref.cheapTransperancy=true;
    r.addObject(new sphere(vec3d(0,4,0),1,red));
//        r.addObject(new triangle(vec3d(0,2,-3),vec3d(0,10,0),vec3d(0,2,3),red));
    
  //  r.addObject(new triangle(vec3d(29,0,-1),vec3d(29,2,0),vec3d(29,0,1),red));
    r.addObject(new sphere(vec3d(7,0,0),2,ref));
    
    material white;
    white.diffuse=vec3d(1,1,1);
    white.specular=vec3d(1,1,1);
    white.transperancyAmt=0;
        r.addObject(new sphere(vec3d(0,-1005,0),1000,white));
    
    
    material light;
    light.radiant=vec3d(5,5,5);
    light.specular=vec3d(.5,.5,.5);
//    r.addLight(new sphere(vec3d(5,25,0),1,light));
    r.addLight(new sphere(vec3d(20,7,0),.25,light));
//    r.addLight(new sphere(vec3d(2,-25,0),5,light));
//    r.addLight(new sphere(vec3d(10,0,10),1,light));
//    r.addLight(new sphere(vec3d(-10,0,10),1,light));
    unsigned char * pixels = r.render(c);
    for(int i=0;i<c.getX()*c.getY()*3; i++){
        if(abs(i -(250*500+250)*3) < 10 && i%3 ==0){
  //          pixels[i]=255;
        }
    }
    writePPM("./render.ppm",pixels,c.getX(),c.getY());
    
    return 0;
}
