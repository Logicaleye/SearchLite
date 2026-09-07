**SearchLite — Software Requirements Specification**

**Project Name:** SearchLite
**Document:** Software Requirements Specification
**Version:** 0.1
**Status:** Initial Draft
**Primary Language:** C++
**Development Approach:** Incremental / Iterative

# 1. Introduction

1.1 **Purpose**

SearchLite is a lightweight search engine designed to efficiently search and retrieve relevant documents from a predefined collection of text-based documents.

The project aims to demonstrate the practical application of Data Structures and Algorithms in a real-world software system. The core search functionality will use an inverted index to efficiently map terms to the documents in which they occur.

The system will progressively incorporate techniques such as:

Hash-based indexing
String processing
Document tokenisation
Inverted indexing
Result ranking
Sorting
Priority queues
Trie-based autocomplete
Caching

The project will initially operate on a local document collection and may later be exposed through an API and web interface.

1.2 **Project Scope**

The initial version of SearchLite will:

Read a collection of text documents.
Process and normalise document text.
Tokenise documents into individual terms.
Build an inverted index.
Accept search queries from users.
Identify documents matching the query.
Calculate a relevance score.
Rank matching documents.
Return the most relevant results.

Future versions may include:

Autocomplete
Phrase search
Boolean queries
TF-IDF ranking
Top-K retrieval
Search-result caching
REST API
Web-based user interface
Performance benchmarking
Deployment as a live application
Out of scope for the initial version

The first version will not attempt to:

Crawl the entire public internet.
Compete with commercial search engines.
Index dynamically changing websites.
Implement distributed indexing.
Use machine-learning-based ranking.

This is important because it keeps the project achievable as a solo development project.


# 2. Problem Statement

Searching a large collection of documents by scanning every document sequentially can become inefficient as the size of the collection increases.
A search system therefore needs an efficient method of determining which documents contain the terms requested by a user.
SearchLite addresses this problem by constructing an inverted index, where each searchable term is associated with the documents containing that term.

For example:

machine      → [Document 1, Document 5, Document 12]

learning     → [Document 1, Document 5, Document 8]

computer     → [Document 2, Document 7]

When a user searches for:

machine learning

the system can retrieve candidate documents through the index rather than scanning every document from scratch.
The project will also investigate how different data structures and algorithms affect search efficiency, memory usage and ranking performance.


# 3. Project Objectives

The major objectives of SearchLite are:

*O1* — Efficient indexing

Develop an inverted-index-based system for storing relationships between terms and documents.

*O2* — Efficient searching

Provide faster document retrieval compared with naive sequential document scanning.

*O3* — Apply DSA in a real system

Demonstrate practical usage of:

Hash maps
Vectors
Sets
Sorting algorithms
Priority queues
Tries
String-processing algorithms

*O4* — Implement relevance ranking

Rank search results according to their relevance to the user's query.

*O5* — Build a maintainable system

Organise the project into independent components such as:

Document Processing
        ↓
Indexing
        ↓
Query Processing
        ↓
Ranking
        ↓
Result Generation

*O6* — Measure performance

Compare different approaches using metrics such as:

Index construction time
Query execution time
Memory consumption
Number of documents indexed
Number of results returned

*O7* — Demonstrate software engineering practices

Use:

Git
GitHub
CMake
Unit testing
Documentation
Modular architecture
Continuous improvement


# 4. Functional Requirement

These describe what SearchLite must do. This distinction is important: functional requirements specify system behaviour, while non-functional requirements specify qualities or constraints on that behaviour.

*FR-01* — Document Loading

The system shall load text documents from a configured data directory.
Input:

data/
├── doc1.txt
├── doc2.txt
├── doc3.txt

Output:
A collection of documents available for indexing.

*FR-02* — Document Identification

Each document shall be assigned a unique identifier.
Example:

Document ID: 101
File: machine_learning.txt

*FR-03* — Text Tokenisation

The system shall divide document text into individual searchable terms.
Example:
"Machine learning is powerful"
↓
["machine", "learning", "is", "powerful"]

*FR-04* — Text Normalisation

The system shall normalise text before indexing.

Initial normalisation may include:
Converting text to lowercase.
Removing or handling punctuation.
Ignoring unnecessary whitespace.

Example:
"Machine, Learning!"
        ↓
"machine learning"

*FR-05* — Inverted Index Construction

The system shall construct an inverted index mapping terms to the documents containing those terms.
Example:
machine → [1, 4, 8]
learning → [1, 4]

*FR-06* — Query Processing

The system shall accept a text-based search query from the user.
Example:
machine learning

*FR-07* — Query Tokenisation

The system shall process the user's query using the same or compatible text-processing rules used during indexing.

*FR-08* — Document Retrieval

The system shall retrieve documents associated with the terms contained in the user's query.

*FR-09* — Relevance Scoring

The system shall calculate a relevance score for candidate documents.
The initial implementation may use a simple term-frequency-based approach.
A later version may implement TF-IDF.

*FR-10* — Result Ranking

The system shall rank matching documents according to their calculated relevance scores.
Higher-scoring documents should appear before lower-scoring documents.

*FR-11* — Top-K Results

The system should support returning the top K most relevant results.
Example:
Query: machine learning
K = 5

Results:
1. Document 14
2. Document 7
3. Document 21
4. Document 3
5. Document 19

*FR-12* — No-Result Handling

If no matching documents are found, the system shall return an appropriate message rather than failing.

Example:

No documents found for query: quantum networking

*FR-13* — Autocomplete (Future)

The system may provide query suggestions based on partially entered words.

Example:

mach
 ↓
machine
machine learning
machine vision

This functionality will potentially use a Trie.

*FR-14* — Text Normalisation

The system shall normalise document text before indexing by converting alphabetic characters to lowercase and handling punctuation appropriately.

*FR-15* — Tokenisation

The system shall divide normalised document text into individual searchable terms.

*FR-16* — Stop-Word Filtering

The system shall support filtering predefined stop words during document processing.

*FR-17* — Document Discovery

The system shall discover supported text documents from a configured data directory.

*FR-18* — Document Loading

The system shall read the contents of supported text documents.

*FR-19* — Document Identification

The system shall assign a unique identifier to each loaded document.

*FR-20* — Index Population

The system shall process each loaded document and populate the inverted index with the resulting searchable terms.


# 5. Non - Functional Requirements

These describe how well the system should operate rather than what functionality it provides. Typical non-functional requirements include performance, reliability, usability, security and scalability.

*NFR-01* — Performance

Search queries should execute efficiently even as the number of indexed documents increases.
We will measure actual performance rather than inventing unrealistic guarantees.
Metrics will include:

Indexing Time
Query Time
Memory Usage
Documents Indexed

*NFR-02* — Scalability

The architecture should allow the document collection to grow without requiring major changes to the core indexing and search logic.

*NFR-03* — Reliability

The system should handle:

Empty documents
Empty queries
Invalid document files
Queries with unknown terms
Duplicate terms

without crashing.

*NFR-04* — Maintainability

The system should follow a modular architecture.
For example:

DocumentManager
TextProcessor
InvertedIndex
QueryProcessor
Ranker
SearchEngine

Each component should have a clearly defined responsibility.

*NFR-05* — Portability

The core application should be designed to compile on commonly used desktop operating systems where the required C++ toolchain and CMake are available.

*NFR-06* — Testability

Important components shall have automated tests.
Testing will eventually cover:

Tokenisation
Normalisation
Index construction
Query processing
Ranking
Edge cases

*NFR-07* — Usability

The initial application should provide a simple command-line interface that allows a user to:

Load/index documents
        ↓
Enter query
        ↓
View ranked results

A graphical/web interface may be added later.

*NFR-08* — Security

The initial local application will not handle sensitive user information.
If the project later exposes a public API, additional requirements such as input validation, rate limiting and secure configuration will be considered.


# 6. Non-Functional Requirements

I recommend adding this section even though our original six-section plan didn't explicitly mention it. A proper SRS normally documents constraints and assumptions because they affect implementation and interpretation of the requirements.

**Constraints**
Initial implementation will use C++.
The project will use CMake as its build system.
The initial dataset will consist primarily of text documents.
Development will be performed by a single developer.
The first version will run locally.
Internet crawling is outside the initial scope.

**Assumptions**
Input documents are readable text files.
Documents are available locally.
The initial dataset is sufficiently small for experimentation.
Users provide text-based queries.
Search results are generated from the indexed document collection.

# 7. Architecture Diagram

*1* Initial Diagram

Documents
    │
    ▼
Document Loader
    │
    ▼
Text Processor
    │
    ├── Normalisation
    ├── Tokenisation
    └── Stop-word Filtering
    │
    ▼
Inverted Index

*2* Diagram processing with document loader
                  ┌─────────────┐
                  │  .txt Files │
                  └──────┬──────┘
                         │
                         ▼
                ┌─────────────────┐
                │ DocumentLoader  │
                └────────┬────────┘
                         │
                         ▼
                ┌─────────────────┐
                │ TextProcessor   │
                │                 │
                │ Normalisation   │
                │ Tokenisation    │
                │ Stop Words      │
                └────────┬────────┘
                         │
                         ▼
                ┌─────────────────┐
                │  InvertedIndex  │
                └────────┬────────┘
                         │
                         ▼
                  Term → Documents