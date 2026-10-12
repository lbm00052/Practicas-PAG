#ifndef PRACTICA1_SHADERPROGRAM_H
#define PRACTICA1_SHADERPROGRAM_H

#include <string>

namespace PAG {

    /**
     * @brief Clase encargada de la gestión de los shader programs
     */
    class ShaderProgram {

        private:

            // Identificadores de los shaders
            GLuint idVS = 0;    // Identificador del vertex shader
            GLuint idFS = 0;    // Identificador del fragment shader
            GLuint idSP = 0;    // Identificador del shader program

        public:

            // Constructor
            ShaderProgram();

            // Destructor
            ~ShaderProgram();

            // Getter del id
            unsigned int getId() const;

            //Método para crear, compilar y enlazar el shader program
            void creaShaderProgram(const std::string& ruta);

    };
}

#endif //PRACTICA1_SHADERPROGRAM_H