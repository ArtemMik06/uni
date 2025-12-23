#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>

enum TokenType {
    PREPROCESSOR,
    COMMENT,
    STRING,
    NUMBER_HEX,
    NUMBER_FLOAT,
    NUMBER_INT,
    KEYWORD,
    OPERATOR,
    DELIMITER,
    IDENTIFIER,
    UNKNOWN
};

struct LexerRule {
    std::regex pattern;
    TokenType type;
    std::string typeName;
};

std::vector<LexerRule> getRules() {
    std::vector<LexerRule> rules;

    rules.push_back({std::regex(R"(^\s*#.*)"), PREPROCESSOR, "PREPROCESSOR"});

    rules.push_back({std::regex(R"(!.*)"), COMMENT, "COMMENT"});

    rules.push_back({std::regex(R"(\"([^\"\\]|\\.)*\"|'([^'\\]|\\.)*')"), STRING, "STRING_LITERAL"});

    rules.push_back({std::regex(R"((Z|B|O)['"][0-9A-F]+['"])", std::regex_constants::icase), NUMBER_HEX, "NUMBER_HEX/BIN/OCT"});

    rules.push_back({std::regex(R"(\b[0-9]+\.[0-9]*([eE][-+]?[0-9]+)?|\b[0-9]+[eE][-+]?[0-9]+)"), NUMBER_FLOAT, "NUMBER_FLOAT"});

    rules.push_back({std::regex(R"(\b[0-9]+\b)"), NUMBER_INT, "NUMBER_INT"});

    rules.push_back({std::regex(R"((\*\*|\/\/|==|\/=|[+\-*/=<>])|(\.(AND|OR|NOT|EQ|NE|GT|LT|GE|LE|TRUE|FALSE)\.))", std::regex_constants::icase), OPERATOR, "OPERATOR"});

    rules.push_back({std::regex(R"([(),:;%])"), DELIMITER, "DELIMITER"});

    std::string keywords = R"(\b(PROGRAM|END|IMPLICIT|NONE|INTEGER|REAL|CHARACTER|LOGICAL|COMPLEX|PARAMETER|IF|THEN|ELSE|ELSEIF|DO|WHILE|CALL|SUBROUTINE|FUNCTION|MODULE|USE|CONTAINS|PRINT|WRITE|READ|ALLOCATABLE|ALLOCATE|DEALLOCATE|TYPE|INTERFACE)\b)";
    rules.push_back({std::regex(keywords, std::regex_constants::icase), KEYWORD, "KEYWORD"});

    rules.push_back({std::regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"), IDENTIFIER, "IDENTIFIER"});

    return rules;
}

void analyzeLine(const std::string& line, int lineNum, const std::vector<LexerRule>& rules) {
    std::string::const_iterator searchStart(line.cbegin());

    while (searchStart != line.cend()) {
        if (std::isspace(*searchStart)) {
            ++searchStart;
            continue;
        }

        bool matched = false;
        std::smatch match;

        for (const auto& rule : rules) {
            if (std::regex_search(searchStart, line.cend(), match, rule.pattern, std::regex_constants::match_continuous)) {

                std::string lexeme = match.str();

                std::cout << "Line " << std::setw(3) << lineNum << ": "
                          << std::setw(20) << std::left << rule.typeName
                          << " | " << lexeme << std::endl;

                searchStart += lexeme.length();
                matched = true;
                break;
            }
        }

        if (!matched) {
            std::cout << "Line " << std::setw(3) << lineNum << ": "
                      << "\033[1;31mUNKNOWN_TOKEN     \033[0m"
                      << " | " << *searchStart << std::endl;
            ++searchStart;
        }
    }
}

int main() {
    std::string filename;
    std::cout << "Enter Fortran file name: ";
    std::cin >> filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return 1;
    }

    std::vector<LexerRule> rules = getRules();
    std::string line;
    int lineNum = 1;

    std::cout << "---------------------------------------------------\n";
    std::cout << " LOC  | TYPE                 | LEXEME\n";
    std::cout << "---------------------------------------------------\n";

    while (std::getline(file, line)) {
        analyzeLine(line, lineNum++, rules);
    }

    file.close();
    return 0;
}