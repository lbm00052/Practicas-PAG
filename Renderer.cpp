#include <iostream>
#include <glad/glad.h>
#include <GL/gl.h>
#include "Renderer.h"
#include <algorithm>
#include <string>

namespace PAG {

    // Inicializamos la instancia a nullptr
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    // Constructor por defecto
    Renderer::Renderer() {
        // Se inicializa el color del fondo en gris
        fondo = {0.6, 0.6, 0.6, 1};
    }

    // Destructor
    Renderer::~Renderer() {
        if ( idVS != 0 ){
            glDeleteShader ( idVS );
        }

        if ( idFS != 0 ){
            glDeleteShader ( idFS );
        }

        if ( idSP != 0 ){
            glDeleteProgram ( idSP );
        }

        if ( idVBO != 0 ){
            glDeleteBuffers ( 1, &idVBO );
        }

        if ( idIBO != 0 ){
            glDeleteBuffers ( 1, &idIBO );
        }

        if ( idVAO != 0 ){
            glDeleteVertexArrays ( 1, &idVAO );
        }
    }

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

        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        glUseProgram ( idSP );
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

    /**
     * Método para crear, compilar y enlazar el shader program
     */
    void PAG::Renderer::creaShaderProgram() {
        std::string miVertexShader =
                "#version 410\n"
                "layout (location = 0) in vec3 posicion;\n"
                "void main ()\n"
                "{  gl_Position = vec4 ( posicion, 1 );\n"
                "}\n";

        std::string miFragmentShader =
                "#version 410\n"
                "out vec4 colorFragmento;\n"
                "void main ()\n"
                "{  colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
                "}\n";

        // Compila el Vertex Shader
        idVS = glCreateShader(GL_VERTEX_SHADER);

            // Comprobación
        if (idVS == 0) {
            /* Ha ocurrido un error al intentar crear el shader.*/
            std::cerr << "[ERROR] Error al crear el shader." << std::endl;
        }

        const GLchar *fuenteVS = miVertexShader.c_str();
        glShaderSource(idVS, 1, &fuenteVS, nullptr);
        glCompileShader(idVS);

            // Comprobación
        GLint estadoVS = 0;
        glGetShaderiv(idVS, GL_COMPILE_STATUS, &estadoVS);

        if (estadoVS == GL_FALSE) {
            GLint tamLog = 0;
            glGetShaderiv(idVS, GL_INFO_LOG_LENGTH, &tamLog);

            if (tamLog > 0) {
                std::string log(tamLog, '\0');
                glGetShaderInfoLog(idVS, tamLog, nullptr, &log[0]);
                std::cerr << "[ERROR] Error al compilar el Vertex Shader:\n" << log << std::endl;
            }
        }

        // Compila el Fragment Shader
        idFS = glCreateShader(GL_FRAGMENT_SHADER);

            // Comprobación
        if (idFS == 0) {
            /* Ha ocurrido un error al intentar crear el shader.*/
            std::cerr << "[ERROR] Error al crear el shader." << std::endl;
        }

        const GLchar *fuenteFS = miFragmentShader.c_str();
        glShaderSource(idFS, 1, &fuenteFS, nullptr);
        glCompileShader(idFS);

            // Comprobación
        GLint estadoFS = 0;
        glGetShaderiv(idFS, GL_COMPILE_STATUS, &estadoFS);

        if (estadoFS == GL_FALSE) {
            GLint tamLog = 0;
            glGetShaderiv(idFS, GL_INFO_LOG_LENGTH, &tamLog);

            if (tamLog > 0) {
                std::string log(tamLog, '\0');
                glGetShaderInfoLog(idFS, tamLog, nullptr, &log[0]);
                std::cerr << "[ERROR] Error al compilar el Fragment Shader:\n" << log << std::endl;
            }
        }

        // Crea y enlaza el programa
        idSP = glCreateProgram();
        glAttachShader(idSP, idVS);
        glAttachShader(idSP, idFS);
        glLinkProgram(idSP);

        // Comprobación de la creación y enlazado del programa
        GLint resultadoEnlazado = 0;
        glGetProgramiv(idSP, GL_LINK_STATUS, &resultadoEnlazado);

        if (resultadoEnlazado == GL_FALSE) {
            /* Ha habido un error en la compilación.
            Para saber qué ha pasado, tenemos que recuperar el mensaje de error de
            OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "";
            glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &tamMsj);

            if (tamMsj > 0) {
                GLchar *mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog(idSP, tamMsj, &datosEscritos, mensajeFormatoC);
                mensaje.assign(mensajeFormatoC);
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                std::cerr << mensaje << std::endl;
            }

        }
    }

    /**
 * Método para crear el VAO para el modelo a renderizar
 */
    void PAG::Renderer::creaModelo( ){
        GLfloat vertices[] = { -.5, -.5, 0,
                              .5, -.5, 0,
                              .0,  .5, 0 };
        GLuint indices[] = { 0, 1, 2 };

        // Creación y configuración del VAO
        glGenVertexArrays(1, &idVAO );
        if (idVAO == 0) {
            std::cerr << "[ERROR] No se pudo generar el VAO." << std::endl;
        }

        glBindVertexArray(idVAO);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo al hacer glBindVertexArray." << std::endl;
        }

        // Creación y carga dle VBO
        glGenBuffers(1, &idVBO);
        if (idVBO == 0) {
            std::cerr << "[ERROR] No se pudo generar el VBO." << std::endl;
        }

        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER)." << std::endl;
        }

        glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo al cargar datos en el VBO (glBufferData)." << std::endl;
        }

        // Configuración del Vertex Attirb
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo en glVertexAttribPointer." << std::endl;
        }

        glEnableVertexAttribArray(0);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo en glEnableVertexAttribArray(0)." << std::endl;
        }

        // Crear y cargar el IBO
        glGenBuffers (1, &idIBO);
        if (idIBO == 0) {
            std::cerr << "[ERROR] No se pudo generar el IBO." << std::endl;
        }

        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, idIBO);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo al hacer glBindBuffer(GL_ELEMENT_ARRAY_BUFFER)." << std::endl;
        }

        glBufferData (GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "[ERROR] Fallo al cargar datos en el IBO (glBufferData)." << std::endl;
        }
    }
}