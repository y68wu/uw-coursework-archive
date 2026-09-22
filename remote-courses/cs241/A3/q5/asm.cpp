#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint> 

// Prints an error to stderr with an "ERROR" line
void formatError(const std::string & message)
{
    std::cerr << "ERROR: " << message << std::endl;
}

/** Convert a string representation of a number to an unsigned integer.
 *  "0" => 0
 *  starting with "0x" => hexadecimal
 *  starting with "0"  => octal
 *  otherwise          => decimal (signed first, then cast)
 */
uint64_t stouint64(const std::string & s)
{
    if (s == "0") return 0;
    if (s.rfind("0x", 0) == 0) return static_cast<uint64_t>(std::stoll(s.substr(2), nullptr, 16));
    if (s.rfind("0", 0)  == 0) return static_cast<uint64_t>(std::stoll(s.substr(1), nullptr, 8));
    return static_cast<uint64_t>(std::stoll(s));  // allow negatives, then cast
}

enum TokenType {
    NONE,
    NEWLINE,
    DIRECTIVE,
    LABEL,
    ID,
    HEXINT,
    REG,
    DEC,
    COMMA,
    LPAREN,
    RPAREN,
};

class Token {
public:
    const TokenType type;
    const std::string lexeme;
    Token(TokenType type, std::string lexeme);
};

Token::Token(TokenType t, std::string l) : type(t), lexeme(l) {}

#define TOKEN_TYPE_PRINTER(t) case t: return #t
const char * tokenTypeString(TokenType t)
{
    switch(t)
    {
        TOKEN_TYPE_PRINTER(NONE);
        TOKEN_TYPE_PRINTER(NEWLINE);
        TOKEN_TYPE_PRINTER(DIRECTIVE);
        TOKEN_TYPE_PRINTER(LABEL);
        TOKEN_TYPE_PRINTER(ID);
        TOKEN_TYPE_PRINTER(HEXINT);
        TOKEN_TYPE_PRINTER(REG);
        TOKEN_TYPE_PRINTER(DEC);
        TOKEN_TYPE_PRINTER(COMMA);
        TOKEN_TYPE_PRINTER(LPAREN);
        TOKEN_TYPE_PRINTER(RPAREN);
    }
    return "";
}
#undef TOKEN_TYPE_PRINTER

#define TOKEN_TYPE_READER(s, t) if(s == #t) return t
TokenType stringToTokenType(const std::string & s)
{
    TOKEN_TYPE_READER(s, NONE);
    TOKEN_TYPE_READER(s, NEWLINE);
    TOKEN_TYPE_READER(s, DIRECTIVE);
    TOKEN_TYPE_READER(s, LABEL);
    TOKEN_TYPE_READER(s, ID);
    TOKEN_TYPE_READER(s, HEXINT);
    TOKEN_TYPE_READER(s, REG);
    TOKEN_TYPE_READER(s, DEC);
    TOKEN_TYPE_READER(s, COMMA);
    TOKEN_TYPE_READER(s, LPAREN);
    TOKEN_TYPE_READER(s, RPAREN);
    return NONE;
}
#undef TOKEN_TYPE_READER

std::ostream & operator<<(std::ostream & out, const Token token)
{
    out << tokenTypeString(token.type) << " " << token.lexeme;
    return out;
}

/** Entrypoint for the assembler.
 *  Reads tokenized MIPS from stdin or a file and prints the parameter list
 *  for each instruction/directive on stdout. Prints a symbol table to stderr.
 */
int main(int argc, char * argv[])
{
    if(argc > 2)
    {
        std::cerr << "Usage: asm [$FILE]" << std::endl;
        return 1;
    }

    std::ifstream fp;
    std::istream & in = (argc > 1 && std::string(argv[1]) != "-")
                        ? (fp.open(argv[1]), fp) : std::cin;
    if(!fp && argc > 1)
    {
        formatError("file '" + std::string(argv[1]) + "' not found!");
        return 1;
    }

    // ---- Read tokens ----
    std::vector<Token> tokens;
    while(!in.eof())
    {
        std::string line;
        std::getline(in, line);
        if(line.empty()) continue;

        std::string tokenType;
        std::string lexeme;
        std::stringstream ss(line);
        ss >> tokenType;
        if(tokenType != "NEWLINE") ss >> lexeme;

        tokens.push_back(Token(stringToTokenType(tokenType), lexeme));
    }

    // ---- Group tokens into logical lines ----
    std::vector<std::vector<Token>> lines;
    std::vector<Token> currentLine;
    for(const auto & token : tokens)
    {
        if(token.type == NEWLINE)
        {
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else
        {
            currentLine.push_back(token);
        }
    }
    if(!currentLine.empty()) lines.push_back(currentLine);

    // ---- Pass 1: build symbol table (absolute addresses) ----
    uint32_t currentAddress = 0;
    std::map<std::string, uint32_t> labelMap;
    std::vector<std::pair<std::string, uint32_t>> labelList;

    for(size_t i = 0; i < lines.size(); i++)
    {
        std::vector<Token> nonLabelTokens;
        for(auto & t : lines[i])
        {
            if(t.type == LABEL)
            {
                if(t.lexeme.empty() || t.lexeme.back() != ':')
                {
                    std::cerr << "ERROR" << std::endl;
                    return 1;
                }
                std::string labelName = t.lexeme.substr(0, t.lexeme.size()-1);
                if(labelMap.find(labelName) != labelMap.end())
                {
                    std::cerr << "ERROR" << std::endl;
                    return 1;
                }
                labelMap[labelName] = currentAddress;
                labelList.push_back({labelName, currentAddress});
            }
            else
            {
                nonLabelTokens.push_back(t);
            }
        }
        if(!nonLabelTokens.empty()) currentAddress += 4;
    }

    // ---- Pass 2: emit parameter lines ----
    uint32_t instrAddress = 0;

    for(size_t i = 0; i < lines.size(); i++)
    {
        std::vector<Token> nonLabelTokens;
        for(auto & t : lines[i])
            if(t.type != LABEL) nonLabelTokens.push_back(t);

        if(nonLabelTokens.empty()) { continue; }

        // Handle .word
        if(nonLabelTokens[0].type == DIRECTIVE && nonLabelTokens[0].lexeme == ".word")
        {
            std::vector<Token> operands;
            for(size_t j = 1; j < nonLabelTokens.size(); j++)
            {
                Token t = nonLabelTokens[j];
                if(t.type == COMMA || t.type == LPAREN || t.type == RPAREN) continue;
                operands.push_back(t);
            }
            if(operands.size() != 1)
            {
                std::cerr << "ERROR" << std::endl;
                return 1;
            }

            uint32_t number;
            if(operands[0].type == DEC || operands[0].type == HEXINT)
            {
                try {
                    uint64_t num = stouint64(operands[0].lexeme);
                    number = static_cast<uint32_t>(num);
                } catch(...) {
                    std::cerr << "ERROR" << std::endl;
                    return 1;
                }
            }
            else if(operands[0].type == ID)
            {
                if(labelMap.find(operands[0].lexeme) == labelMap.end())
                {
                    std::cerr << "ERROR" << std::endl;
                    return 1;
                }
                number = labelMap[operands[0].lexeme]; // absolute
            }
            else
            {
                std::cerr << "ERROR" << std::endl;
                return 1;
            }

            std::cout << "word " << number << " 0 0" << std::endl;
        }
        else
        {
            // Regular instruction line
            if(nonLabelTokens[0].type != ID)
            {
                std::cerr << "ERROR" << std::endl;
                return 1;
            }

            std::string mnemonic = nonLabelTokens[0].lexeme;
            std::vector<uint32_t> operandValues;

            for(size_t j = 1, opIndex = 0; j < nonLabelTokens.size(); j++)
            {
                Token t = nonLabelTokens[j];
                if(t.type == COMMA || t.type == LPAREN || t.type == RPAREN) continue;

                uint32_t value = 0;

                if(t.type == REG)
                {
                    if(t.lexeme.empty() || t.lexeme[0] != '$')
                    {
                        std::cerr << "ERROR" << std::endl;
                        return 1;
                    }
                    std::string regNum = t.lexeme.substr(1);
                    try {
                        uint64_t r = stouint64(regNum);
                        value = static_cast<uint32_t>(r);
                    } catch(...) {
                        std::cerr << "ERROR" << std::endl;
                        return 1;
                    }
                }
                else if(t.type == DEC || t.type == HEXINT)
                {
                    try {
                        uint64_t num = stouint64(t.lexeme);
                        value = static_cast<uint32_t>(num);
                    } catch(...) {
                        std::cerr << "ERROR" << std::endl;
                        return 1;
                    }
                }
                else if(t.type == ID)
                {
                    if(labelMap.find(t.lexeme) == labelMap.end())
                    {
                        std::cerr << "ERROR" << std::endl;
                        return 1;
                    }

                    if(mnemonic == "beq" && opIndex == 2)
                    {
                        uint32_t target = labelMap[t.lexeme];
                        int32_t offset = static_cast<int32_t>(target) - static_cast<int32_t>(instrAddress + 4);
                        offset /= 4;
                        uint32_t branchImmediate = static_cast<uint16_t>(offset);
                        value = branchImmediate;
                    }
                    else
                    {
                        value = labelMap[t.lexeme]; // absolute
                    }
                }
                else
                {
                    std::cerr << "ERROR" << std::endl;
                    return 1;
                }

                operandValues.push_back(value);
                opIndex++;
            }

            while(operandValues.size() < 3) operandValues.push_back(0);

            std::cout << mnemonic << " "
                      << operandValues[0] << " "
                      << operandValues[1] << " "
                      << operandValues[2] << std::endl;
        }

        instrAddress += 4;
    }

    // Print symbol table (stderr) in definition order
    for(auto & entry : labelList)
    {
        std::cerr << entry.first << " " << entry.second << std::endl;
    }

    return 0;
}
