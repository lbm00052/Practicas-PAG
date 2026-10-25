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
            // Es false porque sino no toma los callback que yo he
            // realizado, sino los que crea automaticamente
        ImGui_ImplGlfw_InitForOpenGL(window, false);
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

        // VENTANA DE ELECCIÓN DE SHADER
        ImGui::SetNextWindowPos(ImVec2(10,600), ImGuiCond_Once);
        if (ImGui::Begin("Shader Program")){
            ImGui::SetWindowFontScale(1.0f);

            ImGui::InputText("##", rutaShader,sizeof(rutaShader));

            if(ImGui::Button("Cargar")){
                try{
                    addMensaje("Cargando: " + std::string(rutaShader));
                    PAG::Renderer::getInstancia().getShaderProgram().creaShaderProgram(rutaShader);
                    addMensaje("Shader cargado correctamente");
                } catch (const std::exception& e) {
                    addMensaje(e.what());
                }
            }
        }
        ImGui::End();

        // VENTANA PARA LOS MOVIMIENTOS DE LA CÁMARA
        ImGui::SetNextWindowPos(ImVec2(10, 750), ImGuiCond_Once);
        if (ImGui::Begin("Camara")) {
            ImGui::SetWindowFontScale(1.0f);

            static int movimiento = 0;
            const char* movimientos[] = {"Dolly","Crane","Pan","Tilt","Orbit","Zoom"};

            PAG::Renderer& render = Renderer::getInstancia();
            PAG::Camera& camera = render.getCamera();

            // Selector
            if (ImGui::Combo("##",&movimiento,movimientos,IM_ARRAYSIZE(movimientos))) {

                switch (movimiento) {
                    case 0:
                        render.setMovimientoActual(MovimientosCamara::DOLLY);
                        break;

                    case 1:
                        render.setMovimientoActual(MovimientosCamara::CRANE);
                        break;

                    case 2:
                        render.setMovimientoActual(MovimientosCamara::PAN);
                        break;

                    case 3:
                        render.setMovimientoActual(MovimientosCamara::TILT);
                        break;

                    case 4:
                        render.setMovimientoActual(MovimientosCamara::ORBIT);
                        break;

                    case 5:
                        render.setMovimientoActual(MovimientosCamara::ZOOM);
                        break;
                }
            }

            ImGui::Separator();

            // Botones según selector
            switch (render.getMovimientoActual()) {
                case MovimientosCamara::DOLLY:
                    ImGui::Text("Direccion");

                    if(ImGui::Button("^ Back ^")){
                        camera.dolly(0.0f,-0.1f);
                    }

                    if(ImGui::Button("<- Left")){
                        camera.dolly(-0.1f,0.0f);
                    }

                    ImGui::SameLine();

                    if(ImGui::Button("Right ->")){
                        camera.dolly(0.1f,0.0f);
                    }

                    if(ImGui::Button("v Front v")){
                        camera.dolly(0.0f,0.1f);
                    }

                    break;

                case MovimientosCamara::CRANE:
                    ImGui::Text("Direccion");

                    if(ImGui::Button("^ Up ^")){
                        camera.crane(0.1f);
                    }

                    if(ImGui::Button("v Down v")){
                        camera.crane(-0.1f);
                    }

                    break;

                case MovimientosCamara::PAN:
                    ImGui::Text("Direccion");

                    if(ImGui::Button("<- Left")) {
                        camera.pan(0.1f);
                    }

                    ImGui::SameLine();

                    if(ImGui::Button("Right ->")) {
                        camera.pan(-0.1f);
                    }

                    break;

                case MovimientosCamara::TILT:
                    ImGui::Text("Direccion");

                    if(ImGui::Button("^ Up ^")) {
                        camera.tilt(0.1f);
                    }

                    if(ImGui::Button("v Down v")) {
                        camera.tilt(-0.1f);
                    }

                    break;

                case MovimientosCamara::ORBIT:
                    ImGui::Text("Latitud");

                    if(ImGui::Button("^ North ^")) {
                        camera.orbit(0.0f, 0.1f);
                    }

                    if(ImGui::Button("v South v")) {
                        camera.orbit(0.0f, -0.1f);
                    }

                    ImGui::Spacing();

                    ImGui::Text("Longitud");

                    if(ImGui::Button("<- West")) {
                        camera.orbit(-0.1f, 0.0f);
                    }

                    ImGui::SameLine();

                    if(ImGui::Button("East ->")) {
                        camera.orbit(0.1f, 0.0f);
                    }

                    break;

                case MovimientosCamara::ZOOM:
                    ImGui::Text("Angulo");

                    float fovActual = camera.getFov();

                    if(ImGui::SliderFloat("##Zoom",&fovActual,10.0f,120.0f)) {
                        camera.setFov(fovActual);
                    }

                    break;
            }
        }
        ImGui::End();

        // VENTANA PARA LAS TRANSFORMACIONES DE UN MODELO
        ImGui::SetNextWindowPos(ImVec2(350,10),ImGuiCond_Once);

        if(ImGui::Begin("Transformacion")){

            std::vector<Modelo>& modelos = Renderer::getInstancia().getModelos();

            // Si no hay modelos no se puede hacer nada
            if (!modelos.empty()) {

                Modelo &modelo = Renderer::getInstancia().getModelos()[modeloSeleccionado];

                bool modificado = false;

                ImGui::Text("Traslacion");
                modificado |= ImGui::DragFloat3("T", &modelo.getTraslacion().x, 0.05f);

                ImGui::Text("Rotacion");
                modificado |= ImGui::DragFloat3("R", &modelo.getRotacion().x, 1.0f);

                ImGui::Text("Escala");
                modificado |= ImGui::DragFloat3("S", &modelo.getEscala().x, 0.05f);

                // Para evitar que el modelo se esté actualizando constantemente
                if (modificado) {
                    modelo.actualizarMatrizModelo();
                }
            }
        }
        ImGui::End();

        // VENTANA PARA SELECCIONAR UN MODELO
        if(ImGui::Begin("Modelos")){

            std::vector<Modelo>& modelos = Renderer::getInstancia().getModelos();

            ImGui::Text("Numero de modelos: %d",(int)modelos.size());

            ImGui::InputInt("Seleccionado",&modeloSeleccionado);

            if (!modelos.empty()){
                modeloSeleccionado = std::clamp(modeloSeleccionado, 0, (int) modelos.size() - 1);
            } else {
                modeloSeleccionado = 0;
            }
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

            case GLFW_KEY_DELETE:
                io.AddKeyEvent(ImGuiKey_Delete,action == GLFW_PRESS);
                break;

            case GLFW_KEY_BACKSPACE:
                io.AddKeyEvent(ImGuiKey_Backspace, action == GLFW_PRESS);
                break;

            case GLFW_KEY_ESCAPE:
                io.AddKeyEvent(ImGuiKey_Escape, action == GLFW_PRESS);
                break;
        }
    }

    // Método para eventos de teclado (especifico para letras y números)
        // Es necesario porque he desactivado los callbacks automáticos de ImGUI
    void GUI::onChar(unsigned int c) {
        ImGui::GetIO().AddInputCharacter(c);
    }

    // Método para los eventos de ratón
    void GUI::onMouseButton( int button, int action){
        ImGuiIO& io = ImGui::GetIO();

        if ( action == GLFW_PRESS ) {
            io.AddMouseButtonEvent(button, true );
        } else if ( action == GLFW_RELEASE ) {
            io.AddMouseButtonEvent(button, false );
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT){
            if (action == GLFW_PRESS){
                arrastrando = true;
            } else if (action == GLFW_RELEASE) {
                arrastrando = false;
            }
        }
    }

    // Método para los eventos de scroll
    void GUI::onScroll(float xoffset, float yoffset){
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(xoffset, yoffset);
    }

    // Métodos para el movimiento del ratón
    void GUI::onCursorPosition(double xpos, double ypos) {

        // Función para que ImGui permita seguir usando las
        // ventanas de interfaz
        if(ImGui::GetIO().WantCaptureMouse){
            arrastrando = false;
            return;
        }

        // Si no se está arrastrando no se esta haciendo nada.
        if (!arrastrando) {
            ultimoX = xpos;
            ultimoY = ypos;
            return;
        }

        // Se calcula el desplazamiento que realiza el ratón
        double dx = xpos - ultimoX;
        double dy = ypos - ultimoY;

        Renderer& renderer = Renderer::getInstancia();
        Camera& camera = renderer.getCamera();

        // Según el movimiento actual la cámara se mueve de una manera u otra
        switch(renderer.getMovimientoActual()) {
            case MovimientosCamara::DOLLY:
                camera.dolly(dx * 0.01f, dy * 0.01f);
                break;

            case MovimientosCamara::CRANE:
                camera.crane(dy * 0.01f);
                break;

            case MovimientosCamara::PAN:
                camera.pan(dx * 0.01f);
                break;

            case MovimientosCamara::TILT:
                camera.tilt(dy * 0.01f);
                break;

            case MovimientosCamara::ORBIT:
                camera.orbit(dx * 0.01f, dy * 0.01f);
                break;

            case MovimientosCamara::ZOOM:
                camera.zoom(dy * 0.1f);
                break;

            default:
                break;
        }
        // Posición en que la que se encuentre el ratón al final
        ultimoX = xpos;
        ultimoY = ypos;
    }
}