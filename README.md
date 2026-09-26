# Práctica 2
En esta práctica se han implementado dos mejoras principales en la aplicación:
### 1. Adición de la clase `PAG::Renderer` para desacoplar OpenGL del archivo `main.cpp`.

Se ha añadido la clase `PAG::Renderer` siguiendo el patrón de diseño 
Singleton. Esta clase encapsula toda la lógica relacionada con OpenGL:
- Inicialización del contexto gráfico
- Configuración del color del fondo
- Gestión del viewport
- Refresco del framebuffer
- Respuesta a eventos como el redimensionado o el scroll

Además, el color del fondo ya no se almacena en el `User Pointer` de la ventana,
sino como un atributo privado de la clase `PAG::Renderer`, accesible mediante 
funciones públicas.

### 2. Adición de la biblioteca Dear ImGui a nuestra aplicación

Se ha añadido la biblioteca Dear ImGui para proporcionar una interfaz gráfica
interactiva.
Para ello se han adaptado:
- la inicialización de GLFW y OpenGL
- Los callbacks de teclado, ratón y scroll.
- El ciclo de render
- La estructura del `window_refresh_callback`

Además, se han creado dos ventanas emergentes:
- **Ventana de mensajes**: muestra los mensajes generados por la aplicación.
Incluye un área con scroll automático para visualizar siempre el último mensaje.
Los mensajes se almacenan en un vector asociado a la ventana mediante el uso 
del `User Pointer`.
- **Ventana de selector de color**: permite modificar dinámicamente el color
del fondo de la ventana principal, haciendo uso del widget `ImGui::ColorPicker4`.
El color seleccionado se aplica directamente al renderer, actualizando el color
en tiempo real.

# Práctica 1

## Ejercicio de reflexión

### Arquitectura empleada en la práctica
En esta práctica hemos desarrollado una aplicación básica, donde la gestión
de eventos se realiza mediante callbacks registrados directamente sobre la 
ventana. En mi implementación, el estado necesario para modificar el color 
de fondo se almacena en una estructura `Color`, asociada a la ventana mediante 
`glfwSetWindowUserPointer`. Los callbacks recuperan este estado con 
`glfwGetWindowUserPointer` y actúan sobre él.

El flujo de ejecución permanece dentro de main(), que concentra la 
inicialización de la ventana, la configuración del contexto OpenGL, el 
registro de callbacks y el ciclo principal de eventos. Esta arquitectura es 
funcional, pero presenta limitaciones desde el punto de vista del diseño de 
software.

### Limitaciones
Aunque la aplicación cumple con su objetivo, su estructura presenta las 
siguientes limitaciones:

- **Callbacks globales**: los callbacks están definidos como funciones libres, 
sin pertenecer a ninguna clase. Esto provoca baja modularidad y dificulta 
la encapsulación del comportamiento. La lógica de entrada queda dispersa y 
no existe una relación clara entre los eventos y los componentes de la 
aplicación.

- **Falta de separación de responsabilidades**: el main() asume todas las tareas: 
inicialización, configuración, registro de callbacks, gestión del estado y 
control del ciclo de eventos. En una aplicación más compleja, estas 
responsabilidades deberían estar distribuidas entre componentes especializados.

### Arquitectura ideal
Una aplicación bien estructurada debería contar con una clase encargada del 
renderizado como `PAG::Renderer`. Esta clase encapsularía: el estado gráfico 
(como el color del fondo), la inicialización del contexto OpenGL, la lógica 
de dibujo y las respuestas a eventos.

Así los callbacks de GLFW se limitarían a delegar las acciones a `PAG::renderer`, 
permitiendo encapsular el estado dentro de objetos, mantener una estructura 
modular y escalable, y tener un bucle principal claro y controlado.

### Conclusión
Para solucionar este problema se podría hacer uso del `User Pointer` de GLFW, 
dónde estaría almacenado un puntero al renderer y recuperarlo en los callbacks, 
delegando la lógica a métodos de la clase `PAG::Renderer`.

El objeto de la clase `PAG::Renderer` debería declararse como un objeto normal, 
no dinámico, con vida suficiente para toda la ejecución (en el caso de esta 
aplicación: en `main()`) y el módulo que inicializase dicho objeto debe ser 
aquel que cree la ventana y registre los callbacks (en este caso también en 
`main()`). Esta organización permitiría una estructura más limpia, modular y 
escalable.
