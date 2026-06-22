//
//  DiffuseTexture.hpp
//  VI-RT-V4-PathTracing
//
//  Created by Luis Paulo Santos on 19/03/2025.
//

#ifndef DiffuseTexture_hpp
#define DiffuseTexture_hpp

#include "BRDF.hpp"
#include "ImagePPM.hpp"
#include "triangle.hpp"
#include <cmath>

class DiffuseTexture: public BRDF {
private:
    ImagePPM texture;
    float tex_W, tex_H;
public:
    DiffuseTexture(std::string filename) {
        texture.Load(filename);
        textured=true;
        tex_W = float (texture.W);
        tex_H = float (texture.H);
    }
    RGB GetKd (Vec2 TexCoord) {
        float u = fminf(fmaxf(TexCoord.u, 0.0f), 0.999999f);
        float v = fminf(fmaxf(TexCoord.v, 0.0f), 0.999999f);
        int x = (int)floor(u * tex_W);
        int y = (int)floor(v * tex_H);

        RGB color = Kd*texture.get(x, y);
        return color;
    }
};


#endif /* DiffuseTexture_hpp */
