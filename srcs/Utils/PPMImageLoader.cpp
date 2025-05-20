#include "PPMImageLoader.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

bool loadPPM(const std::string& filename,
             unsigned& width,
             unsigned& height,
             std::vector<unsigned char>& data)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open PPM file: " << filename << "\n";
        return false;
    }
    std::string magic;
    in >> magic;
    if (magic != "P6") {
        std::cerr << "Invalid PPM header: " << magic << "\n";
        return false;
    }
    unsigned maxval;
    while (in.peek() == '#') in.ignore(1000, '\n');
    in >> width >> height >> maxval;
    in.get(); // skip single whitespace

    size_t total = width * height * 3;
    data.resize(total);
    in.read(reinterpret_cast<char*>(data.data()), total);
    if (!in) {
        std::cerr << "Error reading PPM data\n";
        return false;
    }
    std::cout << "PPM Loaded: " << filename << " " << width << "x" << height << std::endl;
    // vertical flip
    for (unsigned y = 0; y < height/2; ++y) {
        unsigned idx1 = y * width * 3;
        unsigned idx2 = (height - 1 - y) * width * 3;
        for (unsigned i = 0; i < width*3; ++i)
            std::swap(data[idx1 + i], data[idx2 + i]);
    }
    return true;
}
