//
//  Texture.h
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 12/5/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#ifndef __Ray_Tracer__Texture__
#define __Ray_Tracer__Texture__

#include <iostream>
#include "Vec3.h"
#include "ImageIO.h"

using namespace std;

class Texture{
    public:
    Texture();
    Texture(ImageIO* textureImage);
    Texture(const Vec3 &stripColor1, const Vec3 &stripColor2, double stripW);
    ~Texture(void);
    Vec3 getColor(float s, float t);
    bool getInvalid();
    
    private:
    ImageIO* image;
    float*** imageTexture;
    int height;
    int width;
    Vec3 strip1;
    Vec3 strip2;
    double stripWidth;
    bool invalid;
};


#endif /* defined(__Ray_Tracer__Texture__) */
