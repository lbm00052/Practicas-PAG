#include <GL/gl.h>
#include "Renderer.h"
#include <algorithm>

namespace PAG {

    // Inicializamos la instancia a nullptr
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    // Constructor por defecto
    Renderer::Renderer(){
        // Se inicializa el color del fondo en gris
        fondo = {0.6,0.6,0.6,1};
    }

    // Destructor
    Renderer::~Renderer() {}

    // Método para obtener la instancia
    PAG::Renderer &PAG::Renderer::getInstancia() {

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
    }

    // Método para cambiar el tamaño de la ventana
    void Renderer::onResize(int width, int height) {
        glViewport(0,0,width,height);
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
    }

    // Método para obtener el color de fondo
    Color& Renderer::getColorFondo() {
        return fondo;
    }

    // Método para cambiar el color del fondo
    void Renderer::cambiarColor(Color c) {
        glClearColor(c.r, c.g, c.b, c.a);
    }
}