#include <iostream>
#include <string>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ImageHandler.h"
#include "Huffman.cpp"
#include "Quadtree.cpp"
#include "SeamCarving.cpp" // CHANGED: Now using our new DP algorithm

using namespace std;

class ImageLoader {
public:
    static Image* loadImage(const char* filename) {
        int width, height, channels;
        unsigned char* data = stbi_load(filename, &width, &height, &channels, 1);
        
        if (data == NULL) {
            cout << "\n[ERROR] Failed to load image: " << filename << "\n";
            cout << "Reason: " << stbi_failure_reason() << "\n";
            return NULL;
        }
        
        Image* img = new Image(width, height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                img->setPixel(x, y, data[y * width + x]);
            }
        }
        
        stbi_image_free(data); 
        cout << "\n[SUCCESS] Loaded: " << filename << " (" << width << "x" << height << ")\n";
        return img;
    }

    static void saveReconstruction(const char* filename, Image* img) {
        if (img == NULL) return;
        unsigned char* data = new unsigned char[img->width * img->height];
        for (int y = 0; y < img->height; y++) {
            for (int x = 0; x < img->width; x++) {
                data[y * img->width + x] = (unsigned char)img->getPixel(x, y);
            }
        }
        stbi_write_png(filename, img->width, img->height, 1, data, img->width);
        cout << "[SUCCESS] Visual reconstruction saved as: " << filename << "\n";
        delete[] data;
    }
};

void displayAlgorithmAdvice() {
    cout << "\n┌─────────────────────────────────────────────────────────────┐\n";
    cout << "│              ALGORITHM SELECTION GUIDE                      │\n";
    cout << "├─────────────────────────────────────────────────────────────┤\n";
    cout << "│ 1. HUFFMAN (Greedy): Best for real-world photographs, noisy │\n";
    cout << "│    images, or images with many different colors.            │\n";
    cout << "│                                                             │\n";
    cout << "│ 2. QUADTREE (D&C): Best for Text documents, logos, or       │\n";
    cout << "│    images with large areas of identical solid colors.       │\n";
    cout << "│                                                             │\n";
    cout << "│ 3. SEAM CARVING (DP): Content-Aware scale. Finds and deletes│\n";
    cout << "│    the most 'boring' pixels without squishing the subject.  │\n";
    cout << "└─────────────────────────────────────────────────────────────┘\n";
}

void compareAlgorithms(Image* img) {
    if (img == NULL) {
        cout << "\n[ERROR] No image loaded!\n";
        return;
    }

    cout << "\nRunning Huffman Analysis...\n";
    HuffmanCompressor* huffman = new HuffmanCompressor();
    CompressionResult hResult = huffman->compress(img);
    delete huffman;

    cout << "\nRunning Quadtree Analysis...\n";
    QuadtreeCompressor* quadtree = new QuadtreeCompressor();
    quadtree->setThreshold(15); 
    CompressionResult qResult = quadtree->compress(img);
    delete quadtree;

    cout << "\nRunning Seam Carving Analysis (Removing 10% width)...\n";
    SeamCarvingCompressor* seam = new SeamCarvingCompressor();
    int seamsToRemove = img->width * 0.10; 
    CompressionResult dpResult = seam->compress(img, seamsToRemove);
    delete seam;

    cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                     COMPARISON RESULTS                       ║\n";
    cout << "╠══════════════════════╦════════════╦════════════╦═════════════╣\n";
    cout << "║ Metric               ║ Huffman    ║ Quadtree   ║ Seam Carve  ║\n";
    cout << "╠══════════════════════╬════════════╬════════════╬═════════════╣\n";
    
    cout << fixed << setprecision(2);
    cout << "║ Original Size (B)    ║ " << setw(10) << hResult.originalSize << " ║ " 
         << setw(10) << qResult.originalSize << " ║ " << setw(11) << dpResult.originalSize << " ║\n";

    cout << "║ Compressed Size (B)  ║ " << setw(10) << hResult.compressedSize << " ║ " 
         << setw(10) << qResult.compressedSize << " ║ " << setw(11) << dpResult.compressedSize << " ║\n";

    cout << "║ Compression Ratio    ║ " << setw(9) << hResult.compressionRatio << "% ║ " 
         << setw(9) << qResult.compressionRatio << "% ║ " << setw(10) << dpResult.compressionRatio << "% ║\n";

    cout << "║ Execution Time (ms)  ║ " << setw(10) << hResult.executionTime << " ║ " 
         << setw(10) << qResult.executionTime << " ║ " << setw(11) << dpResult.executionTime << " ║\n";
    cout << "╚══════════════════════╩════════════╩════════════╩═════════════╝\n";
}

int main() {
    Image* currentImage = NULL;

    cout << "\n=======================================================\n";
    cout << "      IMAGE COMPRESSION ALGORITHM COMPARISON SUITE     \n";
    cout << "=======================================================\n";
    
    displayAlgorithmAdvice();

    while (true) {
        cout << "\n┌──────────────────────────────────────────┐\n";
        cout << "│                MAIN MENU                 │\n";
        cout << "├──────────────────────────────────────────┤\n";
        cout << "│ 1. Upload Image (PNG/JPG/BMP)            │\n";
        cout << "│ 2. Display Image Info & Preview          │\n";
        cout << "│ 3. Compress by Huffman (Greedy)          │\n";
        cout << "│ 4. Compress by Quadtree (D&C)            │\n";
        cout << "│ 5. Compress by Seam Carving (DP)         │\n";
        cout << "│ 6. Compare All Algorithms                │\n";
        cout << "│ 7. Exit                                  │\n";
        cout << "└──────────────────────────────────────────┘\n";
        cout << "Enter choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[ERROR] Invalid input.\n";
            continue;
        }

        if (choice == 7) break;

        switch (choice) {
            case 1: {
                cout << "Enter path to image (e.g., test.png): ";
                string filename;
                cin >> ws;
                getline(cin, filename);

                Image* newImage = ImageLoader::loadImage(filename.c_str());
                if (newImage != NULL) {
                    if (currentImage != NULL) delete currentImage; 
                    currentImage = newImage;
                }
                break;
            }
            case 2: {
                if (currentImage == NULL) cout << "\n[ERROR] No image uploaded.\n";
                else {
                    cout << "\nResolution: " << currentImage->width << "x" << currentImage->height;
                    cout << "\nTotal Pixels: " << currentImage->getTotalPixels() << "\n";
                    if (currentImage->width <= 64 && currentImage->height <= 64) currentImage->display();
                }
                break;
            }
            case 3: {
                if (currentImage == NULL) cout << "\n[ERROR] No image uploaded.\n";
                else {
                    HuffmanCompressor* huffman = new HuffmanCompressor();
                    CompressionResult result = huffman->compress(currentImage);
                    result.display("Huffman Coding (Greedy)");
                    
                    cout << "\nSave Huffman Dictionary to a text file? (Y/N): ";
                    char saveOpt; cin >> saveOpt;
                    if (saveOpt == 'Y' || saveOpt == 'y') huffman->saveDictionaryToFile("huffman_dictionary.txt");
                    delete huffman;
                }
                break;
            }
            case 4: {
                if (currentImage == NULL) cout << "\n[ERROR] No image uploaded.\n";
                else {
                    int t;
                    cout << "Enter uniformity threshold (5 strict, 30 photos): ";
                    cin >> t;
                    QuadtreeCompressor* quadtree = new QuadtreeCompressor();
                    quadtree->setThreshold(t);
                    CompressionResult result = quadtree->compress(currentImage);
                    result.display("Quadtree (Divide & Conquer)");

                    cout << "\nSave visual reconstruction to see block artifacts? (Y/N): ";
                    char saveOpt; cin >> saveOpt;
                    if (saveOpt == 'Y' || saveOpt == 'y') {
                        Image* reconImg = quadtree->getReconstructedImage();
                        ImageLoader::saveReconstruction("quadtree_compressed.png", reconImg);
                        delete reconImg; 
                    }
                    delete quadtree;
                }
                break;
            }
            case 5: { // CHANGED TO SEAM CARVING
                if (currentImage == NULL) cout << "\n[ERROR] No image uploaded.\n";
                else {
                    int numSeams;
                    cout << "How many vertical seams (pixels of width) do you want to remove? (e.g., 50): ";
                    cin >> numSeams;

                    SeamCarvingCompressor* seam = new SeamCarvingCompressor();
                    CompressionResult result = seam->compress(currentImage, numSeams);
                    result.display("Seam Carving (DP + Backtracking)");

                    cout << "\nSave Content-Aware compressed image? (Y/N): ";
                    char saveOpt; cin >> saveOpt;
                    if (saveOpt == 'Y' || saveOpt == 'y') {
                        ImageLoader::saveReconstruction("seam_carved_image.png", seam->finalReconstructedImage);
                    }
                    delete seam;
                }
                break;
            }
            case 6: {
                if (currentImage == NULL) cout << "\n[ERROR] No image uploaded.\n";
                else compareAlgorithms(currentImage);
                break;
            }
            default: cout << "\n[ERROR] Invalid choice!\n";
        }
    }

    if (currentImage != NULL) delete currentImage;
    return 0;
}