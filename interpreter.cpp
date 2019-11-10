
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
                //std::cout << "\tEnd of line :(\n";
                return Token(Term::END, "");
            }
            if (first == '\n') {
                return Token(Term::END, "");
            }
        }
        while (first == ' ' || first == '\n' || first == '\r' || first == '\t');
        
        if (first == '+') {
            //std::cout << "\tPlus found\n";
            return Token(Term::PM, "+");
        }
        
        if (first == '-') {
            //std::cout << "\tMinus found\n";
            return Token(Term::PM, "-");
        }

        if (first == '*') {
            //std::cout << "\tMul found\n";
            return Token(Term::MD, "*");
        }

        if (first == '/') {
            //std::cout << "\tDiv found\n";
            return Token(Term::MD, "/");
        }

        if (first == '(') {
            //std::cout << "\tLB found\n";
            return Token(Term::LB, "");
        }

        if (first == ')') {
            //std::cout << "\tRB found\n";
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

            //std::cout << "\t" << token_str << " digit found\n";
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
        auto result = pmSeries(lexer);
        std::cout << "I computed this: " << result << '\n';
    }

    float pmSeries(Lexer& lexer) {
        //mdSeries
        //next is plus or minus?
        //yes - repeat
        //no - exit
        float plus_minus = 0;
        
        Token action = Token(Term::PM, "+");

        pm_again:
        //std::cout << "Plus-minus looping\n";

        if (action.attr == "+")
            plus_minus += mdSeries(lexer);
        if (action.attr == "-")
            plus_minus -= mdSeries(lexer);

        if (lexer.peek().term == Term::PM) {
            action = lexer.next();
            goto pm_again;
        }
        //std::cout << "Plus-minus exit\n";

        return plus_minus;
    }

    float mdSeries(Lexer& lexer) {
        //is digit? use digit
        //is lb? use braces
        //else syntax error
        //next is mul / div?
        //yes - repeat
        //no - exiti
        
        float sign = 1;
        
         
        unary_again:

        if (lexer.peek().term == Term::PM) {
            //todo: unary operation
            auto current_unary = lexer.next();
            if (current_unary.attr == "-") {
                sign = -sign;
            }
            goto unary_again;
        }
         
        
        float mul_div = 1;
        
        Token action = Token(Term::MD, "*");

        md_again:

        //std::cout << "Mul-div looping\n";
        
        if (lexer.peek().term == Term::DIG) {
            //accept digit
            if (action.attr == "*") {
                mul_div = sign * mul_div * std::stof(lexer.next().attr);
                sign = 1;
            }
            else {
                mul_div = sign * mul_div / std::stof(lexer.next().attr);
                sign = 1;
            }
        }
        else if (lexer.peek().term == Term::LB) {
            //accept subexpr
            if (action.attr == "*")
                mul_div = mul_div * braces(lexer);
            else
                mul_div = mul_div / braces(lexer);
        }
        else {
            //todo: syntax error
        }

        if (lexer.peek().term == Term::MD) {
            action = lexer.next();
            goto md_again;
        }
        //std::cout << "Mul-div exit\n";

        return mul_div;
    }

    float braces(Lexer& lexer) {
        //assert lb
        //std::cout << "Braces assertion\n";
        if (lexer.peek().term == Term::LB) {
            lexer.next();
        }
        else {
            //todo: syntax error
        }
        //pmSeries
        float in_braces = pmSeries(lexer);

        //assert rb
        if (lexer.peek().term == Term::RB) {
            lexer.next();
        }
        else {
            //todo: syntax error
        }
        //std::cout << "Braces exit\n";

        return in_braces;
    }

};


int main() {
    Lexer myLexer{};
    Parser myParser;
    myParser.parse(myLexer);
    return 0;
}
