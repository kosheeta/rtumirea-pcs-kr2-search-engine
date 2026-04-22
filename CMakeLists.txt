cmake_minimum_required(VERSION 3.31)
project(rtumirea_pcs_kr2_search_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(rtumirea_pcs_kr2_search_engine src/main.cpp
        src/InvertedIndex/InvertedIndex.cpp
        src/InvertedIndex/InvertedIndex.h
        src/Indexer/Indexer.cpp
        src/Indexer/Indexer.h
        src/Tokenizer/Tokenizer.cpp
        src/Tokenizer/Tokenizer.h
        src/FileReader/FileReader.cpp
        src/FileReader/FileReader.h
        src/Searcher/Searcher.cpp
        src/Searcher/Searcher.h)
