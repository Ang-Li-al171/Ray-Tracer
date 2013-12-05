//
//  Texture.cpp
//  Ray Tracer
//
//  Created by Sherry Wenshun Liu on 12/5/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include "Texture.h"


Texture::Texture() {
    image = NULL;
    invalid = true;
}

Texture::Texture(ImageIO* textureImage) {
    image = textureImage;
    imageTexture = textureImage->getImage();
    height = image->getHeight();
    width = image->getWidth();
    invalid = false;
}

Texture::Texture(const Vec3 &stripColor1, const Vec3 &stripColor2, double stripW) {
    strip1 = stripColor1;
    strip2 = stripColor2;
    stripWidth = stripW;
    invalid = false;
}

Texture::~Texture() {
    //TODO
}

Vec3 Texture::getColor(float s, float t) {
    int v = t*height;
    int u = s*width;
    if (v < 0) v = 0;
    if (u < 0) u = 0;
    
    return Vec3(imageTexture[v][u][0],
         imageTexture[v][u][1],
         imageTexture[v][u][2]);
    
}

bool Texture::getInvalid() {
    return invalid;
}

