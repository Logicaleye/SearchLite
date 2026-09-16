#include "BooleanParser.h"

#include <cctype>

std::vector<BooleanToken>
BooleanParser::tokenize(
    const std::string& query
) const {

    std::vector<BooleanToken> tokens;

    std::string current;
    bool insideQuotes = false;

    auto addTerm = [&]() {

        if (current.empty()) {
            return;
        }

        std::string upper;

        for (char c : current) {
            upper += static_cast<char>(
                std::toupper(
                    static_cast<unsigned char>(c)
                )
            );
        }

        if (upper == "AND") {

            tokens.push_back({
                TokenType::AND,
                current
            });

        } else if (upper == "OR") {

            tokens.push_back({
                TokenType::OR,
                current
            });

        } else if (upper == "NOT") {

            tokens.push_back({
                TokenType::NOT,
                current
            });

        } else {

            tokens.push_back({
                TokenType::TERM,
                current
            });
        }

        current.clear();
    };

    for (char c : query) {

        // Start or end a quoted phrase
        if (c == '"') {

            if (insideQuotes) {

                if (!current.empty()) {

                    tokens.push_back({
                        TokenType::PHRASE,
                        current
                    });

                    current.clear();
                }

                insideQuotes = false;

            } else {

                addTerm();

                insideQuotes = true;
            }

            continue;
        }

        // Everything inside quotes belongs
        // to the same phrase.
        if (insideQuotes) {

            current += c;
            continue;
        }

        if (std::isspace(
                static_cast<unsigned char>(c)
            )) {

            addTerm();

        } else if (c == '(') {

            addTerm();

            tokens.push_back({
                TokenType::LEFT_PAREN,
                "("
            });

        } else if (c == ')') {

            addTerm();

            tokens.push_back({
                TokenType::RIGHT_PAREN,
                ")"
            });

        } else {

            current += c;
        }
    }

    // An unclosed quote is handled later
    // by parse(), so don't create a PHRASE here.
    if (insideQuotes) {

        return {};
    }

    addTerm();

    return tokens;
}


// expression → OR expression
std::unique_ptr<BooleanNode>
BooleanParser::parseOr() {

    auto left = parseAnd();

    if (!left) {
        return nullptr;
    }
    while (
    current < tokens.size() &&
    tokens[current].type == TokenType::OR
) {
    ++current;

    auto right = parseAnd();

    if (!right) {
            return nullptr;
    }
    auto node =
        std::make_unique<BooleanNode>(
            BooleanNode::Type::OR
    );

    node->left = std::move(left);
    node->right = std::move(right);

    left = std::move(node);
}

return left;
}


// AND has higher precedence than OR
std::unique_ptr<BooleanNode>
BooleanParser::parseAnd() {

    auto left = parseUnary();

    if (!left) {
        return nullptr;
    }
    while (
        current < tokens.size() &&
        (
            tokens[current].type == TokenType::AND ||
            tokens[current].type == TokenType::NOT
        )
    ) {

        TokenType op = tokens[current].type;
        ++current;

        auto right = parseUnary();

        if (!right) {
            return nullptr;
        }

        if (op == TokenType::NOT) {

            auto notNode =
                std::make_unique<BooleanNode>(
                    BooleanNode::Type::NOT
                );

            notNode->left = std::move(right);

            right = std::move(notNode);
        }

        auto node =
            std::make_unique<BooleanNode>(
                BooleanNode::Type::AND
            );

        node->left = std::move(left);
        node->right = std::move(right);

        left = std::move(node);
    }

    return left;
}


// NOT has the highest precedence
std::unique_ptr<BooleanNode>
BooleanParser::parseUnary() {

    if (
        current < tokens.size() &&
        tokens[current].type == TokenType::NOT
    ) {

        ++current;

        auto node =
            std::make_unique<BooleanNode>(
                BooleanNode::Type::NOT
            );

        node->left = parseUnary();

        return node;
    }

    return parsePrimary();
}


// primary → TERM | '(' expression ')'
std::unique_ptr<BooleanNode>
BooleanParser::parsePrimary() {

    if (current >= tokens.size()) {
        return nullptr;
    }

    if (tokens[current].type == TokenType::TERM) {

        auto node =
            std::make_unique<BooleanNode>(
                BooleanNode::Type::TERM,
                tokens[current].value
            );

        ++current;

        return node;
    }

    if (
    tokens[current].type ==
    TokenType::PHRASE
    ) {

    auto node =
        std::make_unique<BooleanNode>(
            BooleanNode::Type::PHRASE,
            tokens[current].value
        );

    ++current;

    return node;
    }

    if (
        tokens[current].type ==
        TokenType::LEFT_PAREN
    ) {

        ++current;

        auto node = parseOr();

        if (!node) {
            return nullptr;
        }

        if (
            current >= tokens.size() ||
            tokens[current].type !=
            TokenType::RIGHT_PAREN
        ) {
            return nullptr;
        }

        ++current;

        return node;
    }

    return nullptr;
}



std::unique_ptr<BooleanNode>
BooleanParser::parse(
    const std::string& query
) {

    tokens = tokenize(query);
    current = 0;

    if (tokens.empty()) {
        return nullptr;
    }

    auto root = parseOr();

    if (current != tokens.size()) {
        return nullptr;
    }

    return root;
}

bool BooleanParser::isBooleanQuery(
    const std::string& query
) const {

    auto queryTokens = tokenize(query);

    for (const auto& token : queryTokens) {

        if (
            token.type == TokenType::AND ||
            token.type == TokenType::OR ||
            token.type == TokenType::NOT ||
            token.type == TokenType::LEFT_PAREN ||
            token.type == TokenType::RIGHT_PAREN
        ) {
            return true;
        }
    }

    return false;
}