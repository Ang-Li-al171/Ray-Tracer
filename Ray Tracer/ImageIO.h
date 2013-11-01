//
//  ImageIO.h
//  Resolution
//
//  Created by Tabitha Peck on 7/23/13.
//  Copyright (c) 2013 Tabitha Peck. All rights reserved.
//

#ifndef __Resolution__ImageIO__
#define __Resolution__ImageIO__

#include <iostream>
#include "Filter.h"

#include <GLUT/glut.h>

using namespace std;

class ImageIO{
public:
    ImageIO(int w, int h, int max);
    ImageIO(const char* file_name);
    ~ImageIO(void);
    void writeImage(const char* file_name);
    int getWidth();
    int getHeight();
    float*** getImage();
    float* getImageDisplayArray();
    ImageIO* scale(double xScale, double yScale);
    ImageIO* removeRed();
    ImageIO* removeGreen();
    ImageIO* removeBlue();
    ImageIO* grayScale();
    ImageIO* combineWith(ImageIO* otherPic, int posX, int posY);
    ImageIO* blur(Filter* f);
    ImageIO* extendEdge(ImageIO* imageToExtend,int r);

private:
    char magic_number[2];
    int width;
    int height;
    int max;
    float*** image;
    float* imageDisplayArray;

};

#endif /* defined(__Resolution__ImageIO__) */
