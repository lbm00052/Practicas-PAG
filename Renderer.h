#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

#include "ShaderProgram.h"
#include "Camera.h"
#include "Modelo.h"

// Espacio de nombres para las prácticas de PAG
namespace PAG {

    // Struct para el color
    struct Color {
        float r, g, b, a;
    };

    /**
     * @brief Clase encargada de encapsular la gestión del área de dibujo OpenGL
     *
     * Esta clase coordina el renderizado de las escenas OpenGL. Se implementa
     * aplicando el patrón Singleton. Está pensada para que las funciones callback
     * hagan llamadas a sus métodos.
     */
    class Renderer {

        private:

            // Puntero al único objeto
            static Renderer* instancia;

            // Constructor
            Renderer();

            // Color para el fondo de la instancia
            Color fondo;

            // Atributo para gestionar el shader program
            ShaderProgram shaderProgram;

            // Cámara virtual de la escena
            Camera camera;

            // Movimiento actual de la cámara (por defecto DOLLY)
            MovimientosCamara movimientoActual = MovimientosCamara::DOLLY;

            // Tamaño del viewport
            int width = 1024;
            int height = 576;

            // Modelo que se va a pintar
            Modelo modelo;

        public:

            // Destructor
            virtual ~Renderer();

            // Método para activar el destructor
            static void destruirInstancia();

            // Método para obtener la instancia
            static Renderer& getInstancia();

            // Método para hacer el refresco de la escena
            void refrescar();

            // Método para cambiar de tamaño de la ventana
            void onResize(int width, int height);

            // Método para cambiar el color del fondo según el scroll
            void onScroll(double y);

            // Método para inicializar OpenGL
            void init();

            // Método para obtener el color de fondo
            Color& getColorFondo();

            // Método para cambiar el color del fondo
            void cambiarColor(Color c);

            //Método para crear, compilar y enlazar el shader program
            ShaderProgram& getShaderProgram();

            // Método para obtener la cámara
            Camera& getCamera();

            // Método para obtener el movimiento actual de la cámara
            MovimientosCamara getMovimientoActual() const;

            // Método para cambiar el movimiento actual de la cámara
            void setMovimientoActual(MovimientosCamara mov);

            // Método para obtener el modelo
            Modelo& getModelo ();
    };
}

#endif //PRACTICA1_RENDERER_H