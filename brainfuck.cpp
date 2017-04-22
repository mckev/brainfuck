// Brainfuck interpreter
// How to compile and run:
//    $ rm -f brainfuck; g++ -Wall -std=c++11 -o brainfuck brainfuck.cpp; ./brainfuck


#include <iostream>
#include <stack>
#include <string>
#include <vector>


class CPU {

private:
	// code
	std::string code;
	unsigned int code_ptr;
	std::stack<unsigned int> loop_ptrs;
	// memory (cells)
	typedef int cell_t;
	std::vector<cell_t> cells;
	unsigned int cell_ptr;

public:
	CPU() {
		reset();
	}

	~CPU() {}

	void reset() {
		code = {};
		code_ptr = 0;
		loop_ptrs = {};
		cells = std::vector<cell_t>(30000);
		cell_ptr = 0;
	}

	void set_code(const std::string& _code) {
		reset();
		code = _code;
	}

	void run() {
		code_ptr = 0;
		while (code_ptr < code.length()) {
			switch (code.at(code_ptr)) {
				case '>':
					cell_ptr++;
					break;
				case '<':
					cell_ptr--;
					break;
				case '+':
					cells.at(cell_ptr)++;
					break;
				case '-':
					cells.at(cell_ptr)--;
					break;
				case '.':
					std::cout << (char) cells.at(cell_ptr);
					break;
				case ',':
					cells.at(cell_ptr) = std::getc(stdin);
					break;
				case '[':
					if (cells.at(cell_ptr) > 0) {
						loop_ptrs.push(code_ptr);
					} else {
						// Special case: If the cells[cell_ptr] is zero, then go to the next matching ']'
						int num_bracket = 0;
						do {
							switch (code.at(code_ptr)) {
								case '[':
									num_bracket++;
									break;
								case ']':
									num_bracket--;
									break;
							}
							code_ptr++;
						} while (num_bracket > 0);
						code_ptr--;
					}
					break;
				case ']':
					if (cells.at(cell_ptr) > 0) {
						// Make a loop
						code_ptr = loop_ptrs.top();
					} else {
						// We have reached the end of the loop
						loop_ptrs.pop();
					}
					break;
				default:
					// Ignore unrecognized characters
					break;
			}
			code_ptr++;
		}
	}
};


int main() {
	CPU cpu;

	// Ref: https://en.wikipedia.org/wiki/Brainfuck
	cpu.set_code("++++++++++ ++++++++++ ++++++++++ ++++++++++ ++++++++++ ++++++++++ +++++ .");
	cpu.run();
	std::cout << std::endl;
	cpu.set_code("++++++ [ > ++++++++++ < - ] > +++++ .");
	cpu.run();
	std::cout << std::endl;
	cpu.set_code("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.");
	cpu.run();

	// Ref: https://esolangs.org/wiki/Brainfuck
	std::string code_determine_cell_type = \
		"This program outputs the cell width of the interpreter                     "\
		"Calculate the value 256 and test if it's zero                              "\
		"If the interpreter errors on overflow this is where it'll happen           "\
		"++++++++[>++++++++<-]>[<++++>-]                                            "\
		"+<[>-<                                                                     "\
		"   Not zero so multiply by 256 again to get 65536                          "\
		"   [>++++<-]>[<++++++++>-]<[>++++++++<-]                                   "\
		"   +>[>                                                                    "\
		"      # Print '32'                                                         "\
		"      ++++++++++[>+++++<-]>+.-.[-]<                                        "\
		"   <[-]<->] <[>>                                                           "\
		"      # Print '16'                                                         "\
		"      +++++++[>+++++++<-]>.+++++.[-]<                                      "\
		"<<-]] >[>                                                                  "\
		"   # Print '8'                                                             "\
		"   ++++++++[>+++++++<-]>.[-]<                                              "\
		"<-]<                                                                       "\
		"# Print ' bit cells\n'                                                     "\
		"+++++++++++[>+++>+++++++++>+++++++++>+<<<<-]>-.>-.+++++++.+++++++++++.<.   "\
		">>.++.+++++++..<-.>>-                                                      "\
		"Clean up used cells                                                        "\
		"[[-]<]                                                                     ";
	cpu.set_code(code_determine_cell_type);
	cpu.run();
	std::cout << std::endl;

	// Ref: http://progopedia.com/example/factorial/18/
	std::string code_factorial = \
		"+++++++++++++++++++++++++++++++++                       c1v33 : ASCII code of !                                                        "\
		">++++++++++++++++++++++++++++++                                                                                                        "\
		"+++++++++++++++++++++++++++++++                         c2v61 : ASCII code of =                                                        "\
		">++++++++++                                             c3v10 : ASCII code of EOL                                                      "\
		">+++++++                                                c4v7  : quantity of numbers to be calculated                                   "\
		">                                                       c5v0  : current number (one digit)                                             "\
		">+                                                      c6v1  : current value of factorial (up to three digits)                        "\
		"<<                                                      c4    : loop counter                                                           "\
		"[                                                       block : loop to print one line and calculate next                              "\
		">++++++++++++++++++++++++++++++++++++++++++++++++.      c5    : print current number                                                   "\
		"------------------------------------------------        c5    : back from ASCII to number                                              "\
		"<<<<.-.>.<.+                                            c1    : print !_=_                                                             "\
		">>>>>                                                   block : print c6 (preserve it)                                                 "\
		">                                                       c7v0  : service zero                                                           "\
		">++++++++++                                             c8v10 : divizor                                                                "\
		"<<                                                      c6    : back to dividend                                                       "\
		"[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]                       c6v0  : divmod algo borrowed from esolangs; results in 0 n d_n%d n%d n/d       "\
		">[<+>-]                                                 c6    : move dividend back to c6 and clear c7                                  "\
		">[-]                                                    c8v0  : clear c8                                                               "\
		">>                                                      block : c10 can have two digits; divide it by ten again                        "\
		">++++++++++                                             c11v10: divizor                                                                "\
		"<                                                       c10   : back to dividend                                                       "\
		"[->-[>+>>]>[+[-<+>]>+>>]<<<<<]                          c10v0 : another divmod algo borrowed from esolangs; results in 0 d_n%d n%d n/d "\
		">[-]                                                    c11v0 : clear c11                                                              "\
		">>[++++++++++++++++++++++++++++++++++++++++++++++++.[-]]c13v0 : print nonzero n/d (first digit) and clear c13                          "\
		"<[++++++++++++++++++++++++++++++++++++++++++++++++.[-]] c12v0 : print nonzero n%d (second digit) and clear c12                         "\
		"<<<++++++++++++++++++++++++++++++++++++++++++++++++.[-] c9v0  : print any n%d (last digit) and clear c9                                "\
		"<<<<<<.                                                 c3    : EOL                                                                    "\
		">>+                                                     c5    : increment current number                                               "\
		"                                                        block : multiply c6 by c5 (don't preserve c6)                                  "\
		">[>>+<<-]                                               c6v0  : move c6 to c8                                                          "\
		">>                                                      c8v0  : repeat c8 times                                                        "\
		"[                                                                                                                                      "\
		"<<<[>+>+<<-]                                            c5v0  : move c5 to c6 and c7                                                   "\
		">>[<<+>>-]                                              c7v0  : move c7 back to c5                                                     "\
		">-                                                                                                                                     "\
		"]                                                                                                                                      "\
		"<<<<-                                                   c4    : decrement loop counter                                                 "\
		"]                                                                                                                                      ";
	cpu.set_code(code_factorial);
	cpu.run();

	// Ref: http://cydathria.com/bf/bf_ex2.html
	std::cout << "Enter a string: ";
	std::string code_reverse_string = "+[->,----------]<[+++++++++++.<]";
	cpu.set_code(code_reverse_string);
	cpu.run();
	std::cout << std::endl;

	return 0;
}
