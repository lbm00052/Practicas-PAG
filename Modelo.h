#ifndef PRACTICA1_MODELO_H
#define PRACTICA1_MODELO_H

// Aquí no puedo poner el include de glad porque da problemas de
// dependencias (la biblioteca glad se propagaba a una clase que
// ya la incluía), por lo que, como GLuint es un unsigned int pongo
// esto para que tome donde ponga GLuint como unsigned int
using GLuint = unsigned int;

namespace PAG {

    /**
     * @brief Clase encargada de representar un modelo
     */
    class Modelo {
        private:
            // Identificadores de la geometría
            GLuint idVAO = 0;   // Identificador del vertex array object
            GLuint idVBO = 0;   // Identificador del vertex buffer object
            GLuint idIBO = 0;   // Identificador del index buffer object

            // Solo para VBO no entrelazado
            GLuint idVBOColor = 0; // Identificador del vertex buffer object del color

        public:

            // Constructor
            Modelo();

            // Destructor
            ~Modelo();

            // Dibuja la geometría del modelo en pantalla
            void dibujar();

            // Método para crear el VAO para el modelo a renderizar
            void crearModelo();
    };
}

#endif //PRACTICA1_MODELO_H