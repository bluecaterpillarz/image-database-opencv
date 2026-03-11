# OpenCV Image Database

A C++ image processing project that stores and reconstructs images 
pixel-by-pixel using an SQLite database.

The project integrates **OpenCV** for image processing and **ImGui** for a 
simple graphical image selection interface. Images can be saved into the 
database, reconstructed from stored pixel data, and manipulated through 
basic image processing operations.

---

## Features

- Store images pixel-by-pixel in an SQLite database
- Reconstruct images from stored pixel data
- Save circular pixel regions from images
- Change dominant colors in images
- GUI-based image selection using ImGui
- Modular C++ project structure

---

## Technologies Used

- **C++**
- **OpenCV**
- **SQLite**
- **Dear ImGui**
- **GLFW**
- **OpenGL**

---

## Project Structure
project/
│
├── src/
│ Source files (main program and implementations)
│
├── include/
│ Header files for the project classes
│
├── third_party/
│ External libraries used by the project
│ ├── imgui/
│ ├── sqlite/
│ └── glfw/
│
├── images/
│ Sample images used by the application
│
├── database/
│ SQLite database file storing image pixel data
│
└── README.md


---

## How It Works

1. The user selects an operation from the console menu.
2. Images can be selected through an ImGui graphical interface.
3. The program processes images using OpenCV.
4. Pixel data is stored or retrieved from the SQLite database.
5. The reconstructed image is displayed using OpenCV.

---

## Example Operations

The program currently supports the following operations:

1. **Save Image**  
   Stores the entire image pixel-by-pixel in the database.

2. **Read Image**  
   Reconstructs an image from pixel data stored in the database.

3. **Save Circle**  
   Extracts pixels forming a circle and saves them in the database.

4. **Change Dominant Color**  
   Detects and replaces dominant colors in the image.

---

## Build Requirements

To compile and run the project you need:

- A C++ compiler
- OpenCV installed
- OpenGL support
- GLFW
- SQLite

Make sure the required libraries are linked properly in your build system.

---

## External Libraries

- OpenCV — https://opencv.org/  
- Dear ImGui — https://github.com/ocornut/imgui  
- SQLite — https://sqlite.org/  
- GLFW — https://www.glfw.org/

---

## Notes

This project was developed as a learning exercise to explore:

- image processing with OpenCV
- storing image data in databases
- modular C++ project structure
- integrating GUI libraries in C++ applications
