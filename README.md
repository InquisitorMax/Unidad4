El Laboratorio Mecánico de Mavix
Después de aprender a controlar el movimiento de los cuerpos dentro del mundo físico, Mavix descubrió algo todavía más interesante: mover objetos no alcanza para construir sistemas complejos.

Una caja puede caer.
Un proyectil puede salir disparado.
Un objeto puede recibir una fuerza o un impulso.
Pero el mundo físico no está formado solo por objetos aislados. Puertas, puentes, plataformas, poleas, resortes y mecanismos articulados existen porque distintas piezas se encuentran conectadas entre sí.

Para seguir avanzando en su exploración, Mavix decidió construir su propio laboratorio mecánico, un espacio de pruebas donde experimentar con distintos tipos de joints para descubrir cómo se relacionan los cuerpos dentro de Box2D.

El objetivo será utilizar los conceptos trabajados en esta unidad para construir un mecanismo físico compuesto por múltiples cuerpos conectados mediante joints.

Objetivo
Construir un sistema físico que utilice al menos dos tipos de joints trabajados en la unidad para formar una estructura o mecanismo funcional dentro del mundo físico.

El ejercicio permitirá experimentar con:

restricciones entre cuerpos

movimiento articulado

sistemas elásticos

mecanismos lineales o rotacionales

comportamiento físico de estructuras compuestas

Situación inicial
Se entrega un proyecto base que contiene:

un mundo físico (b2World)

un suelo estático

integración con Raylib

el bucle principal de simulación

A partir de esta base, los estudiantes deberán construir su mecanismo físico.

Consigna
Desarrollar un programa que represente un mecanismo físico compuesto, utilizando joints de Box2D para conectar varios cuerpos dentro del mundo de simulación.

El sistema deberá incluir:

1. Estructura compuesta
Crear una escena con múltiples cuerpos físicos conectados entre sí.

La construcción debe representar algún sistema mecánico reconocible, por ejemplo:

un péndulo

un puente colgante

una plataforma móvil

una estructura rígida articulada

un sistema con polea

un mecanismo propio diseñado por el estudiante

2. Uso de joints
El mecanismo deberá utilizar al menos dos tipos distintos de joints entre los siguientes:

Distance Joint

Revolute Joint

Prismatic Joint

Pulley Joint

Weld Joint

Cada joint deberá cumplir una función clara dentro del sistema.

3. Interacción con el entorno
El mecanismo debe poder:

moverse dentro del mundo físico

reaccionar a la gravedad

colisionar con otros cuerpos

mostrar claramente el efecto de las restricciones definidas

4. Visualización
El programa deberá representar visualmente todos los cuerpos involucrados utilizando Raylib.

Se recomienda además mostrar de alguna forma:

los puntos de conexión

la estructura general del mecanismo

o los elementos principales del sistema

Comportamiento esperado
Al ejecutar el programa:

debe observarse un sistema compuesto por varias piezas

los cuerpos deben permanecer conectados mediante joints

la estructura debe comportarse de manera coherente con el tipo de mecanismo construido

el efecto de cada joint debe ser visible dentro de la simulación

Opciones sugeridas
Para quienes prefieran partir de una idea concreta, se proponen algunas posibilidades:

Opción 1 — Puente colgante
Construir un puente formado por múltiples segmentos unidos mediante joints.

Opción 2 — Péndulo con estructura
Construir un péndulo suspendido de una estructura rígida.

Opción 3 — Plataforma mecánica
Construir una plataforma que se desplace mediante un Prismatic Joint.

Opción 4 — Sistema elástico
Construir una estructura que utilice Distance Joint para simular resortes.

Opción 5 — Mecanismo libre
Diseñar un sistema original utilizando joints de Box2D.

Entrega
La entrega deberá incluir:

código fuente del proyecto

ejecutable

breve descripción del mecanismo implementado

identificación de los joints utilizados

Frase final del práctico
Mavix ya no observa cuerpos aislados dentro del mundo físico.
Ahora comienza a construir mecanismos, a conectar piezas y a descubrir que el verdadero poder de la simulación aparece cuando los objetos dejan de estar solos.