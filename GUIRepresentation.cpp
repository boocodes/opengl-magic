#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <modules/shader/ShaderDefenition.h>
#include <modules/camera/CameraDefenition.h>
#include <modules/model/ModelDefenition.h>
#include <modules/window/Window.h>



#include <iostream>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void updatePhysics(float deltaTime);



struct SceneObject {
    glm::vec3 position;
    glm::vec3 size; // x=width, y=height, z=depth
};
std::vector<SceneObject> sceneObjects;

// Константы
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// Камера
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Тайминги
float deltaTime = 0.0f;
float lastFrame = 0.0f;


float skyBoxRotataion = 0.0f;

glm::vec3 playerPosition(0.0f, 0.0f, 0.0f);
float playerHeight = 1.8f; // Рост игрока
float playerSpeed = 5.0f;
float jumpForce = 5.0f;
float gravity = -9.8f;
float verticalVelocity = 0.0f;
bool isGrounded = false;


int main()
{

    Window window(1920, 1080, "Root");
    window.makeContextCurrent();
    
    glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetScrollCallback(window.getWindow(), scroll_callback);

    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("src/storage/shaders/model/model_loading.vs", "src/storage/shaders/model/model_loading.fs");

    Model ourModel("resources/objects/skybox/SkyBoxObject.obj");
    Model land("resources/objects/grass.obj");


   
    // Цикл рендеринга
    while (!glfwWindowShouldClose(window.getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.getWindow());
        updatePhysics(deltaTime);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();



        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        
        
        ourShader.setMat4("model", model);
        land.Draw(ourShader);
        model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
        model = glm::rotate(model, skyBoxRotataion, glm::vec3(1.0f, 0.0f, 0.0f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
        skyBoxRotataion += 0.0002f;

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Горизонтальное движение
    glm::vec3 movement(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement += camera.Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement -= camera.Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement -= camera.Right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement += camera.Right;

    // Нормализуем вектор движения (чтобы диагональное движение не было быстрее)
    if (glm::length(movement) > 0.0f)
        movement = glm::normalize(movement) * playerSpeed * deltaTime;

    // Применяем только горизонтальное движение
    playerPosition += glm::vec3(movement.x, 0.0f, movement.z);

    // Прыжок
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded)
    {
        verticalVelocity = jumpForce;
        isGrounded = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        playerSpeed = 10.0f;
    }
    else
    {
        playerSpeed = 5.0f;
    }
}
bool checkGroundCollision(const glm::vec3& position)
{
    // Здесь должна быть реальная проверка столкновений с поверхностью
    // Для простоты будем считать, что земля - это плоскость y = 0
    return position.y <= 0.0f;
}

void updatePhysics(float deltaTime)
{
    // Применяем гравитацию
    verticalVelocity += gravity * deltaTime;
    playerPosition.y += verticalVelocity * deltaTime;

    // Проверка столкновений с землей
    if (checkGroundCollision(playerPosition))
    {
        playerPosition.y = 0.0f; // или другое значение, если земля не на y=0
        verticalVelocity = 0.0f;
        isGrounded = true;
    }
    else
    {
        isGrounded = false;
    }

    // Обновляем позицию камеры
    camera.Position = playerPosition + glm::vec3(0.0f, playerHeight, 0.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
bool checkAABBCollision(const glm::vec3& playerPos, const SceneObject& obj) {
    // Игрок представлен как точка (можно расширить до bounding box)
    glm::vec3 playerMin = playerPos - glm::vec3(0.3f, 0.0f, 0.3f);
    glm::vec3 playerMax = playerPos + glm::vec3(0.3f, playerHeight, 0.3f);

    glm::vec3 objMin = obj.position - obj.size * 0.5f;
    glm::vec3 objMax = obj.position + obj.size * 0.5f;

    return (playerMin.x <= objMax.x && playerMax.x >= objMin.x) &&
        (playerMin.y <= objMax.y && playerMax.y >= objMin.y) &&
        (playerMin.z <= objMax.z && playerMax.z >= objMin.z);
}

// Функция проверки, стоит ли игрок на объекте
bool isPlayerOnObject(const glm::vec3& playerPos, float& groundHeight) {
    glm::vec3 playerFeetPos = playerPos - glm::vec3(0.0f, 0.1f, 0.0f); // Небольшое смещение вниз

    for (const auto& obj : sceneObjects) {
        if (checkAABBCollision(playerFeetPos, obj)) {
            groundHeight = obj.position.y + obj.size.y * 0.5f;
            return true;
        }
    }
    return false;
}