//
//  BuildScenes.cpp
//  VI-RT-InitialVersion
//
//  Created by Luis Paulo Santos on 11/02/2025.
//

#include "BuildScenes.hpp"
#include "DiffuseTexture.hpp"

static int AddDiffuseMat (Scene& scene, RGB const color);
static int AddMat (Scene& scene, RGB const Ka, RGB const Kd, RGB const Ks, RGB const Kt, float const eta=1.f);
static int AddTextMat (Scene& scene, std::string filename, RGB const Ka, RGB const Kd, RGB const Ks, RGB const Kt, float const eta=1.f);

static void AddSphere (Scene& scene, Point const C, float const radius,
                       int const mat_ndx, Vector const motion = Vector(0.f, 0.f, 0.f));

static void AddTriangle (Scene& scene,
                         Point const v1, Point const v2, Point const v3,
                         int const mat_ndx, Vector const motion = Vector(0.f, 0.f, 0.f));

static void AddTriangle (Scene& scene,
                         Point const v1, Point const v2, Point const v3,
                         Vector const m1, Vector const m2, Vector const m3,
                         int const mat_ndx);

static void AddTriangleUV (Scene& scene,
                           Point const v1, Point const v2, Point const v3,
                           Vec2 const uv1, Vec2 const uv2, Vec2 const uv3,
                           int const mat_ndx,
                           Vector const motion = Vector(0.f, 0.f, 0.f));
//static void AddMovingSphere (Scene& scene, Point const C, float const radius,
         //                    Vector const motion, int const mat_ndx);

/*static void AddMovingSphere (Scene& scene, Point const C,
                             float const radius, Vector const motion,
                             int const mat_ndx) {
    Sphere *sphere = new Sphere(C, radius);
    sphere->setMotion(motion);

    Primitive *prim = new Primitive;
    prim->g = sphere;
    prim->material_ndx = mat_ndx;
    scene.AddPrimitive(prim);
}*/

static int AddDiffuseMat (Scene& scene, RGB const color) {
    BRDF *brdf = new BRDF;
    
    brdf->Ka = color;
    brdf->Kd = color;
    brdf->Ks = RGB(0., 0., 0.);
    brdf->Kt = RGB(0., 0., 0.);
    
    return (scene.AddMaterial(brdf));
}

static int AddTextMat (Scene& scene, std::string filename, RGB const Ka, RGB const Kd, RGB const Ks, RGB const Kt, float const eta) {
    DiffuseTexture *brdf = new DiffuseTexture(filename);
    
    brdf->Ka = Ka;
    brdf->Kd = Kd;
    brdf->Ks = Ks;
    brdf->Kt = Kt;
    brdf->eta = eta;
    brdf->textured = true;
    
    return (scene.AddMaterial(brdf));
}


static int AddMat (Scene& scene, RGB const Ka, RGB const Kd, RGB const Ks, RGB const Kt, float const eta) {
    BRDF *brdf = new BRDF;
    
    brdf->Ka = Ka;
    brdf->Kd = Kd;
    brdf->Ks = Ks;
    brdf->Kt = Kt;
    brdf->eta = eta;
    
    return (scene.AddMaterial(brdf));
}

/* STATIC GEOMETRIES
static void AddSphere (Scene& scene, Point const C,
                             float const radius, int const mat_ndx, ) {
    Sphere *sphere = new Sphere(C, radius);
    Primitive *prim = new Primitive;
    prim->g = sphere;
    prim->material_ndx = mat_ndx;
    scene.AddPrimitive(prim);
}

static void AddTriangle (Scene& scene,
                         Point const v1, Point const v2, Point const v3,
                         int const mat_ndx) {
    
    Triangle *tri = new Triangle(v1, v2, v3);
    Primitive *prim = new Primitive;
    prim->g = tri;
    prim->material_ndx = mat_ndx;
    scene.AddPrimitive(prim);
}*/

static void AddTriangle (Scene& scene,
                         Point const v1, Point const v2, Point const v3,
                         int const mat_ndx, Vector const motion) {

    Triangle *tri = new Triangle(v1, v2, v3);
    if (!(motion.X == 0.f && motion.Y == 0.f && motion.Z == 0.f)) {
        tri->set_vertex_motion(motion, motion, motion);
    }

    Primitive *prim = new Primitive;
    prim->g = tri;
    prim->material_ndx = mat_ndx;

    scene.AddPrimitive(prim);
}

static void AddTriangle (Scene& scene,
                         Point const v1, Point const v2, Point const v3,
                         Vector const m1, Vector const m2, Vector const m3,
                         int const mat_ndx) {

    Triangle *tri = new Triangle(v1, v2, v3);
    tri->set_vertex_motion(m1, m2, m3);

    Primitive *prim = new Primitive;
    prim->g = tri;
    prim->material_ndx = mat_ndx;

    scene.AddPrimitive(prim);
}

static void AddSphere (Scene& scene, Point const C,
                       float const radius, int const mat_ndx,
                       Vector const motion) {

    Sphere *sphere = new Sphere(C, radius);
    if (!(motion.X == 0.f && motion.Y == 0.f && motion.Z == 0.f)) {
        sphere->setMotion(motion);
    }

    Primitive *prim = new Primitive;
    prim->g = sphere;
    prim->material_ndx = mat_ndx;

    scene.AddPrimitive(prim);
}

static void AddTriangleUV (Scene& scene,
                           Point const v1, Point const v2, Point const v3,
                           Vec2 const uv1, Vec2 const uv2, Vec2 const uv3,
                           int const mat_ndx,
                           Vector const motion) {
    
    Triangle *tri = new Triangle(v1, v2, v3);
    tri->set_uv(uv1, uv2, uv3);
    if (!(motion.X == 0.f && motion.Y == 0.f && motion.Z == 0.f)) {
        tri->set_vertex_motion(motion, motion, motion);
    }

    Primitive *prim = new Primitive;
    prim->g = tri;
    prim->material_ndx = mat_ndx;

    scene.AddPrimitive(prim);
}

void DeformQuadScene(Scene& scene) {
    int const white_mat = AddDiffuseMat(scene, RGB(0.9, 0.9, 0.9));
    int const red_mat   = AddDiffuseMat(scene, RGB(0.9, 0.2, 0.2));

    // chão
    AddTriangle(scene, Point(-20., -1.,  0.), Point( 20., -1.,  0.), Point( 20., -1., 30.), white_mat);
    AddTriangle(scene, Point(-20., -1.,  0.), Point( 20., -1., 30.), Point(-20., -1., 30.), white_mat);

    // quadrado vertical feito por 2 triângulos
    // vértices de baixo fixos, de cima movem-se
    Point p1(-2.0, 0.0, 10.0);   // baixo esq
    Point p2( 2.0, 0.0, 10.0);   // baixo dir
    Point p3( 2.0, 4.0, 10.0);   // cima dir
    Point p4(-2.0, 4.0, 10.0);   // cima esq

    Vector fixed(0.f, 0.f, 0.f);
    Vector pullCorner(2.5f, 0.f, 3.0f);

    AddTriangle(scene, p1, p2, p3,
                    fixed, fixed, pullCorner,
                    red_mat);

    AddTriangle(scene, p1, p3, p4,
                    fixed, pullCorner, fixed,
                    red_mat);

    AmbientLight *ambient = new AmbientLight(RGB(0.4, 0.4, 0.4));
    scene.lights.push_back(ambient);
    scene.numLights++;

    PointLight *p1light = new PointLight(RGB(0.8,0.8,0.8), Point(0, 8, 0));
    scene.lights.push_back(p1light);
    scene.numLights++;
}

// Scene with single triangle
void SingleTriScene (Scene& scene){
    int const mat = AddDiffuseMat(scene, RGB (0.99, 0.99, 0.99));
    AddTriangle(scene, Point(-5., 5., 0.), Point(0., -5., 0.), Point(5., 5., 0.), mat);
    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    //AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    scene.lights.push_back(ambient);
    scene.numLights++;
    PointLight *p1 = new PointLight(RGB(0.7,0.7,0.7),Point(0,0,-10));
    scene.lights.push_back(p1);
    scene.numLights++;
    return ;
}

// Scene with  spheres
void SpheresScene (Scene& scene, int const N_spheres){
    int const red_mat = AddDiffuseMat(scene, RGB (0.9, 0.1, 0.1));
    AddSphere(scene, Point(0., 0., 3.), 0.8, red_mat);
    //AddMovingSphere(scene, Point(0., 0., 3.), 0.8, Vector(0.4, 0., 0.), red_mat);
    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.5,0.5,0.5));
    //AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    scene.lights.push_back(ambient);
    scene.numLights++;
    PointLight *p1 = new PointLight(RGB(0.7,0.7,0.7),Point(0,2.0,0));
    scene.lights.push_back(p1);
    scene.numLights++;
    return ;
}

// Scene with  sphere and 4 triangles
void SpheresTriScene (Scene& scene) {
    int const red_mat = AddDiffuseMat(scene, RGB (0.9, 0.1, 0.1));
    int const green_mat = AddDiffuseMat(scene, RGB (0.1, 0.9, 0.1));
    AddSphere(scene, Point(0., 0., 3.), 0.8, red_mat);
    AddTriangle(scene, Point(0., 0., 7.), Point(-2., 1.5, 4.), Point(-0.5, 1.5, 5.),green_mat);
    AddTriangle(scene, Point(0., 0., 7.), Point(0.5, 1.5, 5.), Point(2., 1.5, 4.),green_mat);
    AddTriangle(scene, Point(0., 0., 7.), Point(-0.5, -1.5, 5.), Point(-2., -1.5, 4.),green_mat);
    AddTriangle(scene, Point(0., 0., 7.), Point(0.5, -1.5, 5.), Point(2., -1.5, 4.), green_mat);
    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.5,0.5,0.5));
    //AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    scene.lights.push_back(ambient);
    scene.numLights++;
    PointLight *p1 = new PointLight(RGB(0.7,0.7,0.7),Point(0,2.0,0));
    scene.lights.push_back(p1);
    scene.numLights++;
    return ;
}

// Cornell Box
void CornellBox (Scene& scene) {
    int const text_backwall = AddTextMat(scene, "Dog.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const uminho_text = AddTextMat(scene, "UMinho.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const white_mat = AddMat(scene, RGB (0.2, 0.2, 0.2), RGB (0.4, 0.4, 0.4), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const red_mat = AddMat(scene, RGB (0.9, 0., 0.), RGB (0.4, 0., 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const green_mat = AddMat(scene, RGB (0., 0.9, 0.), RGB (0., 0.2, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const blue_mat = AddMat(scene, RGB (0., 0., 0.9), RGB (0., 0., 0.4), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const orange_mat = AddMat(scene, RGB (0.99, 0.65, 0.), RGB (0.37, 0.24, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const mirror_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.));
    int const glass_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.2, 0.2, 0.2), RGB (0.9, 0.9, 0.9), 1.2);
    //int const glass_mat = AddPhongMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.2, 0.2, 0.2), RGB (0.9, 0.9, 0.9), 1, 0.9);
    // Floor
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    AddTriangle(scene, Point(549.6, 0.0, 559.2), Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    // Ceiling
    AddTriangle(scene, Point(556.0, 548.8, 0.0), Point(0.0, 548.8, 0.0), Point(0.0, 548.8, 559.2), white_mat);
    AddTriangle(scene, Point(556.0, 548.8, 559.2), Point(556.0, 548.8, 0.0), Point(0., 548.8, 559.2), white_mat);
    // Back wall
    AddTriangleUV (scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(0.,1.), Vec2(0.,0.), text_backwall);
    AddTriangleUV(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(1.,0.), Vec2(0.,0.), text_backwall);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    // Left Wall
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 0., 559.2), Point(0., 548.8, 559.2), green_mat);
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 548.8, 0.), Point(0., 548.8, 559.2), green_mat);
    // Right Wall
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(549.6, 0., 559.2), Point(549.6, 548.8, 559.2), red_mat);
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(552.8, 548.8, 0.), Point(549.6, 548.8, 559.2), red_mat);
    // Right Wall Mirror
    AddTriangle(scene, Point(552, 50.0, 50.), Point(549, 50., 509.2), Point(549, 488.8, 509.2), mirror_mat);
    AddTriangle(scene, Point(552, 50.0, 50.), Point(552, 488.8, 50.), Point(549, 488.8, 509.2), mirror_mat);
    // short block
    Vector shortBoxMotion(-24.5f, 0.0f, 80.0f);
    Vector shortBoxMotion_slow   (12.25f, 0.0f,  -40.0f);   // 0.5x
    Vector shortBoxMotion_fast (-49.0f,  0.0f, 160.0f);   // 2.0x
    
    //top
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0),Vec2(0.,0.), Vec2(0.,1.), Vec2(1.,1.), uminho_text, shortBoxMotion_fast);

    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(1.,0.), Vec2(1.,1.), uminho_text, shortBoxMotion_fast);

    // bottom
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 82.0, 0.01, 225.0), Point(240.0, 0.01, 272.0), orange_mat, shortBoxMotion_fast);
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 290.0, 0.01, 114.0), Point(240.0, 0.01, 272.0), orange_mat, shortBoxMotion_fast);

    //left
    AddTriangle(scene, Point(290.0, 0.0, 114.0), Point(290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat, shortBoxMotion_fast);
    AddTriangle(scene, Point(290.0, 0.0, 114.0), Point(240.0, 0.0, 272.0), Point(240.0, 165.0, 272.0), orange_mat, shortBoxMotion_fast);

    //back
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(240.0, 165.0, 272.0), Point(82.0, 165.0, 225.0), orange_mat, shortBoxMotion_fast);
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), orange_mat, shortBoxMotion_fast);

    //right
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), Point(130.0, 165.0, 65.0), orange_mat, shortBoxMotion_fast);
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(130.0, 0.0, 65.0), Point(130.0, 165.0, 65.0), orange_mat, shortBoxMotion_fast);

    //front
    AddTriangle(scene, Point(130.0, 0.0, 65.0), Point(130.0, 165.0, 65.0), Point(290.0, 165.0, 114.0), orange_mat, shortBoxMotion_fast);
    AddTriangle(scene, Point(130.0, 0.0, 65.0), Point(290.0, 0.0, 114.0), Point(290.0, 165.0, 114.0), orange_mat, shortBoxMotion_fast);
    /*
    // top
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(0.,1.), Vec2(1.,1.), uminho_text);
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(1.,0.), Vec2(1.,1.), uminho_text);
    //AddTriangle(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), orange_mat);
    //AddTriangle(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    // bottom
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 82.0, 0.01, 225.0), Point(240.0, 0.01, 272.0), orange_mat);
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 290.0, 0.01, 114.0), Point(240.0, 0.01, 272.0), orange_mat);
    // left
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point(  290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point( 240.0,  0.0, 272.0), Point(240.0, 165.0, 272.0), orange_mat);
    // back
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(240.0, 165.0, 272.0), Point(82.0, 165., 225.0), orange_mat);
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), orange_mat);
    // right
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), Point(130.0, 165.0, 65.0), orange_mat);
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(130.0, 0.0, 65.0), Point(130.0, 165.0, 65.0), orange_mat);
    // front
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(130.0, 165.0, 65.0), Point(290.0, 165.0, 114.0), orange_mat);
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(290.0, 0.0, 114.0), Point(290.0, 165.0, 114.0), orange_mat);
    */
    // tall block
    // top
    AddTriangle(scene, Point(423.0, 330.0, 247.0), Point(265.0, 330.0, 296.0), Point(314.0, 330.0, 456.0), blue_mat);
    AddTriangle(scene, Point(423.0, 330.0, 247.0), Point(472.0, 330.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    // bottom
    AddTriangle(scene, Point(423.0, 0.1, 247.0), Point(265.0, 0.1, 296.0), Point(314.0, 0.1, 456.0), blue_mat);
    AddTriangle(scene, Point(423.0, 0.1, 247.0), Point(472.0, 0.1, 406.0), Point(314.0, 0.1, 456.0), blue_mat);
    // left
    AddTriangle(scene, Point(423.0, 0.0, 247.0), Point(423.0, 330.0, 247.0), Point(472.0, 330.0, 406.0), blue_mat);
    AddTriangle(scene, Point(423.0, 0.0, 247.0), Point(472.0, 0.0, 406.0), Point(472.0, 330.0, 406.0), blue_mat);
    // back
    AddTriangle(scene, Point(472.0, 0.0, 406.0), Point(472.0, 330.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    AddTriangle(scene, Point(472.0, 0.0, 406.0), Point(314.0, 0.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    // right
    AddTriangle(scene, Point(314.0, 0.0, 456.0), Point(314.0, 330.0, 456.0), Point(265.0, 330.0, 296.0), blue_mat);
    AddTriangle(scene, Point(314.0, 0.0, 456.0), Point(265.0, 0.0, 296.0), Point(265.0, 330.0, 296.0), blue_mat);
    // front
    AddTriangle(scene, Point(265.0, 0.0, 296.0), Point(265.0, 330.0, 296.0), Point(423.0, 330.0, 247.0), blue_mat);
    AddTriangle(scene, Point(265.0, 0.0, 296.0), Point(423.0, 0.0, 247.0), Point(423.0, 330.0, 247.0), blue_mat);
    
    // transparent sphere
    AddSphere(scene, Point(160., 320., 225.), 90., glass_mat); //esfra fixa
    //AddSphere(scene, Point(160., 320., 225.), 90., glass_mat, Vector(80., 0., 0.));



    // add an ambient light to the scene
    //AmbientLight *ambient = new AmbientLight(RGB(0.15,0.15,0.15));
    /*AmbientLight *ambient = new AmbientLight(RGB(0.07,0.07,0.07));
    scene.lights.push_back(ambient);
    scene.numLights++;*/
#define AREA
#ifndef AREA
    for (int x=-1 ; x<2 ; x++) {
        for (int z=-1 ; z<2 ; z++) {
            PointLight *p = new PointLight(RGB(30000.,30000.,30000.),Point(278.+x*150.,545.,280.+z*150));
            scene.lights.push_back(p);
            scene.numLights++;
        }
    }
#else
    for (int lll=-1 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(250.+lll*150, 548.75, 300.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
#endif
    return ;
}

// Deformed Cornell Box
void DeformCornellBox (Scene& scene) {
    int const text_backwall = AddTextMat(scene, "Dog.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const uminho_text = AddTextMat(scene, "UMinho.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const white_mat = AddMat(scene, RGB (0.2, 0.2, 0.2), RGB (0.4, 0.4, 0.4), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const red_mat = AddMat(scene, RGB (0.9, 0., 0.), RGB (0.4, 0., 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const green_mat = AddMat(scene, RGB (0., 0.9, 0.), RGB (0., 0.2, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const blue_mat = AddMat(scene, RGB (0., 0., 0.9), RGB (0., 0., 0.4), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const orange_mat = AddMat(scene, RGB (0.99, 0.65, 0.), RGB (0.37, 0.24, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const mirror_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.));
    int const glass_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.2, 0.2, 0.2), RGB (0.9, 0.9, 0.9), 1.2);
    //int const glass_mat = AddPhongMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.2, 0.2, 0.2), RGB (0.9, 0.9, 0.9), 1, 0.9);
    // Floor
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    AddTriangle(scene, Point(549.6, 0.0, 559.2), Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    // Ceiling
    AddTriangle(scene, Point(556.0, 548.8, 0.0), Point(0.0, 548.8, 0.0), Point(0.0, 548.8, 559.2), white_mat);
    AddTriangle(scene, Point(556.0, 548.8, 559.2), Point(556.0, 548.8, 0.0), Point(0., 548.8, 559.2), white_mat);
    // Back wall
    AddTriangleUV (scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(0.,1.), Vec2(0.,0.), text_backwall);
    AddTriangleUV(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(1.,0.), Vec2(0.,0.), text_backwall);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    // Left Wall
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 0., 559.2), Point(0., 548.8, 559.2), green_mat);
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 548.8, 0.), Point(0., 548.8, 559.2), green_mat);
    // Right Wall
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(549.6, 0., 559.2), Point(549.6, 548.8, 559.2), red_mat);
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(552.8, 548.8, 0.), Point(549.6, 548.8, 559.2), red_mat);
    // Right Wall Mirror
    AddTriangle(scene, Point(552, 50.0, 50.), Point(549, 50., 509.2), Point(549, 488.8, 509.2), mirror_mat);
    AddTriangle(scene, Point(552, 50.0, 50.), Point(552, 488.8, 50.), Point(549, 488.8, 509.2), mirror_mat);
    
    // short block
    // short block - deformed
    Point b1_(130.0, 0.01,  65.0);
    Point b2_( 82.0, 0.01, 225.0);
    Point b3_(240.0, 0.01, 272.0);
    Point b4_(290.0, 0.01, 114.0);

    Point t1_(130.0, 165.0,  65.0);
    Point t2_( 82.0, 165.0, 225.0);
    Point t3_(240.0, 165.0, 272.0);
    Point t4_(290.0, 165.0, 114.0);

    // base para a esquerda
    //Vector bottomDrag(-60.f, 0.f, 0.f);
    // topo para a direita
    //Vector topDrag(60.f, 0.f, 0.f);
    // novos vectores para nao haver sombras
    Vector topDrag(57.4f, 0.f, 17.6f);
    Vector bottomDrag(-57.4f, 0.f, -17.6f);
    Vector fixed(0.f, 0.f, 0.f);
    
    // top
    AddTriangle(scene, t1_, t2_, t3_,
                    topDrag, topDrag, topDrag,
                    orange_mat);

    AddTriangle(scene, t1_, t3_, t4_,
                    topDrag, topDrag, topDrag,
                    orange_mat);

    // bottom
    AddTriangle(scene, b1_, b2_, b3_,
                    bottomDrag, bottomDrag, bottomDrag,
                    orange_mat);

    AddTriangle(scene, b1_, b3_, b4_,
                    bottomDrag, bottomDrag, bottomDrag,
                    orange_mat);

    // side 1
    AddTriangle(scene, b4_, t4_, t3_,
                    bottomDrag, topDrag, topDrag,
                    orange_mat);

    AddTriangle(scene, b4_, t3_, b3_,
                    bottomDrag, topDrag, bottomDrag,
                    orange_mat);

    // side 2
    AddTriangle(scene, b3_, t3_, t2_,
                    bottomDrag, topDrag, topDrag,
                    orange_mat);

    AddTriangle(scene, b3_, t2_, b2_,
                    bottomDrag, topDrag, bottomDrag,
                    orange_mat);

    // side 3
    AddTriangle(scene, b2_, t2_, t1_,
                    bottomDrag, topDrag, topDrag,
                    orange_mat);

    AddTriangle(scene, b2_, t1_, b1_,
                    bottomDrag, topDrag, bottomDrag,
                    orange_mat);

    // side 4
    AddTriangle(scene, b1_, t1_, t4_,
                    bottomDrag, topDrag, topDrag,
                    orange_mat);

    AddTriangle(scene, b1_, t4_, b4_,
                    bottomDrag, topDrag, bottomDrag,
                    orange_mat);

    /*
    // top
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(0.,1.), Vec2(1.,1.), uminho_text);
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(1.,0.), Vec2(1.,1.), uminho_text);
    // bottom
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 82.0, 0.01, 225.0), Point(240.0, 0.01, 272.0), orange_mat);
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 290.0, 0.01, 114.0), Point(240.0, 0.01, 272.0), orange_mat);
    // left
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point(  290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point( 240.0,  0.0, 272.0), Point(240.0, 165.0, 272.0), orange_mat);
    // back
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(240.0, 165.0, 272.0), Point(82.0, 165., 225.0), orange_mat);
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), orange_mat);
    // right
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), Point(130.0, 165.0, 65.0), orange_mat);
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(130.0, 0.0, 65.0), Point(130.0, 165.0, 65.0), orange_mat);
    // front
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(130.0, 165.0, 65.0), Point(290.0, 165.0, 114.0), orange_mat);
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(290.0, 0.0, 114.0), Point(290.0, 165.0, 114.0), orange_mat);
    */


    // tall block - deformed
    Point b1(423.0, 0.1, 247.0);
    Point b2(265.0, 0.1, 296.0);
    Point b3(314.0, 0.1, 456.0);
    Point b4(472.0, 0.1, 406.0);

    Point t1(423.0, 330.0, 247.0);
    Point t2(265.0, 330.0, 296.0);
    Point t3(314.0, 330.0, 456.0);
    Point t4(472.0, 330.0, 406.0);

    // bottom fixed
    //Vector fixed(0.f, 0.f, 0.f);
    
    Vector pull_t1(0.f,   0.f,   0.f);
    Vector pull_t2(0.f,   0.f,   0.f);

    // puxa a aresta de trás do topo para a direita e para trás
    Vector pull_t3(90.f,  0.f, 120.f);
    Vector pull_t4(90.f,  0.f, 120.f);
    
    // top
    AddTriangle(scene, t1, t2, t3,
                    pull_t1, pull_t2, pull_t3,
                    blue_mat);

    AddTriangle(scene, t1, t4, t3,
                        pull_t1, pull_t4, pull_t3,
                        blue_mat);

    // bottom
    AddTriangle(scene, b1, b2, b3,
                    fixed, fixed, fixed,
                    blue_mat);

    AddTriangle(scene, b1, b4, b3,
                    fixed, fixed, fixed,
                    blue_mat);

    // left
    AddTriangle(scene, b1, t1, t4,
                    fixed, pull_t1, pull_t4,
                    blue_mat);

    AddTriangle(scene, b1, b4, t4,
                    fixed, fixed, pull_t4,
                    blue_mat);

    // back
    AddTriangle(scene, b4, t4, t3,
                    fixed, pull_t4, pull_t3,
                    blue_mat);

    AddTriangle(scene, b4, b3, t3,
                    fixed, fixed, pull_t3,
                    blue_mat);

    // right
    AddTriangle(scene, b3, t3, t2,
                    fixed, pull_t3, pull_t2,
                    blue_mat);

    AddTriangle(scene, b3, b2, t2,
                    fixed, fixed, pull_t2,
                    blue_mat);

    // front
    AddTriangle(scene, b2, t2, t1,
                    fixed, pull_t2, pull_t1,
                    blue_mat);

    AddTriangle(scene, b2, b1, t1,
                    fixed, fixed, pull_t1,
                    blue_mat);
    
    
    /*
    // torção do topo à volta do eixo vertical
    Vector pull_t1( 90.f, 0.f,  45.f);
    Vector pull_t2( 45.f, 0.f, -90.f);
    Vector pull_t3(-90.f, 0.f, -45.f);
    Vector pull_t4(-45.f, 0.f,  90.f);
   // top
    AddTriangle(scene, t1, t2, t3,
                    pull_t1, pull_t2, pull_t3,
                    blue_mat);

    AddTriangle(scene, t1, t4, t3,
                    pull_t1, pull_t4, pull_t3,
                    blue_mat);

    // bottom
    AddTriangle(scene, b1, b2, b3, blue_mat);
    AddTriangle(scene, b1, b4, b3, blue_mat);

    // left
    AddTriangle(scene, b1, t1, t4,
                    fixed, pull_t1, pull_t4,
                    blue_mat);

    AddTriangle(scene, b1, b4, t4,
                    fixed, fixed, pull_t4,
                    blue_mat);

    // back
    AddTriangle(scene, b4, t4, t3,
                    fixed, pull_t4, pull_t3,
                    blue_mat);

    AddTriangle(scene, b4, b3, t3,
                    fixed, fixed, pull_t3,
                    blue_mat);

    // right
    AddTriangle(scene, b3, t3, t2,
                    fixed, pull_t3, pull_t2,
                    blue_mat);

    AddTriangle(scene, b3, b2, t2,
                    fixed, fixed, pull_t2,
                    blue_mat);

    // front
    AddTriangle(scene, b2, t2, t1,
                    fixed, pull_t2, pull_t1,
                    blue_mat);

    AddTriangle(scene, b2, b1, t1,
                    fixed, fixed, pull_t1,
                    blue_mat);
    */
    // transparent sphere
    AddSphere(scene, Point(160., 320., 225.), 90., glass_mat); //esfra fixa
    //AddMovingSphere(scene, Point(160., 320., 225.), 90., Vector(80., 0., 0.), glass_mat);
    //AddSphere(scene, Point(160., 320., 225.), 90., glass_mat, Vector(80., 0., 0.));



    // add an ambient light to the scene
    //AmbientLight *ambient = new AmbientLight(RGB(0.15,0.15,0.15));
    /*AmbientLight *ambient = new AmbientLight(RGB(0.07,0.07,0.07));
    scene.lights.push_back(ambient);
    scene.numLights++;*/
#define AREA
#ifndef AREA
    for (int x=-1 ; x<2 ; x++) {
        for (int z=-1 ; z<2 ; z++) {
            PointLight *p = new PointLight(RGB(30000.,30000.,30000.),Point(278.+x*150.,545.,280.+z*150));
            scene.lights.push_back(p);
            scene.numLights++;
        }
    }
#else
    for (int lll=-1 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(250.+lll*150, 548.75, 300.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
#endif
    return ;
}

// Diffuse Cornell Box
void DiffuseCornellBox (Scene& scene) {
    int const text_backwall = AddTextMat(scene, "Dog.ppm", RGB (0.3, 0.3, 0.3), RGB (0.8, 0.8, 0.8), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const uminho_text = AddTextMat(scene, "UMinho.ppm", RGB (0.3, 0.3, 0.3), RGB (0.6, 0.6, 0.6), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const white_mat = AddMat(scene, RGB (0.1, 0.1, 0.1), RGB (0.6, 0.6, 0.6), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const red_mat = AddMat(scene, RGB (0.1, 0., 0.), RGB (0.6, 0., 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const green_mat = AddMat(scene, RGB (0., 0.1, 0.), RGB (0., 0.6, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const blue_mat = AddMat(scene, RGB (0., 0., 0.1), RGB (0., 0., 0.6), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const orange_mat = AddMat(scene, RGB (0.37, 0.24, 0.), RGB (0.66, 0.44, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    // Floor
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    AddTriangle(scene, Point(549.6, 0.0, 559.2), Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    // Ceiling
    AddTriangle(scene, Point(556.0, 548.8, 0.0), Point(0.0, 548.8, 0.0), Point(0.0, 548.8, 559.2), white_mat);
    AddTriangle(scene, Point(556.0, 548.8, 559.2), Point(556.0, 548.8, 0.0), Point(0., 548.8, 559.2), white_mat);
    // Back wall
    AddTriangleUV (scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(0.,1.), Vec2(0.,0.), text_backwall);
    AddTriangleUV(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(1.,0.), Vec2(0.,0.), text_backwall);
    //AddTriangle (scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    // Left Wall
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 0., 559.2), Point(0., 548.8, 559.2), green_mat);
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 548.8, 0.), Point(0., 548.8, 559.2), green_mat);
    // Right Wall
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(549.6, 0., 559.2), Point(549.6, 548.8, 559.2), red_mat);
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(552.8, 548.8, 0.), Point(549.6, 548.8, 559.2), red_mat);

    // short block
    // top
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(0.,1.), Vec2(1.,1.), uminho_text);
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(1.,0.), Vec2(1.,1.), uminho_text);
    //AddTriangle(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), orange_mat);
    //AddTriangle(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    // bottom
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 82.0, 0.01, 225.0), Point(240.0, 0.01, 272.0), orange_mat);
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 290.0, 0.01, 114.0), Point(240.0, 0.01, 272.0), orange_mat);
    // left
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point(  290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point( 240.0,  0.0, 272.0), Point(240.0, 165.0, 272.0), orange_mat);
    // back
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(240.0, 165.0, 272.0), Point(82.0, 165., 225.0), orange_mat);
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), orange_mat);
    // right
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), Point(130.0, 165.0, 65.0), orange_mat);
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(130.0, 0.0, 65.0), Point(130.0, 165.0, 65.0), orange_mat);
    // front
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(130.0, 165.0, 65.0), Point(290.0, 165.0, 114.0), orange_mat);
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(290.0, 0.0, 114.0), Point(290.0, 165.0, 114.0), orange_mat);

    // tall block
    // top
    AddTriangle(scene, Point(423.0, 330.0, 247.0), Point(265.0, 330.0, 296.0), Point(314.0, 330.0, 456.0), blue_mat);
    AddTriangle(scene, Point(423.0, 330.0, 247.0), Point(472.0, 330.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    // bottom
    AddTriangle(scene, Point(423.0, 0.1, 247.0), Point(265.0, 0.1, 296.0), Point(314.0, 0.1, 456.0), blue_mat);
    AddTriangle(scene, Point(423.0, 0.1, 247.0), Point(472.0, 0.1, 406.0), Point(314.0, 0.1, 456.0), blue_mat);
    // left
    AddTriangle(scene, Point(423.0, 0.0, 247.0), Point(423.0, 330.0, 247.0), Point(472.0, 330.0, 406.0), blue_mat);
    AddTriangle(scene, Point(423.0, 0.0, 247.0), Point(472.0, 0.0, 406.0), Point(472.0, 330.0, 406.0), blue_mat);
    // back
    AddTriangle(scene, Point(472.0, 0.0, 406.0), Point(472.0, 330.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    AddTriangle(scene, Point(472.0, 0.0, 406.0), Point(314.0, 0.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    // right
    AddTriangle(scene, Point(314.0, 0.0, 456.0), Point(314.0, 330.0, 456.0), Point(265.0, 330.0, 296.0), blue_mat);
    AddTriangle(scene, Point(314.0, 0.0, 456.0), Point(265.0, 0.0, 296.0), Point(265.0, 330.0, 296.0), blue_mat);
    // front
    AddTriangle(scene, Point(265.0, 0.0, 296.0), Point(265.0, 330.0, 296.0), Point(423.0, 330.0, 247.0), blue_mat);
    AddTriangle(scene, Point(265.0, 0.0, 296.0), Point(423.0, 0.0, 247.0), Point(423.0, 330.0, 247.0), blue_mat);
    
  
    // add an ambient light to the scene
    //AmbientLight *ambient = new AmbientLight(RGB(0.15,0.15,0.15));
    //AmbientLight *ambient = new AmbientLight(RGB(0.07,0.07,0.07));
    //scene.lights.push_back(ambient);
    //scene.numLights++;
#define AREA
#ifndef AREA
    for (int x=-1 ; x<2 ; x++) {
        for (int z=-1 ; z<2 ; z++) {
            PointLight *p = new PointLight(RGB(0.16,0.16,0.16),Point(278.+x*150.,548.75,280.+z*150));
            scene.lights.push_back(p);
            scene.numLights++;
        }
    }
#else
    for (int lll=-1 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(25000,25000,25000), Point(250.+lll*150, 548., 250.+lll*150), Point(300.+lll*150, 548., 250.+lll*150), Point(300.+lll*150, 548., 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(25000,25000,25000), Point(250.+lll*150, 548., 250.+lll*150), Point(250.+lll*150, 548., 300.+lll*150), Point(300.+lll*150, 548., 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
#endif
    return ;
}

// DLight Challenge
void DLightChallenge (Scene& scene) {
    int const text_backwall = AddTextMat(scene, "Dog.ppm", RGB (0.3, 0.3, 0.3), RGB (0.8, 0.8, 0.8), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const uminho_text = AddTextMat(scene, "UMinho.ppm", RGB (0.3, 0.3, 0.3), RGB (0.6, 0.6, 0.6), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const white_mat = AddMat(scene, RGB (0.1, 0.1, 0.1), RGB (0.6, 0.6, 0.6), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const red_mat = AddMat(scene, RGB (0.1, 0., 0.), RGB (0.5, 0.1, 0.1), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const green_mat = AddMat(scene, RGB (0., 0.1, 0.), RGB (0., 0.6, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const blue_mat = AddMat(scene, RGB (0., 0., 0.1), RGB (0., 0., 0.6), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const orange_mat = AddMat(scene, RGB (0.37, 0.24, 0.), RGB (0.66, 0.44, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    // Floor
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(-100.0, 0.0, 0.0), Point(-100.0, 0.0, 859.2), white_mat);
    AddTriangle(scene, Point(549.6, 0.0, 859.2), Point(552.8, 0.0, 0.0), Point(-100.0, 0.0, 859.2), white_mat);
    // Ceiling
    AddTriangle(scene, Point(556.0, 548.8, 0.0), Point(-100.0, 548.8, 0.0), Point(-100.0, 548.8, 859.2), white_mat);
    AddTriangle(scene, Point(556.0, 548.8, 859.2), Point(556.0, 548.8, 0.0), Point(-100., 548.8, 859.2), white_mat);
    // Back wall
    AddTriangleUV (scene, Point(-100.0, 0.0, 859.2), Point(549.6, 0.0, 859.2), Point(556.0, 548.8, 859.2), Vec2(1.,1.), Vec2(0.,1.), Vec2(0.,0.), text_backwall);
    AddTriangleUV(scene, Point(-100.0, 0.0, 859.2), Point(-100.0, 548.8, 859.2), Point(556.0, 548.8, 859.2), Vec2(1.,1.), Vec2(1.,0.), Vec2(0.,0.), text_backwall);
    // Left Wall
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 0., 459.2), Point(0., 548.8, 459.2), green_mat);
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 548.8, 0.), Point(0., 548.8, 459.2), green_mat);
    // L walls
    AddTriangle(scene, Point(-100.0, 0.0, 459.2), Point(-100., 0., 859.2), Point(-100., 548.8, 859.2), white_mat);
    AddTriangle(scene, Point(-100.0, 0.0, 459.2), Point(-100., 548.8, 459.2), Point(-100., 548.8, 859.2), white_mat);
    AddTriangle (scene, Point(-100.0, 0.0, 459.2), Point(0., 0.0, 459.2), Point(0., 548.8, 459.2),  white_mat);
    AddTriangle(scene, Point(-100.0, 0.0, 459.2), Point(-100.0, 548.8, 459.2), Point(0., 548.8, 459.2), white_mat);
    // Right Wall
    
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(549.6, 0., 859.2), Point(549.6, 548.8, 859.2), red_mat);
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(552.8, 548.8, 0.0), Point(549.6, 548.8, 859.2), red_mat);

    // short block
    // top
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(0.,1.), Vec2(1.,1.), uminho_text);
    AddTriangleUV(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), Vec2(0.,0.), Vec2(1.,0.), Vec2(1.,1.), uminho_text);
    //AddTriangle(scene, Point(130.0, 165.0,  65.0), Point( 82.0, 165.0, 225.0), Point(240.0, 165.0, 272.0), orange_mat);
    //AddTriangle(scene, Point(130.0, 165.0,  65.0), Point( 290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    // bottom
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 82.0, 0.01, 225.0), Point(240.0, 0.01, 272.0), orange_mat);
    AddTriangle(scene, Point(130.0, 0.01,  65.0), Point( 290.0, 0.01, 114.0), Point(240.0, 0.01, 272.0), orange_mat);
    // left
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point(  290.0, 165.0, 114.0), Point(240.0, 165.0, 272.0), orange_mat);
    AddTriangle(scene, Point(290.0,   0.0, 114.0), Point( 240.0,  0.0, 272.0), Point(240.0, 165.0, 272.0), orange_mat);
    // back
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(240.0, 165.0, 272.0), Point(82.0, 165., 225.0), orange_mat);
    AddTriangle(scene, Point(240.0, 0.0, 272.0), Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), orange_mat);
    // right
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(82.0, 165.0, 225.0), Point(130.0, 165.0, 65.0), orange_mat);
    AddTriangle(scene, Point(82.0, 0.0, 225.0), Point(130.0, 0.0, 65.0), Point(130.0, 165.0, 65.0), orange_mat);
    // front
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(130.0, 165.0, 65.0), Point(290.0, 165.0, 114.0), orange_mat);
    AddTriangle(scene, Point( 130.0,   0.0,  65.0), Point(290.0, 0.0, 114.0), Point(290.0, 165.0, 114.0), orange_mat);

    // tall block
    // top
    AddTriangle(scene, Point(423.0, 330.0, 247.0), Point(265.0, 330.0, 296.0), Point(314.0, 330.0, 456.0), blue_mat);
    AddTriangle(scene, Point(423.0, 330.0, 247.0), Point(472.0, 330.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    // bottom
    AddTriangle(scene, Point(423.0, 0.1, 247.0), Point(265.0, 0.1, 296.0), Point(314.0, 0.1, 456.0), blue_mat);
    AddTriangle(scene, Point(423.0, 0.1, 247.0), Point(472.0, 0.1, 406.0), Point(314.0, 0.1, 456.0), blue_mat);
    // left
    AddTriangle(scene, Point(423.0, 0.0, 247.0), Point(423.0, 330.0, 247.0), Point(472.0, 330.0, 406.0), blue_mat);
    AddTriangle(scene, Point(423.0, 0.0, 247.0), Point(472.0, 0.0, 406.0), Point(472.0, 330.0, 406.0), blue_mat);
    // back
    AddTriangle(scene, Point(472.0, 0.0, 406.0), Point(472.0, 330.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    AddTriangle(scene, Point(472.0, 0.0, 406.0), Point(314.0, 0.0, 406.0), Point(314.0, 330.0, 456.0), blue_mat);
    // right
    AddTriangle(scene, Point(314.0, 0.0, 456.0), Point(314.0, 330.0, 456.0), Point(265.0, 330.0, 296.0), blue_mat);
    AddTriangle(scene, Point(314.0, 0.0, 456.0), Point(265.0, 0.0, 296.0), Point(265.0, 330.0, 296.0), blue_mat);
    // front
    AddTriangle(scene, Point(265.0, 0.0, 296.0), Point(265.0, 330.0, 296.0), Point(423.0, 330.0, 247.0), blue_mat);
    AddTriangle(scene, Point(265.0, 0.0, 296.0), Point(423.0, 0.0, 247.0), Point(423.0, 330.0, 247.0), blue_mat);
    
  
    for (int llz=-1 ; llz<2 ; llz++) {
        for (int llx=-1 ; llx<2 ; llx++) {
            AreaLight *a1 = new AreaLight(RGB(5000.-(llx+llz)*2000.,5000. -(llx+llz)*2000.,5000.-(llx+llz)*2000.), Point(250.+llx*150, 545., 250.+llz*150), Point(300.+llx*150, 545., 250.+llz*150), Point(300.+llx*150, 545., 300.+llz*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
            AreaLight *a2 = new AreaLight(RGB(5000.-(llx+llz)*2000.,5000.-(llx+llz)*2000.,5000.-(llx+llz)*2000.), Point(250.+llx*150, 545., 250.+llz*150), Point(250.+llx*150, 545., 300.+llz*150), Point(300.+llx*150, 545., 300.+llz*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
        }
    }
    for (int lll=0 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(15000.+lll*4000,15000.+lll*4000,15000.+lll*4000), Point(-10., 20.+250*lll, 459.3), Point(-10., 90.+250*lll, 459.3), Point(-90, 90.+250*lll, 459.3), Vector (0.,0.,1.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(15000.+lll*4000,15000.+lll*4000,15000.+lll*4000), Point(-10., 20.+250*lll, 459.3), Point(-90., 20.+250*lll, 459.3), Point(-90, 90.+250*lll, 459.3), Vector (0.,0.,1.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
    for (int lll=0 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(2000.-lll*500,2000.-lll*500.,1000. -lll*500), Point(0.01, 20., 20.+lll*200.), Point(0.01, 20., 100.+lll*200.), Point(0.01, 30., 100.+lll*200.), Vector (1.,0.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(2000.-lll*500,2000.-lll*500,1000. -lll*500), Point(0.01, 20., 20.+lll*200.), Point(0.01, 30., 20.+lll*200.), Point(0.01, 30., 100.+lll*200.), Vector (1.,0.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
    for (int lll=0 ; lll<4 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(2000.-lll*450,2000.-lll*450.,1000. -lll*300), Point(549.59, 20., 20.+lll*200.), Point(549.59, 20., 100.+lll*200.), Point(549.59, 30., 100.+lll*200.), Vector (-1.,0.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(2000.-lll*450,2000.-lll*450,1000. -lll*300), Point(549.59, 20., 20.+lll*200.), Point(549.59, 30., 20.+lll*200.), Point(549.59, 30., 100.+lll*200.), Vector (-1.,0.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
    { // blue block light
        AreaLight *a1 = new AreaLight(RGB(4000.,4000.0,10000.), Point(340.0, 0.01, 220.0), Point(340.0, 0.01, 230.0), Point(350.0, 0.01, 230.0), Vector (0.,1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(4000.,4000.0,10000.), Point(340.0, 0.01, 220.0), Point(350.0, 0.01, 220.0), Point(350.0, 0.01, 230.0), Vector (0.,1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
    { // orange block light
        AreaLight *a1 = new AreaLight(RGB(4000.,4000.0,10000.), Point(210.0, 0.01, 60.0), Point(210., 0.01, 70.0), Point(220., 0.01, 70.0), Vector (0.,1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(4000.,4000.0,10000.), Point(210., 0.01, 60.0), Point(220., 0.01, 60.0), Point(220., 0.01, 70.0), Vector (0.,1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
    return ;
}


// Scene for testing defocus blur 5 triangles
void DeFocusTriScene (Scene& scene) {
    int const red_mat = AddDiffuseMat(scene, RGB (0.9, 0.1, 0.1));
    int const green_mat = AddDiffuseMat(scene, RGB (0.1, 0.9, 0.1));
    int const brown_mat = AddDiffuseMat(scene, RGB (210./256.,105./256.,30./256.));
    // floor
    AddTriangle(scene, Point(-20., -0.1, -20.), Point(-20., -0.1, 20.), Point(20., -0.1, 20.),brown_mat);
    AddTriangle(scene, Point(-20., -0.1, -20.), Point(20., -0.1, -20.), Point(20., -0.1, 20.),brown_mat);

    float const Xbase=0.;
    float const Zbase=10.;
    // central triangle
    AddTriangle(scene, Point(Xbase-0.5, 1., Zbase), Point(Xbase+0.5, 1., Zbase), Point(Xbase, 0.1, Zbase),green_mat);
    //AddTriangle(scene, Point(-0.5, 1., 10.), Point(0.5, 1., 10.), Point(0., 0.1, 10.),green_mat);
    AddTriangle(scene, Point(Xbase+0.5, 1., Zbase+1.), Point(Xbase+1.5, 1., Zbase+1.), Point(Xbase+1., 0.1, Zbase+1.),red_mat);
    AddTriangle(scene, Point(Xbase+1.5, 1., Zbase+2.), Point(Xbase+2.5, 1., Zbase+2.), Point(Xbase+2., 0.1, Zbase+2.),green_mat);
    AddTriangle(scene, Point(Xbase-1.0, 1., Zbase-1.), Point(Xbase, 1., Zbase-1.), Point(Xbase-0.5, 0.1, Zbase-1.),red_mat);
    AddTriangle(scene, Point(Xbase-1.5, 1., Zbase-2.), Point(Xbase-0.5, 1., Zbase-2.), Point(Xbase-1., 0.1, Zbase-2.),green_mat);

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.5,0.5,0.5));
    //AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    scene.lights.push_back(ambient);
    scene.numLights++;
    return ;
}

// New Balls Scene
void BallsScene (Scene& scene) {
    int const text_backwall = AddTextMat(scene, "Dog.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const uminho_text = AddTextMat(scene, "UMinho.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const white_mat = AddMat(scene, RGB (0.2, 0.2, 0.2), RGB (0.4, 0.4, 0.4), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const red_mat = AddMat(scene, RGB (0.9, 0., 0.), RGB (0.4, 0., 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const green_mat = AddMat(scene, RGB (0., 0.9, 0.), RGB (0., 0.2, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const blue_mat = AddMat(scene, RGB (0.05, 0.08, 0.9), RGB (0.08, 0.12, 0.75), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const orange_mat = AddMat(scene, RGB (0.99, 0.65, 0.), RGB (0.37, 0.24, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const mirror_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.));
    int const glass_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.2, 0.2, 0.2), RGB (0.9, 0.9, 0.9), 1.2);
    //int const glass_mat = AddPhongMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.2, 0.2, 0.2), RGB (0.9, 0.9, 0.9), 1, 0.9);

    
    // Floor
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    AddTriangle(scene, Point(549.6, 0.0, 559.2), Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    // Ceiling
    AddTriangle(scene, Point(556.0, 548.8, 0.0), Point(0.0, 548.8, 0.0), Point(0.0, 548.8, 559.2), white_mat);
    AddTriangle(scene, Point(556.0, 548.8, 559.2), Point(556.0, 548.8, 0.0), Point(0., 548.8, 559.2), white_mat);
    // Back wall
    AddTriangleUV (scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(0.,1.), Vec2(0.,0.), text_backwall);
    AddTriangleUV(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(1.,0.), Vec2(0.,0.), text_backwall);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    //AddTriangle(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), white_mat);
    // Left Wall
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 0., 559.2), Point(0., 548.8, 559.2), green_mat);
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 548.8, 0.), Point(0., 548.8, 559.2), green_mat);
    // Right Wall
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(549.6, 0., 559.2), Point(549.6, 548.8, 559.2), red_mat);
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(552.8, 548.8, 0.), Point(549.6, 548.8, 559.2), red_mat);
    // Right Wall Mirror
    AddTriangle(scene, Point(552, 50.0, 50.), Point(549, 50., 509.2), Point(549, 488.8, 509.2), mirror_mat);
    AddTriangle(scene, Point(552, 50.0, 50.), Point(552, 488.8, 50.), Point(549, 488.8, 509.2), mirror_mat);
    
    // =====================================================
    // Motion blur test: one ball passes in front of a stationary one,
    // so the quiet ball can be seen through the blurred trail.
    // =====================================================
    float const ballRadius = 90.0f;

    Vector orangeMotion(140.0f, 0.f, 0.f);       // passa de lado à frente da azul

    // bola em movimento: fica mais perto da câmara e passa à frente da azul
    Point orangeCenter(115.0f, ballRadius, 210.0f);
    AddSphere(scene, orangeCenter, ballRadius, orange_mat, orangeMotion);

    // bola parada: fica atrás, visível através do rasto da bola laranja
    Point blueCenter(310.0f, ballRadius, 395.0f);
    AddSphere(scene, blueCenter, ballRadius, blue_mat);

    // referência parada ao lado
    Point greenCenter(455.0f, ballRadius, 515.0f);
    AddSphere(scene, greenCenter, ballRadius, green_mat);
	    

#define AREA
#ifndef AREA
    for (int x=-1 ; x<2 ; x++) {
        for (int z=-1 ; z<2 ; z++) {
            PointLight *p = new PointLight(RGB(30000.,30000.,30000.),Point(278.+x*150.,545.,280.+z*150));
            scene.lights.push_back(p);
            scene.numLights++;
        }
    }
#else
    for (int lll=-1 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a1);
            scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(250.+lll*150, 548.75, 300.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
            scene.lights.push_back(a2);
            scene.numLights++;
    }
#endif
    return ;
}

// Cornell Box test: a moving ball starts in front of a box and rises,
// revealing the box during the shutter interval.
void BallBoxRevealScene (Scene& scene) {
    int const text_backwall = AddTextMat(scene, "Dog.ppm", RGB (0.3, 0.3, 0.3), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const white_mat = AddMat(scene, RGB (0.2, 0.2, 0.2), RGB (0.4, 0.4, 0.4), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const red_mat = AddMat(scene, RGB (0.9, 0., 0.), RGB (0.4, 0., 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const green_mat = AddMat(scene, RGB (0., 0.9, 0.), RGB (0., 0.2, 0.), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const blue_mat = AddMat(scene, RGB (0.05, 0.08, 0.9), RGB (0.08, 0.12, 0.75), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const dark_red_ball_mat = AddMat(scene, RGB (0.55, 0.03, 0.02), RGB (0.42, 0.02, 0.01), RGB (0., 0., 0.), RGB (0., 0., 0.));
    int const mirror_mat = AddMat(scene, RGB (0., 0., 0.), RGB (0., 0., 0.), RGB (0.9, 0.9, 0.9), RGB (0., 0., 0.));
    // Floor
    AddTriangle(scene, Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    AddTriangle(scene, Point(549.6, 0.0, 559.2), Point(552.8, 0.0, 0.0), Point(0.0, 0.0, 559.2), white_mat);
    // Ceiling
    AddTriangle(scene, Point(556.0, 548.8, 0.0), Point(0.0, 548.8, 0.0), Point(0.0, 548.8, 559.2), white_mat);
    AddTriangle(scene, Point(556.0, 548.8, 559.2), Point(556.0, 548.8, 0.0), Point(0., 548.8, 559.2), white_mat);
    // Back wall
    AddTriangleUV (scene, Point(0.0, 0.0, 559.2), Point(549.6, 0.0, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(0.,1.), Vec2(0.,0.), text_backwall);
    AddTriangleUV(scene, Point(0.0, 0.0, 559.2), Point(0.0, 548.8, 559.2), Point(556.0, 548.8, 559.2), Vec2(1.,1.), Vec2(1.,0.), Vec2(0.,0.), text_backwall);
    // Left Wall
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 0., 559.2), Point(0., 548.8, 559.2), green_mat);
    AddTriangle(scene, Point(0.0, 0.0, 0.), Point(0., 548.8, 0.), Point(0., 548.8, 559.2), green_mat);
    // Right Wall
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(549.6, 0., 559.2), Point(549.6, 548.8, 559.2), red_mat);
    AddTriangle(scene, Point(552.8, 0.0, 0.), Point(552.8, 548.8, 0.), Point(549.6, 548.8, 559.2), red_mat);
    // Right Wall Mirror
    AddTriangle(scene, Point(552, 50.0, 50.), Point(549, 50., 509.2), Point(549, 488.8, 509.2), mirror_mat);
    AddTriangle(scene, Point(552, 50.0, 50.), Point(552, 488.8, 50.), Point(549, 488.8, 509.2), mirror_mat);
    
    // a frente da caixa
    //Point const ballCenter(360.0f, 80.0f, 80.0f);
    // ao lado da caixa
    Point const ballCenter(160.0f, 80.0f, 365.0f);
    // para cima
    Vector const ballMotion(0.0f, 145.0f, 0.0f);


    Point b1(300.0, 0.1, 320.0);
    Point b2(420.0, 0.1, 320.0);
    Point b3(420.0, 0.1, 410.0);
    Point b4(300.0, 0.1, 410.0);

    Point t1(300.0, 145.0, 320.0);
    Point t2(420.0, 145.0, 320.0);
    Point t3(420.0, 145.0, 410.0);
    Point t4(300.0, 145.0, 410.0);

    Vector const fixed(0.0f, 0.0f, 0.0f);
    //frente
    //Vector const boxMotion(0.0f, 0.0f, -195.0f);
    // top
    //AddTriangle(scene, t1, t2, t3, boxMotion, boxMotion, boxMotion, blue_mat);
    //AddTriangle(scene, t1, t3, t4, boxMotion, boxMotion, boxMotion, blue_mat);
    // bottom
    //AddTriangle(scene, b1, b3, b2, blue_mat);
    //AddTriangle(scene, b1, b4, b3, blue_mat);
    // front
    //AddTriangle(scene, b1, b2, t2, fixed, fixed, boxMotion, blue_mat);
    //AddTriangle(scene, b1, t2, t1, fixed, boxMotion, boxMotion, blue_mat);
    // right
    //AddTriangle(scene, b2, b3, t3, fixed, fixed, boxMotion, blue_mat);
    //AddTriangle(scene, b2, t3, t2, fixed, boxMotion, boxMotion, blue_mat);
    // back
    //AddTriangle(scene, b3, b4, t4, fixed, fixed, boxMotion, blue_mat);
    //AddTriangle(scene, b3, t4, t3, fixed, boxMotion, boxMotion, blue_mat);
    // left
    //AddTriangle(scene, b4, b1, t1, fixed, fixed, boxMotion, blue_mat);
    //AddTriangle(scene, b4, t1, t4, fixed, boxMotion, boxMotion, blue_mat);


    //lado
    Vector const boxMotion(-80.0f, 0.0f, 0.0f);

    // top
    AddTriangle(scene, t1, t2, t3, boxMotion, boxMotion, boxMotion, blue_mat);
    AddTriangle(scene, t1, t3, t4, boxMotion, boxMotion, boxMotion, blue_mat);
    // bottom
    AddTriangle(scene, b1, b3, b2, blue_mat);
    AddTriangle(scene, b1, b4, b3, blue_mat);
    // front
    AddTriangle(scene, b1, b2, t2, fixed, fixed, boxMotion, blue_mat);
    AddTriangle(scene, b1, t2, t1, fixed, boxMotion, boxMotion, blue_mat);
    // right
    AddTriangle(scene, b2, b3, t3, fixed, fixed, boxMotion, blue_mat);
    AddTriangle(scene, b2, t3, t2, fixed, boxMotion, boxMotion, blue_mat);
    // back
    AddTriangle(scene, b3, b4, t4, fixed, fixed, boxMotion, blue_mat);
    AddTriangle(scene, b3, t4, t3, fixed, boxMotion, boxMotion, blue_mat);
    // left
    AddTriangle(scene, b4, b1, t1, fixed, fixed, boxMotion, blue_mat);
    AddTriangle(scene, b4, t1, t4, fixed, boxMotion, boxMotion, blue_mat);

    AddSphere(scene, ballCenter, 80.0f, dark_red_ball_mat, ballMotion);

#define AREA
#ifndef AREA
    for (int x=-1 ; x<2 ; x++) {
        for (int z=-1 ; z<2 ; z++) {
            PointLight *p = new PointLight(RGB(30000.,30000.,30000.),Point(278.+x*150.,545.,280.+z*150));
            scene.lights.push_back(p);
            scene.numLights++;
        }
    }
#else
    for (int lll=-1 ; lll<2 ; lll++) {
        AreaLight *a1 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 250.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
        scene.lights.push_back(a1);
        scene.numLights++;
        AreaLight *a2 = new AreaLight(RGB(250000.,250000.,250000.), Point(250.+lll*150, 548.75, 250.+lll*150), Point(250.+lll*150, 548.75, 300.+lll*150), Point(300.+lll*150, 548.75, 300.+lll*150), Vector (0.,-1.,0.));
        scene.lights.push_back(a2);
        scene.numLights++;
    }
#endif
    return ;
}
