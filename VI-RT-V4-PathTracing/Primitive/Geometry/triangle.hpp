#ifndef triangle_hpp
#define triangle_hpp

#include "geometry.hpp"
#include "vector.hpp"
#include "MotionBlur.hpp"
#include <algorithm>
#include <math.h>

class Triangle: public Geometry {
    Vector computeBarycentrics (Point p);
    Vec2 interpolateTexture(Vector baryCoord);

public:
    bool BackFaceCulling;
    Point v1, v2, v3;
    Vec2 uv1{0.f, 0.f}, uv2{0.f, 0.f}, uv3{0.f, 0.f};
    Vector normal;
    Vector edge1, edge2, edge3;
    BB bb;

    // NOVO
    bool deforming = false;
    Vector mv1, mv2, mv3;

    void set_vertex_motion(Vector _mv1, Vector _mv2, Vector _mv3,
                           float shutterOpen = ::shutterOpen,
                           float shutterClose = ::shutterClose) {
        deforming = true;
        mv1 = _mv1;
        mv2 = _mv2;
        mv3 = _mv3;
        update_motion_bb(shutterOpen, shutterClose);
    }

    bool intersect (Ray r, Intersection *isect);
    bool isInside(Point p);

    Triangle(Point _v1, Point _v2, Point _v3, Vector _normal, bool backface=true)
        : v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
        edge1 = v1.vec2point(v2);
        edge2 = v1.vec2point(v3);
        edge3 = v2.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z);
        bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2);
        bb.update(v3);
        BackFaceCulling = backface;
    }

    Triangle(Point _v1, Point _v2, Point _v3, bool backface=false)
        : v1(_v1), v2(_v2), v3(_v3), BackFaceCulling(backface) {
        edge1 = v1.vec2point(v2);
        edge2 = v1.vec2point(v3);
        edge3 = v2.vec2point(v3);

        normal = edge1.cross(edge2);
        normal.normalize();

        bb.min.set(v1.X, v1.Y, v1.Z);
        bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2);
        bb.update(v3);
    }

    Triangle(Point _v1, Point _v2, Point _v3,
            Vector m1, Vector m2, Vector m3,
            Vector _normal, bool backface=true)
        : v1(_v1), v2(_v2), v3(_v3), normal(_normal),
        mv1(m1), mv2(m2), mv3(m3), deforming(true) {
        edge1 = v1.vec2point(v2);
        edge2 = v1.vec2point(v3);
        edge3 = v2.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z);
        bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2);
        bb.update(v3);
        BackFaceCulling = backface;
        update_motion_bb(shutterOpen, shutterClose);
    }


    void set_uv (Vec2 _uv1,Vec2 _uv2,Vec2 _uv3) {
        uv1=_uv1;
        uv2=_uv2;
        uv3=_uv3;
    }

    float area () {
        const float len1 = edge1.norm();
        const float len2 = edge2.norm();
        const float len3 = edge3.norm();
        const float hp = (len1+len2+len3)/2.f;
        const float A = sqrtf(hp*(hp-len1)*(hp-len2)*(hp-len3));
        return A;
    }

    float points_area (Point v1, Point v2, Point v3) {
        Vector edge1 = v1.vec2point(v2);
        Vector edge2 = v2.vec2point(v3);
        Vector edge3 = v3.vec2point(v1);
        const float len1 = edge1.norm();
        const float len2 = edge2.norm();
        const float len3 = edge3.norm();
        const float hp = (len1+len2+len3)/2.f;
        const float A = sqrtf(hp*(hp-len1)*(hp-len2)*(hp-len3));
        return A;
    }

private:
    void update_motion_bb(float shutterOpen, float shutterClose) {
        Point open_v1 = v1 + shutterOpen * mv1;
        Point open_v2 = v2 + shutterOpen * mv2;
        Point open_v3 = v3 + shutterOpen * mv3;
        Point close_v1 = v1 + shutterClose * mv1;
        Point close_v2 = v2 + shutterClose * mv2;
        Point close_v3 = v3 + shutterClose * mv3;

        bb.min.set(open_v1.X, open_v1.Y, open_v1.Z);
        bb.max.set(open_v1.X, open_v1.Y, open_v1.Z);
        bb.update(open_v2);
        bb.update(open_v3);
        bb.update(close_v1);
        bb.update(close_v2);
        bb.update(close_v3);
    }
};

#endif
