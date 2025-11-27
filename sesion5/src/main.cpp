#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);

// Shaders
   Shaders shaders;

// Modelos
   Model plane;
   Model cone;

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   float desZ = 0.0;
   float rotZ = 0.0;

   //Apartado 2 SERGIO ANTON BERENGUER 
   int numberAspas = 5;
   //Apartado 3 SERGIO ANTON BERENGUER
   double lastTime = 0.0;
   //Apartado 4 SERGIO ANTON BERENGUER
   bool autoRotate = true;
   //Apartado 6 SERGIO ANTON BERENGUER
   float heightOffset = 0.0f;


int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Sesion 5", NULL, NULL);
    //Apartado 3 SERGIO ANTON BERENGUER   
    lastTime = glfwGetTime();
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback(window, funKey);

 // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void configScene() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane.initModel("resources/models/plane.obj");
    cone.initModel("resources/models/cone.obj");

}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

    //Apartado 3 SERGIO ANTON BERENGUER
      double currentTime = glfwGetTime();
      double elapsed = currentTime - lastTime;

      if (autoRotate && elapsed >= 0.01) {//Apartado 4 SERGIO ANTON BERENGUER 
         rotZ += 3.6f;        
         lastTime = currentTime;
      }

 // Matriz P
    float fovy   = 60.0;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    glm::vec3 eye   (0.0, 5.0, 10.0);
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);

 // Dibujamos la escena
    drawSuelo(P,V,I);

    //Apartado 5 y 6 SERGIO ANTON BERENGUER   
   glm::mat4 R  = glm::rotate(I, glm::radians(rotZ), glm::vec3(0, 0, 1));
   glm::mat4 R2 = glm::rotate(I, glm::radians(-90.0f), glm::vec3(1,0,0));

   float d = 2.5f;


   glm::mat4 T1 = glm::translate(I, glm::vec3(-d, heightOffset,  d));
   drawHelice(P, V, T1 * R2 * R);

   glm::mat4 T2 = glm::translate(I, glm::vec3( d, heightOffset,  d));
   drawHelice(P, V, T2 * R2 * R);

   glm::mat4 T3 = glm::translate(I, glm::vec3(-d, heightOffset, -d));
   drawHelice(P, V, T3 * R2 * R);

   glm::mat4 T4 = glm::translate(I, glm::vec3( d, heightOffset, -d));
   drawHelice(P, V, T4 * R2 * R);


}

void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
        shaders.setVec3("uColor",color);
        model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Color del alambre = 75% del color sólido
    glm::vec3 wireColor = color * 0.75f;
    shaders.setVec3("uColor",wireColor);
    model.renderModel(GL_LINE);

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {


    glm::mat4 S = glm::scale(I, glm::vec3(5.0, 5.0, 5.0));
    drawObject(plane, glm::vec3(0.0, 0.0, 1.0), P, V, M*S);

}
 
//Apartado 1 SERGIO ANTON BERENGUER
void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.25, 1, 0.05));

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0)); 

    drawObject(cone, glm::vec3(1.0, 0.0, 0.0), P, V, M * T * S);
}

//Apartado 1 y 2 SERGIO ANTON BERENGUER
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Rz = glm::rotate(I, glm::radians(360.0f / numberAspas), glm::vec3(0, 0, 1));
    glm::mat4 current = I;

    for(int i = 0; i < numberAspas; ++i) {
        drawAspa(P, V, M * current);
        current = current * Rz;  
    }
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        //Apartado 6 SERGIO ANTON BERENGUER
        case GLFW_KEY_UP:
           if (action == GLFW_PRESS || action == GLFW_REPEAT)
              heightOffset += 0.1f;
        break;

        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
              heightOffset -= 0.1f;
        break;
        case GLFW_KEY_LEFT:  rotZ += 5.0; break;
        case GLFW_KEY_RIGHT: rotZ -= 5.0; break;
        //Apartado 2 SERGIO ANTON BERENGUER
        case GLFW_KEY_1:
            if(action == GLFW_PRESS){
               numberAspas = 1;
            }
        break;
        case GLFW_KEY_2:
            if(action == GLFW_PRESS){
               numberAspas = 2;
            }
        break;
        case GLFW_KEY_3:
            if(action == GLFW_PRESS){
               numberAspas = 3;
            }
        break;
        case GLFW_KEY_4:
            if(action == GLFW_PRESS){
               numberAspas = 4;
            }
        break;
        case GLFW_KEY_5:
            if(action == GLFW_PRESS){
               numberAspas = 5;
            }
        break;
        case GLFW_KEY_6:
            if(action == GLFW_PRESS){
               numberAspas = 6;
            }
        break;
        case GLFW_KEY_7:
            if(action == GLFW_PRESS){
               numberAspas = 7;
            }
        break;
        case GLFW_KEY_8:
            if(action == GLFW_PRESS){
               numberAspas = 8;
            }
        break;
        case GLFW_KEY_9:
            if(action == GLFW_PRESS){
               numberAspas = 9;
            }
        break;
        //Apartado 4 SERGIO ANTON BERENGUER  
        case GLFW_KEY_R:
            if(action == GLFW_PRESS) {
               autoRotate = !autoRotate; 
            }
        break;
        default:
            desZ = 0.0;
            rotZ = 0.0;
    }

}
