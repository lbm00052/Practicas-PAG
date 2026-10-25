#include <iostream>

// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "GUI.h"
#include "Renderer.h"

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc ) {
    std::string aux(desc);
    std::cout << "Error de GLFW numero " << errno << ": " << aux << std::endl;
}

// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window ) {

    // Refresca la escena OpenGL
    PAG::Renderer::getInstancia().refrescar();

    PAG::GUI::getInstancia().beginFrame(); // Inicialización del nuevo frame de ImGui
    PAG::GUI::getInstancia().dibuja(); // Dibuja la interfaz
    PAG::GUI::getInstancia().endFrame(); // Renderiza ImGui

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers( window );
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height ) {
    PAG::Renderer::getInstancia().onResize(width, height);
    PAG::GUI::getInstancia().addMensaje("Resize callback called");
}

// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods ) {
    PAG::GUI::getInstancia().onKey(key,action);

    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    PAG::GUI::getInstancia().addMensaje("Key callback called");
}

// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods ) {
    PAG::GUI::getInstancia().onMouseButton(button,action);

    if ( action == GLFW_PRESS ) {
        PAG::GUI::getInstancia().addMensaje("Pulsado el boton: " + std::to_string(button));
    } else if ( action == GLFW_RELEASE ) {
        PAG::GUI::getInstancia().addMensaje("Soltado el boton: " + std::to_string(button));
    }
}

// Callback para obtener la posición del cursor en pantalla
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    PAG::GUI::getInstancia().onCursorPosition(xpos, ypos);
}

// Callback para eventos de teclado (específico para números y letras)
void char_callback(GLFWwindow* window, unsigned int c){
    PAG::GUI::getInstancia().onChar(c);
}

// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
    // El color de la ventana cambiará al mover la rueda
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset ) {
    PAG::GUI::getInstancia().addMensaje("Movida la rueda del raton " + std::to_string(xoffset) +
                                        " unidades en horizontal y " + std::to_string(yoffset) +
                                        " unidades en vertical");

    // ImGui recibe el evento
    PAG::GUI::getInstancia().onScroll((float)xoffset,(float)yoffset);
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

    // Inicialización de ImGui y de GLFW y OpenGL con ImGui
    PAG::GUI::getInstancia().init(window);

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto 3D construido.
        // Aunque usa OpenGL no lo añado a Renderer porque es información del contexto
    PAG::GUI::getInstancia().addMensaje(std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    PAG::GUI::getInstancia().addMensaje(std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
    PAG::GUI::getInstancia().addMensaje(std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    PAG::GUI::getInstancia().addMensaje(std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
    glfwSetKeyCallback( window, key_callback );
    glfwSetMouseButtonCallback( window, mouse_button_callback );
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetCharCallback(window,char_callback);
    glfwSetScrollCallback( window, scroll_callback );

    // Inicialización de la intancia
    PAG::Renderer::getInstancia().init();

    // Creación de los shaders y el modelo
    try {
        // He colocado antes la creación del modelo porque, independientemente del shader que se
        // aplique (si es que se llega a aplicar alguno), se debe crear el modelo. Si la creación
        // del modelo se encontrara después de la creación de del shader program y este último
        // fallara, no se crearía el modelo nunca

        // Código de prueba para un solo modelo
        //PAG::Renderer::getInstancia().getModelos().emplace_back(); // Añade un nuevo modelo al final del vector
        //PAG::Renderer::getInstancia().getModelos().back().crearModelo(); // Crea el último modelo del vector

        // Código de prueba para varios modelos en escena
        auto& modelos = PAG::Renderer::getInstancia().getModelos();

        modelos.emplace_back();
        modelos.back().crearModelo();
        modelos.emplace_back();
        modelos.back().crearModelo();
        modelos.emplace_back();
        modelos.back().crearModelo();

        modelos[0].getTraslacion().x = -1.5f;
        modelos[0].actualizarMatrizModelo();

        modelos[1].getTraslacion().x = 0.0f;
        modelos[1].actualizarMatrizModelo();

        modelos[2].getTraslacion().x = 1.5f;
        modelos[2].actualizarMatrizModelo();

        PAG::Renderer::getInstancia().getShaderProgram().creaShaderProgram("../pag06");
    } catch (const std::exception& e) {
        PAG::GUI::getInstancia().addMensaje(e.what());
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

    PAG::GUI::destruirInstancia(); // Liberamos recursos de ImGui
    PAG::Renderer::destruirInstancia(); // Liberamos recursos de OpenGL

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    glfwDestroyWindow( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
}