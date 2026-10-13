#ifndef PRACTICA1_CAMERA_H
#define PRACTICA1_CAMERA_H

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace PAG {

    enum class MovimientosCamara{
        DOLLY, CRANE, PAN, TILT, ORBIT, ZOOM
    };

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

            // Campo de visión
            float fov = 45.0f;

            // Angulo acumulado para el movimiento orbit
            float orbitHorizontal = 0.0f;
            float orbitVertical = 0.0f;

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

            // Movimientos de la cámara
            void dolly(float dx, float dz);
            void crane(float dy);
            void pan(float angulo);
            void tilt(float angulo);
            void orbit(float horizontal, float vertical);
            void zoom(float delta);

    };
}

#endif //PRACTICA1_CAMERA_H