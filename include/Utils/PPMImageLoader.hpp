#ifndef PPM_IMAGE_LOADER_HPP
#define PPM_IMAGE_LOADER_HPP

#include <string>
#include <vector>

bool loadPPM(const std::string& filename,
             unsigned& width,
             unsigned& height,
             std::vector<unsigned char>& data);

#endif // PPM_IMAGE_LOADER_HPP
