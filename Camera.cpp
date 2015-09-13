#include "Camera.h"
#include <cstring>
#include <iostream>

using namespace std;

Camera::Camera()    //Constructor
{
    eyeX = 0.0;
    eyeY = 0.0;
    eyeZ = 0.0;
    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;
    upX = 0.0;
    upY = 1.0;
    upZ = 0.0;
}

Camera::~Camera()    //Destructor
{
    //cout << "Destructor Call�_n";
}

double Camera::incX()    //Move Eye in X direction
{
    eyeX++;
    return eyeX;
}

double Camera::incY()    //Move Eye in Y direction
{
    eyeY++;
    return eyeY;
}

double Camera::incZ()        //Move Eye in Z direction
{
    eyeZ++;
    return eyeZ;
}
double Camera::decX()    //Move Eye in X direction
{
    eyeX--;
    return eyeX;
}

double Camera::decY()    //Move Eye in Y direction
{
    eyeY--;
    return eyeY;
}

double Camera::decZ()        //Move Eye in Z direction
{
    eyeZ--;
    return eyeZ;
}
void Camera::printCamera() {
 cout << "Current Camera Coordinates�F" << endl;
 cout << "Camera X�F" << eyeX << endl;
 cout << "Camera Y�F" << eyeY << endl;
 cout << "Camera Z�F" << eyeZ << endl;
 cout << "Center X�F" << centerX << endl;
 cout << "Center Y�F" << centerY << endl;
 cout << "Center Z�F" << centerZ << endl;
 cout << "Up X�F" << upX << endl;
 cout << "Up Y�F" << upY << endl;
 cout << "Up Z�F" << upZ << endl;
 //cout << "\n" << endl;
}

void Camera::resetCamera() {
    eyeX = 0.0;
    eyeY = 0.0;
    eyeZ = 0.0;
    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;
    upX = 0.0;
    upY = 1.0;
    upZ = 0.0;
}
