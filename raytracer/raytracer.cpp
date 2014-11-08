#include "raytracer.h"
#include <stdlib.h>
#include <cmath>

raytracer::raytracer(int maxDepth, int distributionRays){
    this->maxDepth=maxDepth;
    this->distributionRays=distributionRays;
}

void raytracer::addCamera(camera c){
    cameras.push_back(c);
}

void raytracer::addObject(object * o){
    o->id=scene.size();
    scene.push_back(o);
}
void raytracer::addLight(object * o){
    o->id=scene.size();
    lights.push_back(o);
}

/*
To move a ray, take the ray, search the scene graph for objects which the ray might hit.
Call get color on the ray and collision property to figure out id the ray actually collided with anything.
 */
void raytracer::castRay(ray &r){
  collParam cp;
    cp.t0=0;
    cp.t1=INFINITY;
  collParam best;
  best.dist = -1;
    bool col=false;;
  for(object * o : scene){
    if(o->collide(r, cp)){
      if(cp.dist<best.dist || best.dist <0)
          best=cp;
        col=true;
    }
  }
  if(col){
    getColor(best, r);
    //r.irrad=best.m.diffuse;
    }
    else
        r.irrad=vec3d(0);
}


//check the material to figure out how to interact with it;
/*
  Given a particular collision and a ray, compute reflection, refraction, or diffuse coloring properties.
 */
void raytracer::getColor(collParam cp, ray &r){
  //color is equal to irradience
  if(cp.dist<0){
    r.irrad = vec3d(0,0,0);
  }
  else if(cp.m.reflective && r.depth < maxDepth){
    reflect(cp, r);
      r.irrad*=cp.m.mirror;
  }
  else if(cp.m.refractive && r.depth < maxDepth){
    refract(cp, r);
  }
  else{
    irradience(cp, r);
  }
};

/*
Reflects a given ray, implments glossy reflections
 */
void raytracer::reflect(collParam cp, ray & r){
  vec3d reflDir = r.dir - cp.norm*((r.dir.dot(cp.norm))*2);
  r.dir = reflDir;
    r.ori = cp.p;
    //r.dir = squareInterpol(r.dir, cp.m.glossiness);
  r.jitterOut();
  r.depth+=1;
  castRay(r);
}

//TODO:IMPLMENT REFRACTION
void raytracer::refract(collParam cp, ray & r){
}


/*
  A function which takes a point and an objectID and returns to you the total irradience
  from that point

  features soft shadows(renders in a square intorpolated region about the light)
 */
void raytracer::irradience(collParam init, ray& initR){
  collParam cp;
  ray r;
  vec3d irrad;
  bool collided;
  vec3d n = init.norm.normal();
  vec3d h;
  for(object * l : lights){
  //  r.dir = (l->pos-init.p).normal();
    r.ori = init.p;
    r.dir =  (l->within()-r.ori);//squareInterpol(r.dir, l->boundingCube); //old version
    cp.t1=(r.dir).magnitude();
    r.dir.normalize();
    r.jitterOut();
    cp.t0=0;
 
    collided=false;
    for(object * o : scene){
      if(o->id!=init.objectId && o->id!=l->id){
          if(o->collide(r, cp)){
              collided=true;
              break;
          }
      }
    }
    if(!collided){
      h = (r.dir.normal() + (-1*initR.dir).normal()).normal();
        irrad+=(init.m.diffuse*(l->m.radiant))*((0>n.dot(r.dir))?0:n.dot(r.dir)) +
        (init.m.specular*(l->m.radiant))*pow((0>n.dot(h))?0:n.dot(h),init.m.specularHardness);
      //  cout<<pow(n.dot(h),10)<<"\n";
    }
  }
  initR.irrad = irrad;
}




/*
To render a scene we need to perform two main tasks:
1) Fire all the rays from the camera at things
2) collect the returned rays and map them to real colors.

Implmentation model:
 for each pixel, get set of rays from the camera,
 fire,
 accumulate,
 map;
 */
unsigned char * raytracer::render(camera c){
    ray r;
    for(int i=0; i<c.getX(); i++){
        for(int j=0; j<c.getY(); j++){
            for(int k=0; k<distributionRays; k++){
                r = c.getRay(i, j);
                if(i==50 && j ==450){
                    r;
                }
                castRay(r);
                c.irradience[i+c.getX()*j]+=r.irrad;
            }
            c.irradience[i+c.getX()*j]/=distributionRays;
        }
        cout<<"end ray block: "<<i<<"\n";
    }
    c.cutLinearMap(1);
    return c.pixels; // still need to assign the correct return format to this function.
}
