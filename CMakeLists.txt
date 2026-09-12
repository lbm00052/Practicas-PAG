cmake_minimum_required(VERSION 3.29)
project(practica1)

set(CMAKE_CXX_STANDARD 20)

# Define variables agrupando nombres de archivo
file(GLOB MY_FILES *.cpp) # Código fuente de tu proyecto
file(GLOB GLAD_FILES GLAD/src/*.c) # Código fuente de GLAD

    # Lo que tenga la estructura ${VARIABLE} es una variable

# Indica qué archivos hay que compilar para generar el ejecutable
add_executable( ${PROJECT_NAME} ${MY_FILES}
                                ${GLAD_FILES} )

# Indica directorios (adicionales a los por defecto) para buscar includes
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include )

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)
