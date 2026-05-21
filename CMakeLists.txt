cmake_minimum_required(VERSION 3.28)
project(rtumirea_pcs_kr2_search_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(STSE_CORE_SOURCES
        src/core/FileReader.cpp
        src/core/Tokenizer.cpp
        src/core/InvertedIndex.cpp
        src/core/Indexer.cpp
        src/core/Searcher.cpp
        src/core/factory/ModuleFactory.cpp)

add_library(stse_core STATIC ${STSE_CORE_SOURCES})
target_include_directories(stse_core PUBLIC src)

set(STSE_APP_SOURCES
        src/app/main.cpp
        src/gui/MainWindow.cpp
)

add_executable(rtumirea_pcs_kr2_search_engine ${STSE_APP_SOURCES})

target_include_directories(rtumirea_pcs_kr2_search_engine PRIVATE src)
target_link_libraries(rtumirea_pcs_kr2_search_engine PRIVATE Qt6::Widgets stse_core)

enable_testing()
add_subdirectory(tests)
