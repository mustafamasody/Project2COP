//
// Created by Mustafa M on 3/18/2024.
//

#include "Image.h"
#include <iostream>
#include <fstream>
#include <string>



using namespace std;

Image* deepCopy(const Image* src) {
    Image* copy = new Image;
    copy->header = src->header; // Copy header
    copy->pixels = new Pixel*[copy->header.height];
    for (int i = 0; i < copy->header.height; i++) {
        copy->pixels[i] = new Pixel[copy->header.width];
        for (int j = 0; j < copy->header.width; j++) {
            copy->pixels[i][j] = src->pixels[i][j]; // Copy pixel data
        }
    }
    return copy;
}

Image* readFile(const std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: File not found" << std::endl;
        return nullptr;
    }
    Image *img = new Image;
    readData(file, img);
    file.close();
    return img;
}

void readData(std::ifstream &file, Image* img) {
    // Directly modify *img...
    file.read(&img->header.idLength, sizeof(img->header.idLength));
    file.read(&img->header.colorMapType, sizeof(img->header.colorMapType));
    file.read(&img->header.dataTypeCode, sizeof(img->header.dataTypeCode));
    file.read(reinterpret_cast<char*>(&img->header.colorMapOrigin), sizeof(img->header.colorMapOrigin));
    file.read(reinterpret_cast<char*>(&img->header.colorMapLength), sizeof(img->header.colorMapLength));
    file.read(&img->header.colorMapDepth, sizeof(img->header.colorMapDepth));
    file.read(reinterpret_cast<char*>(&img->header.xOrigin), sizeof(img->header.xOrigin));
    file.read(reinterpret_cast<char*>(&img->header.yOrigin), sizeof(img->header.yOrigin));
    file.read(reinterpret_cast<char*>(&img->header.width), sizeof(img->header.width));
    file.read(reinterpret_cast<char*>(&img->header.height), sizeof(img->header.height));
    file.read(&img->header.bitsPerPixel, sizeof(img->header.bitsPerPixel));
    file.read(&img->header.imageDescriptor, sizeof(img->header.imageDescriptor));

    img->pixels = new Pixel*[img->header.height];
    for (int i = 0; i < img->header.height; i++) {
        img->pixels[i] = new Pixel[img->header.width];
        for (int j = 0; j < img->header.width; j++) {
            file.read(reinterpret_cast<char*>(&img->pixels[i][j].blue), sizeof(unsigned char));
            file.read(reinterpret_cast<char*>(&img->pixels[i][j].green), sizeof(unsigned char));
            file.read(reinterpret_cast<char*>(&img->pixels[i][j].red), sizeof(unsigned char));
        }
    }
}


void writeData(const Image& image, const std::string& filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing - " << filename << std::endl;
        return;
    }

    file.write(&image.header.idLength, sizeof(image.header.idLength));
    file.write(&image.header.colorMapType, sizeof(image.header.colorMapType));
    file.write(&image.header.dataTypeCode, sizeof(image.header.dataTypeCode));
    file.write(reinterpret_cast<const char*>(&image.header.colorMapOrigin), sizeof(image.header.colorMapOrigin));
    file.write(reinterpret_cast<const char*>(&image.header.colorMapLength), sizeof(image.header.colorMapLength));
    file.write(&image.header.colorMapDepth, sizeof(image.header.colorMapDepth));
    file.write(reinterpret_cast<const char*>(&image.header.xOrigin), sizeof(image.header.xOrigin));
    file.write(reinterpret_cast<const char*>(&image.header.yOrigin), sizeof(image.header.yOrigin));
    file.write(reinterpret_cast<const char*>(&image.header.width), sizeof(image.header.width));
    file.write(reinterpret_cast<const char*>(&image.header.height), sizeof(image.header.height));
    file.write(&image.header.bitsPerPixel, sizeof(image.header.bitsPerPixel));
    file.write(&image.header.imageDescriptor, sizeof(image.header.imageDescriptor));

    for (int i = 0; i < image.header.height; i++) {
        for (int j = 0; j < image.header.width; j++) {
            file.write(reinterpret_cast<char*>(&image.pixels[i][j].blue), sizeof(image.pixels[i][j].blue));
            file.write(reinterpret_cast<char*>(&image.pixels[i][j].green), sizeof(image.pixels[i][j].green));
            file.write(reinterpret_cast<char*>(&image.pixels[i][j].red), sizeof(image.pixels[i][j].red));
        }
    }

    file.close();
}

Image* multiply(Image& image1, Image& image2) {
    auto* result = new Image;
    result->header = image1.header;
    result->pixels = new Pixel*[result->header.height];

    for (int i = 0; i < result->header.height; i++) {
        result->pixels[i] = new Pixel[result->header.width];
        for (int j = 0; j < result->header.width; j++) {
            // Normalizing color values to [0, 1], performing multiplication, and rescaling back to [0, 255]
            float blue1 = image1.pixels[i][j].blue / 255.0f;
            float green1 = image1.pixels[i][j].green / 255.0f;
            float red1 = image1.pixels[i][j].red / 255.0f;

            float blue2 = image2.pixels[i][j].blue / 255.0f;
            float green2 = image2.pixels[i][j].green / 255.0f;
            float red2 = image2.pixels[i][j].red / 255.0f;

            result->pixels[i][j].blue = static_cast<unsigned char>((blue1 * blue2) * 255 + 0.5);
            result->pixels[i][j].green = static_cast<unsigned char>((green1 * green2) * 255 + 0.5);
            result->pixels[i][j].red = static_cast<unsigned char>((red1 * red2) * 255 + 0.5);
        }
    }
    return result;
}

Image* subtract( Image& image1,  Image& image2) {
    auto* result = new Image();
    result->header = image1.header; // Assuming both images have the same dimensions and format

    result->pixels = new Pixel*[result->header.height];
    for (int i = 0; i < result->header.height; i++) {
        result->pixels[i] = new Pixel[result->header.width];
    }

    for (int i = 0; i < result->header.height; i++) {
        for (int j = 0; j < result->header.width; j++) {
            // Ensure subtraction results are clamped to 0 if negative or minimal positive values
            result->pixels[i][j].blue = std::max(0, image2.pixels[i][j].blue - image1.pixels[i][j].blue);
            result->pixels[i][j].green = std::max(0, image2.pixels[i][j].green - image1.pixels[i][j].green);
            result->pixels[i][j].red = std::max(0, image2.pixels[i][j].red - image1.pixels[i][j].red);
        }
    }

    return result;
}

Image* screen(Image& bottomLayer, Image& topLayer) {
    // Ensure the dimensions of both images match
    auto* result = new Image();
    result->header = bottomLayer.header; // Assuming both images have the same header
    result->pixels = new Pixel*[result->header.height];

    for (int i = 0; i < result->header.height; ++i) {
        result->pixels[i] = new Pixel[result->header.width];
        for (int j = 0; j < result->header.width; ++j) {
            // Normalize the pixel values to [0, 1] for calculation
            float bottomBlue = bottomLayer.pixels[i][j].blue / 255.0f;
            float bottomGreen = bottomLayer.pixels[i][j].green / 255.0f;
            float bottomRed = bottomLayer.pixels[i][j].red / 255.0f;

            float topBlue = topLayer.pixels[i][j].blue / 255.0f;
            float topGreen = topLayer.pixels[i][j].green / 255.0f;
            float topRed = topLayer.pixels[i][j].red / 255.0f;

            // Apply the screen blending mode formula for each color channel
            result->pixels[i][j].blue = (1 - (1 - topBlue) * (1 - bottomBlue)) * 255;
            result->pixels[i][j].green = (1 - (1 - topGreen) * (1 - bottomGreen)) * 255;
            result->pixels[i][j].red = (1 - (1 - topRed) * (1 - bottomRed)) * 255;

            // Clamp the results to the range [0, 255]
            result->pixels[i][j].blue = std::min(std::max(int(result->pixels[i][j].blue + 0.5), 0), 255);
            result->pixels[i][j].green = std::min(std::max(int(result->pixels[i][j].green + 0.5), 0), 255);
            result->pixels[i][j].red = std::min(std::max(int(result->pixels[i][j].red + 0.5), 0), 255);
        }
    }

    return result;
}

Image* combine( Image* trackingImage, std::string& greenLayerPath, std::string& blueLayerPath) {
    // First, make a deep copy of the trackingImage to work on
    Image* combinedImage = deepCopy(trackingImage);

    // Load the green layer
    Image* greenLayer = readFile(greenLayerPath);
    if (!greenLayer) {
        std::cerr << "File does not exist: " << greenLayerPath << std::endl;
        delete combinedImage; // Clean up the combined image before returning
        return nullptr;
    }

    // Load the blue layer
    Image* blueLayer = readFile(blueLayerPath);
    if (!blueLayer) {
        std::cerr << "File does not exist: " << blueLayerPath << std::endl;
        delete greenLayer; // Clean up the green layer before returning
        delete combinedImage; // Clean up the combined image before returning
        return nullptr;
    }

    // Assuming all images have the same dimensions and format
    for (int i = 0; i < combinedImage->header.height; i++) {
        for (int j = 0; j < combinedImage->header.width; j++) {
            // The red channel stays as is from the trackingImage copy
            // Update the green and blue channels from the loaded layers
            combinedImage->pixels[i][j].green = greenLayer->pixels[i][j].green;
            combinedImage->pixels[i][j].blue = blueLayer->pixels[i][j].blue;
        }
    }

    // Clean up the dynamically allocated green and blue layer images
    delete greenLayer;
    delete blueLayer;

    // Return the new image with combined channels
    return combinedImage;
}


// overlay formula:
// {
// NP2 <= 0.5: 2 * NP1 * NP2
// NP2 > 0.5: 1 - 2 * (1 - NP1) * (1 - NP2)
// }

template <typename T>
T clamp(T value, float low, float high) {
    return (value < low) ? low : (value > high) ? high : value;
}
//Image* overlay(Image& baseLayer, Image& blendLayer) {
//    auto* result = new Image();
//    result->header = baseLayer.header; // Assuming both images have the same dimensions and format.
//    result->pixels = new Pixel*[result->header.height];
//
//    for (int i = 0; i < result->header.height; ++i) {
//        result->pixels[i] = new Pixel[result->header.width];
//        for (int j = 0; j < result->header.width; ++j) {
//            float baseBlue = baseLayer.pixels[i][j].blue / 255.0f;
//            float baseGreen = baseLayer.pixels[i][j].green / 255.0f;
//            float baseRed = baseLayer.pixels[i][j].red / 255.0f;
//
//            float blendBlue = blendLayer.pixels[i][j].blue / 255.0f;
//            float blendGreen = blendLayer.pixels[i][j].green / 255.0f;
//            float blendRed = blendLayer.pixels[i][j].red / 255.0f;
//
//            result->pixels[i][j].blue = (blendBlue <= 0.5) ?
//                                        (2 * baseBlue * blendBlue) :
//                                        (1 - 2 * (1 - baseBlue) * (1 - blendBlue));
//            result->pixels[i][j].green = (blendGreen <= 0.5) ?
//                                         (2 * baseGreen * blendGreen) :
//                                         (1 - 2 * (1 - baseGreen) * (1 - blendGreen));
//            result->pixels[i][j].red = (blendRed <= 0.5) ?
//                                       (2 * baseRed * blendRed) :
//                                       (1 - 2 * (1 - baseRed) * (1 - blendRed));
//
//            // Clamping the results back to the 0-255 range.
//            result->pixels[i][j].blue = clamp(result->pixels[i][j].blue * 255, 0.0f, 255.0f);
//            result->pixels[i][j].green = clamp(result->pixels[i][j].green * 255, 0.0f, 255.0f);
//            result->pixels[i][j].red = clamp(result->pixels[i][j].red * 255, 0.0f, 255.0f);
//        }
//    }
//
//    return result;
//}



// addition formula: P1 + P2
Image* addition(Image& image1, Image& image2) {
    auto* result = new Image;
    *result = image1;

    for (int i = 0; i < result->header.height; i++) {
        for (int j = 0; j < result->header.width; j++) {
            result->pixels[i][j].blue = min(255, image1.pixels[i][j].blue + image2.pixels[i][j].blue);
            result->pixels[i][j].green = min(255, image1.pixels[i][j].green + image2.pixels[i][j].green);
            result->pixels[i][j].red = min(255, image1.pixels[i][j].red + image2.pixels[i][j].red);
        }
    }

    return result;
}

Image* overlay(Image& topLayer, Image& bottomLayer) {
    // Assuming both images have the same dimensions and format
    auto* result = new Image();
    result->header = topLayer.header;
    result->pixels = new Pixel*[result->header.height];

    for (int i = 0; i < result->header.height; ++i) {
        result->pixels[i] = new Pixel[result->header.width];
        for (int j = 0; j < result->header.width; ++j) {
            // Normalize pixel values to the range [0, 1] for the calculation
            float topBlue = topLayer.pixels[i][j].blue / 255.0f;
            float topGreen = topLayer.pixels[i][j].green / 255.0f;
            float topRed = topLayer.pixels[i][j].red / 255.0f;

            float bottomBlue = bottomLayer.pixels[i][j].blue / 255.0f;
            float bottomGreen = bottomLayer.pixels[i][j].green / 255.0f;
            float bottomRed = bottomLayer.pixels[i][j].red / 255.0f;

            // Apply overlay blending mode for each channel
            result->pixels[i][j].blue = bottomBlue <= 0.5 ?
                                        2 * topBlue * bottomBlue * 255 :
                                        (1 - 2 * (1 - topBlue) * (1 - bottomBlue)) * 255;

            result->pixels[i][j].green = bottomGreen <= 0.5 ?
                                         2 * topGreen * bottomGreen * 255 :
                                         (1 - 2 * (1 - topGreen) * (1 - bottomGreen)) * 255;

            result->pixels[i][j].red = bottomRed <= 0.5 ?
                                       2 * topRed * bottomRed * 255 :
                                       (1 - 2 * (1 - topRed) * (1 - bottomRed)) * 255;

            // Clamp the results to [0, 255]
            result->pixels[i][j].blue = std::min(std::max(int(result->pixels[i][j].blue + 0.5f), 0), 255);
            result->pixels[i][j].green = std::min(std::max(int(result->pixels[i][j].green + 0.5f), 0), 255);
            result->pixels[i][j].red = std::min(std::max(int(result->pixels[i][j].red + 0.5f), 0), 255);
        }
    }

    return result;
}

Image* flipImageVertically(Image* src) {
    if (!src || !src->pixels) return nullptr; // Safety check

    // Create a deep copy of the source image to work with
    Image* flipped = deepCopy(src);

    int height = flipped->header.height;
    // Temporarily allocate memory for a single row to use during swapping
    Pixel* tempRow = new Pixel[flipped->header.width];

    for (int i = 0; i < height / 2; ++i) {
        // Copy the top row to the temporary row
        std::copy(flipped->pixels[i], flipped->pixels[i] + flipped->header.width, tempRow);
        // Copy the bottom row to the top row
        std::copy(flipped->pixels[height - 1 - i], flipped->pixels[height - 1 - i] + flipped->header.width, flipped->pixels[i]);
        // Copy the temporary row (originally the top row) to the bottom row
        std::copy(tempRow, tempRow + flipped->header.width, flipped->pixels[height - 1 - i]);
    }

    // Free the temporary row memory
    delete[] tempRow;

    return flipped;
}