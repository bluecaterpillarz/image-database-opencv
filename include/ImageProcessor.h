#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <string>
#include <opencv2/opencv.hpp>
#include <sqlite3.h>
#include "DatabaseManager.h"

class ImageProcessor
{
private:
    DatabaseManager& database;
    cv::Mat image;

    void finalizeStatement(sqlite3_stmt*& stmt);
    bool prepareInsertStatement(sqlite3_stmt*& stmt);
    bool insertPixel(sqlite3_stmt* stmt,
                     const std::string& imageName,
                     int x,
                     int y,
                     int b,
                     int g,
                     int r,
                     const std::string& type);

public:
    explicit ImageProcessor(DatabaseManager& dbManager);

    bool loadImage(const std::string& imagePath);
    bool readImageFromArchive(const std::string& imageName);

    void saveFullImage(const std::string& sourceImagePath);
    void saveCircle(const std::string& sourceImagePath, int centerX, int centerY, int radius);
    void changeDominantColor(const std::string& fileName, const std::string& dbName);

    void showCurrentImage(const std::string& windowName);
};

#endif
