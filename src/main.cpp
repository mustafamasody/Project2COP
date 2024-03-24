
#include "Image.h"
#include <iostream>
#include <string>
#include <algorithm> // For std::min and std::max

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Project 2: Image Processing, Spring 2024\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]" << std::endl;
        return 0;
    }
    if (std::string(argv[1]) == "--help") {
        std::cout << "Project 2: Image Processing, Spring 2024\n\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]" << std::endl;
        return 0;
    }
    if(argc<3){
        std::cerr << "Invalid file name." << std::endl;
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

    for (int i = 3; i < argc; ++i) {
        std::string operation = argv[i];
        Image* tempResult = nullptr;

        if (operation == "multiply") {
            std::cout << "\n\n======================================\nStarting multiply operation...\n======================================\n\n" << std::endl;
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            if (imagePath.size() < 4 || imagePath.substr(imagePath.size() - 4) != ".tga") {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                delete trackingImage;
                return 1;
            }
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
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            if (imagePath.size() < 4 || imagePath.substr(imagePath.size() - 4) != ".tga") {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                delete trackingImage;
                return 1;
            }
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
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            if (imagePath.size() < 4 || imagePath.substr(imagePath.size() - 4) != ".tga") {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                delete trackingImage;
                return 1;
            }
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
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string imagePath = argv[++i];
            if (imagePath.size() < 4 || imagePath.substr(imagePath.size() - 4) != ".tga") {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                delete trackingImage;
                return 1;
            }
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
            std::cout << "\n======================================\nStarting combine operation...\n======================================\n" << std::endl;
            if (i + 2 >= argc) {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                delete trackingImage;
                return 1;
            }
            std::string greenLayerPath = argv[++i];
            std::string blueLayerPath = argv[++i];
            tempResult = combine(trackingImage, greenLayerPath, blueLayerPath);
            std::cout << "\n======================================\nFinished combine operation.\n======================================\n" << std::endl;
        }
        else if(operation == "flip"){
            std::cout << "\n======================================\nStarting flip operation...\n======================================\n" << std::endl;
            tempResult = flipImage(trackingImage);
            std::cout << "\n======================================\nFinished flip operation.\n======================================\n" << std::endl;
        }
        else if (operation == "onlyred") {
            tempResult = onlyRed(trackingImage);
        }
        else if (operation == "onlygreen") {
            tempResult = onlyGreen(trackingImage);
        }
        else if (operation == "onlyblue") {
            tempResult = onlyBlue(trackingImage);
        }
        else if (operation == "addred") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            int value;
            try {
                value = std::stoi(argv[i + 1]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = addRed(trackingImage, value);
            ++i;
        }
        else if (operation == "addgreen") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            int value;
            try {
                value = std::stoi(argv[i + 1]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = addGreen(trackingImage, value);
            ++i;
        }
        else if (operation == "addblue") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }
            int value;
            try {
                value = std::stoi(argv[i + 1]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = addBlue(trackingImage, value);
            ++i;
        }
        else if (operation == "scalered") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }

            int scale;
            try {
                scale = std::stoi(argv[++i]);
            } catch (const std::exception& e) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = scaleRed(trackingImage, scale);
        }
        else if (operation == "scalegreen") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }

            int scale;
            try {
                scale = std::stoi(argv[++i]);
            } catch (const std::exception& e) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = scaleGreen(trackingImage, scale);
        }
        else if (operation == "scaleblue") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                delete trackingImage;
                return 1;
            }

            int scale;
            try {
                scale = std::stoi(argv[++i]);
            } catch (const std::exception& e) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                delete trackingImage;
                return 1;
            }
            tempResult = scaleBlue(trackingImage, scale);
        }
        else {
            std::cerr << "Invalid method name." << std::endl;
            delete trackingImage;
            return 1;
        }

        if (tempResult) {
            delete trackingImage;
            trackingImage = tempResult;
        } else {
            std::cerr << "Invalid method name." << std::endl;
            delete trackingImage;
            return 1;
        }
    }

    writeData(*trackingImage, outputFilePath);
    delete trackingImage; // Clean up

    return 0;
}