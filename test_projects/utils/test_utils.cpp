#include "test_utils.h"

std::string generateRandomString(size_t length) {
    const std::string CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += CHARACTERS[distribution(generator)];
    }
    return randomString;
}

std::string generateUniqueFilename(const std::string& directory, const std::string& extension, size_t length) {
    std::string filename;
    do {
        std::string randomString = generateRandomString(length);
        filename = directory + "/" + randomString + extension;
    } while (std::filesystem::exists(filename));
    return filename;
}