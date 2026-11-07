# Práctica 7

## Mejoras implementadas en esta práctica

En esta práctica se han implementado las siguientes mejoras:

### Soporte para propiedades de material en la clase Modelo

Para permitir que cada objeto tenga propiedades de material, se ha definido una estructura `Material` en la clase `Modelo`. Dicha estructura almacena:

- `ambiente`: vector que representa el color de la luz ambiental reflejada por el objeto.
- `difuso`: vector que representa el color base del material bajo luz directa.
- `especular`: vector que representa el color del brillo especular del objeto.
- `exponenteEspecular`: valor flotante que controla la concentración o pulido del brillo especular.

Cada objeto instanciado mediante la clase `Modelo` mantiene sus propias propiedades de `Material` como atributo privado, accesible y modificable mediante métodos getter y setter.

### Implementación de subrutinas GLSL y modos de visualización

Para cambiar entre el modo de renderizado sólido y el de alambre sin añadir condicionales en la GPU, se han implementado subrutinas GLSL dentro del fragment shader, donde se ha definido:

- Una subrutina `subroutine vec4 fCalcularColor()` que actúa como la firma del método de coloreado.
- Un uniform de subrutina `subroutine uniform fCalcularColor uMetodoColorElegido` que actúa como selector de la función activa.
- La implementación `colorAlambre()`, encargada de devolver un color rojo para el modo de alambre.
- La implementación `colorSolido()`, encargada de devolver un color basado en el atributo difuso del material `vec4(uColorDifuso, 1.0)` para el modo sólido.

Para gestionar esto en la CPU, se ha creado un `enum class ModoVisualizacion { SOLIDO, ALAMBRE }` en la clase `Modelo`. Durante la fase de refresco en `Renderer::refrescar()`, se obtiene el índice de la subrutina deseada en la GPU con `glGetSubroutineIndex` y se activa mediante `glUniformSubroutinesuiv`, adaptando también la llamada `glPolygonMode` según corresponda (`GL_FILL` o `GL_LINE`).

### Integración de controles de material y visualización en ImGui

Se ha actualizado la interfaz gráfica `GUI` ampliando la ventana  "Gestión de Modelos" con una cuarta pestaña llamada **"Material"**. Esta pestaña incorpora:

- Controles para editar dinámicamente los colores Ambiente, Difuso y Especular del modelo seleccionado.
- Un control para ajustar el exponente especular entre los valores 1.0 y 128.0.
- Un selector para alternar entre el modo de visualización **Sólido (Relleno)** y **Alambre**.

### Retrocompatibilidad con shaders anteriores

La función de `Renderer::refrescar()` ha sido modificada con comprobaciones para que no haya problemas al cargar shaders anteriores a esta práctica.

---

## Manual de Usuario

En este manual se describen todas las acciones e interacciones que el usuario puede realizar dentro de la aplicación mediante la interfaz gráfica **Dear ImGui** y el control con el ratón/teclado.

### 1. Consola de Mensajes y Registros ("Mensajes")
* **Visualización de Eventos:** En la ventana flotante **Mensajes** se muestra un historial con todas las salidas, avisos e informes del sistema en tiempo real.
* **Scroll Automático:** La lista se desplaza automáticamente hacia el final cada vez que entra una nueva línea para mantener siempre visible el evento más reciente.
* **Información de Inicio:** Al abrir la aplicación se registran las especificaciones del contexto gráfico OpenGL construido (Renderer, Vendor, Versión y Shaders).

### 2. Personalización del Fondo ("Selector de color")
* **Paleta Interactiva:** En la ventana **Selector de color**, el usuario puede cambiar el color de fondo del puerto de vista (*viewport*).
* **Modificación Mediante Rueda del Ratón:** Al mover la rueda del ratón (*scroll*) sobre el puerto de vista principal, el color de fondo se aclara u oscurece de forma gradual.

### 3. Carga Dinámica de Shaders ("Shader Program")
* **Ruta del Shader:** En la ventana **Shader Program**, el cuadro de texto permite escribir la ruta base de los archivos GLSL (por ejemplo, `../pag06`).
* **Botón Cargar:** Al pulsar en **Cargar**, la aplicación compila y enlaza dinámicamente el Vertex Shader y el Fragment Shader. Si ocurre algún error sintáctico o de archivo, el mensaje de excepción se muestra directamente en la consola de mensajes.

### 4. Control y Movimientos de la Cámara Virtual ("Cámara")

#### A. Selección del Modo de Movimiento
En el menú desplegable de la ventana **Cámara**, el usuario puede elegir uno de los 6 tipos de movimiento activos:

1. **Dolly:** Desplazamiento de la cámara en el plano local (ejes X y Z).
2. **Crane:** Desplazamiento vertical de la cámara (eje Y).
3. **Pan:** Rotación de la mirada de la cámara sobre su propio eje vertical.
4. **Tilt:** Rotación de la mirada de la cámara sobre su propio eje lateral.
5. **Orbit:** Órbita circular en coordenadas esféricas alrededor del punto objetivo.
6. **Zoom:** Modificación del campo de visión de la cámara.

#### B. Métodos de Control

* **Mediante la Interfaz ImGui:** Cada modo seleccionado despliega botones específicos en la ventana para mover la cámara.
* **Mediante Arrastre con el Ratón:** 
  - Manteniendo pulsado el **clic izquierdo del ratón** sobre el lienzo y arrastrando el cursor, la cámara se moverá continuamente según el modo seleccionado.
  - *Captura Inteligente de Ratón:* Si el cursor interactúa o arrastra una ventana de ImGui, los movimientos de la cámara quedan automáticamente bloqueados para evitar desplazamientos involuntarios.

### 5. Gestión de Modelos y Objetos 3D ("Gestión de Modelos")

La ventana **Gestión de Modelos** organiza todas las herramientas asociadas a los objetos tridimensionales de la escena dividiéndose en cuatro pestañas:

#### Pestaña 1: Selección y Control
* **Información de Escena:** Muestra el número total de modelos 3D cargados actualmente en memoria.
* **Añadir Triángulo Básico:** Añade instantáneamente un triángulo primario centrado en el origen y lo marca como modelo activo.
* **Eliminar Modelo Seleccionado:** Elimina de la escena el modelo activo actual y ajusta automáticamente los índices para evitar lecturas fuera de rango.
* **Selector de Modelo Activo (`Modelo Activo`):** Control numérico con botones +/- para cambiar el índice del modelo activo al que se le aplicarán las transformaciones, materiales o modos de dibujado.

#### Pestaña 2: Transformaciones
Manipula los atributos geométricos individuales del modelo activo seleccionado:
* **Traslación (`T`)**: Campo numérico vectorial 3D editable o arrastrable con el ratón para desplazar el objeto en los ejes X, Y y Z.
* **Rotación (`R`)**: Campo numérico vectorial 3D editable o arrastrable con el ratón para girar el objeto en grados sobre sus ejes X, Y y Z.
* **Escala (`S`)**: Campo numérico vectorial 3D editable o arrastrable con el ratón para escalar el objeto de forma independiente en X, Y y Z.

#### Pestaña 3: Cargar OBJ
Permite importar geometrías externas en formato `.obj`:
* **Carga por Explorador Nativo:** Pulsando en **Abrir Explorador**, se despliega una ventana modal navegable por el sistema de archivos del equipo que filtra únicamente los archivos con extensión `.obj`. Al seleccionar un archivo y confirmar, el modelo se importa a la GPU y se añade a la escena como modelo activo.
* **Carga por Ruta Manual:** Escribiendo la ruta del fichero en el cuadro de texto y pulsando **Cargar desde Ruta Manual**.

#### Pestaña 4: Material y Modo de Visualización
Ajusta la apariencia visual del modelo activo seleccionado:
* **Color Ambiente:** Selector de color para definir la respuesta del material a la luz ambiental indirecta.
* **Color Difuso:** Selector de color para definir el color base del material bajo luz directa (utilizado en el renderizado sólido).
* **Color Especular:** Selector de color para definir el color del brillo o destello especular.
* **Exponente Especular:** Deslizador numérico para acotar o concentrar el foco del brillo especular.
* **Modo de Visualización (Sólido vs Alambre):** Desplegable que permite alternar entre:
  - **Sólido (Relleno):** Utiliza la subrutina GLSL `colorSolido()` para pintar las caras del objeto rellenas con el color difuso del material.
  - **Alambre:** Utiliza la subrutina GLSL `colorAlambre()` y activa `glPolygonMode(GL_LINE)` para renderizar exclusivamente las aristas de la malla en un color rojo fijo.

En esta práctica, para simplificar, el único color que realiza cambios sobre el modelo es el color difuso.

### 6. Controles Generales y Teclado
* **Cierre de la Aplicación:** Pulsando la tecla **Escape** o el botón de cierre de la ventana del sistema (X), la aplicación finaliza liberando de forma segura los recursos de OpenGL, GLFW e ImGui.

### Diagrama UML

                 +---------------+
                 |    main.cpp   |
                 +---------------+
                  /   |         \
                 /    |          \
                v     v           v
           +------+ +------+     +---------------+     +------+
           | GLFW | | GUI  | --> |  Renderer     | --> |OpenGL|
           +------+ +------+     +---------------+     +------+
                       |         | Camera        |
                       v         | ShaderProgram |
                   +----------+  | Modelo (std::vector<unique_ptr>)
                   |Dear ImGUI|  +---------------+
                   +----------+       /    |    \
                                     /     |     \
                                    v      v      v
                        +------------+ +-------+ +---------------+
                        |   Camera   | |Modelo | | ShaderProgram |
                        +------------+ +-------+ +---------------+
                                           |
                                           +---> [Material (struct)]
                                           |---> [ModoVisualizacion (enum)]
                                           v
                                    +--------------+
                                    |    Assimp    |
                                    +--------------+


# Práctica 6

## Mejoras implementadas en esta práctica
En esta práctica se han implementado las siguientes mejoras:

### Desacoplamiento de la clase Modelo de Renderer

Para continuar con el desacoplamiento de la aplicación, se ha extraído toda 
la responsabilidad geométrica de la clase `Renderer` creando una nueva clase 
llamada `Modelo`. La clase `Modelo` se encarga de:

- Almacenar la geometría (posiciones de vértices) y la topología (índices de 
caras) de los objetos tridimensionales.   
- Generar y gestionar sus propios identificadores de GPU de OpenGL (idVAO, 
idVBO e idIBO).
- Dibujar su propia geometría en pantalla a través del método `dibujar()` que 
activa su VAO y ejecuta la llamada `glDrawElements`.
- Liberar sus propios búferes en el destructor aplicando el patrón RAII.

De esta forma, Renderer deja de almacenar identificadores de búferes concretos 
y pasa a coordinar un vector de modelos dinámico
(`std::vector<std::unique_ptr<Modelo>>`). El uso de punteros inteligentes únicos 
(`std::unique_ptr`) garantiza la gestión automática de memoria y previene fugas
cuando se añaden o eliminan objetos de la escena en tiempo de ejecución.   

### Adición de la matriz de modelado y transformaciones

Para permitir la coexistencia de varios objetos en la escena, 
la clase `Modelo` tiene la capacidad de almacenar y aplicar 
transformaciones geométricas individuales. Cada modelo gestiona 
internamente:

- **traslacion**: vector con el desplazamiento en los ejes X, Y y Z.   
- **rotacion**: vector con los ángulos de giro (en grados) respecto a 
cada eje.   
- **escala**: vector con el factor de escalado en cada dimensión.   
- **model**: matriz de transformación.

Mediante el método `actualizarMatrizModelo()`, se calcula la matriz de modelado 
resultante combinando traslación, rotación y escala. Durante el refresco de 
la escena en `Renderer::refrescar()`, se obtiene la matriz de modelado de cada 
objeto y se envía individualmente al shader mediante el `uniform model` justo 
antes de realizar la llamada de dibujado.

### Modificación de los shaders

Para soportar las transformaciones de modelado de cada objeto, se ha actualizado
el vertex shader incorporando el nuevo `uniform model`: la matriz `model` aplica 
la transformación de modelado individual de cada objeto, posicionándolo, 
orientándolo y escalándolo en el espacio del mundo.   

La posición final de cada vértice en pantalla se calcula en el vertex shader 
multiplicando en orden las tres matrices fundamentales. Además, se han eliminado
los atributos de color antiguos para simplificar la implementación de esta práctica.

### Carga de modelos en formato OBJ con Assimp

Se ha integrado la biblioteca `Assimp` en el proyecto a través de `CMakeLists.txt` 
para permitir la importación de modelos poliédricos en formato .obj desde disco.

La clase `Modelo` incorpora el método `cargarOBJ(const std::string& ruta)`, que 
utiliza `Assimp::Importer` para leer las mallas de triángulos del archivo, 
procesar la lista de vértices e índices y generar los búferes correspondientes 
en la GPU mediante `crearModelo()`. Todo el proceso de carga está incluido en
bloques `try-catch`, capturando cualquier fallo de lectura o archivo inexistente 
y redirigiendo el mensaje de error a la ventana de mensajes de `ImGui` sin detener 
la aplicación.   

### Adición de la interfaz de ImGui para la gestión de modelos

Se ha unificado el control de los objetos de la escena en una ventana flotante 
en `ImGui` llamada "Gestión de Modelos". Para estructurar la interfaz, se ha 
utilizado una barra de pestañas dividida en tres secciones:   

- **Pestaña Selección**: Muestra el número total de modelos cargados y permite 
instanciar un triángulo por defecto, eliminar el modelo seleccionado o cambiar 
el modelo activo mediante un control numérico acotado.   
- **Pestaña Transformaciones**: Muestra controles para modificar 
la traslación, rotación y escala del modelo seleccionado activo. Se ha 
optimizado la actualización comprobando si se ha producido algún cambio en 
los valores antes de recalcular la matriz con actualizarMatrizModelo().
- **Pestaña Cargar OBJ**: Incorpora un cuadro de texto para rutas manuales e 
integra un explorador de archivos nativo que permite navegar por los 
directorios del equipo y seleccionar archivos .obj con el ratón.

### Diagrama UML

                 +---------------+
                 |    main.cpp   |
                 +---------------+
                  /   |         \
                 /    |          \
                v     v           v
           +------+ +------+     +---------------+     +------+
           | GLFW | | GUI  | --> |  Renderer     | --> |OpenGL|
           +------+ +------+     +---------------+     +------+
                       |         | Camera        |
                       v         | ShaderProgram |
                   +----------+  | Modelo (std::vector<unique_ptr>)
                   |Dear ImGUI|  +---------------+
                   +----------+       /    |    \
                                     /     |     \
                                    v      v      v
                        +------------+ +-------+ +---------------+
                        |   Camera   | |Modelo | | ShaderProgram |
                        +------------+ +-------+ +---------------+
                                           |
                                           v
                                    +--------------+
                                    |    Assimp    |
                                    +--------------+


# Práctica 5

## Mejoras implementadas en esta práctica

En esta práctica se han implementado las siguientes mejoras:

### Soporte para la cámara virtual

Para añadir soporte para la cámara virtual se ha creado una nueva clase 
llamada `Camera`, encargada de encapsular toda la información y operaciones
relacionadas con la cámara.

La cámara almacena:

- `pos`: posición de la cámara en el espacio.
- `lookAt`: punto al que mira la cámara.
- `up`: vector vertical de la cámara.
- `fov`: campo de visión.

Además, proporciona métodos para obtener las matrices de visión y proyección
mediante la biblioteca GLM. Estas matrices son necesarias para aplicar las 
transformaciones de visión y proyección en el pipeline gráfico. Durante el 
proceso de refresco se obtienen dichas matrices desde la cámara y posteriormente
ambas se envían al shader mediante uniforms `view` y `projection`, permitiendo que 
OpenGL aplique las transformaciones de visión y proyección antes del proceso 
de rasterización y pueda transformar correctamente la geometría 
según la posición y orientación actuales de la cámara.

#### Modificación de los shaders

Para poder usar la cámara ha sido necesario modificar el vertex shader 
incorporando dos nuevas matrices `uniform`:

- La matriz `view` representa la transformación de visión generada por la cámara 
a partir de su posición, del punto al que mira y de su vector vertical.
- La matriz `projection` representa la transformación de proyección en perspectiva
generada a partir del campo de visión de la cámara.

Para que cualquier modificación realizada sobre la cámara afecte directamente a
la visualización de la escena, la posción de cada vértice se transforma mediante
ambas matrices.

El fragment shader no ha necesitado muchos cambios, solo se ha mantenido la 
interpolación y la visualización del color asociado a cada vértice.

### Integración de la cámara con Renderer

La cámara se ha integrado dentro de la clase `Renderer`, que mantiene una 
instancia propia llamada `camera`. De esta forma, el Renderer actúa como 
coordinador principal de la aplicación. 

Para poder acceder a la cámara, se ha implementado un getter. Además, `Renderer`
también almacena el movimiento actual de la cámara en `movimientoActual` lo que
permite coordinar la interacción entre la interfaz gráfica y la cámara.

### Implementación de los movimientos de la cámara

Para dar movimiento a la cámara, se ha definido un `enum` para identificar los
distintos movimientos disponibles, simplificando la selección y ejecución del 
movimiento activo. La lógica asociada a cada uno de estos movimientos se 
encuentra implementada en la clase `Camera`.

Dichos movimientos deben ser seleccionados en una ventana ImGui. Existen dos 
formas de realizar los movimientos:

- **A través de la interfaz ImGui**: presionando los botones que aparecen en la
ventana donde se selecciona el movimiento.
- **Mediante movimientos del ratón**: desplazando el ratón de manera vertical u
horizontal.

#### Control mediante ratón

Para poder mover la cámara a través del ratón se debe mantener pulsado el clic
izquierdo mientras se arrastra. Para que esto funcionara, ha sido necesario 
registrar un nuevo callback `cursor_position_callback` que comunica la posición
del cursor a la clase ImGui.

Sin embargo, cabe recalcar que, debido a esta implementación, se han desactivado
los callbacks automáticos de ImGui, por lo que, para que la práctica anterior 
(Práctica 4) pudiera seguir funcionando con normalidad, se ha necesitado 
implementar un segundo callback referido a los eventos de teclado `char_callback`.
Este tiene como misión obtener los eventos de teclado referidos únicamente 
a caracteres, todo ello para evitar el mapeo manual de las teclas.

Además, para evitar conflictos entre el movimiento de la cámara y la interacción
con la interfaz ImGui se ha hecho uso de la función 
`ImGUI::GetIO().WantCaptureMouse`. Esta permite que, cuando ImGui está usando
el ratón, los movimientos de la cámara quedan temporalmente deshabilitados, 
permitiendo mover ventanas, pulsar botones o usar controles sin modificar
accidentalmente la cámara.

A continuación se especifican los movimiento de ratón por cada movimiento de 
cámara:

- *Dolly*: arrastre vertical para movimiento en eje Z y arrastre horizontal para
movimiento en eje X.
- *Crane*: arrastre vertical para movimiento en eje Y.
- *Pan*: arrastre horizontal para rotación respecto eje vertical de la cámara.
- *Tilt*: arrastre vertical para rotación respecto eje lateral de la cámara.
- *Orbit*: arrastre vertical para órbita vertical respecto al punto que mira
la cámara y arrastre horizontal para órbita horizontal respecto al punto que
mira la cámara. 
- *Zoom*: arrastre vertical para acortar o aumentar el ángulo de visión.

### Diagrama UML

                 +---------------+
                 |    main.cpp   |
                 +---------------+
                  /   |         \
                 /    |          \
                v     v           v

           +------+ +------+     +---------------+     +------+
           | GLFW | | GUI  |     |  Renderer     | --> |OpenGL|
           +------+ +------+     +---------------+     +------+
                       |         | Camera        |
                       v         | ShaderProgram |
                   +----------+  +---------------+
                   |Dear ImGUI|        /         \
                   +----------+       v           v
                             +------------+  +---------------+
                             |  Camera    |  | ShaderProgram |
                             +------------+  +---------------+

# Práctica 4

### ¿Cómo se ha realizado el desacoplamiento?

Para realizar el desacoplamiento se ha creado la clase `ShaderProgram`, que 
se encarga de la gestión de todo lo relacionado con shaders.

Dicha clase está compuesta por los identificadores de los shaders, dispone de
un constructor por defecto, un destructor, un getter al identificador del 
shader program y un método para crear, compilar y enlazar el shader program.

Así, la funcionalidad relacionada con la gestión de los shaders, que 
inicialmente se encontraba en `Renderer`, se ha trasladado a la nueva clase.

Para permitir la interacción entre ambas clases, se ha añadido el atributo 
`shaderProgram` en la clase `Renderer`, además de un getter que permite usar
los servicios de la nueva clase desde el renderer.

Como resultado, `Renderer` solo mantiene responsabilidades relacionadas con
el renderizado y la gestión de la geometría y `ShaderProgram` se encarga de 
la creación y gestión de los shaders.

## Mejoras implementadas en esta práctica

En esta práctica se han añadido las siguiente mejoras:

### 1. Desacoplamiento de la clase ImGui y pequeña corrección en Renderer

Para desacoplar la gestión de ImGui se ha creado la clase `GUI`, que se
encargará de encapsular toda la funcionalidad relacionada con la interfaz
gráfica, siguiendo el patrón de diseño singleton.

Esta clase gestiona la inicialización y liberación de recursos de ImGui, la 
creación de nuevos frames, el renderizado de la interfaz y el procesamiento
de eventos de entrada.

Ahora, el código relacionado con ImGui que se encontraba disperso a lo largo
de `main.cpp` ha quedado centralizado en esta clase, mejorando la organización.

No obstante, la clase mantiene dependencias con `GLFW` y `Renderer`, ya que
necesita recibir los eventos generados por la ventana y acceder a ciertas 
funcionalidades del renderizado para modificar parámetros de la aplicación
desde la interfaz gráfica. 

Respecto a la correción en Renderer, no estaba segura si se realizaba 
correctamente la liberación de recursos, por lo que se ha agregado una nueva
función que activa el destructor al final de `main.cpp`.

### 2. Desacoplamiento de la gestión de shader programs de la clase Renderer

Dicho desacoplamiento se ha explicado anteriormente en 
"¿Cómo se ha realizado el desacoplamiento?"

### 3. Adición de una ventana para indicar el nombre base de los shaders

Se ha añadido una nueva ventana de ImGui llamada "Shader Program", cuyo objetivo 
es permitir la carga dinámica de shader programs durante la ejecución. 

La ventana incorpora un campo de texto en el que se puede introducir la ruta 
base de los shaders y un botón "Cargar" encargado de iniciar el proceso de carga. 
Al pulsar dicho botón, se invoca el método `crearShaderProgram` de la clase
`ShaderProgram`.

# Práctica 3
### Pregunta: 
**Si redimensionas la ventana de la aplicación, verás que el triángulo
no permanece igual, sino que se deforma al mismo tiempo que la ventana. 
¿A qué crees que se debe este comportamiento?**

Este comportamiento se debe a que estamos dibujando directamente en coordenadas
normalizadas sin aplicar ninguna matriz de proyección. El viewport escala los 
valores X e Y según el tamaño de la ventana, y como la relación de aspecto cambia,
el triángulo se deforma.

## Mejoras implementadas en esta práctica

En esta práctica se han añadido las siguientes mejoras:

### 1. Implementación de los shaders en la clase `PAG::Renderer`

Se han añadido los shader siguiendo las indicaciones del documento, incorporando
los identificadores necesarios a la clase como atributos privados. 

Además se han añadido los métodos:
- `crearShaderProgram()`: crea los shaders, los compila y los enlaza al shader program.
- `creaModelo()`: crea la geometría usando arrays locales para almacenar vértices.

Además, en cada uno de estos métodos, se realiza comprobación de errores y 
lanzamiento de excepciones capturadas en el main para mostrarlas en la terminal
de la ventana.

### 2. Carga del código GLSL desde archivos externos

Se ha movido el código de los shaders a dos archivos: 
`pag03-vs.glsl` y `pag03-fs.glsl`; por lo que ahora el método `creaShaderProgram()`
recibe un parametro con el inicio de la ruta donde se almacenan dichos archivos y 
los carga automáticamente.

### 3. Adición de un segundo atributo por vértice

Se ha implementado el apartado opcional añadiendo color a cada vértice de dos 
formas:

- **VBOs no entrelazados**: un VBO para posiciones y uno independiente para los 
colores.
- **VBO entrelazado**: un único VBO con posiciones y colores intercalados.

Ambas versiones se encuentran en el código. El resultado de aplicar una u otra es
un triángulo con un gradiente de color.


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
