#include <iostream>
#include <glad/glad.h>
#include "Renderer.h"
#include <algorithm>

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
    Renderer::~Renderer() {
        if ( idVBO != 0 ){
            glDeleteBuffers ( 1, &idVBO );
        }

        if ( idIBO != 0 ){
            glDeleteBuffers ( 1, &idIBO );
        }

        if ( idVAO != 0 ){
            glDeleteVertexArrays ( 1, &idVAO );
        }

        if (idVBOColor != 0){
            glDeleteBuffers(1, &idVBOColor);
        }
    }

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
        glBindVertexArray ( idVAO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );
    }

    // Método para cambiar el tamaño de la ventana
    void Renderer::onResize(int width, int height) {
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

    /**
     * Método para crear el VAO para el modelo a renderizar
     */
    void Renderer::creaModelo( ){
        /*
        // VERSIÓN NO ENTRELAZADA ================================
        // Vértices (posición)
        GLfloat posiciones[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };

        // Colores (uno por vértice)
        GLfloat colores[] = {
                1.0f, 0.0f, 0.0f,   // rojo
                0.0f, 1.0f, 0.0f,   // verde
                0.0f, 0.0f, 1.0f    // azul
        };

        GLuint indices[] = { 0, 1, 2 };

        // VAO
        glGenVertexArrays(1, &idVAO);
        if (idVAO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VAO.");
        }

        glBindVertexArray(idVAO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindVertexArray.");
        }

        // VBO de posiciones
        glGenBuffers(1, &idVBO);
        if (idVBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VBO.");
        }
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER).");
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(posiciones), posiciones, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el VBO (glBufferData).");
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(0);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }

        // VBO de colores
        glGenBuffers(1, &idVBOColor);
        if (idVBOColor == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VBO.");
        }
        glBindBuffer(GL_ARRAY_BUFFER, idVBOColor);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER).");
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(colores), colores, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el VBO (glBufferData).");
        }

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(1);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }

        // IBO
        glGenBuffers(1, &idIBO);
        if (idIBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el IBO.");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ELEMENT_ARRAY_BUFFER).");
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el IBO (glBufferData).");
        }*/

        // VERSIÓN ENTRELAZADA ================================
        GLfloat datos[] = {
                // posición        // color
                -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
                0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
        };

        GLuint indices[] = { 0, 1, 2 };

        glGenVertexArrays(1, &idVAO);
        if (idVAO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VAO.");
        }
        glBindVertexArray(idVAO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindVertexArray.");
        }

        glGenBuffers(1, &idVBO);
        if (idVBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VBO.");
        }
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER).");
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(datos), datos, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el VBO (glBufferData).");
        }

        // posición → location = 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(0);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }

        // color → location = 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(1);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }

        glGenBuffers(1, &idIBO);
        if (idIBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el IBO.");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ELEMENT_ARRAY_BUFFER).");
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el IBO (glBufferData).");
        }
    }
}