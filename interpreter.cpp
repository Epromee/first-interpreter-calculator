
#include <iostream>
#include <string>

enum Term {
    END,
    PM,
    MD,
    DIGIT,
    LBRACE,
    RBRACE
};

struct Token {
    
    Term term;
    std::string attr;

};

struct Lexer {
    
    Lexer () {}
    
    bool next() {
        std::string token_str = "";
       
        
        char first;
        do {
            if (!(std::cin >> first)) {
                std::cout << "End of line :(\n";
                return false;
            }
        }
        while (first == ' ');
        
        if (first == '+') {
            std::cout << "Plus found\n";
            return true;
        }
        
        if (first == '-') {
            std::cout << "Minus found\n";
            return true;
        }

        if (first == '*') {
            std::cout << "Mul found\n";
            return true;
        }

        if (first == '/') {
            std::cout << "Div found\n";
            return true;
        }

        if (first == '(') {
            std::cout << "LB found\n";
            return true;
        }

        if (first == ')') {
            std::cout << "RB found\n";
            return true;
        }

        if (first >= '0' && first <= '9') {
            token_str = token_str + first;
            int next = std::cin.peek();
            while (next >= '0' && next <= '9') {
                next = std::cin.get();
                token_str = token_str + (char)next;
                next = std::cin.peek();
            }

            std::cout << token_str << " found\n";
        }
        
    }
    
};


int main() {
    Lexer myLexer{};
    while (myLexer.next()) {};
    return 0;
}
