//
//  RayTracer.h
//  Ray Tracer
//
//  Created by Ang Li on 11/1/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__RayTracer__
#define __Ray_Tracer__RayTracer__

#include <iostream>
#include "ImageIO.h"
#include "Sphere.h"
#include "RayTracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
//#include <GLUT/glut.h>

using namespace std;

class RayTracer{
public:
    RayTracer();
    ~RayTracer(void);
    bool render(int objName, const char* filePath,
                TObject* objectList[], int numObj,
                int projectionType, int depth);
    int getWidth();
    int getHeight();
    float*** getImage();
    Vec3 trace(Vec3 origin, Vec3 d, TObject** sphereList, int size, int depth);
    
private:
    //char magic_number[2];
    int width;
    int height;
    int max;
    int MAX_RAY_DEPTH;
    ImageIO* myImage;
};

#endif /* defined(__Ray_Tracer__RayTracer__) */
