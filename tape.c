#define MEMORY_SIZE 999999
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

typedef unsigned char byte;
typedef struct stack_struct* Stack;
typedef struct tape_struct* Tape;

struct tape_struct {
	byte* cells;
	byte* cursor;
};
Tape new_Tape();
void del_Tape(Tape t);
byte Tape_getValue(Tape t);
byte Tape_backward(Tape t);
byte Tape_forward(Tape t);
byte Tape_increaseValue(Tape t);
byte Tape_decreaseValue(Tape t);
byte Tape_setValue(Tape t, byte v);
void Tape_dbg(const char* name, Tape t);

Tape new_Tape()
{
	Tape t = malloc(sizeof(struct tape_struct));
	t->cells = malloc(MEMORY_SIZE * sizeof(byte));
	t->cursor = t->cells;
	return t;
}

void del_Tape(Tape t)
{
	free(t->cells);
	free(t);
}

byte Tape_forward(Tape t) {
	t->cursor++;
	return *t->cursor;
}

byte Tape_backward(Tape t) {
	t->cursor--;
	if (t->cursor < t->cells) {
		fprintf(stderr, "Tried moving left of the tape\n");
		exit(-1);
	} else {
		return *t->cursor;
	}
}

byte Tape_increaseValue(Tape t) {
	(*t->cursor)++;
}

byte Tape_decreaseValue(Tape t) {
	(*t->cursor)--;
}

byte Tape_getValue(Tape t) {
	return *t->cursor;
}

byte Tape_setValue(Tape t, byte v) {
	return *t->cursor = v;
}

void Tape_dbg(const char* name, Tape t) {
	fprintf(stderr, "---TAPE DBG '%s'\n\t", name);
	for (byte* dbg_cursor = max(t->cursor - 5, t->cells); dbg_cursor < t->cursor + 5; ++dbg_cursor) {
		fprintf(stderr, " %2x ", *dbg_cursor);
	}
	fprintf(stderr, "\n\t");
	for (byte* dbg_cursor = max(t->cursor - 5, t->cells); dbg_cursor < t->cursor + 5; ++dbg_cursor) {
		if (t->cursor == dbg_cursor) {
			fprintf(stderr, " ^^ ");
		} else {
			fprintf(stderr, "    ");
		}
	}
	fprintf(stderr, "\n\n");
}