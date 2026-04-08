#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <iostream>
using namespace std;

// Simple structure to hold image data
class Image {
public:
    int width;
    int height;
    int** pixels;  // 2D array for pixel values (0-255)
    
    // Constructor
    Image(int w, int h) {
        width = w;
        height = h;
        
        // Allocate 2D array
        pixels = new int*[height];
        for (int i = 0; i < height; i++) {
            pixels[i] = new int[width];
        }
    }
    
    // Destructor
    ~Image() {
        for (int i = 0; i < height; i++) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
    
    // Get pixel value at (x, y)
    int getPixel(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y][x];
        }
        return 0;
    }
    
    // Set pixel value at (x, y)
    void setPixel(int x, int y, int value) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y][x] = value;
        }
    }
    
    // Display image (text representation)
    void display() {
        cout << "\nImage " << width << "x" << height << ":\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (pixels[y][x] == 0) cout << "  "; // Black
                else if (pixels[y][x] < 128) cout << "░░"; // Dark
                else if (pixels[y][x] < 200) cout << "▒▒"; // Medium
                else cout << "██"; // White
            }
            cout << "\n";
        }
    }
    
    // Calculate total pixels
    int getTotalPixels() {
        return width * height;
    }
};

// Structure to hold compression results
struct CompressionResult {
    int originalSize;      // in bytes
    int compressedSize;    // in bytes
    double compressionRatio; // percentage
    double executionTime;  // in milliseconds
    
    void display(const char* algorithmName) {
        cout << "\n========================================\n";
        cout << "Algorithm: " << algorithmName << "\n";
        cout << "========================================\n";
        cout << "Original Size:      " << originalSize << " bytes\n";
        cout << "Compressed Size:    " << compressedSize << " bytes\n";
        cout << "Compression Ratio:  " << compressionRatio << "%\n";
        cout << "Execution Time:     " << executionTime << " ms\n";
        cout << "========================================\n";
    }
};

#endif