#include "Juego.h"

const float ESCALA = 30.0f;

void Juego::Iniciar()
{
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Mavi II - Unidad 3");
    InitAudioDevice();
    SetTargetFPS(60);
    musicaTetris = LoadMusicStream("Assets/musicaTetris.mp3");
    PlayMusicStream(musicaTetris);
    colorSuelo = Fade(DARKGREEN, 0.7f);
    texVoltorb = LoadTexture("Assets/voltorb.png");

    CrearEscenaFisica();
}

void Juego::CrearEscenaFisica()
{
    velocidadPlataforma = 3.0f;

    // Suelo y Paredes
    
    b2BodyDef defSuelo;
    defSuelo.type = b2_staticBody;
    defSuelo.position.Set((ANCHO_PANTALLA / 2.0f) / ESCALA, (ALTO_PANTALLA - 30.0f) / ESCALA);
    cuerpoSuelo = mundo.CreateBody(&defSuelo);

    b2PolygonShape formaSuelo;
    formaSuelo.SetAsBox((ANCHO_PANTALLA / 2.0f) / ESCALA, 30.0f / ESCALA);
    cuerpoSuelo->CreateFixture(&formaSuelo, 0.0f);

    b2BodyDef defIzg;
    defIzg.type = b2_staticBody;
    defIzg.position.Set(0.0f, (ALTO_PANTALLA / 2.0f) / ESCALA);
    b2Body* cuerpoIzg = mundo.CreateBody(&defIzg);
    b2PolygonShape formaIzg;
    formaIzg.SetAsBox(10.0f / ESCALA, (ALTO_PANTALLA / 2.0f) / ESCALA);
    cuerpoIzg->CreateFixture(&formaIzg, 0.0f);

    b2BodyDef defDer;
    defDer.type = b2_staticBody;
    defDer.position.Set((float)ANCHO_PANTALLA / ESCALA, (ALTO_PANTALLA / 2.0f) / ESCALA);
    b2Body* cuerpoDer = mundo.CreateBody(&defDer);
    b2PolygonShape formaDer;
    formaDer.SetAsBox(10.0f / ESCALA, (ALTO_PANTALLA / 2.0f) / ESCALA);
    cuerpoDer->CreateFixture(&formaDer, 0.0f);

    
    //Plataforma movil (uso Prismatic Joint)
    
    b2BodyDef defPlat;
    defPlat.type = b2_dynamicBody;
    defPlat.position.Set(250.0f / ESCALA, (ALTO_PANTALLA - 350.0f) / ESCALA);
    cuerpoPlataformaMovil = mundo.CreateBody(&defPlat);

    b2PolygonShape formaPlat;
    formaPlat.SetAsBox(80.0f / ESCALA, 15.0f / ESCALA);

    b2FixtureDef fixPlat;
    fixPlat.shape = &formaPlat;
    fixPlat.density = 1.0f;
    fixPlat.friction = 1.0f;
    cuerpoPlataformaMovil->CreateFixture(&fixPlat);

    b2PrismaticJointDef prismaticDef;
    b2Vec2 ejeMundo(1.0f, 0.0f);
    prismaticDef.Initialize(cuerpoSuelo, cuerpoPlataformaMovil, cuerpoPlataformaMovil->GetPosition(), ejeMundo);
    prismaticDef.enableLimit = true;
    prismaticDef.lowerTranslation = -150.0f / ESCALA;
    prismaticDef.upperTranslation = 400.0f / ESCALA;

    prismaticJoint = (b2PrismaticJoint*)mundo.CreateJoint(&prismaticDef);


    //Molinete (uso Revolute Joint)
    
    b2BodyDef defRotatorio;
    defRotatorio.type = b2_dynamicBody;
    
    defRotatorio.position.Set(750.0f / ESCALA, (ALTO_PANTALLA - 150.0f) / ESCALA);
    cuerpoRotatorio = mundo.CreateBody(&defRotatorio);

    b2PolygonShape formaRotatorio;
    formaRotatorio.SetAsBox(100.0f / ESCALA, 15.0f / ESCALA); // tamaño de 200x30

    b2FixtureDef fixRotatorio;
    fixRotatorio.shape = &formaRotatorio;
    fixRotatorio.density = 2.0f;
    cuerpoRotatorio->CreateFixture(&fixRotatorio);

    //bisagra (Revolute Joint)
    b2RevoluteJointDef revoluteDef;
    //plataforma unida al suelo
    revoluteDef.Initialize(cuerpoSuelo, cuerpoRotatorio, cuerpoRotatorio->GetPosition());

    
    revoluteDef.enableMotor = true;
    revoluteDef.motorSpeed = 3.0f; // Velocidad de giro
    revoluteDef.maxMotorTorque = 10000.0f; 

    revoluteJoint = (b2RevoluteJoint*)mundo.CreateJoint(&revoluteDef);


    //Cuerpo circular
    
    b2BodyDef defVoltorb;
    defVoltorb.type = b2_dynamicBody;
    defVoltorb.position.Set(250.0f / ESCALA, (ALTO_PANTALLA - 350.0f - 15.0f - radioVoltorb - 5.0f) / ESCALA);
    cuerpoVoltorb = mundo.CreateBody(&defVoltorb);

    b2CircleShape formaVoltorb;
    formaVoltorb.m_radius = radioVoltorb / ESCALA;

    b2FixtureDef fixVoltorb;
    fixVoltorb.shape = &formaVoltorb;
    fixVoltorb.density = 1.0f;
    fixVoltorb.friction = 0.8f;
    fixVoltorb.restitution = 0.4f;
    cuerpoVoltorb->CreateFixture(&fixVoltorb);
}

void Juego::Actualizar()
{
    UpdateMusicStream(musicaTetris);

    //Prismatic Joint
    if (prismaticJoint->GetJointTranslation() >= prismaticJoint->GetUpperLimit()) {
        velocidadPlataforma = -3.5f;
    }
    else if (prismaticJoint->GetJointTranslation() <= prismaticJoint->GetLowerLimit()) {
        velocidadPlataforma = 3.5f;
    }
    cuerpoPlataformaMovil->SetLinearVelocity(b2Vec2(velocidadPlataforma, 0.0f));

    //Movimiento de Voltorb
    float fuerzaMovimiento = 15.0f;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        cuerpoVoltorb->ApplyForceToCenter(b2Vec2(fuerzaMovimiento, 0.0f), true);
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        cuerpoVoltorb->ApplyForceToCenter(b2Vec2(-fuerzaMovimiento, 0.0f), true);
    }

    //Salto Realista
    
    bool tocandoSuperficie = false;

    
    for (b2ContactEdge* contacto = cuerpoVoltorb->GetContactList(); contacto != nullptr; contacto = contacto->next) {
        
        if (contacto->contact->IsTouching()) {
            tocandoSuperficie = true;
            break;
        }
    }

    // tecla ESPACIO para saltar
    if (IsKeyPressed(KEY_SPACE) && tocandoSuperficie) {
       
        b2Vec2 impulso(0.0f, -20.0f);
        cuerpoVoltorb->ApplyLinearImpulse(impulso, cuerpoVoltorb->GetWorldCenter(), true);
    }
    

    mundo.Step(1.0f / 60.0f, 8, 3);

    if (IsKeyPressed(KEY_R)) {
        Reiniciar();
    }
}

void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);

    // Suelo
    DrawRectangle(0, ALTO_PANTALLA - 60, ANCHO_PANTALLA, 60, colorSuelo);
    
    
    //Linea punteada para ver el riel del prismatic joint
    float inicioRielX = 250.0f - 150.0f;
    float finRielX = 250.0f + 400.0f;

    // el centro inicial en Y
    float rielY = ALTO_PANTALLA - 350.0f;

    // Dibujo los topes en los extremos
    DrawRectangle(inicioRielX - 5, rielY - 10, 10, 20, DARKGRAY);
    DrawRectangle(finRielX - 5, rielY - 10, 10, 20, DARKGRAY);

    // dibujar los guiones de la linea
    for (float x = inicioRielX; x < finRielX; x += 20.0f) {
        // el DrawLineEx dibuja una linea con grosor
        // los arametros son 'Punto Inicio', 'Punto Fin', 'Grosor' y 'Color'
        DrawLineEx({ x, rielY }, { x + 10.0f, rielY }, 2.0f, LIGHTGRAY);
    }
    //dibujo la platafrma 
    b2Vec2 posPlat = cuerpoPlataformaMovil->GetPosition();
    DrawRectanglePro(
        { posPlat.x * ESCALA, posPlat.y * ESCALA, 160.0f, 30.0f },
        { 80.0f, 15.0f },
        cuerpoPlataformaMovil->GetAngle() * RAD2DEG,
        GRAY
    );

    //Dibujo el molinete 
    b2Vec2 posRot = cuerpoRotatorio->GetPosition();
    DrawRectanglePro(
        { posRot.x * ESCALA, posRot.y * ESCALA, 200.0f, 30.0f },
        { 100.0f, 15.0f },
        cuerpoRotatorio->GetAngle() * RAD2DEG,
        ORANGE
    );
    //eje de la bisagra
    DrawCircle(posRot.x * ESCALA, posRot.y * ESCALA, 6.0f, BLACK);

    //Voltorb
    b2Vec2 posV = cuerpoVoltorb->GetPosition();
    float angV = cuerpoVoltorb->GetAngle() * RAD2DEG;

    Rectangle src = { 0.0f, 0.0f, (float)texVoltorb.width, (float)texVoltorb.height };
    Rectangle dest = { posV.x * ESCALA, posV.y * ESCALA, radioVoltorb * 2.0f, radioVoltorb * 2.0f };
    Vector2 centroVoltorb = { radioVoltorb, radioVoltorb };

    DrawTexturePro(texVoltorb, src, dest, centroVoltorb, angV, WHITE);

    
    DrawRectangle(25, 20, 950, 85, Fade(BLACK, 0.35f));
    DrawText("ENTREGA GUIA UNIDAD 3", 60, 30, 20, GOLD);
    DrawText("USO 2 JOINTS: Prismatic Joint (Plataforma Movil) Y Revolute Joint (Molinete)", 60, 55, 15, RAYWHITE);
    DrawText("Controles: A/D o Flechas = Moverse | ESPACIO = Saltar | R = Reiniciar", 60, 75, 14, LIGHTGRAY);

    EndDrawing();
}

void Juego::Reiniciar()
{
    b2Body* cuerpoActual = mundo.GetBodyList();

    while (cuerpoActual != nullptr) {
        b2Body* cuerpoABorrar = cuerpoActual;
        cuerpoActual = cuerpoActual->GetNext();
        mundo.DestroyBody(cuerpoABorrar);
    }

    CrearEscenaFisica();
}

void Juego::Cerrar()
{
    UnloadTexture(texVoltorb);
    UnloadMusicStream(musicaTetris);
    CloseAudioDevice();
    CloseWindow();
}

bool Juego::DebeTerminar() const
{
    return WindowShouldClose();
}