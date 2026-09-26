#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

// Espacio de nombres para las prácticas de PAG
namespace PAG {
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

        public:

            // Destructor
            virtual ~Renderer();

            // Devuelvo el espacio de memoria mediante referencia (para acceder al objeto sin copiarlo)
            static Renderer& getInstancia();

            // Método para hacer el refresco de la escena
            void refrescar();
    };
}

#endif //PRACTICA1_RENDERER_H