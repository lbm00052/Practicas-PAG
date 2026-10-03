#include <iostream>

// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

#include "Renderer.h"

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc ) {
    std::string aux(desc);
    std::cout << "Error de GLFW numero " << errno << ": " << aux << std::endl;
}

// Función que gestiona la adición de mensajes en el vector de mensajes
// creado en el userpointer
void logMensajes ( GLFWwindow *window, const std::string& msg ) {

    // Obtención del puntero de usuario
    std::vector<std::string>* mensajes = static_cast<std::vector<std::string>*>(glfwGetWindowUserPointer(window));
    // Adición del nuevo mensaje al vector
    mensajes->push_back( msg );
    std::cout << msg << std::endl; // Muestra el mensaje
}

// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window ) {

    // Refresca la escena OpenGL
    PAG::Renderer::getInstancia().refrescar();

    // Inicialización del nuevo frame de ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Ventana de mensajes
        // Obtención del puntero de usuario
    std::vector<std::string>* mensajes = static_cast<std::vector<std::string>*>(glfwGetWindowUserPointer(window));
        // Posición de la ventana (solo la primera vez)
    ImGui::SetNextWindowPos(ImVec2 (10, 10), ImGuiCond_Once);
        // Abre la ventana
    if ( ImGui::Begin("Mensajes") ) {
        ImGui::SetWindowFontScale(1.0f); // Ajuste de la fuente

        // Zona con scroll automático
        ImGui::BeginChild("scroll_region",ImVec2(0,0),true);

        for ( const std::string& s : *mensajes ) {
            ImGui::Text("%s",s.c_str()); // Dibuja cada mensaje
        }

        // Desplaza el scroll al final de cada frame (para que se muestre el último mensaje)
        ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild(); // Fin de la ventana de scroll
    }
    ImGui::End(); // Fin de la ventana de mensajes

    // Ventana se selección de color
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

    // Renderiza ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers( window );

    //std::cout << "Refresh callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height ) {
    PAG::Renderer::getInstancia().onResize(width, height);
    logMensajes(window,"Resize callback called");
}

// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods ) {

    // GLFW trata las teclas de forma diferente a ImGui. Mientras que en GLFW un int
    // se corresponde con una tecla, en el caso de ImGui estas se encuentran dentro de
    // un enum, por lo que hay que mapearlas manualmente.

    ImGuiIO& io = ImGui::GetIO();

    // Mapear teclas especiales
    if ( key == GLFW_KEY_LEFT ) {
        io.AddKeyEvent(ImGuiKey_LeftArrow, action == GLFW_PRESS);
    }

    if ( key == GLFW_KEY_RIGHT ) {
        io.AddKeyEvent(ImGuiKey_RightArrow, action == GLFW_PRESS);
    }

    if ( key == GLFW_KEY_UP ) {
        io.AddKeyEvent(ImGuiKey_UpArrow, action == GLFW_PRESS);
    }

    if ( key == GLFW_KEY_DOWN ) {
        io.AddKeyEvent(ImGuiKey_DownArrow, action == GLFW_PRESS);
    }

    if ( key == GLFW_KEY_ESCAPE ) {
        io.AddKeyEvent(ImGuiKey_Escape, action == GLFW_PRESS);
        if ( action == GLFW_PRESS ) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    logMensajes(window,"Key callback called");
}

// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods ) {

    if ( action == GLFW_PRESS ) {
        logMensajes(window,"Pulsado el boton: " + std::to_string(button));

        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(button, true );

    } else if ( action == GLFW_RELEASE ) {
        logMensajes(window,"Soltado el boton: " + std::to_string(button));

        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(button, false );
    }
}

// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
    // El color de la ventana cambiará al mover la rueda
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset ) {

    logMensajes(window,"Movida la rueda del raton " + std::to_string(xoffset) +
                            " unidades en horizontal y " + std::to_string(yoffset) +
                            " unidades en vertical");

    // ImGui necesita recibir el evento
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent((float)xoffset, (float)yoffset);

    PAG::Renderer::getInstancia().onScroll(yoffset);
}

int main() {

    std::cout << "Starting Application PAG - Prueba 01" << std::endl;

    // - Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback((GLFWerrorfun) error_callback);

    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    // En caso de fallar finaliza el programa
    if ( glfwInit() != GLFW_TRUE ) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialiasing con 4 muestras.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL Core Profile 4.3.

    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y la creamos
    GLFWwindow *window;

    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow(1024, 576, "PAG Introduction", nullptr, nullptr);

    // - Comprobamos si la creación de la ventana ha tenido éxito.
    // Si no ha tenido éxito se liberan recursos y se acaba el programa.
    if ( window == nullptr ) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent( window );

    // - Ahora inicializamos GLAD. En caso de falla se liberan recursos y se acaba el programa
    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
        std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate();
        return -3;
    }

    // Puntero de usuario de la ventana apunta a un vector de string que almacena los mensajes
    // que se mostraran en la terminal
    std::vector<std::string> mensajes;
    glfwSetWindowUserPointer(window,&mensajes);

    // Inicialización de ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Inicialización de GLFW y OpenGL con ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto 3D construido.
        // Aunque usa OpenGL no lo añado a Renderer porque es información del contexto
    logMensajes(window,std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    logMensajes(window,std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
    logMensajes(window,std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    logMensajes(window,std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
    glfwSetKeyCallback( window, key_callback );
    glfwSetMouseButtonCallback( window, mouse_button_callback );
    glfwSetScrollCallback( window, scroll_callback );

    // Inicialización de la intancia
    PAG::Renderer::getInstancia().init();

    // Creación de los shaders y el modelo
    try {
        PAG::Renderer::getInstancia().creaShaderProgram("../pag03");
        PAG::Renderer::getInstancia().creaModelo();
    } catch (const std::exception& e) {
        logMensajes(window,e.what());
    }

    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose( window ) ) { // Mientras que NO se cierre la ventana

        // - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
        // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
        // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents();

        // Refresco de la ventana
        window_refresh_callback(window);
    }

    // Liberamos recursos de ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    glfwDestroyWindow( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
}