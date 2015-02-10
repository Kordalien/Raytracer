#include "raytracer.h"
#include <stdlib.h>
#include <cmath>

raytracer::raytracer(int maxDepth, int distributionRays){
    this->maxDepth=maxDepth;
    this->distributionRays=distributionRays;
    this->subRays=sqrt(distributionRays);
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
      vec3d dir;
    getColor(best, r);
      r.lastD=best.dist;
      r.dir=dir;
    //r.irrad=best.m.diffuse;
    }
  else{
        r.irrad=vec3d(0);
      r.lastD=-1;
  }
}


//check the material to figure out how to interact with it;
/*
  Given a particular collision and a ray, compute reflection, refraction, or diffuse coloring properties.
 */
void raytracer::getColor(collParam cp, ray &r){
  //color is equal to irradience
  if(cp.dist<0){
    r.irrad = vec3d(0,0,0);
      return;
  }
    ray reflected =r;
    ray refracted =r;
  if(cp.m.reflective && r.depth < maxDepth){
    reflect(cp, reflected);
    reflected.irrad*=cp.m.mirror;
  } else{
      reflected.irrad=vec3d(0);
  }
    
  if(cp.m.refractive && r.depth < maxDepth){
      ray rf =refracted;
      double ang = refracted.dir.dot(cp.norm);
      double c=0;
      vec3d debeers;
      bool fail=false;
      if(ang<0){ //if angle between normal and direction is less than 0
          // entering
          //currently not stacked
          debeers = cp.m.transperancy;
          //debeers=vec3d(1,1,1);
          c = (-1*refracted.dir).dot(cp.norm);
          if(refract(cp,refracted,true)){
              
          }
          else{ //in the event of toal internal refraction, reflect.
              castRay(refracted);
              refracted= rf;
              fail= true;;
          }
      }else{//then do the reflection using the stack queued reflection +embedded
          //currently exiting
          
          debeers = cp.m.debeers;
          debeers =debeers^(-refracted.lastD);
          debeers *= cp.m.transperancy;
          if(refract(cp, refracted,false)){
              c =refracted.dir.dot(cp.norm);
          }
          else{ //in the event of toal internal refraction, reflect.
            castRay(refracted);
            refracted= rf;
            fail=true;
          }
      }
      if(!fail){
    castRay(refracted);
      double n = cp.m.indexOfRefraction;
      double r0= (n-1)*(n-1)/((n+1)*(n+1));
      double R = r0 +(1-r0)*pow(1-c,5);
      if(R > 1e-5)
          reflect(cp,rf);
      else
          rf.irrad =vec3d(0,0,0);
      refracted.irrad = debeers*(R*rf.irrad+(1-R)*refracted.irrad);
      }
  } else{
      refracted.irrad=vec3d(0);
  }

    if(cp.m.transperancyAmt < 1 -1e-6)
        irradience(cp, r);
    else
        r.irrad = vec3d(0,0,0);

    r.irrad +=reflected.irrad+ cp.m.transperancyAmt*refracted.irrad;
};

/*
Reflects a given ray, implments glossy reflections
 */
void raytracer::reflect(collParam cp, ray & r){
  vec3d reflDir = r.dir - cp.norm*((r.dir.dot(cp.norm))*2);
  r.dir = reflDir.normalize();
    r.ori = cp.p;
    //r.dir = squareInterpol(r.dir, cp.m.glossiness);
  r.jitterOut();
  r.depth+=1;
  castRay(r);
}

//TImplments refraction by the schlick approximation--irrelevant for non polarized lights.
bool raytracer::refract(collParam cp, ray & r, bool entering){
    double nt = entering? cp.m.indexOfRefraction: 1;
    double n = entering? 1: cp.m.indexOfRefraction;
    vec3d norm = entering? cp.norm : cp.norm*-1;
    double det = 1-(n*n*(1-pow(r.dir.dot(norm),2)))/(nt*nt);
    
    vec3d nd = n*(r.dir-norm*(r.dir.dot(norm)))/nt - norm*sqrt(det);
    r.dir = n*(r.dir-norm*(r.dir.dot(norm)))/nt - norm*sqrt(det);
    r.dir.normalize();
    r.depth+=1;
    r.ori=cp.p;
    r.jitterOut();
    if(det<0){
        int a =1;
    }
    return det>0;
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
    double falloff;
    vec3d sv = vec3d(1,1,1);
    vec3d diff;
    if (init.m.shading == init.m.TEXTURE)
        diff = init.m.texture(init);
    else
        diff = init.m.diffuse;
  for(object * l : lights){
  //  r.dir = (l->pos-init.p).normal();
    r.ori = init.p;
    r.dir =  (l->within()-r.ori);
    cp.t1=(r.dir).magnitude();
    r.dir.normalize();
    r.jitterOut();
    cp.t0=0;
    collided=false;
    sv = vec3d(1,1,1);
    for(object * o : scene){
      if(o->id!=init.objectId && o->id!=l->id){
          if(o->collide(r, cp)){
                  if(!cp.m.cheapTransperancy){
                      collided=true;
                      break;
                  }
                  else{
                  sv *= cp.m.transperancyAmt*cp.m.diffuse.normalMax();
                  }
          }
      }
    }
    if(!collided){
        double dst = cp.t1;
        if(l->m.hasfallOff)
            falloff = std::min(1.0,(1/vec3d(1,dst,dst*dst)).dot(l->m.fallOff));
        else
            falloff =1;
      h =  (r.dir.normal() + (-1*initR.dir).normal()).normal();
          irrad+=  l->m.radiant*sv*falloff*(
                                            (1-init.m.transperancyAmt)*diff*std::max(0.0,n.dot(r.dir))
                                            +(init.m.specular)* pow(std::max(0.0,n.dot(h)),init.m.specularHardness)
                                              );
      //  cout<<pow(n.dot(h),10)<<"\n";
    }
  }
    irrad+= (1-init.m.transperancyAmt)*init.m.ambient*init.m.diffuse;
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
                r = c.getRayStrat(i, j,k,subRays);
                if(i==50 && j ==450){
                    r;
                }
                r.px = i;
                r.py=j;
                castRay(r);
                c.irradience[i+c.getX()*j]+=r.irrad;
            }
            c.irradience[i+c.getX()*j]/=distributionRays;
        }
        cout<<"end ray block: "<<i<<"\n";
    }
    c.flatMap(1);
    return c.pixels; // still need to assign the correct return format to this function.
}
