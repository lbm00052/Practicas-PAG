# include <GL/gl.h>
#include "Renderer.h"

namespace PAG {

    // Inicializamos la instancia a nullptr
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    // Constructor por defecto
    Renderer::Renderer() {}

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
}