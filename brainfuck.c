#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tape.c"

typedef void (*MachineHandle) (Tape, Tape, int*);

void memory_forward(Tape instructions, Tape memory, int* parenthesis) {
	Tape_forward(memory);
	Tape_forward(instructions);
}

void memory_backward(Tape instructions, Tape memory, int* parenthesis) {
    Tape_backward(memory);
	Tape_forward(instructions);
}

void increment_cell(Tape instructions, Tape memory, int* parenthesis) {
	Tape_increaseValue(memory);
    Tape_forward(instructions);
}

void decrement_cell(Tape instructions, Tape memory, int* parenthesis) {
    Tape_decreaseValue(memory);
    Tape_forward(instructions);
}

void push_parenthesis(Tape instructions, Tape memory, int* parenthesis) {
	*parenthesis++;
	if (*parenthesis < 0) {
		Tape_backward(instructions);
	} else {
		Tape_forward(instructions);
	}
}

void pop_parenthesis(Tape instructions, Tape memory, int* parenthesis) {
	*parenthesis--;
	if (*parenthesis < 0) {
		Tape_backward(instructions);
	} else {
		Tape_forward(instructions);
	}
}

void print_cell(Tape instructions, Tape memory, int* parenthesis) {
	printf("%c", Tape_getValue(memory));
    Tape_forward(instructions);
}

void read_input(Tape instructions, Tape memory, int* parenthesis) {
	Tape_setValue(instructions, getchar());
	Tape_forward(instructions);
}

void jump_forward_if_zero(Tape instructions, Tape memory, int* parenthesis) {
	if (Tape_getValue(memory) == 0) {
		*parenthesis++;
    }
	if (*parenthesis < 0) {
		Tape_backward(instructions);
	} else {
		Tape_forward(instructions);
	}
}

void jump_backward_if_nonzero(Tape instructions, Tape memory, int* parenthesis) {
	if (Tape_getValue(memory) != 0) {
		*parenthesis--;
	}
	if (*parenthesis < 0) {
		Tape_backward(instructions);
	} else {
		Tape_forward(instructions);

	}
}

void seek_parenthesis(Tape instructions, Tape memory, int* parenthesis) {
	if (*parenthesis < 0) {
		Tape_backward(instructions);
	} else {
		Tape_forward(instructions);
	}
}

void default_behaviour(Tape instructions, Tape memory, int* parenthesis) {
	Tape_forward(instructions);
}

void brainfuck_machine(FILE* source) {
	MachineHandle states[2][256] = {{default_behaviour}, {seek_parenthesis}};
	states[0]['+'] = increment_cell;
	states[0]['-'] = decrement_cell;
	states[0]['.'] = print_cell;
	states[0][','] = read_input;
	states[0]['>'] = memory_forward;
	states[0]['<'] = memory_backward;
	states[0]['['] = jump_forward_if_zero;
	states[0][']'] = jump_backward_if_nonzero;
	states[1]['['] = push_parenthesis;
	states[1][']'] = pop_parenthesis;
	Tape instructions = new_Tape();
	Tape memory = new_Tape();
	int parenthesis_seeking = 0;

	char c = 0;
	while ((c = Tape_getValue(instructions)) != EOF) {
		if (false) {
			Tape_dbg("Instructions tape", instructions);
			Tape_dbg("Memory tape", memory);
			fprintf(stderr, "parenthesis_seeking: %d\n-------------------\n\n", parenthesis_seeking);
		}
		if (c == 0) {
			Tape_setValue(instructions, fgetc(source));
			continue;
		}
		MachineHandle func = states[!parenthesis_seeking][c];
		if (func != NULL) {
			func(instructions, memory, &parenthesis_seeking);
		}
		/*if (parenthesis_seeking) {
			switch(c) {
				case '[':
					parenthesis_seeking ++;
					break;
				case ']':
					parenthesis_seeking --;
					break;
			}
		} else {
			switch(c) {
				case '>':
					Tape_forward(memory);
					break;

				case '<':
					Tape_backward(memory);
					break;

				case '+':
					Tape_increaseValue(memory);
					break;

				case '-':
					Tape_decreaseValue(memory);
					break;

				case '.':
					printf("%c", Tape_getValue(memory));
					break;

				case ',':
					Tape_setValue(memory, getchar());
					break;

				case '[':
					if (Tape_getValue(memory) == 0) {
						parenthesis_seeking++;
					}
					break;
				case ']':
					if (Tape_getValue(memory) != 0) {
						parenthesis_seeking--;
					}
					break;

				default:
					break;
			}
		}
		if (parenthesis_seeking < 0) {
			Tape_backward(instructions);
		} else {
			Tape_forward(instructions);
		}*/
	};

	del_Tape(instructions);
	del_Tape(memory);
}

int main(int argc, char** argv)
{
	FILE* src = stdin;
	if (argc == 2) {
		src = fopen(argv[1], "r");
		if (!src) {
			fprintf(stderr, "Could not open file %s\n", argv[1]);
			exit(-1);
		}
	}
	brainfuck_machine(src);
	printf("\n");
	return 0;
}
