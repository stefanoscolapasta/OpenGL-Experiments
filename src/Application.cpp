#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    //based on where we specify the texture coordinate compared to vertex positions, OpenGL will interpolate accordingly 
    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f, //first two are vertex pos in NDC (normal. dev. coordin.), other two are Text Coords (0,0)...(1,1)
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f,  200.0f, 1.0f, 1.0f,
        100.0f,  200.0f, 0.0f, 1.0f
    };

    //Ok now here we have the indices that make up our triangles, we want to send them to the GPU and use them there
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells how to blend alpha pixel

    VertexArray va;
    VertexBuffer vb(positions, sizeof(float) * 4 * 4);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); //Based on aspect ratio which is 4:3 currently, you give boundaries but then it remaps to an NDC space
    glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(-100, 0, 0));

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.setUniform4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);

    Texture texture("res/textures/The_Cherno_Logo.png");
    int texSlot = 0;
    texture.Bind(texSlot);
    shader.setUniform1i("u_Texture", texSlot); //where to sample from in shader

    //Reset them all before the draw calls
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    // Setup style
    ImGui::StyleColorsDark();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    glm::vec3 translation(0, 0, 0);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear(); //clear all colors and depth buffer info
        ImGui_ImplGlfwGL3_NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0), translation);
        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.setUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        {       
            ImGui::SliderFloat3("Translation", &translation[0], 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); //Swap buffers when you are done, all gl functions return immediately and CPU will be idle
        //all events called from OpenGL go to the GPU event queue and are executed as soon as possible but not immediately,
        //instead GL functions return immediately
        glfwPollEvents();
    }
    
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}