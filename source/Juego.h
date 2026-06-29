#pragma once
#include "raylib.h"
#include <box2d.h>
#include "Jugador.h"
#include "Enemigo.h"
#include "Plataformas.h"

enum class TipoEntidad {
    JUGADOR = 1, //es para que contactlistener no me lo tome como 0
    ENEMIGO,
    META,
    SUELO,
    ESTRUCTURA,
    BOTON
};

enum class EstadoJuego
{
    INICIO,
    JUGANDO,
    VICTORIA,
    DERROTA
};


class Juego;

class MiContactListener : public b2ContactListener
{
private:
    Juego* juego;

public:
    MiContactListener(Juego* j) : juego(j) {}

    void BeginContact(b2Contact* contact) override;
};

class Juego
{
public:
    void ProcesarContactoJugador(TipoEntidad otroTipo);
    void Iniciar();
    void Actualizar();
    void Dibujar();
    void Cerrar();
    void Reiniciar();
    bool DebeTerminar() const;

private:
    MiContactListener* contactListener = nullptr;
    void CrearEscenaFisica();
    //void VerificarColisiones(); esto ya no lo necesito
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
    Texture2D texFondo = {};
    Texture2D texPlataforma = {};

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
    Enemigo* enemigoMitad = nullptr; // sumo otro enemigo
    Plataforma* plataforma = nullptr;
    
    //para el boton
    b2Body* cuerpoBoton = nullptr;
    bool molineteActivo = true;

    //pantalla de inicio
    bool esPrimeraVez = true;
};