//
// Created by Nikita Koshelenko on 12.03.2026.
//

#ifndef RTUMIREA_PCS_KR2_SEARCH_ENGINE_FILEREADER_H
#define RTUMIREA_PCS_KR2_SEARCH_ENGINE_FILEREADER_H


#include <iostream>

class FileReader {
public:
    static std::vector<std::string> listFiles(const std::string& path);
    static std::string readFile(const std::string& path);
};


#endif //RTUMIREA_PCS_KR2_SEARCH_ENGINE_FILEREADER_H
