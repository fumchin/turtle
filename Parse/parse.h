#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <ctype.h>
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 50
#define FIRSTINDEX 0
#define SECONDINDEX 1
#define NEXT_STEP 1

typedef struct prog {
    
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw; //current word?
} Program;

void Prog(Program* p);
void Inslst(Program* p);
void Ins(Program* p);

void Fwd(Program* p);
void Rgt(Program* p);
void Col(Program* p);
void Loop(Program* p);
void Set(Program* p);

void Varnum(Program* p);
void Num();
void Var();
void Word();
void Ltr();
void Lst(Program* p);
void Pfix(Program* p);
void Items(Program* p);
void Op();
void Item(Program* p);


bool is_forward(Program* p);
bool is_right(Program* p);
bool is_colour(Program* p);
bool is_loop(Program* p);
bool is_set(Program* p);
bool is_var(Program* p);
bool is_letter(Program* p);
bool is_num(Program* p);
bool is_word(Program* p);
bool is_op(Program* p);
bool is_varnum(Program* p);
bool is_item(Program* p);

