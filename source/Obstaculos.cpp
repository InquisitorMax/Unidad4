#include "Obstaculos.h"
#include <box2d.h>
#include "raylib.h"

const float ESCALA = 30.0f;

void Obstaculos::Iniciar(b2World& mundo, float x, float y, float ancho, float alto, Color color)
{
    this->ancho = ancho;
    this->alto = alto;
    this->color = color;

    b2BodyDef definicion;
    definicion.type = b2_dynamicBody;
    definicion.position.Set((x + ancho / 2.0f) / ESCALA, (y - alto / 2.0f) / ESCALA);
    cuerpo = mundo.CreateBody(&definicion);

    b2PolygonShape forma;
    forma.SetAsBox((ancho / 2.0f) / ESCALA, (alto / 2.0f) / ESCALA);

    b2FixtureDef accesorio;
    accesorio.shape = &forma;
    accesorio.density = 1.0f;
    accesorio.friction = 0.4f;
    accesorio.restitution = 0.3f;
    cuerpo->CreateFixture(&accesorio);
}

void Obstaculos::Dibujar() const
{
    //cuerpo es dinámico - posición y angulo de Box2D
    b2Vec2 pos = cuerpo->GetPosition();
    float angulo = cuerpo->GetAngle() * RAD2DEG;

    float pantallaX = pos.x * ESCALA;
    float pantallaY = pos.y * ESCALA;

    Rectangle destino = { pantallaX, pantallaY, ancho, alto };
    Vector2   centro = { ancho / 2.0f, alto / 2.0f };

    DrawRectanglePro(destino, centro, angulo, color);
    DrawRectanglePro(destino, centro, angulo, { 50, 50, 50, 80 }); // borde simulado
}