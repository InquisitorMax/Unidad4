
Consigna
Desarrollar un programa donde una interacción dentro del mundo físico genere una respuesta lógica visible en el juego.

El sistema deberá incluir:

1. Objeto interactivo
Crear al menos un cuerpo dinámico que actúe como elemento principal de la interacción. Por ejemplo:

• un proyectil
• un objeto en movimiento
• un cuerpo controlado por el usuario

Este objeto deberá poder moverse dentro del mundo físico.

2. Elemento de detección
Implementar al menos uno de los siguientes:

• un objetivo físico (colisión normal)
• una zona sensor (isSensor = true)

Este elemento deberá permitir detectar cuándo ocurre una interacción relevante.

3. Detección del evento
Implementar un b2ContactListener que permita:

• detectar el inicio del contacto (BeginContact)
• identificar los cuerpos involucrados
• determinar si la interacción corresponde al evento esperado

4. Identificación de cuerpos
Utilizar userData (u otra estrategia equivalente) para:

• distinguir los distintos tipos de objetos
• reconocer cuándo ocurre la interacción correcta

5. Reacción del sistema
Cuando ocurra el evento, el sistema deberá generar una respuesta visible. Por ejemplo:

• mostrar un mensaje en pantalla
• cambiar el estado del juego
• reiniciar la escena
• activar/desactivar un mecanismo
• modificar el comportamiento de un objeto

6. Uso de estados (recomendado)
Organizar el comportamiento del sistema utilizando estados.

Por ejemplo:

• Waiting
• Running
• EventDetected
• Finished

Esto permitirá mantener ordenada la lógica del programa.

7. Visualización
El programa deberá mostrar claramente:

• los cuerpos involucrados
• la interacción que ocurre
• la respuesta del sistema

Se recomienda incluir:

• mensajes en pantalla
• indicadores visuales
• cambios de estado visibles

Comportamiento esperado
Al ejecutar el programa:

• debe existir al menos una interacción detectable
• el sistema debe reconocer correctamente el evento
• la lógica del juego debe reaccionar ante esa interacción
• el resultado debe ser visible para el usuario

Opciones sugeridas
Para quienes prefieran partir de una idea concreta:

Opción 1 — Tiro al objetivo
Un proyectil impacta contra un objetivo. Al impactar:

• se muestra un mensaje
• se cambia el estado

Opción 2 — Zona de activación
Un objeto entra en una región sensor.

Al ingresar: se activa un evento
Al salir: se desactiva

Opción 3 — Sistema de checkpoint
Un objeto atraviesa distintas zonas. Cada zona:

• registra un estado
• modifica el comportamiento

Opción 4 — Activador de mecanismo
Un contacto activa un sistema físico. Ejemplo:

• una plataforma comienza a moverse
• un objeto cambia su comportamiento

Opción 5 — Sistema libre
Diseñar un sistema original donde:

• la física genere eventos
• el juego los interprete
• y produzca una respuesta

