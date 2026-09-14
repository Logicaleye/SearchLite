#ifndef BOOLEAN_PARSER_H
#define BOOLEAN_PARSER_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

enum class TokenType {
    TERM,
    AND,
    OR,
    NOT,
    LEFT_PAREN,
    RIGHT_PAREN
};

struct BooleanToken {
    TokenType type;
    std::string value;
};

struct BooleanNode {
    enum class Type {
        TERM,
        AND,
        OR,
        NOT
    };

    Type type;
    std::string value;

    std::unique_ptr<BooleanNode> left;
    std::unique_ptr<BooleanNode> right;

    BooleanNode(
        Type type,
        const std::string& value = ""
    )
        : type(type),
          value(value) {
    }
};

class BooleanParser {
private:
    std::vector<BooleanToken> tokens;
    std::size_t current = 0;

    std::unique_ptr<BooleanNode> parseOr();
    std::unique_ptr<BooleanNode> parseAnd();
    std::unique_ptr<BooleanNode> parseUnary();
    std::unique_ptr<BooleanNode> parsePrimary();

public:
    std::vector<BooleanToken> tokenize(
        const std::string& query
    ) const;

    std::unique_ptr<BooleanNode> parse(
        const std::string& query
    );

    bool isBooleanQuery(
    const std::string& query
    ) const;
};

#endif