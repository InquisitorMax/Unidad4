#include "Juego.h"
#include <cmath>

void Juego::Iniciar()
{
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "TRABAJO PRACTICO INTEGRADOR - MAVII");
    InitAudioDevice();
    SetTargetFPS(60);

    musicaFondo = LoadMusicStream("Assets/musicaFondo.mp3");
    PlayMusicStream(musicaFondo);

    texVoltorb = LoadTexture("Assets/voltorb.png");
    texGastly = LoadTexture("Assets/Gastly.png");
    texGastlyInv = LoadTexture("Assets/GastlyInv.png");

    CrearEscenaFisica();
}

void Juego::CrearEscenaFisica()
{
    estado = EstadoJuego::JUGANDO;
    tiempoFinal = 0.0f;

    // suelo
    {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.Set(ANCHO_PANTALLA / 2.0f / ESCALA, (ALTO_PANTALLA - 20.0f) / ESCALA);
        cuerpoSuelo = mundo.CreateBody(&def);

        b2PolygonShape forma;
        forma.SetAsBox(ANCHO_PANTALLA / 2.0f / ESCALA, 20.0f / ESCALA);

        b2FixtureDef fixSuelo;
        fixSuelo.shape = &forma;
        fixSuelo.filter.groupIndex = -1; 
        cuerpoSuelo->CreateFixture(&fixSuelo);
    }

    // paredes
    {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.Set(10.0f / ESCALA, ALTO_PANTALLA / 2.0f / ESCALA);
        cuerpoParedIzq = mundo.CreateBody(&def);
        b2PolygonShape forma;
        forma.SetAsBox(10.0f / ESCALA, ALTO_PANTALLA / 2.0f / ESCALA);
        cuerpoParedIzq->CreateFixture(&forma, 0.0f);
    }
    {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.Set((ANCHO_PANTALLA - 10.0f) / ESCALA, ALTO_PANTALLA / 2.0f / ESCALA);
        cuerpoParedDer = mundo.CreateBody(&def);
        b2PolygonShape forma;
        forma.SetAsBox(10.0f / ESCALA, ALTO_PANTALLA / 2.0f / ESCALA);
        cuerpoParedDer->CreateFixture(&forma, 0.0f);
    }

    // entidades
    jugador = new Jugador(&mundo, 120.0f, ALTO_PANTALLA - 80.0f, ESCALA, &texVoltorb);
    enemigo = new Enemigo(&mundo, 750.0f, ALTO_PANTALLA - 90.0f, ESCALA, &texGastly, &texGastlyInv, ANCHO_PANTALLA);
    plataforma = new Plataforma(&mundo, 250.0f, 350.0f, ESCALA, colorPlat);

    // molinete 
    {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(700.0f / ESCALA, 280.0f / ESCALA);
        cuerpoMolinete = mundo.CreateBody(&def);

        b2PolygonShape forma;
        forma.SetAsBox(90.0f / ESCALA, 10.0f / ESCALA);
        b2FixtureDef fix;
        fix.shape = &forma;
        fix.density = 2.0f;
        cuerpoMolinete->CreateFixture(&fix);

        b2RevoluteJointDef rjd;
        rjd.Initialize(cuerpoSuelo, cuerpoMolinete, cuerpoMolinete->GetPosition());
        rjd.enableMotor = true;
        rjd.motorSpeed = 0.5f; // giro
        rjd.maxMotorTorque = 50000.0f;
        revoluteJoint = (b2RevoluteJoint*)mundo.CreateJoint(&rjd);
    }

    // meta
    {
        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.Set(800.0f / ESCALA, 80.0f / ESCALA);
        cuerpoBandera = mundo.CreateBody(&def);

        b2PolygonShape forma;
        forma.SetAsBox(anchoBandera / ESCALA, altoBandera / ESCALA);
        b2FixtureDef fix;
        fix.shape = &forma;
        fix.isSensor = true;
        cuerpoBandera->CreateFixture(&fix);
    }
}

void Juego::Actualizar()
{
    UpdateMusicStream(musicaFondo);

    if (estado != EstadoJuego::JUGANDO)
    {
        tiempoFinal += GetFrameTime();
        if (IsKeyPressed(KEY_R)) Reiniciar();
        return;
    }

    
    if (jugador) jugador->Actualizar();
    if (enemigo) enemigo->Actualizar();
    if (plataforma) plataforma->Actualizar();

    mundo.Step(1.0f / 60.0f, 8, 3);
    VerificarColisiones();

    if (IsKeyPressed(KEY_R)) Reiniciar();
}

void Juego::VerificarColisiones()
{
    if (!jugador || !enemigo) return;

    b2Vec2 posV = jugador->GetCuerpo()->GetPosition();
    b2Vec2 posG = enemigo->GetCuerpo()->GetPosition();
    b2Vec2 posB = cuerpoBandera->GetPosition();

    float radioV_m = jugador->GetRadio() / ESCALA;
    float radioG_m = enemigo->GetRadio() / ESCALA;
    float radioB_m = anchoBandera / ESCALA;

    // distancia con el gastly
    float dxG = posV.x - posG.x;
    float dyG = posV.y - posG.y;
    if (sqrtf(dxG * dxG + dyG * dyG) < (radioV_m + radioG_m))
    {
        estado = EstadoJuego::DERROTA;
        return;
    }

    // distancia a la bandera
    float dxB = posV.x - posB.x;
    float dyB = posV.y - posB.y;
    if (sqrtf(dxB * dxB + dyB * dyB) < (radioV_m + radioB_m + 0.25f))
    {
        estado = EstadoJuego::VICTORIA;
    }
}

void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);

    DibujarEscena();
    DibujarHUD();

    if (estado != EstadoJuego::JUGANDO) DibujarPantalllaFinal();

    EndDrawing();
}

void Juego::DibujarEscena()
{
    // escenario
    DrawRectangle(0, ALTO_PANTALLA - 40, ANCHO_PANTALLA, 40, colorSuelo);
    DrawRectangle(0, 0, 20, ALTO_PANTALLA, colorSuelo);
    DrawRectangle(ANCHO_PANTALLA - 20, 0, 20, ALTO_PANTALLA, colorSuelo);

    // dibujo entidades
    if (plataforma) plataforma->Dibujar();
    if (enemigo) enemigo->Dibujar();
    if (jugador) jugador->Dibujar();

    // dibujo molinete
    if (cuerpoMolinete)
    {
        b2Vec2 pos = cuerpoMolinete->GetPosition();
        float ang = cuerpoMolinete->GetAngle() * (180.0f / PI);
        DrawRectanglePro({ pos.x * ESCALA, pos.y * ESCALA, 180.0f, 20.0f }, { 90.0f, 10.0f }, ang, PURPLE);
        DrawCircle((int)(pos.x * ESCALA), (int)(pos.y * ESCALA), 7.0f, WHITE);
        DrawRectanglePro({ pos.x * ESCALA, pos.y * ESCALA, 180.0f, 20.0f }, { 90.0f, 10.0f }, ang + 90.0f, VIOLET);
    }

    // dibujo de la bandera
    if (cuerpoBandera)
    {
        b2Vec2 pos = cuerpoBandera->GetPosition();
        float px = pos.x * ESCALA;
        float py = pos.y * ESCALA;
        DrawRectangle((int)px - 3, (int)(py - altoBandera), 6, (int)(altoBandera * 2), LIGHTGRAY);
        DrawTriangle({ px + 3, py - altoBandera }, { px + 3, py - altoBandera + 35 }, { px + 40, py - altoBandera + 17 }, colorMeta);
        DrawText("META", (int)px - 20, (int)(py - altoBandera - 20), 16, colorMeta);
    }
}

void Juego::DibujarHUD()
{
    DrawRectangle(10, 10, 980, 50, Fade(BLACK, 0.45f));
    DrawText("TRABAJO PRACTICO INTEGRADOR - MAVII", 20, 18, 18, GOLD);
    DrawText("A/D: Moverse  |  ESPACIO: Saltar  |  R: Reiniciar  |  Llega a la META", 20, 38, 13, LIGHTGRAY);

    DrawRectangle(10, ALTO_PANTALLA - 30, 980, 25, Fade(BLACK, 0.4f));
    DrawText("Joints usados:  [Prismatic] Plataforma movil   [Revolute + motor] Molinete giratorio", 20, ALTO_PANTALLA - 24, 13, GRAY);
}

void Juego::DibujarPantalllaFinal()
{
    DrawRectangle(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA, Fade(BLACK, 0.65f));
    if (estado == EstadoJuego::VICTORIA)
    {
        DrawText("VICTORIA!", 300, 200, 72, GOLD);
        DrawText("Voltorb llego a la meta!", 280, 290, 28, RAYWHITE);
    }
    else
    {
        DrawText("DERROTA...", 280, 200, 72, RED);
        DrawText("Gastly atrapo a Voltorb!", 280, 290, 28, RAYWHITE);
    }
    if ((int)(tiempoFinal * 2.0f) % 2 == 0)
        DrawText("Presiona R para reiniciar", 310, 370, 24, LIGHTGRAY);
}

void Juego::Reiniciar()
{
    // memory leaks antes de limpiar el b2World
    delete jugador; jugador = nullptr;
    delete enemigo; enemigo = nullptr;
    delete plataforma; plataforma = nullptr;

    b2Body* actual = mundo.GetBodyList();
    while (actual != nullptr)
    {
        b2Body* aEliminar = actual;
        actual = actual->GetNext();
        mundo.DestroyBody(aEliminar);
    }

    cuerpoSuelo = nullptr;
    cuerpoParedIzq = nullptr;
    cuerpoParedDer = nullptr;
    cuerpoMolinete = nullptr;
    cuerpoBandera = nullptr;
    revoluteJoint = nullptr;

    CrearEscenaFisica();
}

void Juego::Cerrar()
{
    delete jugador;
    delete enemigo;
    delete plataforma;

    UnloadTexture(texVoltorb);
    UnloadTexture(texGastly);
    UnloadTexture(texGastlyInv);
    UnloadMusicStream(musicaFondo);
    CloseAudioDevice();
    CloseWindow();
}

bool Juego::DebeTerminar() const
{
    return WindowShouldClose();
}