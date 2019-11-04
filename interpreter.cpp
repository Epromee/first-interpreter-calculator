
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
                std::cout << "\tEnd of line :(\n";
                return Token(Term::END, "");
            }
        }
        while (first == ' ' || first == '\n' || first == '\r' || first == '\t');
        
        if (first == '+') {
            std::cout << "\tPlus found\n";
            return Token(Term::PM, "+");
        }
        
        if (first == '-') {
            std::cout << "\tMinus found\n";
            return Token(Term::PM, "-");
        }

        if (first == '*') {
            std::cout << "\tMul found\n";
            return Token(Term::MD, "*");
        }

        if (first == '/') {
            std::cout << "\tDiv found\n";
            return Token(Term::MD, "/");
        }

        if (first == '(') {
            std::cout << "\tLB found\n";
            return Token(Term::LB, "");
        }

        if (first == ')') {
            std::cout << "\tRB found\n";
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

            std::cout << "\t" << token_str << " digit found\n";
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
        pmSeries(lexer);
    }

    void pmSeries(Lexer& lexer) {
        //mdSeries
        //next is plus or minus?
        //yes - repeat
        //no - exit
        pm_again:
        std::cout << "Plus-minus looping\n";
        mdSeries(lexer);
        if (lexer.peek().term == Term::PM) {
            unary_again_pm:
            lexer.next();
            if (lexer.peek().term == Term::PM) {
                goto unary_again_pm;
            }
            goto pm_again;
        }
        std::cout << "Plus-minus exit\n";
    }

    void mdSeries(Lexer& lexer) {
        //is digit? use digit
        //is lb? use braces
        //else syntax error
        //next is mul / div?
        //yes - repeat
        //no - exiti
        

        if (lexer.peek().term == Term::PM) {
            //todo: unary operation
            unary_again:
            lexer.next();
            std::cout << "Unary caught\n";
            if (lexer.peek().term == Term::PM) {
                goto unary_again;
            }
        }

        md_again:

        std::cout << "Mul-div looping\n";
        
        if (lexer.peek().term == Term::DIG) {
            //accept digit
            lexer.next();
        }
        else if (lexer.peek().term == Term::LB) {
            //accept subexpr
            braces(lexer);
        }
        else {
            //todo: syntax error
        }

        if (lexer.peek().term == Term::MD) {
            lexer.next();
            goto md_again;
        }
        std::cout << "Mul-div exit\n";
    }

    void braces(Lexer& lexer) {
        //assert lb
        std::cout << "Braces assertion\n";
        if (lexer.peek().term == Term::LB) {
            lexer.next();
        }
        else {
            //todo: syntax error
        }
        //pmSeries
        pmSeries(lexer);

        //assert rb
        if (lexer.peek().term == Term::RB) {
            lexer.next();
        }
        else {
            //todo: syntax error
        }
        std::cout << "Braces exit\n";
    }

};


int main() {
    Lexer myLexer{};
    Parser myParser;
    myParser.parse(myLexer);
    return 0;
}
