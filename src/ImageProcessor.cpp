#include "include/ImageProcessor.h"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

ImageProcessor::ImageProcessor(DatabaseManager& dbManager)
    : database(dbManager) {}

void ImageProcessor::finalizeStatement(sqlite3_stmt*& stmt)
{
    if (stmt)
    {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
}

bool ImageProcessor::prepareInsertStatement(sqlite3_stmt*& stmt)
{
    const char* sql =
        "INSERT INTO dosyalar(name, x_cor, y_cor, b_color, g_color, r_color, type) "
        "VALUES(?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(database.getDb(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "INSERT statement could not be prepared: "
             << sqlite3_errmsg(database.getDb()) << endl;
        return false;
    }
    return true;
}

bool ImageProcessor::insertPixel(sqlite3_stmt* stmt,
                                 const string& imageName,
                                 int x,
                                 int y,
                                 int b,
                                 int g,
                                 int r,
                                 const string& type)
{
    sqlite3_bind_text(stmt, 1, imageName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, x);
    sqlite3_bind_int(stmt, 3, y);
    sqlite3_bind_int(stmt, 4, b);
    sqlite3_bind_int(stmt, 5, g);
    sqlite3_bind_int(stmt, 6, r);
    sqlite3_bind_text(stmt, 7, type.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cerr << "Pixel could not be inserted." << endl;
        sqlite3_clear_bindings(stmt);
        sqlite3_reset(stmt);
        return false;
    }

    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);
    return true;
}

bool ImageProcessor::loadImage(const string& imagePath)
{
    image = imread(imagePath);
    if (image.empty())
    {
        cerr << "Image could not be loaded: " << imagePath << endl;
        return false;
    }
    return true;
}

bool ImageProcessor::readImageFromArchive(const string& imageName)
{
    const char* sql =
        "SELECT x_cor, y_cor, b_color, g_color, r_color "
        "FROM arsiv WHERE name = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(database.getDb(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "SELECT statement could not be prepared: "
             << sqlite3_errmsg(database.getDb()) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, imageName.c_str(), -1, SQLITE_TRANSIENT);

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            if (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int b = sqlite3_column_int(stmt, 2);
                int g = sqlite3_column_int(stmt, 3);
                int r = sqlite3_column_int(stmt, 4);
                image.at<Vec3b>(i, j) = Vec3b(b, g, r);
            }
        }
    }

    finalizeStatement(stmt);
    return true;
}

void ImageProcessor::saveFullImage(const string& sourceImagePath)
{
    string saveName;
    cout << "Enter a name for the image: ";
    cin >> saveName;

    if (!loadImage(sourceImagePath))
        return;

    if (!database.beginTransaction())
        return;

    sqlite3_stmt* stmt = nullptr;
    if (!prepareInsertStatement(stmt))
        return;

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            Vec3b pixel = image.at<Vec3b>(i, j);

            if (!insertPixel(stmt, saveName, i, j, pixel[0], pixel[1], pixel[2], "full_image"))
            {
                finalizeStatement(stmt);
                return;
            }
        }
    }

    finalizeStatement(stmt);
    database.commitTransaction();

    imshow("Saved Image", image);
}

void ImageProcessor::saveCircle(const string& sourceImagePath, int centerX, int centerY, int radius)
{
    string saveName;
    cout << "Enter a name for the circle: ";
    cin >> saveName;

    if (!loadImage(sourceImagePath))
        return;

    if (!database.beginTransaction())
        return;

    sqlite3_stmt* stmt = nullptr;
    if (!prepareInsertStatement(stmt))
        return;

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            float dx = static_cast<float>(i - centerY);
            float dy = static_cast<float>(j - centerX);
            float distance = sqrt(dx * dx + dy * dy);

            if (abs(static_cast<float>(radius) - distance) < 0.5f)
            {
                Vec3b pixel = image.at<Vec3b>(i, j);

                if (!insertPixel(stmt, saveName, i, j, pixel[0], pixel[1], pixel[2], "circle"))
                {
                    finalizeStatement(stmt);
                    return;
                }

                image.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
            }
        }
    }

    finalizeStatement(stmt);
    database.commitTransaction();

    imshow("Circle Save Result", image);
}

void ImageProcessor::changeDominantColor(const string& fileName, const string& dbName)
{
    if (!loadImage(fileName))
        return;

    if (!readImageFromArchive(dbName))
        return;

    int sourceColor, targetColor;

    cout << "Which color do you want to change?\n"
         << "1- Blue\n"
         << "2- Green\n"
         << "3- Red\n";
    cin >> sourceColor;

    cout << "Which color do you want to change it to?\n";
    if (sourceColor != 1) cout << "1- Blue\n";
    if (sourceColor != 2) cout << "2- Green\n";
    if (sourceColor != 3) cout << "3- Red\n";
    cin >> targetColor;

    if (sourceColor == targetColor)
    {
        cout << "Same color selected. No changes were made." << endl;
        imshow("Modified Image", image);
        return;
    }

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            Vec3b& pixel = image.at<Vec3b>(i, j);

            if (sourceColor == 1 && pixel[0] > pixel[1] && pixel[0] > pixel[2])
            {
                int value = pixel[0];
                pixel[0] = 0;

                if (targetColor == 2) pixel[1] = value;
                else if (targetColor == 3) pixel[2] = value;
            }
            else if (sourceColor == 2 && pixel[1] > pixel[0] && pixel[1] > pixel[2])
            {
                int value = pixel[1];
                pixel[1] = 0;

                if (targetColor == 1) pixel[0] = value;
                else if (targetColor == 3) pixel[2] = value;
            }
            else if (sourceColor == 3 && pixel[2] > pixel[0] && pixel[2] > pixel[1])
            {
                int value = pixel[2];
                pixel[2] = 0;

                if (targetColor == 1) pixel[0] = value;
                else if (targetColor == 2) pixel[1] = value;
            }
        }
    }

    imshow("Modified Image", image);
}

void ImageProcessor::showCurrentImage(const string& windowName)
{
    if (!image.empty())
        imshow(windowName, image);
}
