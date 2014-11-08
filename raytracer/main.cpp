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
#include "triangle.h";

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
            tmp[(height-j)*width*3+i] = pixels[j*width*3+i];
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
    raytracer r(10, 100);
    vec3d eye(10,2,0);
    vec3d lookat(0,0,0);
    vec3d up(0,1,0);
    
    camera c(eye, 3.14159/3., lookat, up, 500, 500, .00025, .00025,.025/500.,.025/500);
    
    material red;
    red.diffuse=vec3d(1,0,0);
    red.specular=vec3d(1,1,1);
    material ref =red;
    ref.reflective=true;
    ref.mirror=vec3d(1,1,1);
    //r.addObject(new sphere(vec3d(0),4,red));
        r.addObject(new triangle(vec3d(0,0,-3),vec3d(0,10,0),vec3d(0,0,3),red));
    //r.addObject(new sphere(vec3d(0,8,0),4,ref));
    
    material white;
    white.diffuse=vec3d(1,1,1);
    white.specular=vec3d(1,1,1);
        r.addObject(new sphere(vec3d(0,-1005,0),1000,white));
    
    
    material light;
    light.radiant=vec3d(1,1,1);
    light.specular=vec3d(1,0,0);
    r.addLight(new sphere(vec3d(0,25,0),1,light));
    r.addLight(new sphere(vec3d(10,15,0),1,light));
//    r.addLight(new sphere(vec3d(2,-25,0),5,light));
//    r.addLight(new sphere(vec3d(10,0,10),1,light));
//    r.addLight(new sphere(vec3d(-10,0,10),1,light));
    unsigned char * pixels = r.render(c);
    for(int i=0;i<500*500*3; i++){
        if(pixels[i]>0)
            i;
    }
    writePPM("./render.ppm",pixels,500,500);
    
    return 0;
}
