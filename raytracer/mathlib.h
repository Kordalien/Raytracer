/*
  A collection of math cosntructs for the raytracer; notably a nice implmentation of:
  3d vectors;
  3d matrix's;
  4d matrix's?
*/
#include <iostream>
#include <cmath>

#ifndef MATHLIB_
#define MATHLIB_
class mat3d;
class vec3d{
 public:
  union {
    double elements[3];
    struct {double x,y,z;};
    struct {double r,g,b;};
  };

  vec3d(double x=0, double y=0, double z=0){
    elements[0]=x; elements[1]=y; elements[2]=z;
  };

  inline operator const double*() const {return elements;};
  inline double& operator[](int i){
    if (i>=0 && i<=2)
      return elements[i];
    //if you try to access a non existant element, warn and fail gracefully
    std::cout<<"Invalid vector access; returning first element."<<std::endl;
    return elements[0];  
  };
  
  inline bool operator ==(const vec3d& v) { return (this->x-v.x<1e-9 && this->x-v.x>-1e-9&&this->y-v.y<1e-9 && this->y-v.y>-1e-9&&this->z-v.z<1e-9 && this->z-v.z>-1e-9  );};

  inline vec3d& operator +=(const vec3d& v) { x+=v.x; y+=v.y; z+=v.z; return *this;};
  inline vec3d& operator -=(const vec3d& v) { x-=v.x; y-=v.y; z-=v.z; return *this;};
  inline vec3d& operator *=(const vec3d& v) { x*=v.x; y*=v.y; z*=v.z; return *this;};
  inline vec3d& operator /=(const vec3d& v) { x/=v.x; y/=v.y; z/=v.z; return *this;};
  inline vec3d& operator +=(const double& v) { x+=v; y+=v; z+=v; return *this;};
  inline vec3d& operator -=(const double& v) { x-=v; y-=v; z-=v; return *this;};
  inline vec3d& operator *=(const double& v) { x*=v; y*=v; z*=v; return *this;};
  inline vec3d& operator /=(const double& v) { x/=v; y/=v; z/=v; return *this;};

  inline vec3d operator +(const vec3d& v) { return vec3d(x+v.x, y+v.y, z+v.z);};
  inline vec3d operator -(const vec3d& v) { return vec3d(x-v.x, y-v.y, z-v.z);};
  inline vec3d operator *(const vec3d& v) { return vec3d(x*v.x, y*v.y, z*v.z);};
  inline vec3d operator /(const vec3d& v) { return vec3d(x/v.x, y/v.y, z/v.z);};
  inline vec3d operator +(const double v) { return vec3d(x+v, y+v, z+v);};
  inline vec3d operator -(const double v) { return vec3d(x-v, y-v, z-v);};
  inline vec3d operator *(const double v) { return vec3d(x*v, y*v, z*v);};
  inline vec3d operator /(const double v) { return vec3d(x/v, y/v, z/v);};
  inline vec3d operator ^(const double v) { return vec3d(pow(x,v), pow(y,v), pow(z,v));};
  //we should get warnings going from double to int... but not int to double.
  //inline vec3i operator vec3i() { return vec3i(x,y,z) };
  //1x3*3x3->1,3?
  //(the multiplication is just equal to the dot product of the transpose with this vector...)
  //inline vec3d operator *(const mat3d &v);
  //fetch the magnitude
  double magnitude() const{
    return sqrt(x*x+y*y+z*z);
  };

  //normalizes and returns; use normal to just get the normal
  vec3d normalize(){
    double m = magnitude();
    if(m!=0){
      x/=m;
      y/=m;
      z/=m;
    }
    return *this;
  };

  //if it's a zero vector, the normal is left as a zero vector
  vec3d normal() const{
    double m = magnitude();
    if(m!=0){
      return vec3d(x/m,y/m,z/m);
    }
    return vec3d();
  };
  
  //return the dot product with another vector
  double dot(const vec3d &v) const{
    return x*v.x+y*v.y+z*v.z;
  };
  

  vec3d abs() const{
    return vec3d((x>0)?x:-x,(y>0)?y:-y,(z>0)?z:-z);
  }

  //return the crossproduct of two vectors
  vec3d cross(const vec3d &v) const{
    return vec3d(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
  };
  
  //return the max value of the vector.
  double max(){
      return (x>y)? ((x>z)? x : z) : ((y>z)? y :z) ;
  };
  
};

class vec3i{
 public:
  union {
    int elements[3];
    struct {int x,y,z;};
    struct {int r,g,b;};
  };

  vec3i(int x=0, int y=0, int z=0){
    elements[0]=x; elements[1]=y; elements[2]=z;
  };

  inline operator const int*() const {return elements;};
  inline int& operator[](int i){
    if (i>=0 && i<=2)
      return elements[i];
    //if you try to access a non existant element, warn and fail gracefully
    std::cout<<"Invalid vector access; returning first element."<<std::endl;
    return elements[0];  
  };
  
  inline bool operator ==(const vec3i& v) { return (this->x==v.x&&this->y==v.y&&this->z==v.z);};

  inline vec3i& operator +=(const vec3i& v) { x+=v.x; y+=v.y; z+=v.z; return *this;};
  inline vec3i& operator -=(const vec3i& v) { x-=v.x; y-=v.y; z-=v.z; return *this;};
  inline vec3i& operator *=(const vec3i& v) { x*=v.x; y*=v.y; z*=v.z; return *this;};
  inline vec3i& operator /=(const vec3i& v) { x/=v.x; y/=v.y; z/=v.z; return *this;};
  inline vec3i& operator +=(const int& v) { x+=v; y+=v; z+=v; return *this;};
  inline vec3i& operator -=(const int& v) { x-=v; y-=v; z-=v; return *this;};
  inline vec3i& operator *=(const int& v) { x*=v; y*=v; z*=v; return *this;};
  inline vec3i& operator /=(const int& v) { x/=v; y/=v; z/=v; return *this;};


  inline vec3i operator +(const vec3i& v) { return vec3i(x+v.x, y+v.y, z+v.z);};
  inline vec3i operator -(const vec3i& v) { return vec3i(x-v.x, y-v.y, z-v.z);};
  inline vec3i operator *(const vec3i& v) { return vec3i(x*v.x, y*v.y, z*v.z);};
  inline vec3i operator /(const vec3i& v) { return vec3i(x/v.x, y/v.y, z/v.z);};
  inline vec3i operator +(const int v) { return vec3i(x+v, y+v, z+v);};
  inline vec3i operator -(const int v) { return vec3i(x-v, y-v, z-v);};
  inline vec3i operator *(const int v) { return vec3i(x*v, y*v, z*v);};
  inline vec3i operator /(const int v) { return vec3i(x/v, y/v, z/v);};
  inline vec3i operator ^(const int v) { return vec3i(pow(x,v), pow(y,v), pow(z,v));};
  inline operator vec3d() { return vec3d(x,y,z); };
  //1x3*3x3->1,3?
  //(the multiplication is just equal to the dot product of the transpose with this vector...)
  //inline vec3d operator *(const mat3d &v);
  //fetch the magnitude
  double magnitude() const{
    return sqrt(x*x+y*y+z*z);
  };


  //if it's a zero vector, the normal is left as a zero vector
  vec3d normal() const{
    double m = magnitude();
    if(m!=0){
      return vec3d(x/m,y/m,z/m);
    }
    return vec3d();
  };
  
  //return the dot product with another vector
  int dot(const vec3i &v) const{
    return x*v.x+y*v.y+z*v.z;
  };
  
  //return the crossproduct of two vectors
  vec3i cross(const vec3i &v) const{
    return vec3i(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
  };
  
  
};

class mat3d{
 public:
  vec3d rows[3];
  inline mat3d operator +(const mat3d& v) { 
    mat3d a;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	a[i][j]=v[i][j]+rows[i][j];
    return a;
  };

  inline mat3d operator -(const mat3d& v) {  
    mat3d a;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	a[i][j]=rows[i][j]-v[i][j];
    return a;
  };

  inline mat3d operator *(const mat3d& v) {
    mat3d tmp = v.transpose();
    mat3d res;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	res[i][j]=rows[i].dot(tmp[j]);
    return res;
  };

  inline vec3d operator *(const vec3d& v) { 
    return vec3d(v.dot(rows[0]), v.dot(rows[1]), v.dot(rows[2]));
  };

  inline operator const vec3d*() const {return rows;};
  inline vec3d& operator[](int i){
    if (i>=0 && i<=2)
      return rows[i];
    //if you try to access a non existant element, warn and fail gracefully
    std::cout<<"Invalid matrix 3 access; returning first row"<<std::endl;
    return rows[0];  
  };
  //default constructor is the identity matrix
  mat3d(vec3d r1 = vec3d(1,0,0), vec3d r2 = vec3d(0,1,0), vec3d r3 = vec3d(0,0,1)){
    rows[0]=r1;
    rows[1]=r2;
    rows[2]=r3;
  };
  // initialze all values to a single double value
  mat3d(double v){
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	rows[i][j]=v;
  };
  //returns the transpose
  mat3d transpose() const{
    mat3d a;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	a[i][j]=rows[j][i];
    return a;
  };
  
  //transposes the matrix and returns itself
  mat3d& transposed(){
    mat3d a;
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	a[i][j]=rows[j][i];
    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
	rows[i][j]=a[i][j];
    return *this;
  };
};


inline vec3d operator *(const double v, const vec3d& right) { return vec3d(right.x*v, right.y*v, right.z*v);};
inline vec3d operator +(const double v, const vec3d& right) { return vec3d(right.x+v, right.y+v, right.z+v);};

inline vec3d operator *(const vec3d &left, const mat3d &v) { 
  mat3d t = v.transpose();
  return vec3d(left.dot(t[0]), left.dot(t[1]), left.dot(t[2]));
};

#endif
