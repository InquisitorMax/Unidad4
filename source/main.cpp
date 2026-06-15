#ifndef NDEBUG
#include <vld.h>
#endif

#include "Juego.h"

int main(void)
{
    Juego juego;
    juego.Iniciar();

    while (!juego.DebeTerminar())
    {
        juego.Actualizar();
        juego.Dibujar();
    }

    juego.Cerrar();
    return 0;
}

// para algunas partes de ayudé con IA, por ejemplo, implementar isSensor, que me parece re útil para este y futuros proyectos
