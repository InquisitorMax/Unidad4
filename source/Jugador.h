#pragma once
#include "raylib.h"
#include <box2d.h>

class Jugador
{
public:
    // constructor
    Jugador(b2World* mundo, float x, float y, float escala, Texture2D* textura);
    ~Jugador() = default;

    void Actualizar();
    void Dibujar();

    b2Body* GetCuerpo() const { return cuerpo; }
    float GetRadio() const { return radio; }

private:
    b2Body* cuerpo = nullptr;
    Texture2D* texVoltorb = nullptr;
    float radio = 25.0f;
    float escalaFisica = 30.0f;
    bool miraDerecha = true; // para voltear el sprite
};
