//
//  Sphere.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 08/02/2025.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "geometry.hpp"
#include "vector.hpp"
#include "MotionBlur.hpp"
#include "algorithm"
#include <math.h>

class Sphere: public Geometry {
public:
    Point C;
    float radius;
    float radiusSq;
    BB bb;      // sphere bounding box
                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
    bool moving = false;
    Vector motion = Vector(0.f, 0.f, 0.f);
    
    bool intersect (Ray r, Intersection *isect);
    
    Sphere(Point _C, float _r): C(_C), radius(_r) {
        radiusSq = radius * radius;
        bb.min.set(C.X-radius, C.Y-radius, C.Z-radius);
        bb.max.set(C.X+radius, C.Y+radius, C.Z+radius);
    }

    void setMotion (Vector m, float shutterOpen = ::shutterOpen, float shutterClose = ::shutterClose) {
        motion = m;
        moving = true;

        // update bounding box for motion blur
        Point C_open = C + shutterOpen * motion;
        Point C_close = C + shutterClose * motion;
        
        bb.min.set(
            std::min(C_open.X, C_close.X) - radius,
            std::min(C_open.Y, C_close.Y) - radius,
            std::min(C_open.Z, C_close.Z) - radius
        );

        bb.max.set(
            std::max(C_open.X, C_close.X) + radius,
            std::max(C_open.Y, C_close.Y) + radius,
            std::max(C_open.Z, C_close.Z) + radius
        );
    }

};


#endif /* Sphere_hpp */
