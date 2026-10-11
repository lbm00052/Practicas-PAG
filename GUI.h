#ifndef PRACTICA1_GUI_H
#define PRACTICA1_GUI_H

#include "iostream"
#include <GLFW/glfw3.h>
#include "vector"

namespace PAG {

    /**
     * @brief Clase encargada de toda la interacción con ImGui y toda la interfaz de usuario de la aplicación
     *
     * Se implementa aplicando el patrón singleton.
     */
    class GUI {

        private:

            // Puntero al único objeto
            static GUI *instancia;

            // Vector para guardar los mensajes de la terminal
            std::vector<std::string> mensajes;

            // Constructor
            GUI();

        public:

            // Destructor
            virtual ~GUI();

            // Método para activar el destructor
            static void destruirInstancia();

            // Método para obtener la instancia
            static GUI &getInstancia();

            // Método para inicializar imGUI
            void init(GLFWwindow *window);

            // Método para inicialiazar el nuevo frame
            void beginFrame();

            // Método para dibujar la interfaz
            void dibuja();

            // Finalización/renderizado de ImGui
            void endFrame();

            // Método para añadir los mensajes que se mostrarán en terminal
            void addMensaje(const std::string &msg);

            // Método para los eventos de teclado
            void onKey(int key, int action);

            // Método para los eventos de ratón
            void onMouseButton(int button, int action);

            // Método para los eventos de scroll
            void onScroll(float xoffset, float yoffset);
    };
}

#endif //PRACTICA1_GUI_H