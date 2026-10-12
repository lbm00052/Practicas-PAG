#include <glad/glad.h>
#include <stdexcept>
#include "ShaderProgram.h"
#include <fstream>
#include <sstream>

namespace PAG {

    //Método auxiliar para cargar los archivos GLSL
    std::string cargarArchivoGLSL(const std::string &ruta) {
        std::ifstream archivo;
        archivo.open(ruta);

        if (!archivo.is_open()) {
            throw std::runtime_error("[ERROR] No se pudo abrir el archivo GLSL: " + ruta);
        }

        std::stringstream streamShader;
        streamShader << archivo.rdbuf();
        archivo.close();

        return streamShader.str();
    }

    // Constructor
    ShaderProgram::ShaderProgram() {}

    // Destructor
    ShaderProgram::~ShaderProgram() {
        if (idVS != 0) {
            glDeleteShader(idVS);
        }

        if (idFS != 0) {
            glDeleteShader(idFS);
        }

        if (idSP != 0) {
            glDeleteProgram(idSP);
        }
    }

    // Getter del id
    unsigned int ShaderProgram::getId() const {
        return idSP;
    }

    /**
     * Método para crear, compilar y enlazar el shader program
     */
    void ShaderProgram::creaShaderProgram(const std::string &ruta) {

        // Limpiamos las variables por si antes se le había asignado algo
        if (idVS != 0) {
            glDeleteShader(idVS);
            idVS=0;
        }

        if (idFS != 0) {
            glDeleteShader(idFS);
            idFS=0;
        }

        if (idSP != 0) {
            glDeleteProgram(idSP);
            idSP=0;
        }

        std::string miVertexShader = cargarArchivoGLSL(ruta + "-vs.glsl");
        std::string miFragmentShader = cargarArchivoGLSL(ruta + "-fs.glsl");

        // Compila el Vertex Shader
        idVS = glCreateShader(GL_VERTEX_SHADER);

        // Comprobación
        if (idVS == 0) {
            /* Ha ocurrido un error al intentar crear el shader.*/
            throw std::runtime_error("[ERROR] Error al crear el shader.");
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
                throw std::runtime_error("[ERROR] Error al compilar el Vertex Shader:\n" + log);
            }
        }

        // Compila el Fragment Shader
        idFS = glCreateShader(GL_FRAGMENT_SHADER);

        // Comprobación
        if (idFS == 0) {
            /* Ha ocurrido un error al intentar crear el shader.*/
            throw std::runtime_error("[ERROR] Error al crear el shader.");
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
                throw std::runtime_error("[ERROR] Error al compilar el Fragment Shader:\n" + log);
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

                throw std::runtime_error(mensaje);
            }

        }
    }
}