#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <filesystem>

std::string generateUniqueFilename(const std::string directory = ".", const std::string extension = ".txt", size_t length = 64);