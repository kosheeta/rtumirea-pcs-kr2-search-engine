set(STSE_TEST_SOURCES
        test_tokenizer.cpp
        test_filereader.cpp
        test_inverted_index.cpp
        test_indexer.cpp
        test_searcher.cpp
        e2e/01_happy_path.cpp
        e2e/02_empty_directory.cpp
        e2e/03_fuzzy_typo.cpp
        e2e/04_multi_token.cpp)

add_executable(stse_unit_tests ${STSE_TEST_SOURCES})
target_include_directories(stse_unit_tests PRIVATE ../src)
target_link_libraries(stse_unit_tests PRIVATE stse_core GTest::gtest_main)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)
gtest_discover_tests(stse_unit_tests)
