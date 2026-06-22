//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "triangle.hpp"
#include "BB.hpp"


// Function to compute barycentric coordinates
Vector Triangle::computeBarycentrics (Point p) {
    Vector bary;
        //v0v1 = edge1
        //v0v2 = edge2
    Vector v1p  = v1.vec2point (p);
    
    float areaABC = area(); // Triangle area

    // Compute lambda1
    Vector v2p  = v2.vec2point(p);
    Vector n1 = { edge3.Y * v2p.Z - edge3.Z * v2p.Y,
        edge3.Z * v2p.X - edge3.X * v2p.Z,
        edge3.X * v2p.Y - edge3.Y * v2p.X };

    float areaPBC = n1.norm() / 2.0f;
    bary.X = areaPBC / areaABC;

    // Compute lambda2
    Vector v3v1 = -1.f * edge2;
    Vector v3p  = v3.vec2point(p);
    Vector n2 = { v3v1.Y * v3p.Z - v3v1.Z * v3p.Y,
        v3v1.Z * v3p.X - v3v1.X * v3p.Z,
        v3v1.X * v3p.Y - v3v1.Y * v3p.X };

    float areaPCA = n2.norm() / 2.0f;
    bary.Y = areaPCA / areaABC;

    // Compute lambda3
    bary.Z = 1.0f - bary.X - bary.Y;
    
    return bary;
}

// Function to map texture coordinates using barycentric coordinates
Vec2 Triangle::interpolateTexture(Vector baryCoord) {
    Vec2 uv;
    uv.u = baryCoord.X * uv1.u + baryCoord.Y * uv2.u + baryCoord.Z * uv3.u;
    uv.v = baryCoord.X * uv1.v + baryCoord.Y * uv2.v + baryCoord.Z * uv3.v;
    return uv;
}
// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Moller Trumbore intersection algorithm
bool Triangle::intersect(Ray r, Intersection *isect) {

    if (!bb.intersect(r)) {
        return false;
    }

    Point tv1 = v1;
    Point tv2 = v2;
    Point tv3 = v3;

    Vector te1 = edge1;
    Vector te2 = edge2;
    Vector tnormal = normal;

    if (deforming) {
        tv1 = v1 + r.time * mv1;
        tv2 = v2 + r.time * mv2;
        tv3 = v3 + r.time * mv3;

        te1 = tv1.vec2point(tv2);
        te2 = tv1.vec2point(tv3);

        tnormal = te1.cross(te2);
        tnormal.normalize();
    }

    const float par = tnormal.dot(r.dir);
    if ((BackFaceCulling && par > -EPSILON) || (!BackFaceCulling && std::abs(par) < EPSILON)) {
        return false;
    }

    Vector h, s, q;
    float a, ff, u, v;

    h = r.dir.cross(te2);
    a = te1.dot(h);

    if (std::abs(a) < EPSILON) {
        return false;
    }

    ff = 1.0f / a;
    s = tv1.vec2point(r.o);
    u = ff * s.dot(h);
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    q = s.cross(te1);
    v = ff * r.dir.dot(q);
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    float t = ff * te2.dot(q);
    if (t > EPSILON) {
        Point pHit = r.o + t * r.dir;

        Vector wo = -1.0f * r.dir;
        Vector const for_normal = tnormal.Faceforward(wo);

        isect->p = pHit;
        isect->gn = for_normal;
        isect->sn = for_normal;
        isect->wo = wo;
        isect->depth = t;
        isect->FaceID = -1;
        isect->pix_x = r.pix_x;
        isect->pix_y = r.pix_y;
        isect->incident_eta = r.propagating_eta;

        float w = 1.0f - u - v;
        isect->TexCoord.u = w * uv1.u + u * uv2.u + v * uv3.u;
        isect->TexCoord.v = w * uv1.v + u * uv2.v + v * uv3.v;
        isect->time = r.time;
        return true;
    }

    return false;
}

bool Triangle::isInside(Point p) {
    /* Calculate area of this triangle ABC */
    float A = area ();
  
    /* Calculate area of triangle p v1 v2 */
    float A1 = points_area (p, v1, v2);
    /* Calculate area of triangle p v2 v3 */
    float A2 = points_area (p, v2, v3);
    /* Calculate area of triangle p v3 v1 */
    float A3 = points_area (p, v3, v1);
    
   /* Check if sum of A1, A2 and A3 is same as A */
   return (A == A1 + A2 + A3);
}
