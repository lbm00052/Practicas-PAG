#include "GUI.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Renderer.h"

namespace PAG {

    // Inicializamos la instancia a nulptr
    GUI* GUI::instancia = nullptr;

    // Constructor
    GUI::GUI() {}

    // Método para activar el destructor
    void GUI::destruirInstancia(){
        delete instancia;
        instancia = nullptr;
    }

    // Destructor
    GUI::~GUI(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // Método para obtener la instancia
    // Devuelvo el espacio de memoria mediante referencia (para acceder al objeto sin copiarlo)
    GUI& GUI::getInstancia(){
        if(!instancia)
            instancia = new GUI();

        return *instancia;
    }

    // Método para inicializar imGUI
    void GUI::init(GLFWwindow* window){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Inicialización de GLFW y OpenGL con ImGui
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    // Método para añadir los mensajes que se mostrarán en terminal
    void GUI::addMensaje(const std::string& msg){
        mensajes.push_back(msg);
    }

    // Método para inicialiazar el nuevo frame
    void GUI::beginFrame(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // Método para dibujar la interfaz
    void GUI::dibuja(){

        // VENTANA DE MENSAJES ===========
            // Posición de la ventana (solo la primera vez)
        ImGui::SetNextWindowPos(ImVec2 (10, 10), ImGuiCond_Once);
            // Abre la ventana
        if ( ImGui::Begin("Mensajes") ) {
            ImGui::SetWindowFontScale(1.0f); // Ajuste de la fuente

            // Zona con scroll automático
            ImGui::BeginChild("scroll_region",ImVec2(0,0),true);

            for ( const std::string& s : mensajes ) {
                ImGui::Text("%s",s.c_str()); // Dibuja cada mensaje
            }

            // Desplaza el scroll al final de cada frame (para que se muestre el último mensaje)
            ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild(); // Fin de la ventana de scroll
        }
        ImGui::End(); // Fin de la ventana de mensajes

        // VENTANA DE SELECCIÓN DE COLOR ==========
        ImGui::SetNextWindowPos(ImVec2 (10, 250), ImGuiCond_Once);
        if ( ImGui::Begin("Selector de color") ) {
            ImGui::SetWindowFontScale(1.0f);

            // Obtener referencia al color de fondo
            PAG::Color& c = PAG::Renderer::getInstancia().getColorFondo();
            // Selector de color RGB
            ImGui::ColorPicker4("Color", (float*)&c);
            // Cambia el color en el Renderer
            PAG::Renderer::getInstancia().cambiarColor(c);
        }
        ImGui::End();
    }

    // Finalización/renderizado de ImGui
    void GUI::endFrame(){
        // Renderiza ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Método para los eventos de teclado
    void GUI::onKey(int key, int action){

        // GLFW trata las teclas de forma diferente a ImGui. Mientras que en GLFW un int
        // se corresponde con una tecla, en el caso de ImGui estas se encuentran dentro de
        // un enum, por lo que hay que mapearlas manualmente.

        ImGuiIO& io = ImGui::GetIO();

        // Mapear teclas especiales
        switch (key) {
            case GLFW_KEY_LEFT:
                io.AddKeyEvent(ImGuiKey_LeftArrow, action == GLFW_PRESS);
                break;

            case GLFW_KEY_RIGHT:
                io.AddKeyEvent(ImGuiKey_RightArrow, action == GLFW_PRESS);
                break;

            case GLFW_KEY_UP:
                io.AddKeyEvent(ImGuiKey_UpArrow, action == GLFW_PRESS);
                break;

            case GLFW_KEY_DOWN:
                io.AddKeyEvent(ImGuiKey_DownArrow, action == GLFW_PRESS);
                break;

            case GLFW_KEY_ESCAPE:
                io.AddKeyEvent(ImGuiKey_Escape, action == GLFW_PRESS);
        }
    }

    // Método para los eventos de ratón
    void GUI::onMouseButton( int button, int action){
        ImGuiIO& io = ImGui::GetIO();

        if ( action == GLFW_PRESS ) {
            io.AddMouseButtonEvent(button, true );
        } else if ( action == GLFW_RELEASE ) {
            io.AddMouseButtonEvent(button, false );
        }
    }

    // Método para los eventos de scroll
    void GUI::onScroll(float xoffset, float yoffset){
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(xoffset, yoffset);
    }
}