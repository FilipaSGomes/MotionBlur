//
//  Sphere.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 08/02/2025.
//

#include <stdio.h>
#include "Sphere.hpp"

bool Sphere::intersect(Ray r, Intersection *isect) {
    
    if (!bb.intersect(r)) {
        return false;
    }
    
    // from https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/simplifyingtheray-sphereintersectioncode
    // centro da esfera neste instante do raio para o motion blur
    Point Ct = C; // para o caso de não haver motion blur
    if (moving) {
        Ct = C + r.time * motion;
    }

    //intersecçao para um centro que se move com o tempo do raio (motion blur)
    Vector oc = r.o.vec2point(Ct);

    //float a = r.dir.normSQ();
    //float a = 1.f;   // ray direction is normalized
    float h = r.dir.dot(oc);
    float c = oc.normSQ() - radiusSq;
    float discriminant = h*h - c;
    if (discriminant < EPSILON) {
        return (false);
    }
    
    // intersection distance along ray
    float t = h - std::sqrt(discriminant);
    
    if (t > EPSILON) // ray intersection
    {
        Point pHit = r.o + t* r.dir;
        Vector normal = Ct.vec2point(pHit); // normal para um centro que se move com o tempo do raio (motion blur)
        normal.normalize();
        
        // Fill Intersection data from triangle hit : pag 165
        Vector wo = -1.f * r.dir;
        // make sure the normal points to the same side of the surface as wo
        Vector const for_normal = normal.Faceforward(wo);
        isect->p = pHit;
        isect->gn = for_normal;
        isect->sn = for_normal;
        isect->wo = wo;
        isect->depth = t;
        isect->FaceID = -1;
        isect->pix_x = r.pix_x;
        isect->pix_y = r.pix_y;
        isect->incident_eta = r.propagating_eta;
        
        return true;
    }
    else  {// This means that there is a line intersection but not a ray intersection.
        return false;
    }
    
    return false;
}
 
