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
    src/Ranker.cpp
    src/Trie.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/SearchCache.cpp
    src/IndexPersistence.cpp
    src/IndexMetadata.cpp
    src/IndexValidator.cpp
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
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/SearchCache.cpp
    tests/test_query_processor.cpp
)

target_include_directories(TestQueryProcessor PRIVATE
    include
)

# Ranker Tests
add_executable(TestRanker
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/QueryProcessor.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/Ranker.cpp
    src/SearchCache.cpp
    tests/test_ranker.cpp
)

target_include_directories(TestRanker PRIVATE
    include
)

# Trie Tests
add_executable(TestTrie
    src/Trie.cpp
    tests/test_trie.cpp
)

target_include_directories(TestTrie PRIVATE
    include
)

# Trie Integration Tests
add_executable(TestTrieIntegration
    src/Trie.cpp
    src/TextProcessor.cpp
    tests/test_trie_integration.cpp
)

target_include_directories(TestTrieIntegration PRIVATE
    include
)

# Test Phrase Search 
add_executable(TestPhraseSearch
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/QueryProcessor.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/SearchCache.cpp
    tests/test_phrase_search.cpp
)

target_include_directories(TestPhraseSearch PRIVATE include)

# TestBooleanSearch
add_executable(TestBooleanSearch
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/QueryProcessor.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/SearchCache.cpp
    tests/test_boolean_search.cpp
)
target_include_directories(TestBooleanSearch PRIVATE
    include
)

#TestTopK
add_executable(TestTopK
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/QueryProcessor.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/Ranker.cpp
    src/SearchCache.cpp
    tests/test_top_k.cpp
)

target_include_directories(TestTopK PRIVATE include)

#TestBooleanParser
add_executable(TestBooleanParser
    src/BooleanParser.cpp
    tests/test_boolean_parser.cpp
)
target_include_directories(TestBooleanParser PRIVATE include)

#TestBooleanEvaluater
add_executable(TestBooleanEvaluator
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    tests/test_boolean_evaluator.cpp
)
target_include_directories(TestBooleanEvaluator PRIVATE include)

# Test Search Cache
add_executable(TestSearchCache
    tests/test_search_cache.cpp
    src/SearchCache.cpp
)

target_include_directories(TestSearchCache PRIVATE
    include
)

# Index Persistence Tests
add_executable(TestIndexPersistence
    src/InvertedIndex.cpp
    src/TextProcessor.cpp
    src/QueryProcessor.cpp
    src/BooleanParser.cpp
    src/BooleanEvaluator.cpp
    src/SearchCache.cpp
    src/IndexPersistence.cpp
    tests/test_index_persistence.cpp
)

target_include_directories(TestIndexPersistence PRIVATE
    include
)

add_executable(TestIndexMetadata
    tests/test_index_metadata.cpp
    src/IndexMetadata.cpp
)

target_include_directories(
    TestIndexMetadata
    PRIVATE
    include
)

add_executable(TestIndexValidator
    tests/test_index_validator.cpp
    src/IndexMetadata.cpp
    src/IndexValidator.cpp
)

target_include_directories(
    TestIndexValidator
    PRIVATE
    include
)

add_executable(TestIndexMetadataPersistence
    tests/test_index_metadata_persistence.cpp
    src/IndexMetadata.cpp
    src/IndexPersistence.cpp
    src/InvertedIndex.cpp
)

target_include_directories(
    TestIndexMetadataPersistence
    PRIVATE
    include
)