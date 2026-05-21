#ifndef STSE_FILE_READER_H
#define STSE_FILE_READER_H

#include "interfaces/IFileReader.h"

class FileReader : public IFileReader {
public:
    std::vector<std::string> listFiles(const std::string& path) override;
    std::string readFile(const std::string& path) override;
};

#endif
