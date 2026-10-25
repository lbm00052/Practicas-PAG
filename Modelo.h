#ifndef PRACTICA1_MODELO_H
#define PRACTICA1_MODELO_H

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

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

            // Matriz de modelado
            glm::mat4 model = glm::mat4(1.0f);

            // Matrices para las transformaciones
            glm::vec3 traslacion = glm::vec3(0.0f);
            glm::vec3 rotacion = glm::vec3(0.0f);
            glm::vec3 escala = glm::vec3(1.0f);

        public:

            // Constructor
            Modelo();

            // Destructor
            ~Modelo();

            // Dibuja la geometría del modelo en pantalla
            void dibujar();

            // Método para crear el VAO para el modelo a renderizar
            void crearModelo();

            // Getter para la matriz de modelado
            glm::mat4& getModelMatrix();

            // Getters de las matrices para las transformaciones
            glm::vec3& getTraslacion();
            glm::vec3& getRotacion();
            glm::vec3& getEscala();

            // Actualiza la matriz de modelado
            void actualizarMatrizModelo();
    };
}

#endif //PRACTICA1_MODELO_H