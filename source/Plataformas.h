#pragma once
#include "raylib.h"
#include <box2d.h>

class Plataforma
{
public:
<<<<<<< HEAD
    Plataforma(b2World* mundo, float x, float y, float escala, Texture2D* textura);
=======
    Plataforma(b2World* mundo, float x, float y, float escala, Color color);
>>>>>>> cedc80cc3211d13d464c8d89e62b0110f4ba23ca
    ~Plataforma() = default;

    void Actualizar();
    void Dibujar();

private:
    b2Body* cuerpoAncla = nullptr;
    b2Body* cuerpoPlat = nullptr;
    b2PrismaticJoint* prismaticJoint = nullptr;
    float escalaFisica = 30.0f;
    float velPlataforma = 2.5f;
<<<<<<< HEAD
    //Color colorPlat;
    Texture2D* texPlat = nullptr;
=======
    Color colorPlat;
>>>>>>> cedc80cc3211d13d464c8d89e62b0110f4ba23ca
};