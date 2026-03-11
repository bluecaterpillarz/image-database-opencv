#include "ImageSelector.h"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

GLuint ImageSelector::matToTexture(const Mat& img)
{
    Mat rgb;
    cvtColor(img, rgb, COLOR_BGR2RGB);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 rgb.cols,
                 rgb.rows,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 rgb.data);

    return textureID;
}

bool ImageSelector::showSelectionScreen(string& selectedImage)
{
    if (!glfwInit())
    {
        cerr << "GLFW could not be initialized." << endl;
        return false;
    }

    GLFWwindow* window = glfwCreateWindow(600, 400, "Select Image", nullptr, nullptr);
    if (!window)
    {
        cerr << "Window could not be created." << endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    vector<string> options =
    {
        "selale.jif",
        "gelincik.jif",
        "gul_demeti.jpg",
        "dag_manzarasi.jif",
        "gol_manzarasi.jpg"
    };

    vector<GLuint> textures;
    vector<ImVec2> sizes;

    for (const auto& file : options)
    {
        Mat img = imread(file);

        if (!img.empty())
        {
            Mat thumb;
            resize(img, thumb, Size(120, 80));
            textures.push_back(matToTexture(thumb));
            sizes.push_back(ImVec2(120, 80));
        }
        else
        {
            textures.push_back(0);
            sizes.push_back(ImVec2(120, 80));
        }
    }

    bool chosen = false;

    while (!glfwWindowShouldClose(window) && !chosen)
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowSize(ImVec2(580, 380));
        ImGui::Begin("Which image do you want to use?");

        for (int i = 0; i < static_cast<int>(options.size()); i++)
        {
            ImGui::BeginGroup();

            if (textures[i] != 0)
                ImGui::Image((ImTextureID)(intptr_t)textures[i], sizes[i]);
            else
                ImGui::Text("Image could not be loaded");

            if (ImGui::Button(options[i].c_str()))
            {
                selectedImage = options[i];
                chosen = true;
            }

            ImGui::EndGroup();

            if ((i + 1) % 3 != 0)
                ImGui::SameLine(0, 20);
        }

        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    for (auto texture : textures)
    {
        if (texture != 0)
            glDeleteTextures(1, &texture);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return chosen;
}
