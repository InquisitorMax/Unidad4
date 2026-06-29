#include "Jugador.h"

Jugador::Jugador(b2World* mundo, float x, float y, float escala, Texture2D* textura)
    : texVoltorb(textura), radio(25.0f), escalaFisica(escala), miraDerecha(true)
{
    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(x / escalaFisica, y / escalaFisica);
    cuerpo = mundo->CreateBody(&def);
    cuerpo->SetLinearDamping(0.2f); // para el salto

    b2CircleShape forma;
    forma.m_radius = radio / escalaFisica;

    b2FixtureDef fix;
    fix.shape = &forma;
    fix.density = 1.0f;
    fix.friction = 0.6f;
    fix.restitution = 0.2f;
    cuerpo->CreateFixture(&fix);
}

void Jugador::Actualizar()
{
    //  detecta si esta tocando superficie para saltar
    bool tocaSuelo = false;
    for (b2ContactEdge* c = cuerpo->GetContactList(); c; c = c->next)
    {
        if (c->contact->IsTouching())
        {
            tocaSuelo = true;
            break;
        }
    }

    // valores de la velcidad
    float fuerzaMov = tocaSuelo ? 18.0f : 5.0f;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        cuerpo->ApplyForceToCenter(b2Vec2(fuerzaMov, 0.0f), true);
        miraDerecha = true;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        cuerpo->ApplyForceToCenter(b2Vec2(-fuerzaMov, 0.0f), true);
        miraDerecha = false;
    }

    // impulso del salto
    if (IsKeyPressed(KEY_SPACE) && tocaSuelo)
    {
        cuerpo->ApplyLinearImpulse(b2Vec2(0.0f, -20.0f), cuerpo->GetWorldCenter(), true);
    }
}

void Jugador::Dibujar()
{
    b2Vec2 pos = cuerpo->GetPosition();
    float ang = cuerpo->GetAngle() * (180.0f / PI); // conversion a grados
    float diam = radio * 2.1f;

    Rectangle src = { 0, 0, (float)texVoltorb->width, (float)texVoltorb->height };
    Rectangle dest = { pos.x * escalaFisica, pos.y * escalaFisica, diam, diam };
    Vector2 orig = { radio, radio };
    DrawTexturePro(*texVoltorb, src, dest, orig, ang, WHITE);
}