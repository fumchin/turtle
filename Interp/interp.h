#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "../neillsimplescreen.h"
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 50
#define WIDTH 51
#define HEIGHT 33
#define PI 3.14159265358979323846
#define PS_START_X 30
#define PS_START_Y 40
#define COMMAND_LINE_LENGTH 50
#define FIRSTINDEX 0
#define SECONDINDEX 1
#define ALPHABET_SIZE 26
#define NEXT_STEP 1
#define STACK_CONSTANT_ONE 1
#define STACK_CONSTANT_TWO 2
#define MAX_COMMAND_LENGTH 50
#define RADIUS 180
#define WAIT_TIME 1
#define INDEX_ADJUST 1
#define MARGIN 0

enum save_type{screen, txt, ps};
typedef enum save_type save_type;

typedef struct varia {

    double num;
    char word[MAXTOKENSIZE];

} Variable;

typedef struct prog {
    
    char wds[MAXNUMTOKENS][MAXTOKENSIZE];
    int cw; //current word
    char board[HEIGHT][WIDTH]; //to be a canvas

    Variable variable_array[ALPHABET_SIZE];
    
    //current position
    double current_r;
    double current_c;
    double angle;

    double ps_current_x;
    double ps_current_y;

    neillcol colour;

    //create "a new file" or not
    FILE* output_file; 
    save_type output_type;

} Program;

enum Items_type {num, word};
typedef enum Items_type Items_type;

typedef struct Items_List {

    Variable array[MAXNUMTOKENS];
    int current_index;
    Items_type stored_type;

} Items_List;

typedef struct Stack {

    double array[MAXNUMTOKENS];
    int current_index;

} Stack;

void Prog(Program* p);
void Inslst(Program* p);
void Ins(Program* p);

void Fwd(Program* p);
void Rgt(Program* p);
void Col(Program* p);
void Loop(Program* p);
void Set(Program* p);

double Varnum(Program* p);
double Num(Program* p);
double Var_number(Program* p);
char* Var_string(Program* p);
char* Word(Program* p);
char Ltr(Program* p);
Items_List* Lst(Program* p);
void Items(Program* p, Items_List* items_list);
void Item(Program* p, Items_List* items_list);
void Pfix(Program* p, Stack* stack);
char Op(Program* p);

bool is_forward(Program* p);
bool is_right(Program* p);
bool is_colour(Program* p);
bool is_loop(Program* p);
bool is_set(Program* p);
bool is_var(Program* p);
bool is_letter(Program* p);
bool is_letter_var(Program* p);
bool is_num(Program* p);
bool is_word(Program* p);
bool is_op(Program* p);
bool is_varnum(Program* p);
bool is_item(Program* p);

void test();
void test_Prog();
void test_Inslst();
void test_Ins();

void test_forward();
void test_rgt();
void test_col();
void test_loop();
void test_set();

void test_varnum();
void test_num();
void test_var_number();
void test_var_string();
void test_word();
void test_ltr();
void test_lst();
void test_items();
void test_item();
void test_pfix();
void test_op();

void test_is_forward();
void test_is_right();
void test_is_colour();
void test_is_loop();
void test_is_set();
void test_is_var();
void test_is_letter();
void test_is_num();
void test_is_word();
void test_is_op();
void test_is_varnum();
void test_is_item();
