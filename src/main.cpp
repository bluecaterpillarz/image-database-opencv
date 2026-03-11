#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "include/DatabaseManager.h"
#include "include/ImageProcessor.h"
#include "include/ImageSelector.h"

using namespace std;
using namespace cv;

bool selectArchiveImage(string& fileName, string& dbName)
{
    short imageChoice;

    cout << "Which archive image do you want to read?\n"
         << "1- gelincik\n"
         << "2- gul_demeti\n"
         << "3- dag_manzarasi\n"
         << "4- gol_manzarasi\n";

    cin >> imageChoice;

    switch (imageChoice)
    {
        case 1: fileName = "gelincik.jif";      break;
        case 2: fileName = "gul_demeti.jpg";    break;
        case 3: fileName = "dag_manzarasi.jif"; break;
        case 4: fileName = "gol_manzarasi.jpg"; break;
        default:
            cerr << "Invalid selection!" << endl;
            return false;
    }

    size_t dotPos = fileName.find('.');
    dbName = (dotPos != string::npos) ? fileName.substr(0, dotPos) : fileName;

    return true;
}

int main()
{
    DatabaseManager dbManager;
    if (!dbManager.open("resimler.db"))
        return -1;

    ImageProcessor processor(dbManager);
    ImageSelector selector;

    short menuChoice;

    cout << "Which operation do you want to perform?\n"
         << "1- Save image\n"
         << "2- Read image\n"
         << "3- Save circle\n"
         << "4- Change dominant color\n";
    cin >> menuChoice;

    if (menuChoice == 1)
    {
        string selectedImage = "selale.jif";
        if (selector.showSelectionScreen(selectedImage))
            processor.saveFullImage(selectedImage);
    }
    else if (menuChoice == 2)
    {
        string fileName, dbName;

        if (selectArchiveImage(fileName, dbName))
        {
            if (processor.loadImage(fileName) && processor.readImageFromArchive(dbName))
                processor.showCurrentImage("Image");
        }
    }
    else if (menuChoice == 3)
    {
        string selectedImage = "selale.jif";

        if (selector.showSelectionScreen(selectedImage))
        {
            int x, y, radius;
            cout << "Enter center x, center y and radius: ";
            cin >> x >> y >> radius;

            processor.saveCircle(selectedImage, x, y, radius);
        }
    }
    else if (menuChoice == 4)
    {
        string fileName, dbName;

        if (selectArchiveImage(fileName, dbName))
            processor.changeDominantColor(fileName, dbName);
    }
    else
    {
        cout << "Invalid selection!" << endl;
    }

    waitKey(0);
    return 0;
}
