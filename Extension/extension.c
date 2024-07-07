#include "extension.h"


int main(int argc, char* argv[]) {

    test();
    
    // read .ttl file
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL) {
        fprintf(stderr, "Cannot open file? \n");
        exit(EXIT_FAILURE);
    }

    //get space to store "command" in .ttl file
    Program* prog = calloc(1, sizeof(Program));


    if(argc == 3) {

        prog -> output_file = fopen(argv[2], "w");

        //get only "extension" of file
        char* extension;
        extension = strrchr(argv[2], '.');

        if(strcmp(extension, ".txt") == 0) {

            //set "output_type"
            prog -> output_type = txt;
        } else if (strcmp(extension, ".ps") == 0) {
            
            prog -> output_type = ps;

            //write the first two lines in .ps (default all the same)
            fprintf(prog -> output_file, "0.2 setlinewidth\n10 10 scale\n");

        } else {
            exit(1);
        }
        
    } else {

        //set "output_type"
        prog -> output_type = screen;
    }


    //default
    prog -> cw = 0;
    for(int r = 0; r < HEIGHT; r++) {
        for(int c = 0; c < WIDTH; c++) {
            prog -> board[r][c] = ' ';
        }
    }
    //(start position should be set in "middle")
    prog -> current_c = (double) (WIDTH - 1)/2;
    prog -> current_r = (double) (HEIGHT - 1)/2;
    prog -> angle = 0;
    prog -> ps_current_x = PS_START_X;
    prog -> ps_current_y = PS_START_Y;
    
    
    int i = 0;
    //get "string" in file and save it to "variable"
    while(fscanf(fp, "%s", prog -> wds[i]) != EOF) {
        // printf("%s \n", prog -> wds[i]);
        i++;
    }
    
    Prog(prog);

    if((prog -> output_type) == txt) {

        //save board to "txt"
        for(int r = 0; r < HEIGHT; r++) {
            for(int c = 0; c < WIDTH; c++) {

                fprintf(prog -> output_file, "%c", prog -> board[r][c]);
            }
            //move to "next row"
            fprintf(prog -> output_file, "\n");
        }

    } else if((prog -> output_type) == ps) {

        //write "last line" in .ps file
        fprintf(prog -> output_file, "showpage\n");

    } else {

    }


    // printf("Parsed OK\n");

    fclose(prog -> output_file);

    //ps -> pdf
    if((prog -> output_type) == ps) {

        //make a "command line" string
        char command[COMMAND_LINE_LENGTH];

        //convert argv[2] to "string" and combine with ps2pdf
        snprintf(command, MAX_COMMAND_LENGTH, "ps2pdf %s", argv[2]);

        system(command);
    }
    

    free(prog);

    return 0;
}

void Prog(Program* p) {
    
    if(strcmp(p -> wds[p -> cw], "START") != 0) {
        printf("No START statement ?");
        exit(1);
    }
    p -> cw = p -> cw + NEXT_STEP;
    Inslst(p);
}

void Inslst(Program* p) {

    if(strcmp(p -> wds[p -> cw], "END") == 0) {
        return;
    }
    Ins(p);

    //move to next command (recursion)
    p -> cw = p -> cw + NEXT_STEP;
    Inslst(p);
}


void Ins(Program* p) {

    if(is_forward(p)) {
        Fwd(p);
    } else if (is_right(p)) {
        Rgt(p);
    } else if (is_colour(p)) {
        Col(p);
    } else if(is_loop(p)) {
        Loop(p);
    } else if(is_set(p)) {
        Set(p);
    } else if(is_if(p)) {
        If(p);
    }else {
        exit(1);
    }
}

void Fwd(Program* p) { //(ex. Forward 5)
    
    double steps = 0;

    //move to next command
    p -> cw = p -> cw + NEXT_STEP;
    //parse the "following num" to get moving steps
    steps = Varnum(p);


    if((p -> output_type) == ps) {

        //start "writing command" in .ps file (ex. 30 43 moveto)
        fprintf(p -> output_file, "newpath\n");

        //move to 
        fprintf(p -> output_file, "%.6lf %.6lf moveto\n", (p -> ps_current_x), (p -> ps_current_y));

        //decide "next position"
        p -> ps_current_y = (p -> ps_current_y) + (steps) * cos((p ->angle) * (PI/RADIUS));
        p -> ps_current_x = (p -> ps_current_x) + (steps) * sin((p ->angle) * (PI/RADIUS));

        //line to 
        fprintf(p -> output_file, "%.6lf %.6lf lineto\n", (p -> ps_current_x), (p -> ps_current_y));

        if(p->colour == black) {
            fprintf(p -> output_file, "0 0 0 setrgbcolor\n");
        } else if (p->colour == red) {
            fprintf(p -> output_file, "1 0 0 setrgbcolor\n");
        } else if (p->colour == green) {
            fprintf(p -> output_file, "0 1 0 setrgbcolor\n");
        } else if (p->colour == yellow) {
            fprintf(p -> output_file, "1 1 0 setrgbcolor\n");
        } else if (p->colour == blue) {
            fprintf(p -> output_file, "0 0 1 setrgbcolor\n");
        } else if (p->colour == magenta) {
            fprintf(p -> output_file, "1 0 1 setrgbcolor\n");
        } else if (p->colour == cyan) {
            fprintf(p -> output_file, "0 1 1 setrgbcolor\n");
        } else if (p->colour == white) {
            fprintf(p -> output_file, "0.8 0.8 0.8 setrgbcolor\n");
        } else { // No colour set -> defalut "white"
            fprintf(p -> output_file, "0.8 0.8 0.8 setrgbcolor\n");
        }

        fprintf(p -> output_file, "stroke\n");

    } else { //(txt, screen)

        //start "drawing" board (ex. Forward 5)
        //(screen) add "one" letter on board -> draw entire board (on screen)
        //<In "main"> (txt) read finish all .ttl file -> draw entire board (to txt)
        for(int i = 0; i < steps; i++) {

            int r = (int)round(p -> current_r);
            int c = (int)round(p -> current_c);

            //edge case
            if(r < MARGIN) {
                r = MARGIN;
            } 
            if(r > HEIGHT - INDEX_ADJUST) {
                r = HEIGHT - INDEX_ADJUST;
            } 
            if(c < MARGIN) {
                c = MARGIN;
            } 
            if(c > WIDTH - INDEX_ADJUST) {
                c = WIDTH - INDEX_ADJUST;
            }

            //put "letter" or "color" on board
            if(p -> colour == black) {
                p -> board[r][c] = 'K';
            } else if(p -> colour == red) {
                p -> board[r][c] = 'R';
            } else if(p -> colour == green) {
                p -> board[r][c] = 'G';
            } else if(p -> colour == yellow) {
                p -> board[r][c] = 'Y';
            } else if(p -> colour == blue) {
                p -> board[r][c] = 'B';
            } else if(p -> colour == magenta) {
                p -> board[r][c] = 'M';
            } else if(p -> colour == cyan) {
                p -> board[r][c] = 'C';
            } else if(p -> colour == white) {
                p -> board[r][c] = 'W';
            } else { // No colour set -> defalut "white"
                p -> board[r][c] = 'W';
            }
            


            //(print on "screen" (if output_type == screen))
            if((p -> output_type) == screen) {

                //defalut
                neillclrscrn();
                neillcursorhome();

                //print out whole "current board"
                for(int r = 0; r < HEIGHT; r++) {
                    for(int c = 0; c < WIDTH; c++) {

                        //I. change color
                        if((p -> board[r][c]) == 'K') {
                            neillfgcol(black);
                            neillbgcol(black);
                        } else if((p -> board[r][c]) == 'R') {
                            neillfgcol(red);
                            neillbgcol(red);
                        } else if((p -> board[r][c]) == 'G') {
                            neillfgcol(green);
                            neillbgcol(green);
                        } else if((p -> board[r][c]) == 'Y') {
                            neillfgcol(yellow);
                            neillbgcol(yellow);
                        } else if((p -> board[r][c]) == 'B') {
                            neillfgcol(blue);
                            neillbgcol(blue);
                        } else if((p -> board[r][c]) == 'M') {
                            neillfgcol(magenta);
                            neillbgcol(magenta);
                        } else if((p -> board[r][c]) == 'C') {
                            neillfgcol(cyan);
                            neillbgcol(cyan);
                        } else if((p -> board[r][c]) == 'W') {
                            neillfgcol(white);
                            neillbgcol(white);

                        } else { //no letter on board --> black
                            neillfgcol(black);
                            neillbgcol(black);
                        }

                        //II. print out "current board"
                        printf("%c", p -> board[r][c]);
                    }
                    //move to "next row"
                    printf("\n");
                }
                //wait one sec and print new board
                neillbusywait(WAIT_TIME);
            }

            //decide next position (if there is "RIGHT..." before it)
            p -> current_r = (p -> current_r) - cos((p ->angle) * (PI/RADIUS));
            p -> current_c = (p -> current_c) + sin((p ->angle) * (PI/RADIUS));
        }
    }
    
}

void Rgt(Program* p) {

    double angle = 0; 

    p -> cw = p -> cw + NEXT_STEP;
    angle = Varnum(p);

    //adjust angle
    p -> angle = (p -> angle) + angle;
}

void Col(Program* p) {

    char* colour_string;
    bool need_free = false;

    p -> cw = p -> cw + NEXT_STEP;
    
    if(is_var(p)) {
        colour_string = Var_string(p);
    } else if (is_word(p)) {
        colour_string = Word(p);
        need_free = true; //do calloc in Word function
    }else {
        exit(1);
    }

    //"change colour" base on color_string
    if(strcmp(colour_string, "BLACK") == 0) {
        p -> colour = black;
    } else if(strcmp(colour_string, "RED") == 0) {
        p -> colour = red;
    } else if(strcmp(colour_string, "GREEN") == 0) {
        p -> colour = green;
    }  else if(strcmp(colour_string, "YELLOW") == 0) {
        p -> colour = yellow;
    }  else if(strcmp(colour_string, "BLUE") == 0) {
        p -> colour = blue;
    }  else if(strcmp(colour_string, "CYAN") == 0) {
        p -> colour = cyan;
    }  else if(strcmp(colour_string, "MAGENTA") == 0) {
        p -> colour = magenta;
    }  else if(strcmp(colour_string, "WHITE") == 0) {
        p -> colour = white;
    } else {
        printf("Invalid colour \n");
        exit(1);
    }

    if(need_free) {
        free(colour_string);
    }

}

void Loop(Program* p) {

    char var_char; //(ex. LOOP A OVER {1 2 3} --> "A")
    int current_point;
    Items_List* items_list;

    p -> cw = p -> cw + NEXT_STEP;
    if(is_letter(p)) {
        var_char = Ltr(p); //ex.("A" -> 'A')

        p -> cw = p -> cw + NEXT_STEP;
        if(strcmp(p -> wds[p -> cw], "OVER") == 0) {
            
            p -> cw = p -> cw + NEXT_STEP;
            //make an "array" according to item inside {} (ex.{1 2 3 "BLUE"} -> loop_array = [1, 2, 3, "BLUE"])
            items_list = Lst(p); 

            p -> cw = p -> cw + NEXT_STEP;
            current_point = p -> cw; //save this point, so everytime "loop start from here"

            //loop through array
            for(int i = 0; i < items_list -> current_index; i++) {

                //put array_item in "$A", so "FOWARD $A" can get the value
                if(items_list -> stored_type == num) {

                    //ex. ($A -> variable_array[0])
                    p -> variable_array[(int)(var_char - 'A')].num = items_list -> array[i].num;

                } else { //(word)

                    strcpy(p -> variable_array[(int)(var_char - 'A')].word, items_list -> array[i].word);

                }

                //go through "instructions" after '}' until "END"
                p -> cw = current_point; //reset point to loop start
                Inslst(p); 
            }

            // move to corresponding "END" instruction of LOOP
            if(items_list -> current_index == 0){
                p -> cw = p -> cw + NEXT_STEP;
            }

            free(items_list);

        } else {
            exit(1);
        }
    }

} 

void Set(Program* p) {

    char var_char; //(ex. SET A (0) --> "A")
    //1. make an "array" --> store number, variable appeared in Pfix function
    Stack stack;
    //default
    stack.current_index = 0;

    p -> cw = p -> cw + NEXT_STEP;
    var_char = Ltr(p); //ex.("A" -> 'A')

    p -> cw = p -> cw + NEXT_STEP;
    //get the "first char" of string (ex. "(" --> '(')
    if((p -> wds[p -> cw][FIRSTINDEX] == '(') && (p -> wds[p -> cw][SECONDINDEX] == '\0')) {
        
        p -> cw = p -> cw + NEXT_STEP;
        Pfix(p, &stack); //2. get one "number, variable or operand" until ')' appear
 
        //3. put stack_result in "$A", so later instructions (ex. FOWARD $A) can get the value
        p -> variable_array[(int)(var_char - 'A')].num = stack.array[FIRSTINDEX];

    } else {
        exit(1);
    }
}

void If(Program* p) {

    double num_1 = 0;
    double num_2 = 0;

    p -> cw = p -> cw + NEXT_STEP;

    if(is_varnum(p)) {

        //get the "value" of varnum_1 --> compare with varnum_2
        num_1 = Varnum(p);

        p -> cw = p -> cw + NEXT_STEP;
        if(strcmp(p -> wds[p->cw], "EQUALS") == 0) {

            p -> cw = p -> cw + NEXT_STEP;
            if(is_varnum(p)) {

                //get the "value" of varnum_2
                num_2 = Varnum(p);

                //compare num1 and num2
                if((int)num_1 == (int)num_2) {

                    p -> cw = p -> cw + NEXT_STEP;
                    Inslst(p);
                } 
            }
 
        } else if(strcmp(p -> wds[p->cw], "LESS") == 0) {

            p -> cw = p -> cw + NEXT_STEP;
            if(is_varnum(p)) {

                //get the "value" of varnum_2
                num_2 = Varnum(p);

                //compare num1 and num2
                if(num_1 < num_2) {

                    p -> cw = p -> cw + NEXT_STEP;
                    Inslst(p);
                } 
            }
        

        } else if(strcmp(p -> wds[p->cw], "GREATER") == 0) {

            p -> cw = p -> cw + NEXT_STEP;
            if(is_varnum(p)) {

                //get the "value" of varnum_2
                num_2 = Varnum(p);

                //compare num1 and num2
                if(num_1 > num_2) {

                    p -> cw = p -> cw + NEXT_STEP;
                    Inslst(p);
                } 
            }
        

        } else {
            exit(1);
        }

        // if condition "not satisfied" --> make (p->cw) jump to "END"
        while(strcmp(p -> wds[p->cw], "END") != 0) {

            (p -> cw)++;
        }
        Inslst(p);

    }
}

double Varnum(Program* p) {

    double result = 0;

     if(is_var(p)) {
        result = Var_number(p); //(ex: "$A" -> 2)
    } else if (is_num(p)) {
        result = Num(p); //(ex. "1" -> 1)
    }else {
        exit(1);
    }

    return result;
}


double Num(Program* p) {

    double result = 0;
    char* end = NULL;
    double num = 0;

    //check if the string can be converted to "num" 
    //strtod -> success: "123" -> 123 / failed: "12ab" -> 0
    result = strtod(p -> wds[p -> cw], &end);

    //dereference to get the "value" of end pointer
    if(*end != '\0') {
        //error
        exit(1);
    } else {
        //convert success
        num = result;
        return num;
    }
}

double Var_number(Program* p) { //(ex. $A)  ($A == 5) ($A == "BLUE")

    char key; 
    int index;

    //(ex. $A -> A)
    key = p -> wds[p->cw][SECONDINDEX]; 
    
    //(ex. A -> 0, B -> 1, Z -> 25)
    index = (int)(key - 'A');

    //(ex. variable_array[A] == 5)
    return p -> variable_array[index].num;
}

char* Var_string(Program* p) { //(ex. $A) ($A == "BLUE")

    char key; 
    int index;

    //(ex. $A -> A)
    key = p -> wds[p->cw][SECONDINDEX]; 
    
    //(ex. A -> "BLUE", B -> "RED", Z -> "YELLOW")
    index = (int)(key - 'A');

    //(ex. variable_array[A] == BLUE)
    return p -> variable_array[index].word;
}

char* Word(Program* p) { //ex. ("BLUE" -> BLUE)

    int last_index;
    char* word = (char*)calloc(strlen(p -> wds[p->cw]) - 1, sizeof(char)); //string length + "NULL char"

    last_index = strlen(p -> wds[p->cw]) - 2; //(get "E" index)

    for(int index = 1; index <= last_index; index++) {

        word[index - 1] = p -> wds[p->cw][index]; //(ex. word[0] = "BLUE"[1])
    }

    //add "NULL char" at the end
    word[last_index] = '\0';

    return word;
}

char Ltr(Program* p) {

    return p->wds[p->cw][FIRSTINDEX];
}


// make an array according to item inside {} (ex.{NEXT_STEP 2 3 "BLUE"} -> loop_array = [1, 2, 3, "BLUE"])
Items_List* Lst(Program* p) { 

    Items_List* items_list = (Items_List*)calloc(MAXTOKENSIZE, sizeof(Items_List));
    //default
    items_list -> current_index = 0;

    if(strcmp(p -> wds[p -> cw], "{") == 0) {
        
        p -> cw = p -> cw + NEXT_STEP;
        Items(p, items_list); //append items to array
    }

    return items_list;
}

void Pfix(Program* p, Stack* stack) {

    double num = 0;
    char operand;
    double result;

     if(strcmp(p -> wds[p -> cw], ")") == 0) {
        return;

    } else if (is_op(p)) {

        operand = Op(p); // instruction string -> char (ex. "+" -> +)

        //calculate the "result" according to the operand
        if(operand == '+') {
            result = (stack -> array[(stack -> current_index) - STACK_CONSTANT_TWO]) + (stack -> array[(stack -> current_index) - STACK_CONSTANT_ONE]);
        } else if (operand == '-') {
            result = (stack -> array[(stack -> current_index) - STACK_CONSTANT_TWO]) - (stack -> array[(stack -> current_index) - STACK_CONSTANT_ONE]);
        } else if (operand == '*') {
            result = (stack -> array[(stack -> current_index) - STACK_CONSTANT_TWO]) * (stack -> array[(stack -> current_index) - STACK_CONSTANT_ONE]);
        } else if (operand == '/') {
            result = (stack -> array[(stack -> current_index) - STACK_CONSTANT_TWO]) / (stack -> array[(stack -> current_index) - STACK_CONSTANT_ONE]);
        } else {
            exit(1);
        }

        //stored "result" back to array
        stack -> array[(stack -> current_index) - STACK_CONSTANT_TWO] = result;

        //reset current_index
        stack -> current_index = (stack -> current_index) - STACK_CONSTANT_ONE;  

        p -> cw = p -> cw + NEXT_STEP;
        Pfix(p, stack);


    } else if (is_varnum(p)) {

        num = Varnum(p); // instruction string -> num (ex. "123" -> 123 / "$A" -> 123)

        //append "num" to array
        stack -> array[stack -> current_index] = num;
        (stack -> current_index)++;

        p -> cw = p -> cw + NEXT_STEP;
        Pfix(p, stack);

    } else {
        exit(1);
    }
}

// check if '}' exists
void Items(Program* p, Items_List* items_list) {

    if(strcmp(p -> wds[p -> cw], "}") == 0) {
        return;

    } else if (is_item(p)) {

        Item(p, items_list); //append items to array

        p -> cw = p -> cw + NEXT_STEP;
        Items(p, items_list);

    } else {
        exit(1);
    }
}

// append items to array
void Item(Program* p, Items_List* items_list) {
    
    double n;
    char* w;

    if(is_varnum(p)) { //(ex: 1 2 4)
        n = Varnum(p);

        //1. assign the "stored_type" of items_list
        items_list -> stored_type = num;

        //2. append "num" to array
        items_list -> array[items_list -> current_index].num = n;
        
        (items_list -> current_index)++;

    } else if (is_word(p)) { //(ex: BLUE)
        w = Word(p);

        //1. assign the "stored_type" of items_list
        items_list -> stored_type = word;

        //2. append "word" to array (* "strcpy" to assign)
        strcpy(items_list -> array[items_list -> current_index].word, w);
        (items_list -> current_index)++;
        free(w);

    }else {
        exit(1);
    }


}

char Op(Program* p) { //(ex. "+" -> '+')

    return p -> wds[p -> cw][FIRSTINDEX];
}

bool is_forward(Program* p) {

    if(strcmp(p -> wds[p -> cw], "FORWARD") == 0) {
        return true;
    }
    return false;
}

bool is_right(Program* p) {

    if(strcmp(p -> wds[p -> cw], "RIGHT") == 0) {
        return true;
    }
    return false;
}

bool is_colour(Program* p) {

    if(strcmp(p -> wds[p -> cw], "COLOUR") == 0) {
        return true;
    }
    return false;
}

bool is_loop(Program* p) {

    if(strcmp(p -> wds[p -> cw], "LOOP") == 0) {
        return true;
    }
    return false;
}

bool is_set(Program* p) {

    if(strcmp(p -> wds[p -> cw], "SET") == 0) {
        return true;
    }
    return false;
}

bool is_if(Program* p) {

    if(strcmp(p -> wds[p ->cw], "IF") == 0) {
        return true;
    }
    return false;
}

bool is_var(Program* p){

    //get the "first char" of string (ex. $A --> '$')
    if(p -> wds[p -> cw][FIRSTINDEX] == '$') {
        if(is_letter_var(p)) {
            Ltr(p); //not used
            return true;
        }
    }
    return false;
}

bool is_letter(Program* p) {
    
    //get the "first char" of string (ex. "A" --> 'A')
    if(isalpha(p -> wds[p -> cw][FIRSTINDEX]) && isupper(p -> wds[p -> cw][FIRSTINDEX]) && (p -> wds[p->cw][SECONDINDEX] == '\0')) {

            return true;
     }
     return false;
}

bool is_letter_var(Program* p) {
    
    //get the "second char" of string (ex. $A --> 'A')
    if(isalpha(p -> wds[p -> cw][SECONDINDEX]) && isupper(p -> wds[p -> cw][SECONDINDEX])) {

            return true;
     }
     return false;
}

bool is_num(Program* p) {

    char* end = NULL;

    //check if the string can be converted to "num"
    strtod(p -> wds[p -> cw], &end);

    //dereference to get the "value" of end pointer
    if(*end != '\0') {
        //error
        return false;
    } else {
        //convert success
        return true;
    }
}

bool is_word(Program* p) {

    int last_index = 0;

    last_index = strlen(p -> wds[p -> cw]) - 1;

    //find the "command" in wds(string array), which started with ", and ended with "
    //get the "first char" of string (ex. "Red" --> '"')
    if((p -> wds[p -> cw][FIRSTINDEX] == '"') && (p -> wds[p -> cw][last_index] == '"')) {
        return true;
    }
    return false;
}

bool is_op(Program* p) {

    if(strcmp(p -> wds[p -> cw], "+") == 0) { 
        return true;
    } else if(strcmp(p -> wds[p -> cw], "-") == 0) {
        return true;
    } else if(strcmp(p -> wds[p -> cw], "/") == 0) {
        return true;
    } else if(strcmp(p -> wds[p -> cw], "*") == 0) {
        return true;
    } else {
        return false;
    }
}

bool is_varnum(Program* p) {

     if(is_var(p) || is_num(p)) {
        return true;
    } else {
        return false;
    }
}

bool is_item(Program* p) {

    if(is_varnum(p) || is_word(p)) {
        return true;
    }else {
        return false;
    }
}

void test() {

    test_if();

}

void test_if() {

    Program* prog = (Program*)calloc(1, sizeof(Program));

    //default
    prog -> cw = 0;

    prog -> variable_array[0].num = 5; //set $A=5
    prog -> variable_array[2].num = 3; //set $C=3

    //if $A==5 --> $C=10
    //if $A!=5 --> $C=3
    strcpy(prog -> wds[0], "IF");
    strcpy(prog -> wds[1], "$A");
    strcpy(prog -> wds[2], "EQUALS");
    strcpy(prog -> wds[3], "5");
    strcpy(prog -> wds[4], "SET");
    strcpy(prog -> wds[5], "C");
    strcpy(prog -> wds[6], "(");
    strcpy(prog -> wds[7], "10");
    strcpy(prog -> wds[8], ")");
    strcpy(prog -> wds[9], "END");

    If(prog);

    assert((int)prog -> variable_array[2].num == 10);
    free(prog);

}
