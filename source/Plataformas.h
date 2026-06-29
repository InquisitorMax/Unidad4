#pragma once
#include "raylib.h"
#include <box2d.h>

class Plataforma
{
public:
    Plataforma(b2World* mundo, float x, float y, float escala, Texture2D* textura);
    ~Plataforma() = default;

    void Actualizar();
    void Dibujar();

private:
    b2Body* cuerpoAncla = nullptr;
    b2Body* cuerpoPlat = nullptr;
    b2PrismaticJoint* prismaticJoint = nullptr;
    float escalaFisica = 30.0f;
    float velPlataforma = 2.5f;
    //Color colorPlat;
    Texture2D* texPlat = nullptr;
};