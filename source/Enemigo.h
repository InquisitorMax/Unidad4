#pragma once
#include "raylib.h"
#include <box2d.h>

class Enemigo
{
public:
<<<<<<< HEAD
    Enemigo(b2World* mundo, float x, float y, float escala, Texture2D* texNormal, Texture2D* texInvertido, int anchoPantalla, float velocidad = 3.5f);
=======
    Enemigo(b2World* mundo, float x, float y, float escala, Texture2D* texNormal, Texture2D* texInvertido, int anchoPantalla);
>>>>>>> cedc80cc3211d13d464c8d89e62b0110f4ba23ca
    ~Enemigo() = default;

    void Actualizar();
    void Dibujar();

    b2Body* GetCuerpo() const { return cuerpo; }
    float GetRadio() const { return radio; }

private:
    b2Body* cuerpo = nullptr;
    Texture2D* texGastly = nullptr;
    Texture2D* texGastlyInv = nullptr;
    float radio = 28.0f;
    float escalaFisica = 30.0f;
    float velGastly = 3.5f; // velocidad 
    bool miraDerecha = false;
    int limiteAnchoPantalla = 1000;
};