#include <iostream>

// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc ) {
    std::string aux(desc);
    std::cout << "Error de GLFW numero " << errno << ": " << aux << std::endl;
}

// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window ) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers( window );

    std::cout << "Refresh callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height ) {
    glViewport(0, 0, width, height);
    std::cout << "Resize callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods ) {
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    std::cout << "Key callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods ) {
    if ( action == GLFW_PRESS ) {
        std::cout << "Pulsado el boton: " << button << std::endl;
    } else if ( action == GLFW_RELEASE ) {
        std::cout << "Soltado el boton: " << button << std::endl;
    }
}

// - Struct para el color de una ventana
struct Color {
    float r, g, b, a;
};

// - Función para cambiar el color del fondo de una ventana según
// el movimiento de la rueda del ratón
    // y : dirección vertical en que se movió el ratón
void cambio_color_fondo ( GLFWwindow *window, double y ){

    // Obtención del puntero de usuario (en este caso es el puntero al color)
    Color* c = static_cast<Color*>( glfwGetWindowUserPointer(window) );

    float desp = y * 0.1, cota_inf = 0.0, cota_sup = 1.0; // variables

    // Cambio del color del fondo de pantalla
        // std::clamp(valor, cota inferior, cota superior) -> sirve para acotar nºs (librería: algorith)
    c->r = std::clamp((c->r += desp), cota_inf, cota_sup);
    c->g = std::clamp((c->g += desp), cota_inf, cota_sup);
    c->b = std::clamp((c->b += desp), cota_inf, cota_sup);

    glClearColor(c->r, c->g, c->b, c->a); // Colorear
    window_refresh_callback( window ); // Refrescar
    // No refresco dentro del bucle porque solo me interesa el cambio que produce esta función
}

// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
    // El color de la ventana cambiará al mover la rueda
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset ) {
    std::cout << "Movida la rueda del raton " << xoffset
              << " Unidades en horizontal y " << yoffset
              << " unidades en vertical" << std::endl;

    cambio_color_fondo( window,yoffset );
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

    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto 3D construido.
    std::cout << glGetString(GL_RENDERER) << std::endl
              << glGetString(GL_VENDOR) << std::endl
              << glGetString(GL_VERSION) << std::endl
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
    glfwSetKeyCallback( window, key_callback );
    glfwSetMouseButtonCallback( window, mouse_button_callback );
    glfwSetScrollCallback( window, scroll_callback );

    // - Establecemos un gris medio como color con el que se borrará el frame buffer.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    Color fondo = {0.6,0.6,0.6,1};

        // Para poder acceder al color de la ventana en cualquier lugar donde se tenga
        // acceso a la ventana asignamos un puntero de usuario (puntero asociado a una
        // ventana que puede tener cualquier propósito). En este caso, como solo nos
        // interesa conocer el color del fondo, dicho puntero apuntará al color que hemos
        // asignado al fondo.
    glfwSetWindowUserPointer( window, &fondo);

    glClearColor(fondo.r, fondo.g, fondo.b, fondo.a);

    // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glEnable(GL_DEPTH_TEST);

    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose( window ) ) { // Mientras que NO se cierre la ventana

        // - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
        // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
        // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    glfwDestroyWindow( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
}