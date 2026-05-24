#pragma once
#include "raylib.h"
#include <box2d.h>

class Juego
{
public:
    void Iniciar();
    void Actualizar();
    void Dibujar();
    void Cerrar();
    void Reiniciar();
    bool DebeTerminar() const;

private:
    
    void CrearEscenaFisica();

    static const int ANCHO_PANTALLA = 1000;
    static const int ALTO_PANTALLA = 600;
    Music musicaTetris = {};

    //Gravedad
    b2World mundo{ b2Vec2(0.0f, 9.8f) };

    
    Texture2D texVoltorb = {};

    
    Color colorFondo = { 110, 100, 215, 255 };
    Color colorTextoPrincipal = RAYWHITE;
    Color colorTextoSecundario = DARKPURPLE;
    Color colorSuelo = {};

    
    b2Body* cuerpoSuelo = nullptr;
    b2Body* cuerpoPlataformaMovil = nullptr;
    b2Body* cuerpoRotatorio = nullptr;
    b2Body* cuerpoVoltorb = nullptr;

    // Joints prismatico y de revolucion
    b2PrismaticJoint* prismaticJoint = nullptr;
    b2RevoluteJoint* revoluteJoint = nullptr;

    // Parametros
    float velocidadPlataforma = 3.0f;
    float radioVoltorb = 30.0f;
};
