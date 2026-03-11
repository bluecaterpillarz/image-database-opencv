#ifndef IMAGE_SELECTOR_H
#define IMAGE_SELECTOR_H

#include <string>
#include <opencv2/opencv.hpp>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class ImageSelector
{
private:
    GLuint matToTexture(const cv::Mat& img);

public:
    bool showSelectionScreen(std::string& selectedImage);
};

#endif
