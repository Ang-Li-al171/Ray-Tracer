//
//  main.cpp
//  C++_2
//
//  Created by Ang Li on 10/18/13.
//  Copyright (c) 2013 Ang Li. All rights reserved.
//

#include <unistd.h>
#include <iostream>
#include <math.h>
#include "ImageIO.h"
#include "Texture.h"
#include "Filter.h"
#include "RayTracer.h"
#include "Vec3.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

float win_width = 512;
float win_height = 512;

ImageIO* current_image;

void init( void ){
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
	
}

void idle( void ){
    
    //glutPostRedisplay();
    
}

void drawImage( void ){

    if(current_image != NULL){
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2i(0, 0);
        glDrawPixels(current_image->getWidth(), current_image->getHeight(), GL_RGB,
                     GL_FLOAT, current_image->getImageDisplayArray());
    }

}

void display( void )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0, win_width, 0, win_height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawImage();
    
    glFlush();
}

void reshape( int w, int h )
{
    win_width = w;
    win_height = h;
    glViewport( 0, 0, win_width, win_height );
    
    glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {
    switch(key) {
        case 27: // Escape key
            exit(0);
            break;
    }
}

const char* appendWithCWD(const char* fileName){
    char buffer[200];
    getcwd( buffer, 200);
    std::string file = std::string(buffer) + fileName;
    const char * filePath = file.c_str();
    return filePath;
}

int main(int argc, char * argv[])
{
    
    int objListSize = 10;
    
    // import the images used for texturing
    
    ImageIO** texList = new ImageIO*[objListSize];
    texList[0] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/WoodenFloor.ppm");
    texList[1] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/RoomBG.ppm");
    texList[1] = texList[1]->scale(1.5, 1.5);
    texList[2] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/RoomFront2.ppm");
    texList[3] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/WallTextureTiled.ppm");
    texList[4] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/WallTexture2.ppm");
    texList[5] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/ball.ppm");
    texList[6] = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/Earth.ppm");
    
    TObject** objectList = new TObject*[objListSize];
    
      // refractive ball
      objectList[0] = new Sphere(Vec3(-55, -50, 100), 30, 0.0, 0.8, true);
    
//    // number 8 ball
//    objectList[1] = new Sphere(Vec3(150, -145, 0), 50, Vec3(0, 0, 0), Vec3(0, 0, 0),
//                               0, 0,
//                               texList[5]->getImage(), texList[5]->getWidth(),
//                               texList[5]->getHeight(), true);
//
    Texture* tex = new Texture(texList[6]);
    objectList[1] = new Cylinder(Vec3(0, 0, 0), 50, 90, Vec3(0, 1, 0), Vec3(0, 0, 0), Vec3(0, 0, 0),0, 0, tex, true);

    // reflection ball in the foreground
    objectList[2] = new Sphere(Vec3(-50, -150, 50), 50, 0.7, 0, true);
    
    // IMPORTANT: define x and y directions for the plane, x is rightwards, y is DOWNWARDS
    //room front
    objectList[3] = new Plane(Vec3(0, texList[2]->getHeight()/2-200,700), Vec3(0, 0, -1),
                              Vec3(-1, 0, 0), Vec3(0, -1, 0),
                              texList[2]->getWidth(), texList[2]->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texList[2]->getImage(), false);
    //wooden floor
    objectList[4] = new Plane(Vec3(0, -200, 0), Vec3(0, 1, 0),
                              Vec3(1, 0, 0), Vec3(0, 0, 1),
                              texList[0]->getWidth(), texList[0]->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texList[0]->getImage(), false);
    //roomBG
    objectList[5] = new Plane(Vec3(0, texList[1]->getHeight()/2-200, -350), Vec3(0, 0, 1),
                              Vec3(1, 0, 0), Vec3(0, -1, 0),
                              texList[1]->getWidth(), texList[1]->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texList[1]->getImage(), false);
    //leftWall
    objectList[6] = new Plane(Vec3(-400, texList[3]->getHeight()/2-200, 0), Vec3(1, 0, 0),
                              Vec3(0, 0, -1), Vec3(0, -1, 0),
                              texList[3]->getWidth(), texList[3]->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texList[3]->getImage(), false);
    //rightWall
    objectList[7] = new Plane(Vec3(400, texList[3]->getHeight()/2-200, 0), Vec3(-1, 0, 0),
                              Vec3(0, 0, 1), Vec3(0, -1, 0),
                              texList[3]->getWidth(), texList[3]->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texList[3]->getImage(), false);
    //roof
    objectList[8] = new Plane(Vec3(0, texList[1]->getHeight(), 0), Vec3(0, -1, 0),
                              Vec3(1, 0, 0), Vec3(0, 0, -1),
                              texList[4]->getWidth(), texList[4]->getHeight(),
                              Vec3(0, 0, 0), Vec3(0, 0, 0),
                              0.0, 0.0, texList[4]->getImage(), false);
    
    // glass ball
    objectList[9] = new Sphere(Vec3(-150, -150, -100), 50, Vec3(0, 0, 0), Vec3(0, 0, 0),
                               0, 0,
                               texList[6]->getImage(), texList[6]->getWidth(),
                               texList[6]->getHeight(), true);
    
//    // mirror
//    objectList[10] = new Plane(Vec3(-100, 0, 100), Vec3(0, 0, 1),
//                              Vec3(1, 0, 0), Vec3(0, -1, 0),
//                              150, 150,
//                              0.9, 0.0, false);
    
    // apply ray tracing and write output image to a file
    RayTracer trial1 = RayTracer();
    trial1.render(1, "/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/testTraceSphere.ppm", objectList, objListSize, 0, 5);
    
    // read the ray traced image back and display it
    ImageIO * the_image = new ImageIO("/Users/Sherry/Desktop/Academics/Compsci 344/Final Project/Ray-Tracer/DerivedData/Ray Tracer/Build/Products/Debug/testTraceSphere.ppm");
    current_image = the_image;
    
    
    win_height = current_image->getHeight();
    win_width = current_image->getWidth();
    
    glutInit( &argc, argv );
    
    glutInitDisplayMode( GLUT_RGB );
    glutInitWindowSize( win_width, win_height );
	
    glutCreateWindow( "Image" );
    
    init();
	
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( idle );
    
    
    glutMainLoop();
    
    delete the_image;
    delete current_image;
    for (int i=0; i<objListSize; i++){
        delete objectList[i];
        delete texList[i];
    }
    
    return 0;
}
