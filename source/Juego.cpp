#include "Juego.h"
#include <cmath>



void MiContactListener::BeginContact(b2Contact* contact)
{
    b2Body* cuerpoA = contact->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contact->GetFixtureB()->GetBody();

    uintptr_t tipoA = cuerpoA->GetUserData().pointer;
    uintptr_t tipoB = cuerpoB->GetUserData().pointer;

    if (tipoA == (uintptr_t)TipoEntidad::JUGADOR) {
        juego->ProcesarContactoJugador((TipoEntidad)tipoB);
    }
    else if (tipoB == (uintptr_t)TipoEntidad::JUGADOR) {
        juego->ProcesarContactoJugador((TipoEntidad)tipoA);
    }
}

void Juego::Iniciar()
{
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "TRABAJO PRACTICO UNIDAD 4 - MAVII");
    InitAudioDevice();
    SetTargetFPS(60);

    musicaFondo = LoadMusicStream("Assets/musicaFondo.mp3");
    PlayMusicStream(musicaFondo);

    texVoltorb = LoadTexture("Assets/voltorb.png");
    texGastly = LoadTexture("Assets/Gastly.png");
    texGastlyInv = LoadTexture("Assets/GastlyInv.png");
    texFondo = LoadTexture("Assets/fondoFijo.png");
    texPlataforma = LoadTexture("Assets/texturaPlataforma.png");

    CrearEscenaFisica();
}

void Juego::CrearEscenaFisica()
{
    if (esPrimeraVez)
    {
        estado = EstadoJuego::INICIO;
        esPrimeraVez = false; // para que se vea una sola vez la pantalla incial
    }
    else
    {
        estado = EstadoJuego::JUGANDO; 
    }
    tiempoFinal = 0.0f;
    molineteActivo = true;

    // Contact Listener en el mundo
    contactListener = new MiContactListener(this);
    mundo.SetContactListener(contactListener);

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

    cuerpoSuelo->GetUserData().pointer = (uintptr_t)TipoEntidad::SUELO;

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
    jugador->GetCuerpo()->GetUserData().pointer = (uintptr_t)TipoEntidad::JUGADOR;

    enemigo = new Enemigo(&mundo, 750.0f, ALTO_PANTALLA - 90.0f, ESCALA, &texGastly, &texGastlyInv, ANCHO_PANTALLA);
    enemigo->GetCuerpo()->GetUserData().pointer = (uintptr_t)TipoEntidad::ENEMIGO;
    enemigoMitad = new Enemigo(&mundo, 500.0f, 300.0f, ESCALA, &texGastly, &texGastlyInv, ANCHO_PANTALLA, 1.5f); // el enemigo de mitad de pantalla
    enemigoMitad->GetCuerpo()->GetUserData().pointer = (uintptr_t)TipoEntidad::ENEMIGO;

    plataforma = new Plataforma(&mundo, 250.0f, 370.0f, ESCALA, &texPlataforma);

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

    // codeo la meta sobre la plataforma tocando al Voltorb chico
    float metaX = 900.0f; 
    float metaY = 220.0f; 

    // pequeña plataforma fija (ancho de 80, alto de 12)
    {
        b2BodyDef defPlatFija;
        defPlatFija.type = b2_staticBody;
        defPlatFija.position.Set(metaX / ESCALA, metaY / ESCALA);
        b2Body* cuerpoPlatFija = mundo.CreateBody(&defPlatFija);

        b2PolygonShape formaPlatFija;
        // Mitad de tamaño que la móvil
        formaPlatFija.SetAsBox(40.0f / ESCALA, 6.0f / ESCALA);

        b2FixtureDef fixPlatFija;
        fixPlatFija.shape = &formaPlatFija;
        fixPlatFija.friction = 0.5f;
        cuerpoPlatFija->CreateFixture(&fixPlatFija);
    }

    // Sensor de Voltorb chico
    {
        b2BodyDef defVoltorbMeta;
        defVoltorbMeta.type = b2_staticBody;
        // Lo posicionos un poquito más arriba de la plataforma fija 
        defVoltorbMeta.position.Set(metaX / ESCALA, (metaY - 20.0f) / ESCALA);
        cuerpoBandera = mundo.CreateBody(&defVoltorbMeta); 

        b2CircleShape formaVoltorbMeta;
        formaVoltorbMeta.m_radius = 20.0f / ESCALA; // un poquito más chico

        b2FixtureDef fixVoltorbMeta;
        fixVoltorbMeta.shape = &formaVoltorbMeta;
        fixVoltorbMeta.isSensor = true; //un sensor para que no choque solo detecte
        cuerpoBandera->CreateFixture(&fixVoltorbMeta);

        // META para que el ContactListener active la VICTORIA
        cuerpoBandera->GetUserData().pointer = (uintptr_t)TipoEntidad::META;
    }

    //para el boton
    {
        b2BodyDef defBoton;
        defBoton.type = b2_staticBody;
        defBoton.position.Set(450.0f / ESCALA, (ALTO_PANTALLA - 45.0f) / ESCALA); // Ubicación en el suelo
        cuerpoBoton = mundo.CreateBody(&defBoton);

        b2PolygonShape formaBoton;
        formaBoton.SetAsBox(25.0f / ESCALA, 5.0f / ESCALA); // botón chato

        b2FixtureDef fixBoton;
        fixBoton.shape = &formaBoton;
        fixBoton.isSensor = true; // solo detecta, no lo bloquea 
        cuerpoBoton->CreateFixture(&fixBoton);

        
        cuerpoBoton->GetUserData().pointer = (uintptr_t)TipoEntidad::BOTON;
    }
}

void Juego::Actualizar()
{
    UpdateMusicStream(musicaFondo);

    if (estado == EstadoJuego::INICIO)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            estado = EstadoJuego::JUGANDO; // para empezar a jugar
        }
        return; 
    }

    if (estado != EstadoJuego::JUGANDO)
    {
        tiempoFinal += GetFrameTime();
        if (IsKeyPressed(KEY_R)) Reiniciar();
        return;
    }

    
    if (jugador) jugador->Actualizar();
    if (enemigo) enemigo->Actualizar();
    if (enemigoMitad) enemigoMitad->Actualizar();
    if (plataforma) plataforma->Actualizar();

    mundo.Step(1.0f / 60.0f, 8, 3);
    //VerificarColisiones(); // ya no lo necesito

    if (IsKeyPressed(KEY_R)) Reiniciar();
}

void Juego::ProcesarContactoJugador(TipoEntidad otroTipo)
{
    if (estado != EstadoJuego::JUGANDO) return;

    if (otroTipo == TipoEntidad::ENEMIGO)
    {
        estado = EstadoJuego::DERROTA;
    }
    else if (otroTipo == TipoEntidad::META)
    {
        estado = EstadoJuego::VICTORIA;
    }
    else if (otroTipo == TipoEntidad::BOTON)
    {
        // estado del movimiento
        molineteActivo = !molineteActivo;

        
        if (molineteActivo) {
            revoluteJoint->SetMotorSpeed(0.5f);  // Vuelve a girar
        }
        else {
            revoluteJoint->SetMotorSpeed(0.0f);  // Se detiene en seco
        }
    }
}

void Juego::Dibujar()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(texFondo,
        { 0.0f, 0.0f, (float)texFondo.width, (float)texFondo.height },
        { 0.0f, 0.0f, (float)ANCHO_PANTALLA, (float)ALTO_PANTALLA },
        { 0.0f, 0.0f }, 0.0f, WHITE);    DibujarEscena();
    DibujarHUD();

    // visual según el estado actual 
    if (estado == EstadoJuego::INICIO)
    {
        // Fondo oscurecido para que resalte el texto
        DrawRectangle(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA, Fade(BLACK, 0.75f));

        // Título
        DrawText("Escapa, VOLTORB!", 280, 150, 50, GOLD);

        // Instrucciones
        DrawRectangleLines(100, 240, 800, 160, GRAY);

        DrawText("Voltorb tiene que escapar de los Gastlys y llegar junto al Voltorb chico.", 140, 270, 20, RAYWHITE);
        DrawText("Podes pulsar el boton del suelo para detener o accionar el molinete", 140, 310, 18, LIGHTGRAY);
        DrawText("y llegar mas facil a la meta.", 140, 340, 18, LIGHTGRAY);

        // Indicación para empezar
        DrawText("Presiona ENTER para empezar", 320, 460, 22, GREEN);
    }
    else if (estado != EstadoJuego::JUGANDO)
    {
        DibujarPantalllaFinal();
    }

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
    if (enemigoMitad) enemigoMitad->Dibujar();
    if (jugador) jugador->Dibujar();

    // dibujo molinete
    
    if (cuerpoMolinete)
    {
        b2Vec2 pos = cuerpoMolinete->GetPosition();
        float ang = cuerpoMolinete->GetAngle() * (180.0f / PI);

        
        float anchoAspa = 180.0f;
        float altoAspa = 20.0f;

        
        Rectangle sourceRec = { 0.0f, 0.0f, (float)texPlataforma.width, (float)texPlataforma.height };

        
        Rectangle destRec = { pos.x * ESCALA, pos.y * ESCALA, anchoAspa, altoAspa };

        
        Vector2 origen = { anchoAspa / 2.0f, altoAspa / 2.0f };

        // ASPA 1
        DrawTexturePro(texPlataforma, sourceRec, destRec, origen, ang, BLUE);

        // ASPA 2
        DrawTexturePro(texPlataforma, sourceRec, destRec, origen, ang + 90.0f, SKYBLUE);

        // eje de rotación
        DrawCircle((int)(pos.x * ESCALA), (int)(pos.y * ESCALA), 7.0f, WHITE);
    }
    // dibujo de la bandera
    if (cuerpoBandera)
    {
        // Dibujo la la pqueña plataforma fija 
//      // calculo la posición restando los 20 píxeles de desfase
        float platFijaX = cuerpoBandera->GetPosition().x * ESCALA;
        float platFijaY = (cuerpoBandera->GetPosition().y * ESCALA) + 20.0f;

        float anchoMiniPlat = 80.0f;  // Mitad de tamaño 
        float altoMiniPlat = 12.0f;   // Mitad de tamaño 

        Rectangle srcPlat = { 0.0f, 0.0f, (float)texPlataforma.width, (float)texPlataforma.height };
        Rectangle destPlat = { platFijaX, platFijaY, anchoMiniPlat, altoMiniPlat };
        Vector2 origenPlat = { anchoMiniPlat / 2.0f, altoMiniPlat / 2.0f };

        
        DrawTexturePro(texPlataforma, srcPlat, destPlat, origenPlat, 0.0f, WHITE);


        // Voltorb de la Meta 
        float voltorbMetaX = cuerpoBandera->GetPosition().x * ESCALA;
        float voltorbMetaY = cuerpoBandera->GetPosition().y * ESCALA;
        float radioVoltorbMeta = 20.0f; 

        Rectangle srcVoltorb = { 0.0f, 0.0f, (float)texVoltorb.width, (float)texVoltorb.height };
        
        Rectangle destVoltorb = { voltorbMetaX, voltorbMetaY, radioVoltorbMeta * 2.0f, radioVoltorbMeta * 2.0f };
        Vector2 origenVoltorb = { radioVoltorbMeta, radioVoltorbMeta };

        //dibujo estatico
        DrawTexturePro(texVoltorb, srcVoltorb, destVoltorb, origenVoltorb, 0.0f, WHITE);
    }

    //boton
    if (cuerpoBoton)
    {
        b2Vec2 pos = cuerpoBoton->GetPosition();
        // Si está activo es verde, si detuvo el molinete se pone rojo
        Color colorBoton = molineteActivo ? GREEN : RED;

        DrawRectanglePro(
            { pos.x * ESCALA, pos.y * ESCALA, 50.0f, 10.0f },
            { 25.0f, 5.0f },
            0.0f,
            colorBoton
        );
    }
}

void Juego::DibujarHUD()
{
    DrawRectangle(10, 10, 980, 50, Fade(BLACK, 0.45f));
    DrawText("TRABAJO PRACTICO INTEGRADOR - MAVII", 20, 18, 18, GOLD);
    DrawText("A/D: Moverse  |  ESPACIO: Saltar  |  R: Reiniciar  |  Llega con el otro Voltorb", 20, 38, 13, LIGHTGRAY);

    DrawRectangle(10, ALTO_PANTALLA - 30, 980, 25, Fade(BLACK, 0.4f));
    DrawText("Joints usados:  [Prismatic] Plataforma movil   [Revolute + motor] Molinete giratorio", 20, ALTO_PANTALLA - 24, 13, GRAY);
}

void Juego::DibujarPantalllaFinal()
{
    DrawRectangle(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA, Fade(BLACK, 0.65f));
    if (estado == EstadoJuego::VICTORIA)
    {
        DrawText("Lo lograste!", 300, 200, 72, GOLD);
        DrawText("Voltorb se reunio con su par!", 280, 290, 28, RAYWHITE);
    }
    else
    {
        DrawText("DERROTA...", 280, 200, 72, RED);
        DrawText("Un Gastly atrapo a Voltorb!", 280, 290, 28, RAYWHITE);
    }
    if ((int)(tiempoFinal * 2.0f) % 2 == 0)
        DrawText("Presiona R para reiniciar", 310, 370, 24, LIGHTGRAY);
}

void Juego::Reiniciar()
{
    
    mundo.SetContactListener(nullptr);

    
    delete contactListener;
    contactListener = nullptr;

    delete jugador; jugador = nullptr;
    delete enemigo; enemigo = nullptr;
    delete enemigoMitad; enemigoMitad = nullptr;
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
    cuerpoBoton = nullptr;
    molineteActivo = true; // Reestablecemos el estado inicial por defecto

    
    CrearEscenaFisica();
}

void Juego::Cerrar()
{
    delete contactListener;
    contactListener = nullptr;
    delete jugador;
    delete enemigo;
    delete enemigoMitad;
    delete plataforma;

    UnloadTexture(texVoltorb);
    UnloadTexture(texGastly);
    UnloadTexture(texGastlyInv);
    UnloadTexture(texFondo);
    UnloadTexture(texPlataforma);
    UnloadMusicStream(musicaFondo);
    CloseAudioDevice();
    CloseWindow();
}

bool Juego::DebeTerminar() const
{
    return WindowShouldClose();
}