
#include <iostream>
#include <string>

enum Term {
    END,
    PM,
    MD,
    DIG,
    LB,
    RB
};

struct Token {
    
    Term term;
    std::string attr;

    Token(const Term lterm, const std::string lattr) : term(lterm), attr(lattr) { }
    
    operator bool() {
        return term != Term::END;
    }
};

struct Lexer {
    
    Token cashed;
    bool has_cashed;

    Lexer () : cashed(Term::END, ""), has_cashed(false) {}
    
    Token next() {

        if (has_cashed) {
            has_cashed = false;
            return cashed;
        }

        std::string token_str = "";
       
        
        char first;
        do {
            if (!(std::cin >> first)) {
                std::cout << "End of line :(\n";
                return Token(Term::END, "");
            }
        }
        while (first == ' ' || first == '\n' || first == '\r' || first == '\t');
        
        if (first == '+') {
            std::cout << "Plus found\n";
            return Token(Term::PM, "+");
        }
        
        if (first == '-') {
            std::cout << "Minus found\n";
            return Token(Term::PM, "-");
        }

        if (first == '*') {
            std::cout << "Mul found\n";
            return Token(Term::MD, "*");
        }

        if (first == '/') {
            std::cout << "Div found\n";
            return Token(Term::MD, "/");
        }

        if (first == '(') {
            std::cout << "LB found\n";
            return Token(Term::LB, "");
        }

        if (first == ')') {
            std::cout << "RB found\n";
            return Token(Term::RB, "");
        }

        if (first >= '0' && first <= '9') {
            token_str = token_str + first;
            int next = std::cin.peek();
            while (next >= '0' && next <= '9') {
                next = std::cin.get();
                token_str = token_str + (char)next;
                next = std::cin.peek();
            }

            std::cout << token_str << " digit found\n";
            return Token(Term::DIG, token_str);
        }
        
    }

    Token peek() {
        if (!has_cashed) {
            cashed = next();
            has_cashed = true;
        }
        return cashed;
    }
          
};

struct Parser {
    
    Parser() {};

    void parse(Lexer& lexer) {
        //while (lexer.next()) {};
        
    }

};


int main() {
    Lexer myLexer{};
    Parser myParser;
    myParser.parse(myLexer);
    return 0;
}
