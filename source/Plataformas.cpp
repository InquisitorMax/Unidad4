#include "Plataformas.h"

Plataforma::Plataforma(b2World* mundo, float x, float y, float escala, Color color)
    : escalaFisica(escala), velPlataforma(2.5f), colorPlat(color)
{
    float platX = x / escalaFisica;
    float platY = y / escalaFisica;

    // ancla 
    b2BodyDef defAncla;
    defAncla.type = b2_staticBody;
    defAncla.position.Set(platX, platY);
    cuerpoAncla = mundo->CreateBody(&defAncla);

    // plataforma movil
    b2BodyDef defPlat;
    defPlat.type = b2_dynamicBody;
    defPlat.position.Set(platX, platY);
    cuerpoPlat = mundo->CreateBody(&defPlat);

    b2PolygonShape forma;
    forma.SetAsBox(80.0f / escalaFisica, 12.0f / escalaFisica);

    b2FixtureDef fix;
    fix.shape = &forma;
    fix.density = 5.0f;
    fix.friction = 1.0f;
    fix.filter.groupIndex = -1; // para que no tome colisiones con el suelo
    cuerpoPlat->CreateFixture(&fix);

    // prismatic joint
    b2PrismaticJointDef pjd;
    b2Vec2 ejeVertical(0.0f, 1.0f);
    pjd.Initialize(cuerpoAncla, cuerpoPlat, cuerpoPlat->GetPosition(), ejeVertical);

    pjd.enableLimit = true;
    pjd.lowerTranslation = -180.0f / escalaFisica;
    pjd.upperTranslation = 130.0f / escalaFisica;

    pjd.enableMotor = true;
    pjd.motorSpeed = velPlataforma;
    pjd.maxMotorForce = 80000.0f;

    prismaticJoint = (b2PrismaticJoint*)mundo->CreateJoint(&pjd);
}

void Plataforma::Actualizar()
{
    float traslacion = prismaticJoint->GetJointTranslation();
    float tolerancia = 0.05f; 

    if (velPlataforma > 0.0f && traslacion >= (prismaticJoint->GetUpperLimit() - tolerancia))
    {
        velPlataforma = -2.5f; // para que suba
    }
    else if (velPlataforma < 0.0f && traslacion <= (prismaticJoint->GetLowerLimit() + tolerancia))
    {
        velPlataforma = 2.5f;  // para que baje
    }

    prismaticJoint->SetMotorSpeed(velPlataforma);
}

void Plataforma::Dibujar()
{
    // riel guia visual
    float rielX = 250.0f;
    float rielY1 = 370.0f - 200.0f; // pruebo valores
    float rielY2 = 370.0f + 100.0f;
    DrawRectangle((int)rielX - 3, (int)rielY1, 6, (int)(rielY2 - rielY1), Fade(GRAY, 0.3f));
    DrawRectangle((int)rielX - 10, (int)rielY1 - 4, 20, 8, DARKGRAY);
    DrawRectangle((int)rielX - 10, (int)rielY2 - 4, 20, 8, DARKGRAY);

    // de la plataforma
    b2Vec2 pos = cuerpoPlat->GetPosition();
    float ang = cuerpoPlat->GetAngle() * (180.0f / PI);
    DrawRectanglePro(
        { pos.x * escalaFisica, pos.y * escalaFisica, 160.0f, 24.0f },
        { 80.0f, 12.0f },
        ang,
        colorPlat
    );
}