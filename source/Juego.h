#pragma once
#include "raylib.h"
#include <box2d.h>
#include "Jugador.h"
#include "Enemigo.h"
#include "Plataformas.h"

enum class EstadoJuego
{
    JUGANDO,
    VICTORIA,
    DERROTA
};

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
    void VerificarColisiones();
    void DibujarEscena();
    void DibujarHUD();
    void DibujarPantalllaFinal();

    static const int ANCHO_PANTALLA = 1000;
    static const int ALTO_PANTALLA = 600;
    static constexpr float ESCALA = 30.0f;

    Music musicaFondo = {};
    Texture2D texVoltorb = {};
    Texture2D texGastly = {};
    Texture2D texGastlyInv = {};

    Color colorFondo = { 15, 10, 30, 255 };
    Color colorSuelo = { 40, 30, 60, 255 };
    Color colorPlat = { 80, 60, 120, 255 };
    Color colorMeta = { 255, 215, 0, 255 };

    EstadoJuego estado = EstadoJuego::JUGANDO;
    b2World mundo{ b2Vec2(0.0f, 9.8f) };

    // los elementos del mapa 
    b2Body* cuerpoSuelo = nullptr;
    b2Body* cuerpoParedIzq = nullptr;
    b2Body* cuerpoParedDer = nullptr;
    b2Body* cuerpoMolinete = nullptr;
    b2RevoluteJoint* revoluteJoint = nullptr;
    b2Body* cuerpoBandera = nullptr;

    float anchoBandera = 20.0f;
    float altoBandera = 50.0f;
    float tiempoFinal = 0.0f;

    // instancias de los objetos
    Jugador* jugador = nullptr;
    Enemigo* enemigo = nullptr;
    Plataforma* plataforma = nullptr;
};