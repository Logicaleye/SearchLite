### FR-09: Relevance Scoring

The system shall calculate a relevance score for matching documents using Term Frequency (TF).

The score shall represent the combined frequency of the query terms within the document.

### FR-10: Result Ranking

The system shall rank matching documents in descending order of relevance score.

If two documents have equal scores, the document with the smaller document ID shall be ranked first.

### FR-11: Top-K Results

The system shall support limiting the number of returned search results to the top K most relevant documents.

This requirement is planned for a future implementation.