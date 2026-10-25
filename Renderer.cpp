#include <glad/glad.h>
#include "Renderer.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

namespace PAG {

    // Inicializamos la instancia a nullptr
    Renderer *Renderer::instancia = nullptr;

    // Constructor por defecto
    Renderer::Renderer() {
        // Se inicializa el color del fondo en gris
        fondo = {0.6, 0.6, 0.6, 1};
    }

    // Método para activar el destructor
    void Renderer::destruirInstancia(){
        delete instancia;
        instancia = nullptr;
    }

    // Destructor
    Renderer::~Renderer() {}

    // Método para obtener la instancia
    // Devuelvo el espacio de memoria mediante referencia (para acceder al objeto sin copiarlo)
    Renderer &Renderer::getInstancia() {

        // Si aun no existe la instancia, se crea.
        if (!instancia) {
            instancia = new Renderer();
        }
        // Devuelve el espacio de memoria del objeto
        return *instancia;
    }

    // Método para hacer el refresco de la escena
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Si existe un shader program se dibuja el modelo, sino no.
        if(shaderProgram.getId() == 0) return;

        // Activación del shader program y dibujo del triángulo
        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        glUseProgram ( shaderProgram.getId() ); // idSP

        // Obtener la matriz de la vista de la cámara y la de proyección
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix((float)width/(float)height);

        // Obtener la localización del uniform "view", "projection" y "model"
        GLint idView = glGetUniformLocation(shaderProgram.getId(),"view");
        GLint idProjection = glGetUniformLocation(shaderProgram.getId(),"projection");
        GLint idModel = glGetUniformLocation(shaderProgram.getId(),"model");

        // Enviar las matrices al shader
        glUniformMatrix4fv(idView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(idProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Por cada modelo que se vaya a dibujar
        for(Modelo& modelo : modelos) {
            // Obtiene la matriz de modelado de un modelo
            glm::mat4 model = modelo.getModelMatrix();
            // Envia la matriz de ese modelo al shader
            glUniformMatrix4fv(idModel,1,GL_FALSE,glm::value_ptr(model));
            // Dibuja la geometría del modelo en pantalla
            modelo.dibujar();
        }
    }

    // Método para cambiar el tamaño de la ventana
    void Renderer::onResize(int width, int height) {
        this->width = width;
        this->height = height;

        glViewport(0, 0, width, height);
    }

    // Método para cambiar el color de fondo según el movimiento de la rueda del ratón
    // y : dirección vertical en que se movió el ratón
    void Renderer::onScroll(double y) {
        float desp = y * 0.1, cota_inf = 0.0, cota_sup = 1.0; // variables

        // Cambio del color del fondo de pantalla
        // std::clamp(valor, cota inferior, cota superior) -> sirve para acotar nºs (librería: algorithm)
        fondo.r = std::clamp((fondo.r += desp), cota_inf, cota_sup);
        fondo.g = std::clamp((fondo.g += desp), cota_inf, cota_sup);
        fondo.b = std::clamp((fondo.b += desp), cota_inf, cota_sup);

        cambiarColor(fondo); // Colorear
    }

    // Método para inicializar OpenGL
    void Renderer::init() {
        // Inicializa el color del fondo
        cambiarColor(fondo);

        // Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
        glEnable(GL_DEPTH_TEST);
        // Activa el anti-aliasing
        glEnable(GL_MULTISAMPLE);
    }

    // Método para obtener el color de fondo
    Color &Renderer::getColorFondo() {
        return fondo;
    }

    // Método para cambiar el color del fondo
    void Renderer::cambiarColor(Color c) {
        glClearColor(c.r, c.g, c.b, c.a);
    }

    // Método para obtener el shaderProgram
    ShaderProgram &Renderer::getShaderProgram() {
        return shaderProgram;
    }

    // Método para obtener la cámara
    Camera &Renderer::getCamera() {
        return camera;
    }

    // Método para obtener el movimiento actual de la cámara
    MovimientosCamara Renderer::getMovimientoActual() const{
        return movimientoActual;
    }

    // Método para cambiar el movimiento actual de la cámara
    void Renderer::setMovimientoActual(MovimientosCamara mov) {
        this->movimientoActual = mov;
    }

    std::vector<Modelo>& Renderer::getModelos(){
        return modelos;
    }
}