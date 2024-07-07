#include "parse.h"


int main(int argc, char* argv[]) {

    
    // read .ttl file
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL) {
        printf("argc = %d\n", argc);
        fprintf(stderr, "Cannot open file? \n");
        exit(EXIT_FAILURE);
    }

    //get space to store "command" in .ttl file
    Program* prog = calloc(1, sizeof(Program));
    //default
    prog -> cw = 0;
    
    
    int i = 0;
    while(fscanf(fp, "%s", prog -> wds[i]) != EOF) {
        // printf("%s \n", prog -> wds[i]);
        i++;
    }
    
    Prog(prog);

    // printf("Parsed OK\n");
    
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
    }else {
        exit(1);
    }
}

void Fwd(Program* p) {
    
    //move to next command
    p -> cw = p -> cw + NEXT_STEP;
    Varnum(p);
    
}

void Rgt(Program* p) {

    p -> cw = p -> cw + NEXT_STEP;
    Varnum(p);
}

void Col(Program* p) {

    p -> cw = p -> cw + NEXT_STEP;
    
    if(is_var(p)) {
        Var();
    } else if (is_word(p)) {
        Word();
    }else {
        exit(1);
    }
}

void Loop(Program* p) {

    p -> cw = p -> cw + NEXT_STEP;
    Ltr(p);

    p -> cw = p -> cw + NEXT_STEP;
    if(strcmp(p -> wds[p -> cw], "OVER") == 0) {
        
        p -> cw = p -> cw + NEXT_STEP;
        Lst(p);
        p -> cw = p -> cw + NEXT_STEP;
        Inslst(p);

    } else {
        exit(1);
    }

} 

void Set(Program* p) {

    p -> cw = p -> cw + NEXT_STEP;
    Ltr();

    p -> cw = p -> cw + NEXT_STEP;
    //get the "first char" of string (ex. "(" --> '(')
    if((p -> wds[p -> cw][FIRSTINDEX] == '(') && (p -> wds[p -> cw][SECONDINDEX] == '\0')) {
        
        p -> cw = p -> cw + NEXT_STEP;
        Pfix(p);

    } else {
        exit(1);
    }
}

void Varnum(Program* p) {

     if(is_var(p)) {
        Var(p);
    } else if (is_num(p)) {
        Num();
    }else {
        exit(1);
    }
}

void Num() {

    return;
}

void Var() {

    return;
}

void Word() {

    return;
}

void Ltr() {

    return;
}

void Lst(Program* p) {

    if(strcmp(p -> wds[p -> cw], "{") == 0) {
        
        p -> cw = p -> cw + NEXT_STEP;
        Items(p);
    }

}

void Pfix(Program* p) {

     if(strcmp(p -> wds[p -> cw], ")") == 0) {
        return;

    } else if (is_op(p)) {

        Op();

        p -> cw = p -> cw + NEXT_STEP;
        Pfix(p);

    } else if (is_varnum(p)) {

        Varnum(p);

        p -> cw = p -> cw + NEXT_STEP;
        Pfix(p);

    } else {
        exit(1);
    }
}

void Items(Program* p) {

    if(strcmp(p -> wds[p -> cw], "}") == 0) {
        return;

    } else if (is_item(p)) {

        Item(p);

        p -> cw = p -> cw + NEXT_STEP;
        Items(p);

    } else {
        exit(1);
    }
}

void Op() {

    return;
}

void Item(Program* p) {
    
    if(is_varnum(p)) {
        Varnum(p);
    } else if (is_word(p)) {
        Word();
    }else {
        exit(1);
    }
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

bool is_var(Program* p){

    //get the "first char" of string (ex. $A --> '$')
    if(p -> wds[p -> cw][FIRSTINDEX] == '$') {
        if(is_letter(p)) {
            return true;
        }
    }
    return false;
}

bool is_letter(Program* p) {
    
    //get the "second char" of string (ex. $A --> 'A')
    if(isalpha(p -> wds[p -> cw][SECONDINDEX]) && isupper(p -> wds[p -> cw][SECONDINDEX])) {

            Ltr();

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
