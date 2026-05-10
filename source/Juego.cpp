#include "Juego.h"

const float ESCALA = 30.0f;

void Juego::Iniciar()
{
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "MAVI II - Unidad 2");
    InitAudioDevice();
    SetTargetFPS(60);

    colorSuelo = Fade(DARKGREEN, 0.7f);
    texVoltorb = LoadTexture("Assets/voltorb.png");
    texCatapulta = LoadTexture("Assets/catapulta.png");
    gritoVoltorb = LoadSound("Assets/gritoVoltorb.wav");    
    musicaTetris = LoadMusicStream("Assets/musicaTetris.mp3");

    PlaySound(gritoVoltorb);   // una sola vez 
    sonidoReproducido = false;
    CrearParedes();

    // los tres rectángulos en el centro de la pantalla
    float baseX = 460.0f;          // posición X de la pila de los rectangulos
    float baseY = ALTO_PANTALLA - 60.0f; // nivel del suelo

    float ancho = 100.0f;
    float alto0 = 40.0f;
    float alto1 = 50.0f;
    float alto2 = 60.0f;

    obstaculos.resize(3);
    obstaculos[0].Iniciar(mundo, baseX, baseY, ancho, alto0, Fade(BROWN, 0.9f));
    obstaculos[1].Iniciar(mundo, baseX, baseY - alto0, ancho, alto1, Fade(MAROON, 0.9f));
    obstaculos[2].Iniciar(mundo, baseX, baseY - alto0 - alto1, 30.0f, 90.0f, Fade(DARKBROWN, 0.9f));

    float radio = 30.0f;
    voltorb.Iniciar(mundo, texVoltorb, 120.0f, ALTO_PANTALLA - 60.0f - radio);
}

void Juego::CrearParedes()
{
    // Suelo
    b2BodyDef defSuelo;
    defSuelo.type = b2_staticBody;
    defSuelo.position.Set((ANCHO_PANTALLA / 2.0f) / ESCALA, (ALTO_PANTALLA - 40.0f) / ESCALA);
    b2Body* cuerpoSuelo = mundo.CreateBody(&defSuelo);
    b2PolygonShape formaSuelo;
    formaSuelo.SetAsBox((ANCHO_PANTALLA / 2.0f) / ESCALA, 20.0f / ESCALA);
    cuerpoSuelo->CreateFixture(&formaSuelo, 0.0f);

    // Pared izquierda
    b2BodyDef defIzquierda;
    defIzquierda.type = b2_staticBody;
    defIzquierda.position.Set(0.0f, (ALTO_PANTALLA / 2.0f) / ESCALA);
    b2Body* cuerpoIzquierda = mundo.CreateBody(&defIzquierda);
    b2PolygonShape formaIzquierda;
    formaIzquierda.SetAsBox(10.0f / ESCALA, (ALTO_PANTALLA / 2.0f) / ESCALA);
    cuerpoIzquierda->CreateFixture(&formaIzquierda, 0.0f);

    // Pared derecha
    b2BodyDef defDerecha;
    defDerecha.type = b2_staticBody;
    defDerecha.position.Set((float)ANCHO_PANTALLA / ESCALA, (ALTO_PANTALLA / 2.0f) / ESCALA);
    b2Body* cuerpoDerecha = mundo.CreateBody(&defDerecha);
    b2PolygonShape formaDerecha;
    formaDerecha.SetAsBox(10.0f / ESCALA, (ALTO_PANTALLA / 2.0f) / ESCALA);
    cuerpoDerecha->CreateFixture(&formaDerecha, 0.0f);
}

void Juego::Actualizar()
{
    if (!sonidoReproducido && !IsSoundPlaying(gritoVoltorb))
    {
        PlayMusicStream(musicaTetris);
        sonidoReproducido = true;
    }

    if (sonidoReproducido)
        UpdateMusicStream(musicaTetris);

    if (IsKeyPressed(KEY_SPACE))
        voltorb.AplicarImpulso();

    mundo.Step(1.0f / 60.0f, 8, 3);
    
if (IsKeyPressed(KEY_R))
        Reiniciar();
} 


void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(colorFondo);

    DrawRectangle(0, ALTO_PANTALLA - 60, ANCHO_PANTALLA, 40, colorSuelo);

    for (const auto& obs : obstaculos)
        obs.Dibujar();

    voltorb.Dibujar();

    DrawRectangle(90, 70, 820, 90, Fade(BLACK, 0.18f));
    
    DrawText("Presiona R para reiniciar", 320, 125, 22, colorTextoSecundario);
    DrawText("Presiona ESPACIO para lanzar a Voltorb", 300, 540, 20, RAYWHITE);
    DrawTexturePro(
        texCatapulta,
        { 0, 0, (float)texCatapulta.width, (float)texCatapulta.height },
        { 120.0f, (float)ALTO_PANTALLA - 10.0f, 100.0f, 80.0f }, // posicion y tamaño
        { 50.0f, 80.0f },  // centrado en la base
        0.0f,
        WHITE
    );
    EndDrawing();
}

void Juego::Reiniciar()
{
    // mundo físico
    mundo.~b2World();
    new (&mundo) b2World(b2Vec2(0.0f, 9.8f));

    obstaculos.clear();
    CrearParedes();

    float alto0 = 40.0f, alto1 = 50.0f;
    float baseX = 460.0f;
    float baseY = ALTO_PANTALLA - 60.0f;

    obstaculos.resize(3);
    obstaculos[0].Iniciar(mundo, baseX, baseY, 100.0f, alto0, Fade(BROWN, 0.9f));
    obstaculos[1].Iniciar(mundo, baseX, baseY - alto0, 100.0f, alto1, Fade(MAROON, 0.9f));
    obstaculos[2].Iniciar(mundo, baseX, baseY - alto0 - alto1, 30.0f, 90.0f, Fade(DARKBROWN, 0.9f));

    float radio = 30.0f;
    voltorb.Iniciar(mundo, texVoltorb, 120.0f, ALTO_PANTALLA - 60.0f - radio);
}

void Juego::Cerrar()
{
    UnloadTexture(texVoltorb);
    UnloadTexture(texCatapulta);
    UnloadSound(gritoVoltorb);
    UnloadMusicStream(musicaTetris);
    CloseAudioDevice();
    CloseWindow();
}

bool Juego::DebeTerminar() const
{
    return WindowShouldClose();
}