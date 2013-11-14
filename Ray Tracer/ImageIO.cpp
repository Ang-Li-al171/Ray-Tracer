//
//  ImageIO.cpp
//  Resolution
//
//  Created by Tabitha Peck on 7/23/13.
//  Copyright (c) 2013 Tabitha Peck. All rights reserved.
//  based on code from stack overflow: http://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c


#include "ImageIO.h"
#include <stdio.h>
#include <stdlib.h>

#include <GLUT/glut.h>

ImageIO::ImageIO(int w, int h, int m){
    width = w;
    height = h;
    max = m;
    image = new float**[height];
    for(int i = 0; i < height; i++){
        image[i] = new float*[width];
        for(int j = 0; j < width; j++){
            image[i][j] = new float[3];
            image[i][j][0] = 0;
            image[i][j][1] = 0;
            image[i][j][2] = 0;
        }
    }
    imageDisplayArray = NULL;
}

ImageIO::ImageIO(const char* file_name){
    
    FILE* file;
    char buff[16];
    float r, g, b;
    
    file = fopen(file_name, "r"); // open file for reading
    
    if(!file){
        fprintf(stderr, "Unable to open file %s", file_name);
        exit(1);
    }
    
    fscanf(file, "%s%*[^\n]%*c", magic_number); //read magic number and white space
    
    if(magic_number[0] != 'P' || magic_number[1] != '3'){
        printf("Incorrect file type");
        exit(1);
    }
    
    //check for comments
    fscanf(file, "%s", buff);
    while (strncmp(buff, "#", 1) == 0) {
        fscanf(file, "%s%*[^\n]%*c", buff);
    }
    
    if (fscanf(file, "%d %d %d", &width, &height, &max) != 3) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", file_name);
        exit(1);
    }
    
    image = new float **[height];
    for (int i=0; i<height; i++){
        image[i] = new float*[width];
        for (int j=0; j<width; j++){
            image[i][j] = new float[3];
            if (fscanf(file, "%f %f %f", &r, &g, &b) != 3){
                fprintf(stderr, "Invalid pixel reading");
                exit(1);
            }
            image[i][j][0] = r/255;
            image[i][j][1] = g/255;
            image[i][j][2] = b/255;
        }
    }
    
    fclose(file);
    imageDisplayArray = NULL;
}

ImageIO::~ImageIO(void){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            delete [] image[i][j];
        }
        delete [] image[i];
    }
    delete [] image;
    
    delete [] imageDisplayArray;
}

// apply the given filter with the original image
// assumes the filter is applied in both directions, horizontally first and then vertically
ImageIO* ImageIO::blur(Filter* f){
    ImageIO* extendedImage = extendEdge(this, f->getR());
    ImageIO* blurredHorizontal = new ImageIO(width, height, max);
    float*** extended = extendedImage->image;
    float*** blurredH = blurredHorizontal->image;
    double* filterArray = f->getFilterArray();
    int r = f->getR();
    
    // apply the filter horizontally first
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            for (int k=0;k<2*r+1;k++){
                blurredH[i][j][0] += extended[i+r][j+k][0] * filterArray[k];
                blurredH[i][j][1] += extended[i+r][j+k][1] * filterArray[k];
                blurredH[i][j][2] += extended[i+r][j+k][2] * filterArray[k];
            }
        }
    }
    
    extendedImage = extendEdge(blurredHorizontal, f->getR());
    extended = extendedImage->image;
    ImageIO* blurredBoth = new ImageIO(width, height, max);
    float*** blurredB = blurredBoth->image;
    // apply the filter vertically
    for (int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            for (int k=0;k<2*r+1;k++){
                blurredB[i][j][0] += extended[i+k][j+r][0] * filterArray[k];
                blurredB[i][j][1] += extended[i+k][j+r][1] * filterArray[k];
                blurredB[i][j][2] += extended[i+k][j+r][2] * filterArray[k];
            }
        }
    }
    return blurredBoth;
}

// extend the original image in all four directions by r pixels, preparing for convolution
ImageIO* ImageIO::extendEdge(ImageIO* imageToExtend, int r){
    ImageIO* extendedImage = new ImageIO(width+2*r, height+2*r, max);
    float*** original = imageToExtend->image;
    float*** extended = extendedImage->image;
    // copy the original image to the center
    for (int i=0; i<height;i++){
        for (int j=0;j<width;j++){
            extended[i+r][j+r][0] = original[i][j][0];
            extended[i+r][j+r][1] = original[i][j][1];
            extended[i+r][j+r][2] = original[i][j][2];
        }
    }
    // fill in the edges up, down, left, and right
    for (int i=r;i<height+r;i++){
        // left edge
        for (int j=0;j<r;j++){
            extended[i][j][0] = original[i-r][width-r+j][0];
            extended[i][j][1] = original[i-r][width-r+j][1];
            extended[i][j][2] = original[i-r][width-r+j][2];
        }
        // right edge
        for (int j=width+r;j<width+2*r;j++){
            extended[i][j][0] = original[i-r][j-width-r][0];
            extended[i][j][1] = original[i-r][j-width-r][1];
            extended[i][j][2] = original[i-r][j-width-r][2];
        }
    }
    for (int j=r;j<width+r;j++){
        // up edge
        for (int i=0;i<r;i++){
            extended[i][j][0] = original[height-r+i][j-r][0];
            extended[i][j][1] = original[height-r+i][j-r][1];
            extended[i][j][2] = original[height-r+i][j-r][2];
        }
        // down edge
        for (int i=height+r;i<height+2*r;i++){
            extended[i][j][0] = original[i-height-r][j-r][0];
            extended[i][j][1] = original[i-height-r][j-r][1];
            extended[i][j][2] = original[i-height-r][j-r][2];
        }
    }
    
    return extendedImage;
}

ImageIO* ImageIO::removeRed(){
    ImageIO* red = new ImageIO(width, height, max);

    float*** red_removed_image = red->image;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            red_removed_image[i][j][0]=0;
            red_removed_image[i][j][1]=image[i][j][1];
            red_removed_image[i][j][2]=image[i][j][2];
        }
    }
    return red;
}

ImageIO* ImageIO::removeGreen(){
    ImageIO* green = new ImageIO(width, height, max);

    float*** green_removed_image = green->image;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            green_removed_image[i][j][0]=image[i][j][0];
            green_removed_image[i][j][1]=0;
            green_removed_image[i][j][2]=image[i][j][2];
        }
    }
    return green;
}

ImageIO* ImageIO::removeBlue(){
    ImageIO* blue = new ImageIO(width, height, max);

    float*** blue_removed_image = blue->image;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            blue_removed_image[i][j][0]=image[i][j][0];
            blue_removed_image[i][j][1]=image[i][j][1];
            blue_removed_image[i][j][2]=0;
        }
    }
    return blue;
}

ImageIO* ImageIO::grayScale(){
    ImageIO* gray = new ImageIO(width, height, max);
    float*** gray_image = gray->image;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            float grey_Y = 0.2126*image[i][j][0] + 0.7251*image[i][j][1] + 0.0722*image[i][j][2];
            gray_image[i][j][0]=grey_Y;
            gray_image[i][j][1]=grey_Y;
            gray_image[i][j][2]=grey_Y;
        }
    }
    return gray;
}

// scaling the picture to (height * yScale) * (width * xScal). yScale and xScale are double values.
ImageIO* ImageIO::scale(double xScale, double yScale){
    ImageIO* scaled = new ImageIO(width*xScale, height*yScale, max);
    

    for(int i=0;i<(int)(height*yScale);i++){
        for(int j=0;j<(int)(width*xScale);j++){
            scaled->image[i][j][0]=image[(int)(i/yScale)][(int)(j/xScale)][0];
            scaled->image[i][j][1]=image[(int)(i/yScale)][(int)(j/xScale)][1];
            scaled->image[i][j][2]=image[(int)(i/yScale)][(int)(j/xScale)][2];
        }
    }
    return scaled;
}

// combined this image with ohterPic, otherPic will be drawn at posX and posY of this image.
// assuming placing otherPic at posX, posY will not go out of this image's x and y boundaries.
ImageIO* ImageIO::combineWith(ImageIO* otherPic, int posX, int posY){
    ImageIO* combined = new ImageIO(width, height, max);
    combined->image = image;
    
    // assuming the image parameter passed in is always smaller in size than original image
    for(int i=posY;i<otherPic->height+posY;i++){
        for(int j=posX;j<otherPic->width+posX;j++){
            // combine each pixel by adding the color values
            combined->image[i][j][0]+=otherPic->image[i-posY][j-posX][0];
            combined->image[i][j][1]+=otherPic->image[i-posY][j-posX][1];
            combined->image[i][j][2]+=otherPic->image[i-posY][j-posX][2];
            
            // make sure it doesn't go over the max (255 in this case)
            combined->image[i][j][0] = combined->image[i][j][0]>max?max:combined->image[i][j][0];
            combined->image[i][j][1] = combined->image[i][j][1]>max?max:combined->image[i][j][1];
            combined->image[i][j][2] = combined->image[i][j][2]>max?max:combined->image[i][j][2];
        }
    }
    
    return combined;
}

int ImageIO::getWidth(){
    return width;
}

int ImageIO::getHeight(){
    return height;
}

float*** ImageIO::getImage(){
    return image;
}

float* ImageIO::getImageDisplayArray() {
    if(imageDisplayArray != NULL)
        return imageDisplayArray;
        
    imageDisplayArray = new float[width * height * 3];
    int index = 0;
    
    //put the 3-dimensional array into a 1-dimensional array
    for(int i=height-1;i>=0;i--){
        for(int j=0;j<width;j++){
            for(int k=0;k<3;k++){
                imageDisplayArray[index++]=image[i][j][k];
            }
        }
    }
    
    return imageDisplayArray;
}

void ImageIO::writeImage(const char* file_name){
    FILE* file;
    file = fopen(file_name, "w");
    if(!file){
        fprintf(stderr, "Unable to open fileeeeeee %s", file_name);
        exit(1);
    }
    
    fprintf(file, "P3\n");
    fprintf(file, "# Created by Ang Li using fprintf. Btw, why is the Magic Number P3?...\n");
    fprintf(file, "%d %d %d\n", width, height, max);
    // your code goes here
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            fprintf(file, "%d %d %d\n", (int)(image[i][j][0]*255),
                                        (int)(image[i][j][1]*255),
                                        (int)(image[i][j][2]*255));
        }
    }
    
    fclose(file);
    
}



