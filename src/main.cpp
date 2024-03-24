
#include "Image.h"
#include <iostream>
#include <string>
#include <algorithm> // For std::min and std::max

int main(int argc, char* argv[]) {
    if(argc<3){
        std::cerr << "Invalid file name." << std::endl;
    }
    if (std::string(argv[1]) == "--help") {
        std::cout << "Usage: " << argv[0] << " output_file.tga input_file.tga [operation ...]\n"
                  << "Supported operations: multiply, subtract, ... (list all supported operations here)\n"
                  << "\tEach operation may require additional arguments.\n";
        return 0;
    }

    std::string outputFilePath = argv[1];
    if (outputFilePath.size() < 4 || outputFilePath.substr(outputFilePath.size() - 4) != ".tga") {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[2];
    if (inputFilePath.size() < 4 || inputFilePath.substr(inputFilePath.size() - 4) != ".tga") {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    Image* trackingImage = readFile(inputFilePath);
    if (!trackingImage) {
        std::cerr << "File does not exist." << std::endl;
        return 1;
    }

    // Process each operation
    for (int i = 3; i < argc; ++i) {
        std::string operation = argv[i];
        Image* tempResult = nullptr;

        if (operation == "multiply") {
            std::cout << "\n\n======================================\nStarting multiply operation...\n======================================\n\n" << std::endl;
            if (i + 1 >= argc) {
                std::cerr << "Missing argument: multiply requires an image file." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            Image* otherImage = readFile(imagePath);
            if (!otherImage) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = multiply(*trackingImage, *otherImage);
            delete otherImage;
            std::cout << "\n======================================\nFinished multiply operation.\n======================================\n" << std::endl;
        }
        else if(operation == "subtract"){
            std::cout << "\n======================================\nStarting subtract operation...\n======================================\n" << std::endl;
            if (i + 1 >= argc) {
                std::cerr << "Missing argument: subtract requires a second image." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            Image* otherImage = readFile(imagePath);
            if (!otherImage) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = subtract(*trackingImage, *otherImage);
            delete otherImage;
            std::cout << "\n======================================\nFinished subtract operation.\n======================================\n" << std::endl;
        }
        else if(operation=="overlay"){
            std::cout << "\n======================================\nStarting overlay operation...\n======================================\n" << std::endl;
            if (i + 1 >= argc) {
                std::cerr << "Missing argument: overlay requires a second image file." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            Image* otherImage = readFile(imagePath);
            if (!otherImage) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = overlay(*trackingImage, *otherImage);
            delete otherImage;
            std::cout << "\n======================================\nFinished overlay operation.\n======================================\n" << std::endl;
        }
        else if(operation == "screen"){
            std::cout << "\n======================================\nStarting screen operation...\n======================================\n" << std::endl;
            if (i + 1 >= argc) {
                std::cerr << "Missing argument: screen requires a second image file." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            Image* otherImage = readFile(imagePath);
            if (!otherImage) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = screen(*trackingImage, *otherImage);
            delete otherImage;
            std::cout << "\n======================================\nFinished screen operation.\n======================================\n" << std::endl;
        }
        else if(operation == "combine"){
            /*
             * This method is similar to what you did in task 9 of Part 2, where three individual files each provide one channel of the resulting image. This method takes two additional arguments, the source for the green layer (the first additional argument), and the source for the blue layer (the second additional argument). The source for the red layer is the tracking image.
             */
            std::cout << "\n======================================\nStarting combine operation...\n======================================\n" << std::endl;
            if (i + 2 >= argc) {
                std::cerr << "Missing argument: combine requires two additional image files." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string greenLayerPath = argv[++i];
            std::string blueLayerPath = argv[++i];
            tempResult = combine(trackingImage, greenLayerPath, blueLayerPath);
            std::cout << "\n======================================\nFinished combine operation.\n======================================\n" << std::endl;
        }
        else {
            std::cerr << "Invalid operation: " << operation << std::endl;
            delete trackingImage;
            return 1;
        }
        // Handle other operations...

        if (tempResult) {
            delete trackingImage; // Free the previous tracking image
            trackingImage = tempResult; // Update the tracking image
        } else {
            std::cerr << "Invalid method name." << std::endl;
            delete trackingImage;
            return 1;
        }
    }

    // Finally, write the tracking image to the output file
    writeData(*trackingImage, outputFilePath);
    delete trackingImage; // Clean up

    return 0; // Indicate successful completion
}