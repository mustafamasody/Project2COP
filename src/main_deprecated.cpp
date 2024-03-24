#include "Image.h"
#include <iostream>
#include <string>
#include <algorithm> // For std::min and std::max

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

void part1(Image* image4, Image* image6) {
    Image* task1 = new Image;
    *task1 = *multiply(*image4, *image6);
    writeData(*task1, "output/part1.tga");
    delete task1;
}

void part2(Image* image5, Image* image0) {
    Image* task2 = new Image;
    *task2 = *subtract(*image5, *image0);
    writeData(*task2, "output/part2.tga");
    delete task2;
}

void part3(Image* image4, Image* image7, Image* image2) {
    Image* task3 = new Image;
    *task3 = *multiply(*image4, *image7);
    screen(*task3, *image2);
    writeData(*task3, "output/part3.tga");
    delete task3;
}

void part4(Image* image5, Image* image1, Image* image7) {
    Image* tempResult = multiply(*image5, *image1);
    Image* finalResult = subtract(*image7, *tempResult);
    writeData(*finalResult, "output/part4.tga");
    delete tempResult;
    delete finalResult;
}

void part5(Image* image4, Image* image6) {
    Image* task5 = new Image;
    *task5 = *overlay(*image4, *image6);
    writeData(*task5, "output/part5.tga");
    delete task5;
}

void part6(Image* image0) {
    Image* greenModifiedImage = deepCopy(image0);
    for (int i = 0; i < greenModifiedImage->header.height; i++) {
        for (int j = 0; j < greenModifiedImage->header.width; j++) {
            greenModifiedImage->pixels[i][j].green = std::min(255, greenModifiedImage->pixels[i][j].green + 200);
        }
    }
    writeData(*greenModifiedImage, "output/part6.tga");
    delete greenModifiedImage;
}

void part7(Image* image0) {
    Image* colorScaledImage = deepCopy(image0);
    for (int i = 0; i < colorScaledImage->header.height; i++) {
        for (int j = 0; j < colorScaledImage->header.width; j++) {
            colorScaledImage->pixels[i][j].red = std::min(255, colorScaledImage->pixels[i][j].red * 4);
            colorScaledImage->pixels[i][j].blue = 0;
        }
    }
    writeData(*colorScaledImage, "output/part7.tga");
    delete colorScaledImage;
}

void part8(Image* image0) {
    Image* redChannelImage = deepCopy(image0);
    Image* greenChannelImage = deepCopy(image0);
    Image* blueChannelImage = deepCopy(image0);
    for (int i = 0; i < redChannelImage->header.height; i++) {
        for (int j = 0; j < redChannelImage->header.width; j++) {
            unsigned char redValue = redChannelImage->pixels[i][j].red;
            redChannelImage->pixels[i][j] = {redValue, redValue, redValue};
            unsigned char greenValue = greenChannelImage->pixels[i][j].green;
            greenChannelImage->pixels[i][j] = {greenValue, greenValue, greenValue};
            unsigned char blueValue = blueChannelImage->pixels[i][j].blue;
            blueChannelImage->pixels[i][j] = {blueValue, blueValue, blueValue};
        }
    }
    writeData(*redChannelImage, "output/part8_r.tga");
    writeData(*greenChannelImage, "output/part8_g.tga");
    writeData(*blueChannelImage, "output/part8_b.tga");
    delete redChannelImage;
    delete greenChannelImage;
    delete blueChannelImage;
}

void part9(Image* image8, Image* image9, Image* image10) {
    Image* task9 = new Image;
    *task9 = *deepCopy(image8);
    for (int i = 0; i < task9->header.height; i++) {
        for (int j = 0; j < task9->header.width; j++) {
            task9->pixels[i][j].green = image9->pixels[i][j].green;
            task9->pixels[i][j].blue = image10->pixels[i][j].blue;
        }
    }
    writeData(*task9, "output/part9.tga");
    delete task9;
}

void part10(Image* image3) {
    Image* task10 = deepCopy(image3);
    int height = task10->header.height;
    int width = task10->header.width;
    if (task10->pixels != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] task10->pixels[i];
        }
        delete[] task10->pixels;
    }
    task10->pixels = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        task10->pixels[i] = new Pixel[width];
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int inv_i = height - 1 - i;
            int inv_j = width - 1 - j;
            task10->pixels[i][j] = image3->pixels[inv_i][inv_j];
        }
    }
    writeData(*task10, "output/part10.tga");
    for (int i = 0; i < height; ++i) {
        delete[] task10->pixels[i];
    }
    delete[] task10->pixels;
    delete task10;
}

int main() {
    std::string paths[] = {
            "input/car.tga",
            "input/circles.tga",
            "input/text.tga",
            "input/text2.tga",
            "input/layer1.tga",
            "input/layer2.tga",
            "input/pattern1.tga",
            "input/pattern2.tga",
            "input/layer_red.tga",
            "input/layer_green.tga",
            "input/layer_blue.tga"
    };

    Image* images[11]{}; // Initialize array of Image pointers to nullptr.

    // Load images
    for (int i = 0; i < 11; ++i) {
        images[i] = readFile(paths[i]);
        if (!images[i]) {
            std::cerr << "Failed to load image: " << paths[i] << std::endl;
            // Cleanup and early exit if critical resources fail to load
            for (int j = 0; j <= i; ++j) {
                delete images[j]; // Safe delete, will ignore nullptr
            }
            return -1;
        }
    }

    std::string outputPath;

    // Task 1: Multiply layer1.tga with pattern1.tga
    Image* image4C = deepCopy(images[4]);
    Image* image6C = deepCopy(images[6]);
    Image* task1 = new Image;
    *task1 = *multiply(*image4C, *image6C);
    outputPath = "output/part1.tga";
    writeData(*task1, outputPath);
    delete task1;
    task1 = nullptr;
    delete image4C;
    delete image6C;

    // Task 2: use  Subtract blending mode to combine layer2.tga (bottom layer) with car.tga (top
    //layer).
    Image* task2 = new Image;
    Image* image5CTask2 = deepCopy(images[5]);
    Image* image0CTask2 = deepCopy(images[0]);
    *task2 = *subtract(*image5CTask2, *image0CTask2);
    outputPath = "output/part2.tga";
    writeData(*task2, outputPath);
    delete task2;
    task2 = nullptr;
    delete image5CTask2;
    delete image0CTask2;

    // Task 3: Multiply layer1.tga with pattern2.tga, store the results temporarily in memory (do not write to disk, just store the pixel values), load the image text.tga and use that as the bottom layer, combine
    // it with the previous results of layer1/pattern2, and write the final result to disk using screen blending mode.
    Image* task3 = new Image;
    Image* image4CTask3 = deepCopy(images[4]);
    Image* image7CTask3 = deepCopy(images[7]);
    Image* image2CTask3 = deepCopy(images[2]);
    *task3 = *multiply(*image4CTask3, *image7CTask3);
    screen(*task3, *image2CTask3);
    outputPath = "output/part3.tga";
    writeData(*task3, outputPath);
    delete task3;
    task3 = nullptr;
    delete image4CTask3;
    delete image7CTask3;
    delete image2CTask3;

    std::cout << "Starting task 4" << std::endl;
    // Task 4: Multiply layer2.tga with circles.tga, and store it. Load pattern2.tga and, using that
    //as the bottom layer, combine it with the previous result using the Subtract blending mode.

    Image* tempResult = multiply(*images[5], *images[1]);
    Image* finalResult = subtract(*images[7], *tempResult);
    if (finalResult != nullptr) {
        outputPath = "output/part4.tga";
        writeData(*finalResult, outputPath);
        delete tempResult;
        delete finalResult;
    }

    // Task 5: Combine layer1.tga (as the top layer) with pattern1.tga using the Overlay blending mode
    if(images[4] && images[6]){
        Image* task5Copy = deepCopy(images[4]);
        Image* img4Task5 = deepCopy(images[4]);
        Image* img6Task5 = deepCopy(images[6]);
        *task5Copy = *overlay(*img4Task5, *img6Task5);
        outputPath = "output/part5.tga";
        writeData(*task5Copy, outputPath);
        delete task5Copy;
    }
//    Image* task5 = new Image;
//    *task5 = *overlay(*images[4], *images[6]);
//    outputPath = "output/part5.tga";
//    writeData(*task5, outputPath);
//    delete task5;
//    task5 = nullptr;

    // Task 6: Load car.tga and add 200 to the green channel.
    if (images[0]) {
        Image* greenModifiedImage = deepCopy(images[0]); // Create a deep copy for modification

        for (int i = 0; i < greenModifiedImage->header.height; i++) {
            for (int j = 0; j < greenModifiedImage->header.width; j++) {
                // Add 200 to green channel, ensuring it doesn't exceed 255
                greenModifiedImage->pixels[i][j].green = std::min(255, greenModifiedImage->pixels[i][j].green + 200);
            }
        }
        outputPath = "output/part6.tga";
        writeData(*greenModifiedImage, outputPath); // Write the modified copy
        delete greenModifiedImage; // Clean up
    }

    // Task 7: Load car.tga and scale (multiply) the red channel by 4, and the blue channel by 0. This will
    //increase the intensity of any red in the image, while negating any blue it may have.
    if (images[0]) {
        Image* colorScaledImage = deepCopy(images[0]);

        for (int i = 0; i < colorScaledImage->header.height; i++) {
            for (int j = 0; j < colorScaledImage->header.width; j++) {
                // Scale red by 4, ensuring it doesn't exceed 255
                colorScaledImage->pixels[i][j].red = std::min(255, colorScaledImage->pixels[i][j].red * 4);
                // Set blue to 0
                colorScaledImage->pixels[i][j].blue = 0;
            }
        }
        outputPath = "output/part7.tga";
        writeData(*colorScaledImage, outputPath); // Write the modified copy
        delete colorScaledImage; // Clean up
    }

    // Task 8: Load car.tga and write each channel to a separate file: the red channel should be part8_r.tga,
    //the green channel should be part8_g.tga, and the blue channel should be part8_b.tga. (Hint:
    //If your red channel image appears all red, try writing [Red, Red, Red] instead of [Red, 0, 0] to the
    //file—ditto for green and blue!)

    // part8_r.tga
    if (images[0]) {
        Image* original = images[0]; // Original image
        Image* redChannelImage = deepCopy(original); // Create a deep copy for modification

        for (int i = 0; i < redChannelImage->header.height; i++) {
            for (int j = 0; j < redChannelImage->header.width; j++) {
                unsigned char redValue = redChannelImage->pixels[i][j].red;
                redChannelImage->pixels[i][j] = {redValue, redValue, redValue}; // Modify copy
            }
        }
        outputPath = "output/part8_r.tga";
        writeData(*redChannelImage, outputPath); // Write the modified copy
        delete redChannelImage; // Clean up
    }

    // part8_g.tga
    if (images[0]) {
        Image* original = images[0]; // Original image
        Image* greenChannelImage = deepCopy(original); // Create a deep copy for modification

        for (int i = 0; i < greenChannelImage->header.height; i++) {
            for (int j = 0; j < greenChannelImage->header.width; j++) {
                unsigned char greenValue = greenChannelImage->pixels[i][j].green;
                greenChannelImage->pixels[i][j] = {greenValue, greenValue, greenValue}; // Modify copy
            }
        }
        outputPath = "output/part8_g.tga";
        writeData(*greenChannelImage, outputPath); // Write the modified copy
        delete greenChannelImage; // Clean up
    }

    // part8_b.tga
    if (images[0]) {
        Image* original = images[0]; // Original image
        Image* blueChannelImage = deepCopy(original); // Create a deep copy for modification

        for (int i = 0; i < blueChannelImage->header.height; i++) {
            for (int j = 0; j < blueChannelImage->header.width; j++) {
                unsigned char blueValue = blueChannelImage->pixels[i][j].blue;
                blueChannelImage->pixels[i][j] = {blueValue, blueValue, blueValue}; // Modify copy
            }
        }
        outputPath = "output/part8_b.tga";
        writeData(*blueChannelImage, outputPath); // Write the modified copy
        delete blueChannelImage; // Clean up
    }

    // Task 9: . Load layer_red.tga, layer_green.tga and layer_blue.tga, and combine the three files
    //into one file. The data from layer_red.tga is the red channel of the new image, layer_green is
    //green, and layer_blue is blue
    if (images[8] && images[9] && images[10]) {
        Image* task9 = new Image;
        *task9 = *deepCopy(images[8]);
        for (int i = 0; i < task9->header.height; i++) {
            for (int j = 0; j < task9->header.width; j++) {
                task9->pixels[i][j].green = images[9]->pixels[i][j].green;
                task9->pixels[i][j].blue = images[10]->pixels[i][j].blue;
            }
        }
        outputPath = "output/part9.tga";
        writeData(*task9, outputPath);
        delete task9;
    }

    // Task 10: Load text2.tga, and rotate it 180 degrees, flipping it upside down. This is easier than you think!
    //Try diagramming the data of an image (such as earlier in this document). What would the data
    //look like if you flipped it? Now, how to write some code to accomplish that...?
    if (images[3]) {
        Image* task10 = deepCopy(images[3]);

        int height = task10->header.height;
        int width = task10->header.width;

        std::cout << "1" << std::endl;

        if (task10->pixels != nullptr) {
            for (int i = 0; i < height; ++i) {
                delete[] task10->pixels[i]; // Free each row of pixels
            }
            delete[] task10->pixels; // Free the array of pointers
        }

        std::cout << "2" << std::endl;

        task10->pixels = new Pixel*[height];
        for (int i = 0; i < height; ++i) {
            task10->pixels[i] = new Pixel[width];
        }

        std::cout << "3" << std::endl;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int inv_i = height - 1 - i; // Inverted row index
                int inv_j = width - 1 - j; // Inverted column index
                task10->pixels[i][j] = images[3]->pixels[inv_i][inv_j];
            }
        }

        std::cout << "4" << std::endl;

        outputPath = "output/part10.tga";
        writeData(*task10, outputPath);


        // Clean up the dynamically allocated memory for task10
//        for (int i = 0; i < height; ++i) {
//            delete[] task10->pixels[i];
//        }
//        delete[] task10->pixels;
//        delete task10;
    }

    // Ensure cleanup of all loaded images to prevent memory leaks
    for (auto& image : images) {
        delete image;
    }

    return 0;
}