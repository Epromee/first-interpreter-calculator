
#include <iostream>
#include <string>
#include <stdexcept>
#include <ios>

/*
    Author: Egor Promyshlennikov
    E-mail: eprom1234@gmail.com
    Date: 24 Nov 2019

    ------------------------------------------------------------------------

    This is my first attempt to create a simple working interpreter.
    The program calculates arithmetical expression with floating point.
    For example:
    (2 + 3 * 5) / (6.3 - 7.155)

    This is just a prototype for me to make the shit work.
    I'm not going to neither fix bugs nor add new features there.
    
    If you think this code is an inefficient shit - that is, for sure.

    The next my target is a new better interpreter.
    Step by step, I will have designed my own full-fledged compiler.
    
    ------------------------------------------------------------------------

    Oh, license? This is a pirate code, arrr. I write it, you pirate it :)
*/

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

    Lexer () : cashed(Term::END, ""), has_cashed(false) { }
    
    Token next() {

        if (has_cashed) {
            has_cashed = false;
            return cashed;
        }

        std::string token_str = "";
       
        
        char first;
        do {
            first = std::cin.get();
            if (std::cin.eof() || first == '\n') {
                return Token(Term::END, "");
            }
        }
        while (first == ' ' || first == '\r'|| first == '\t');

        if (first == '+') {
            return Token(Term::PM, "+");
        }
        
        if (first == '-') {
            return Token(Term::PM, "-");
        }

        if (first == '*') {
            return Token(Term::MD, "*");
        }

        if (first == '/') {
            return Token(Term::MD, "/");
        }

        if (first == '(') {
            return Token(Term::LB, "");
        }

        if (first == ')') {
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

            /* Can capture floating point number dot, but only once */
            if ((char) next == '.') {
                token_str = token_str + (char)next;
                std::cin.get();
                next = std::cin.peek();

                /* One digit must definitely exist after floating point */
                if (next >= '0' && next <= '9') {
                   token_str = token_str + (char) next;
                   std::cin.get();
                }
                else {
                    throw std::runtime_error("[Lexer] Digit was expected after floating point");
                }

                /* Several more digits */
                next = std::cin.peek();
                while (next >= '0' && next <= '9') {
                    next = std::cin.get();
                    token_str = token_str + (char)next;
                    next = std::cin.peek();
                }
            }

            return Token(Term::DIG, token_str);
        }

        if (first == '.') {
            throw std::runtime_error("[Lexer] Dot really? Not this time dude!");
        }

        throw std::runtime_error("[Lexer] Unknown character, can't recognize");
        
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
        
        float result;
        
        try {
            result = pmSeries(lexer);
            std::cout << "\033[92;1mYour result: " << std::defaultfloat << result << "\033[0m\n";
            if (lexer.next().term != Term::END) {
                std::cout << "\033[91;1m" << "Odd characters after evaluation\n" << "\033[0m\n";
            }
        }
        catch (std::runtime_error e) {
            std::cout << "\033[91;1m" << e.what() << "\033[0m\n";
        }

        
    }

    float pmSeries(Lexer& lexer) {
        float plus_minus = 0;
        
        Token action = Token(Term::PM, "+");

        while (true) {

            if (action.attr == "+")
                plus_minus += mdSeries(lexer);
            if (action.attr == "-")
                plus_minus -= mdSeries(lexer);

            if (lexer.peek().term == Term::PM) {
                action = lexer.next();
            }
            else
                break;
        }

        return plus_minus;
    }

    float mdSeries(Lexer& lexer) {
        
        float sign = 1;
        
        while (true) {
            if (lexer.peek().term == Term::PM) {
                auto current_unary = lexer.next();
                if (current_unary.attr == "-") {
                    sign = -sign;
                }
            }
            else
                break;
        }
         
        
        float mul_div = 1;
        
        Token action = Token(Term::MD, "*");

        while (true) {
            
            if (lexer.peek().term == Term::DIG) {
                if (action.attr == "*") {
                    mul_div = sign * mul_div * std::stof(lexer.next().attr);
                    sign = 1;
                }
                else {
                    auto del = std::stof(lexer.next().attr);
                    if (del != 0) {
                        mul_div = sign * mul_div / del;
                        sign = 1;
                    }
                    else
                        throw std::runtime_error("[Parser] Attempted to divide by zero");
                }
            }
            else if (lexer.peek().term == Term::LB) {
                if (action.attr == "*") {
                    mul_div = sign * mul_div * braces(lexer);
                    sign = 1;
                }
                else {
                    auto del = braces(lexer);
                    if (del != 0) {
                        mul_div = sign * mul_div / del;
                        sign = 1;
                    }
                    else
                        throw std::runtime_error("[Parser] Attempted to divide by zero");
                }
            }
            else {
                throw std::runtime_error("[Parser] Digit or braced expression was expected");
            }

            if (lexer.peek().term == Term::MD) {
                action = lexer.next();
            }
            else
                break;
        }

        return mul_div;
    }

    float braces(Lexer& lexer) {
        if (lexer.peek().term == Term::LB) {
            lexer.next();
        }
        else {
            throw std::runtime_error("[Parser] Left brace was expected");
        }
        float in_braces = pmSeries(lexer);

        if (lexer.peek().term == Term::RB) {
            lexer.next();
        }
        else {
            throw std::runtime_error("[Parser] Right brace was expected");
        }

        return in_braces;
    }

};


int main() {
    Lexer myLexer{};
    Parser myParser;
    myParser.parse(myLexer);
    return 0;
}
