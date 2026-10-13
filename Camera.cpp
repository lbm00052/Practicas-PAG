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

    // Obtener la matriz de proyección
    glm::mat4 Camera::getProjectionMatrix(float aspecto) const {
        return glm::perspective(glm::radians(fov), aspecto, 0.1f, 100.f);
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

    // MOVIMIENTOS DE LA CÁMARA ==============

    void Camera::dolly(float dx, float dz) {
        // Desplazamiento horinzontal del ratón (eje X)
        pos.x += dx;
        lookAt.x += dx;

        // Desplazamiento vertical del ratón (eje Z)
        pos.z += dz;
        lookAt.z += dz;
    }

    void Camera::crane(float dy) {
        // Desplazamiento vertical del ratón (eje Y)
        pos.y += dy;
        lookAt.y += dy;
    }

    // Rotación vertical sobre la cámara
    void Camera::pan(float angulo) {

        // Se calcula hacia donde mira la cámara
        glm::vec3 direccion = lookAt - pos;

        // Se construye una matriz de rotación respecto al eje Y
        glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f),angulo,glm::vec3(0.0f, 1.0f, 0.0f));

        // Se rota el vector dirección de la camara
        glm::vec3 nuevaDireccion = glm::vec3(rotacion * glm::vec4(direccion, 0.0f));

        // Calcula el hacia donde mira ahora la cámara
        lookAt = pos + nuevaDireccion;
    }

    // Rotación horizontal sobre la cámara
    void Camera::tilt(float angulo) {

        // Se calcula hacia donde mira la cámara
        glm::vec3 direccion = lookAt - pos;

        // Se calcula el eje lateral de la cámara
        glm::vec3 ejeDerecha = glm::normalize(glm::cross(direccion,up));

        // Se crea la matiz de rotación respecto al eje lateral de la cámara
        glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f),angulo,ejeDerecha);

        // Se rota el vector dirección de la camara
        glm::vec3 nuevaDireccion = glm::vec3(rotacion *glm::vec4(direccion, 0.0f));

        // Se calcula el nuevo punto al que rota la cámara
        lookAt = pos + nuevaDireccion;
    }

    // Orbitar al rededor del modelo
    void Camera::orbit(float horizontal, float vertical) {

        // Se guardan los ángulos acumulados
        orbitHorizontal += horizontal;
        orbitVertical += vertical;

        // Acotamos la rotación en vertical (solo puede subir hasta los 89 grados)
        orbitVertical = glm::clamp(orbitVertical,glm::radians(-89.0f),glm::radians(89.0f));

        // Se calcula la distancia entre la posición de la camara y el punto al que mira
        // para obtener el radio de la circunferencia sobre la que rota.
        float radio = glm::length(pos - lookAt);

        // Cacula la posición de la cámara usando coordenadas esféricas
        pos.x = lookAt.x + radio * cos(orbitVertical) * sin(orbitHorizontal);
        pos.y = lookAt.y + radio * sin(orbitVertical);
        pos.z = lookAt.z + radio * cos(orbitVertical) * cos(orbitHorizontal);
    }

    // Acortar o alargar el campo de visión
    void Camera::zoom(float delta) {

        // Se modifica el campo de visión
        fov -= delta;
        // Se acota el campo de visión para que no se produzcan deformaciones
        fov = glm::clamp(fov,10.0f,120.0f);
    }
}