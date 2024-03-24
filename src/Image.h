#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // For std::min

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Image {
    Header header;
    Pixel **pixels;

    ~Image() { // Destructor to free allocated memory
        if (pixels) {
            for (int i = 0; i < header.height; ++i) {
                delete[] pixels[i];
            }
            delete[] pixels;
        }
    }
};

Image* readFile( std::string filename);
void readData(std::ifstream& file, Image* img);
void writeData( Image& image,  std::string& filename);
Image* multiply( Image& image1,  Image& image2);
Image* screen( Image& image1,  Image& image2);
Image* subtract( Image& image1,  Image& image2);
Image* addition( Image& image1,  Image& image2);
Image* overlay( Image& image1,  Image& image2);
// void combine(Image*& trackingImage,  std::string& greenLayerPath,  std::string& blueLayerPath) {
Image* combine(Image* trackingImage, std::string& greenLayerPath, std::string& blueLayerPath);
Image* flipImageVertically(Image* src);

Image* onlyRed( Image* src);
Image* onlyGreen( Image* src);
Image* onlyBlue( Image* src);
Image* addRed( Image* src, int value);
Image* addGreen( Image* src, int value);
Image* addBlue( Image* src, int value);