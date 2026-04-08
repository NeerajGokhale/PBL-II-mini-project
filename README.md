# Image Compression Algorithm Comparison Suite

A C++ console application developed for **PBL-II (Problem Based Learning – 2)**, 4th Semester B.Tech. This project demonstrates and compares three classical image compression and resizing algorithms, each representing a distinct algorithmic paradigm studied in Advanced Data Structures & Algorithm Design.

---

## Algorithms Implemented

| Algorithm | Paradigm | Best Use Case |
|---|---|---|
| Huffman Coding | Greedy | Real-world photos with varied pixel intensity |
| Quadtree Decomposition | Divide & Conquer | Logos, diagrams, images with large uniform regions |
| Seam Carving | Dynamic Programming + Backtracking | Content-aware image resizing |

---

## Project Structure

```
pbl project/
├── main.cpp                        # Entry point — interactive menu and algorithm orchestration
├── ImageHandler.h                  # Image class (2D pixel array) and CompressionResult struct
├── Huffman.cpp                     # Huffman tree construction and prefix code generation
├── Quadtree.cpp                    # Recursive quadtree builder and visual reconstruction
├── SeamCarving.cpp                 # DP energy table, seam finding, and seam removal
├── stb_image.h                     # Single-header library for loading PNG/JPG/BMP
├── stb_image_write.h               # Single-header library for saving PNG output
├── lena.png                        # Sample test image
├── huffman_dictionary.txt          # Output: Huffman prefix code table (generated at runtime)
├── quadtree_compressed_preview.png # Output: Visual block reconstruction from Quadtree
├── seam_carved_image.png           # Output: Content-aware resized image from Seam Carving
└── .vscode/                        # VS Code build and debug configuration
```

---

## How Each Algorithm Works

### 1. Huffman Coding (Greedy)
Builds a frequency table of pixel intensities (0–255), then constructs a binary tree by repeatedly merging the two lowest-frequency nodes. More frequent pixel values get shorter binary codes, reducing the total number of bits needed to represent the image. The result is a lossless compression scheme.

- **Data structure used:** Binary tree (heap-style greedy selection)
- **Output:** Compression ratio and an exportable prefix code dictionary

### 2. Quadtree Decomposition (Divide & Conquer)
Recursively divides the image into four quadrants. If all pixels in a region fall within a configurable uniformity threshold, that region is stored as a single leaf node with one color value. Otherwise, the region is subdivided further.

- **Data structure used:** Quadtree (recursive tree with four children per node)
- **Output:** Compression stats and an optional visual reconstruction showing the block artifacts

### 3. Seam Carving (Dynamic Programming)
Computes a per-pixel energy map based on contrast with neighboring pixels. A DP table accumulates the minimum-energy path from top to bottom. The lowest-energy vertical "seam" is then backtracked and removed, shrinking the image width by one pixel per iteration. Repeating this removes visually unimportant content while preserving the subject.

- **Data structure used:** 2D DP table, backtracking array
- **Output:** A resized image with a user-specified number of seams removed

---

## Getting Started

### Prerequisites
- A C++17-compatible compiler (e.g., `g++` via MinGW on Windows, or `g++` on Linux/macOS)
- VS Code with the C/C++ extension (optional, configuration files included)

### Build

Using g++ directly:
```bash
g++ main.cpp -o main -std=c++17
```

Or use the included VS Code task: **Terminal → Run Build Task** (`Ctrl+Shift+B`).

### Run
```bash
./main        # Linux / macOS
main.exe      # Windows
```

---

## Usage

On launch, the program displays an **Algorithm Selection Guide** and then presents a menu:

```
│ 1. Upload Image (PNG/JPG/BMP)            │
│ 2. Display Image Info & Preview          │
│ 3. Compress by Huffman (Greedy)          │
│ 4. Compress by Quadtree (D&C)            │
│ 5. Compress by Seam Carving (DP)         │
│ 6. Compare All Algorithms                │
│ 7. Exit                                  │
```

1. Start by selecting **option 1** and entering an image path (e.g., `lena.png`).
2. Run any individual algorithm or use **option 6** to benchmark all three side-by-side.
3. For Huffman, you can export the prefix code dictionary to `huffman_dictionary.txt`.
4. For Quadtree, you can save a visual reconstruction to see the block-compression effect.
5. For Seam Carving, enter the number of vertical seams (pixels of width) to remove, then optionally save the output image.

### Comparison Output Example
```
╠══════════════════════╬════════════╬════════════╬═════════════╣
║ Metric               ║ Huffman    ║ Quadtree   ║ Seam Carve  ║
╠══════════════════════╬════════════╬════════════╬═════════════╣
║ Original Size (B)    ║     262144 ║     262144 ║      262144 ║
║ Compressed Size (B)  ║     183201 ║     134556 ║      236544 ║
║ Compression Ratio    ║     30.09% ║     48.67% ║      10.00% ║
║ Execution Time (ms)  ║       12.4 ║        8.1 ║      841.3  ║
```

---

## Key Concepts (PBL-II Curriculum)

- **Greedy Algorithms** — Huffman coding makes a locally optimal choice at each step (merge the two minimum-frequency nodes) to build a globally optimal prefix tree.
- **Divide & Conquer** — Quadtree recursively breaks the problem into four independent subproblems and combines results at leaf nodes.
- **Dynamic Programming** — Seam Carving builds a DP table where each cell's value depends on the row above it, avoiding redundant recomputation of energy paths.
- **Backtracking** — Used in Seam Carving to trace the optimal path back from the minimum at the bottom row to the top.
- **Tree Data Structures** — Both Huffman (binary tree) and Quadtree (4-ary tree) are implemented from scratch without STL containers.

---

## Dependencies

| Library | Purpose | Included |
|---|---|---|
| `stb_image.h` | Load PNG, JPG, BMP files | Yes (header-only) |
| `stb_image_write.h` | Save output as PNG | Yes (header-only) |

No external library installation is required.

---

## Notes

- Images are loaded in **grayscale** (single channel) for simplicity. Color image support can be added by extending the `Image` class to store RGB channels.
- The Quadtree works on power-of-two grid sizes; the code automatically pads to the next power of two.
- Seam Carving is computationally intensive for large images — removing many seams from a high-resolution image may take several seconds.
