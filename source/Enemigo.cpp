#include "Enemigo.h"
#include <cmath>

Enemigo::Enemigo(b2World* mundo, float x, float y, float escala, Texture2D* texNormal, Texture2D* texInvertido, int anchoPantalla, float velocidad)
    : texGastly(texNormal), texGastlyInv(texInvertido), radio(28.0f), escalaFisica(escala), velGastly(velocidad), miraDerecha(false), limiteAnchoPantalla(anchoPantalla)
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x / escalaFisica, y / escalaFisica);
    def.fixedRotation = true; // para que no ruede por el piso
    cuerpo = mundo->CreateBody(&def);

    b2CircleShape forma;
    forma.m_radius = radio / escalaFisica;

    b2FixtureDef fix;
    fix.shape = &forma;
    fix.density = 0.0f; // le asigno cero para que no le afecte la gravedad
    fix.friction = 0.0f;
    fix.restitution = 0.0f;
    fix.isSensor = true; // agrego esto para que la plataforma no choque contra el Gastly
    cuerpo->CreateFixture(&fix);

    // empieza moviendose para la izquierda
    cuerpo->SetLinearVelocity(b2Vec2(-velGastly, 0.0f));
}

void Enemigo::Actualizar()
{
    b2Vec2 velActual = cuerpo->GetLinearVelocity();
    b2Vec2 posG = cuerpo->GetPosition();
    float px = posG.x * escalaFisica;

    // rebota con los bordes de la pantalla
    if (px <= 30.0f && velActual.x < 0.0f)
    {
        velActual.x = velGastly;
        miraDerecha = true;
    }
    else if (px >= limiteAnchoPantalla - 80.0f && velActual.x > 0.0f)
    {
        velActual.x = -velGastly;
        miraDerecha = false;
    }

    velActual.y = 0.0f; // para que se mantenga flotando a misma altura 
    cuerpo->SetLinearVelocity(velActual);
}

void Enemigo::Dibujar()
{
    b2Vec2 pos = cuerpo->GetPosition();
    float px = pos.x * escalaFisica;
    float py = pos.y * escalaFisica-20; // para que se vea sobre el suelo
    float diam = radio * 3.5f; // pruebo valores para que los tamaños sean creibles

    
    Texture2D& texG = miraDerecha ? *texGastlyInv : *texGastly;

    Rectangle src = { 0, 0, (float)texG.width, (float)texG.height };
    Rectangle dest = { px, py, diam, diam };
    Vector2 orig = { radio, radio };
    DrawTexturePro(texG, src, dest, orig, 0.0f, WHITE);
}