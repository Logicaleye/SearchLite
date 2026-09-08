cmake_minimum_required(VERSION 3.20)

project(SearchLite VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SearchLite
    src/main.cpp
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/DocumentLoader.cpp
    src/QueryProcessor.cpp
)

target_include_directories(SearchLite PRIVATE
    include
)

add_executable(TestInvertedIndex
    src/InvertedIndex.cpp
    tests/test_inverted_index.cpp
)

target_include_directories(TestInvertedIndex PRIVATE
    include
)

add_executable(TestTextProcessor
    src/TextProcessor.cpp
    tests/test_text_processor.cpp
)

target_include_directories(TestTextProcessor PRIVATE
    include
)

# Document Loader Tests
add_executable(TestDocumentLoader
    src/DocumentLoader.cpp
    tests/test_document_loader.cpp
)

target_include_directories(TestDocumentLoader PRIVATE
    include
)

# Query Processor Tests
add_executable(TestQueryProcessor
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/QueryProcessor.cpp
    tests/test_query_processor.cpp
)

target_include_directories(TestQueryProcessor PRIVATE
    include
)