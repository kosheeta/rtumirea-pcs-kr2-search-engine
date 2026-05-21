#ifndef STSE_I_FILE_READER_H
#define STSE_I_FILE_READER_H

#include <string>
#include <vector>

class IFileReader {
public:
    virtual ~IFileReader() = default;
    virtual std::vector<std::string> listFiles(const std::string& path) = 0;
    virtual std::string readFile(const std::string& path) = 0;
};

#endif
