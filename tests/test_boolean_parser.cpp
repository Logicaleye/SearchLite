#include "BooleanParser.h"

#include <cassert>
#include <iostream>

int main() {

    BooleanParser parser;

    auto tokens =
        parser.tokenize(
            "machine AND (learning OR algorithms)"
        );

    assert(tokens.size() == 7);

    assert(tokens[0].type == TokenType::TERM);
    assert(tokens[0].value == "machine");

    assert(tokens[1].type == TokenType::AND);

    assert(tokens[2].type == TokenType::LEFT_PAREN);

    assert(tokens[3].type == TokenType::TERM);
    assert(tokens[3].value == "learning");

    assert(tokens[4].type == TokenType::OR);

    assert(tokens[5].type == TokenType::TERM);
    assert(tokens[5].value == "algorithms");

    assert(tokens[6].type == TokenType::RIGHT_PAREN);

    // Case-insensitive operators

    auto caseTokens =
        parser.tokenize(
            "machine and learning"
        );

    assert(caseTokens.size() == 3);
    assert(caseTokens[1].type == TokenType::AND);

    // Parentheses without spaces

    auto compactTokens =
        parser.tokenize(
            "(machine OR learning)"
        );

    assert(compactTokens.size() == 5);
    assert(
        compactTokens[0].type ==
        TokenType::LEFT_PAREN
    );
    assert(
        compactTokens[4].type ==
        TokenType::RIGHT_PAREN
    );

    // --------------------------------------------
// Parser tests
// --------------------------------------------

auto simple =
    parser.parse("machine AND learning");

assert(simple != nullptr);
assert(
    simple->type ==
    BooleanNode::Type::AND
);

assert(simple->left != nullptr);
assert(simple->right != nullptr);

assert(
    simple->left->value == "machine"
);

assert(
    simple->right->value == "learning"
);


// --------------------------------------------
// Operator precedence
// machine OR learning AND algorithms
// --------------------------------------------

auto precedence =
    parser.parse(
        "machine OR learning AND algorithms"
    );

assert(precedence != nullptr);

assert(
    precedence->type ==
    BooleanNode::Type::OR
);

assert(
    precedence->left->value == "machine"
);

assert(
    precedence->right->type ==
    BooleanNode::Type::AND
);


// --------------------------------------------
// Parentheses
// (machine OR learning) AND algorithms
// --------------------------------------------

auto parentheses =
    parser.parse(
        "(machine OR learning) AND algorithms"
    );

assert(parentheses != nullptr);

assert(
    parentheses->type ==
    BooleanNode::Type::AND
);

assert(
    parentheses->left->type ==
    BooleanNode::Type::OR
);

assert(
    parentheses->right->value ==
    "algorithms"
);


// --------------------------------------------
// NOT
// --------------------------------------------

auto notQuery =
    parser.parse("NOT machine");

assert(notQuery != nullptr);

assert(
    notQuery->type ==
    BooleanNode::Type::NOT
);

assert(
    notQuery->left->value ==
    "machine"
);

    std::cout
        << "All Boolean parser tests passed!\n";

    return 0;
}