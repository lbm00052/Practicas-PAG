#ifndef PRACTICA1_CAMERA_H
#define PRACTICA1_CAMERA_H

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace PAG {

    /**
     * @brief Clase encargada de representar la cámara virtual
     */
    class Camera {

        private:

            // Posición de la cámara en el espacio
            glm::vec3 pos;
            // Punto del espacio hacia el que mira la cámara
            glm::vec3 lookAt;
            // Vector que indica la dirección vertical de al cámara
            glm::vec3 up;

        public:

            // Constructor
            Camera();

            // Obtiene la matriz de vista
            glm::mat4 getViewMatrix() const;

            // Obtener la matriz de proyección
            glm::mat4 getProjectionMatrix(float aspecto) const;

            // Getters de los atributos
            glm::vec3& getPos();
            glm::vec3& getLookAt();
            glm::vec3& getUp();
    };
}

#endif //PRACTICA1_CAMERA_H