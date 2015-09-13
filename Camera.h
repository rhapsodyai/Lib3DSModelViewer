#ifndef _INC_CAMERA
#define _INC_CAMERA

#include <iostream>

using namespace std;

class Camera            //�N���X�錾
{
private:
    //*char name = "My Camera";
    
public:
    double eyeX, eyeY, eyeZ;
    double centerX, centerY, centerZ;
    double upX, upY, upZ;
    double Camera::incX();
    double Camera::incY();
    double Camera::incZ();
    double Camera::decX();
    double Camera::decY();
    double Camera::decZ();
    void Camera::resetCamera();
    void Camera::printCamera();
    Camera();
    ~Camera();
};

// �o�͉��Z�q.
//std::ostream& operator<<(std::ostream& lhs, const Camera& rhs);

#endif /* _INC_CAMERA */
