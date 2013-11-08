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
#include "RTMath.h"
//#include <GLUT/glut.h>

using namespace std;

class RayTracer{
public:
    RayTracer();
    ~RayTracer(void);
    bool render(int objType, const char* filePath, ImageIO* texture);
    int getWidth();
    int getHeight();
    float*** getImage();
    
private:
    //char magic_number[2];
    int width;
    int height;
    int max;
    ImageIO* myImage;
};

#endif /* defined(__Ray_Tracer__RayTracer__) */
