#include "Modelo.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PAG{

    // Constructor
    Modelo::Modelo() {}

    // Destructor
    Modelo::~Modelo() {
        if ( idVBO != 0 ){
            glDeleteBuffers ( 1, &idVBO );
        }
        if ( idIBO != 0 ){
            glDeleteBuffers ( 1, &idIBO );
        }
        if ( idVAO != 0 ){
            glDeleteVertexArrays ( 1, &idVAO );
        }
        if (idVBOColor != 0){
            glDeleteBuffers(1, &idVBOColor);
        }
    }

    // Dibuja la geometría del modelo en pantalla
    void Modelo::dibujar() {
        glBindVertexArray(idVAO); // Activa el VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO); // Activa el IBO
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr); // Pinta la geometría
    }

    /**
     * Método para crear el VAO para el modelo a renderizar
     */
    void Modelo::crearModelo() {
        /*
        // VERSIÓN NO ENTRELAZADA ================================
        // Vértices (posición)
        GLfloat posiciones[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };

        // Colores (uno por vértice)
        GLfloat colores[] = {
                1.0f, 0.0f, 0.0f,   // rojo
                0.0f, 1.0f, 0.0f,   // verde
                0.0f, 0.0f, 1.0f    // azul
        };

        GLuint indices[] = { 0, 1, 2 };

        // VAO
        glGenVertexArrays(1, &idVAO);
        if (idVAO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VAO.");
        }

        glBindVertexArray(idVAO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindVertexArray.");
        }

        // VBO de posiciones
        glGenBuffers(1, &idVBO);
        if (idVBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VBO.");
        }
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER).");
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(posiciones), posiciones, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el VBO (glBufferData).");
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(0);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }

        // VBO de colores
        glGenBuffers(1, &idVBOColor);
        if (idVBOColor == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VBO.");
        }
        glBindBuffer(GL_ARRAY_BUFFER, idVBOColor);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER).");
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(colores), colores, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el VBO (glBufferData).");
        }

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(1);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }

        // IBO
        glGenBuffers(1, &idIBO);
        if (idIBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el IBO.");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ELEMENT_ARRAY_BUFFER).");
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el IBO (glBufferData).");
        }*/

        // VERSIÓN ENTRELAZADA ================================
        /*GLfloat datos[] = {
                // posición                        // color
                -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
                0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
        };*/

        //GLuint indices[] = { 0, 1, 2 };

        glGenVertexArrays(1, &idVAO); // Crea el VAO
        if (idVAO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VAO.");
        }
        glBindVertexArray(idVAO); // Activa el VAO
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindVertexArray.");
        }

        glGenBuffers(1, &idVBO); // Crea el VBO
        if (idVBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el VBO.");
        }
        glBindBuffer(GL_ARRAY_BUFFER, idVBO); // Activa el VBO
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ARRAY_BUFFER).");
        }
        // Transfiere la información para rellenar el VBO
            // A que VBO se transfiere, tamaño de los datos, los datos, uso de los datos
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el VBO (glBufferData).");
        }

        // Cómo están organizados los datos de un atributo de vértice en el VBO activo
            // parametros: id del atributo, "cuantos hay", tipo del atributo, ¿se normaliza?, separación entre el primer
            // atributo de este tipo y el siguiente, punto donde se encuentre al primer atributo
            // posición → location = 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(0); // Activa un atributo (en este caso posiciones) de vértice almacenado en el VAO activo
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }
/*
        // color → location = 1
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glVertexAttribPointer.");
        }
        glEnableVertexAttribArray(1);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo en glEnableVertexAttribArray(0).");
        }
*/
        glGenBuffers(1, &idIBO); // Crea el IBO
        if (idIBO == 0) {
            throw std::runtime_error("[ERROR] No se pudo generar el IBO.");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO); // Activa el IBO
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al hacer glBindBuffer(GL_ELEMENT_ARRAY_BUFFER).");
        }
        // Transfiere la infomación (los índices) para rellenar el IBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("[ERROR] Fallo al cargar datos en el IBO (glBufferData).");
        }
    }

    // Getter para la matriz de modelado
    glm::mat4 &Modelo::getModelMatrix() {
        return model;
    }

    // Getters de las matrices para las transformaciones
    glm::vec3& Modelo::getTraslacion() {
        return traslacion;
    }
    glm::vec3& Modelo::getRotacion() {
        return rotacion;
    }
    glm::vec3& Modelo::getEscala() {
        return escala;
    }

    // Actualiza la matriz de modelado
    void Modelo::actualizarMatrizModelo() {

        model = glm::mat4(1.0f);

        model = glm::translate(model,traslacion);

        model = glm::rotate(model,glm::radians(rotacion.x),glm::vec3(1,0,0));
        model = glm::rotate(model,glm::radians(rotacion.y),glm::vec3(0,1,0));
        model = glm::rotate(model,glm::radians(rotacion.z),glm::vec3(0,0,1));

        model = glm::scale(model,escala);
    }

    // Carga objetos obj
    void Modelo::cargarOBJ(const std::string &ruta) {

        // Limpia los vectores
        vertices.clear();
        indices.clear();
        
        Assimp::Importer importer;
        const aiScene* escena = importer.ReadFile(ruta, aiProcess_Triangulate);

        if (!escena){
            throw std::runtime_error(importer.GetErrorString());
        }

        aiMesh* mesh = escena->mMeshes[0];

        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);
        }

        for(unsigned int i = 0; i < mesh->mNumFaces;i++) {
            aiFace cara = mesh->mFaces[i];

            for(unsigned int j = 0; j < cara.mNumIndices; j++){
                indices.push_back(cara.mIndices[j]);
            }
        }

        numIndices = indices.size();
    }
}