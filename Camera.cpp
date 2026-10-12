#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace PAG {

    // Constructor
        // La cámara se encuentra en el punto (0, 0, 3), mirando
        // al origen de coordenadas y usando el eje Y como
        // dirección vertical
    Camera::Camera() :
        pos(0.0f, 0.0f, 3.0f),
        lookAt(0.0f, 0.0f, 0.0f),
        up(0.0f, 1.0f, 0.0f) {}

    // Obtiene la matriz de vista
    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(pos, lookAt, up);
    }

    // Getter de la posición
    glm::vec3 &Camera::getPos() {
        return pos;
    }

    // Getter del punto al que mira la cámara
    glm::vec3 &Camera::getLookAt() {
        return lookAt;
    }

    // Getter de la vertical de la cámara
    glm::vec3 &Camera::getUp() {
        return up;
    }
}