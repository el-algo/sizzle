#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define stack_len 8

int stack[stack_len] = {0};
int sp = -1;

void prints(int* stack, int len)
{
    printf("[");
    int i;
    for(i = 0; i < len - 1; i++)
    {
        printf("%d, ", stack[i]);
    }
    printf("%d] -> %d\n", stack[i], stack[sp]);
}

// Push
void give(int val)
{
    if(sp == stack_len - 1)
    {
        printf("Overflow...\n");
    }
    else stack[++sp] = val;
}

// Pop
int take()
{
    if(sp == -1) printf("Underflow...\n");
    else
    {
        return stack[sp--];
    }
}

int top()
{
    return stack[sp];
}

void print_top()
{
    printf("TOP -> %d\n", top());
}

// Add
void heal()
{
    int a = take();
    int b = take();
    give(b + a);
}

// Sub
void hit()
{
    int a = take();
    int b = take();
    give(b - a);
}

// Mul
void buff()
{
    int a = take();
    int b = take();
    give(b * a);
}

// Div
void debuff()
{
    int a = take();
    int b = take();
    give(b / a);
    give(b % a);
}

// Halt
void wait_halt()
{
    while(getchar() != '\n');
}

// Duplicate
void copy()
{
    int a = take();
    give(a);
    give(a);
}

void ask()
{
	int n;
	scanf("%d", &n);
	give(n);
}

void say(char* str)
{
    printf("%s\n", str);
}

void show()
{
    prints(stack, stack_len);
}

struct Token {
    char lexeme[128];
    int position;
    int line;
};

int samestr(char *a, char *b)
{
	if(strcmp(a, b) == 0) return 1;
	else return 0;
}

int starts_with(char *str, char chr)
{
    if(str[0] == chr) return 1;
    else return 0;
}

int ends_with(char *str, char chr)
{
    int len = strlen(str) - 1;
    if(str[len] == chr) return 1;
    else return 0;
}

char* str_clean(char *str)
{
    int len = strlen(str) - 1;
    if(starts_with(str, '"'))
    {
        if(ends_with(str, '"')) str[len] = '\0';
        return str + 1;
    }
    return str;
}

void move(int *i, char *next, int total_lines)
{
    if(isdigit(next[0]))
    {
        int line_num = atoi(next);
        if(line_num > 0 && line_num <= total_lines && line_num != *i)
        {
            *i = line_num - 2;
        }
    }
}


int execute(struct Token list[1024], int lexemes)
{
    int total_lines = list[lexemes - 1].line;

    for(int i = 0; i < lexemes; i++)
    {
        char *curr = list[i].lexeme;
        char *next = list[i + 1].lexeme;
        
        if(samestr(curr, "ASK")) ask();
        else if(samestr(curr, "WAIT")) wait_halt();
        else if(samestr(curr, "GIVE"))
        {
            if(isdigit(next[0]))
            {
                give(atoi(next));
                i++;
            }
        }
        else if(samestr(curr, "SAY"))
        {
            if(starts_with(next, '"'))
            {
                say(str_clean(next));
                i++;
            }
        }
        else if(samestr(curr, "TAKE")) take();
        else if(samestr(curr, "SHOW")) show();
        else if(samestr(curr, "TOP")) print_top();
        else if(samestr(curr, "COPY")) copy();

        else if(samestr(curr, "HIT")) hit();
        else if(samestr(curr, "HEAL")) heal();

        else if(samestr(curr, "BUFF")) buff();
        else if(samestr(curr, "DEBUFF")) debuff();
        else if(samestr(curr, "MOVE"))
        {
            move(&i, next, total_lines);
        }
        else if(samestr(curr, "MOVEZ"))
        {
            if(top() == 0) move(&i, next, total_lines);
            else i++;
        }
        else if(samestr(curr, "MOVENZ"))
        {
            if(top() != 0) move(&i, next, total_lines);
            else i++;
        }
        else
        {
            printf("Invalid instruction on line %d.\n", list[i].line);
            return 1;
        }
    }
    return 0;
}

int check_extension(const char *fspec)
{
    char *ext = strrchr(fspec, '.');
    if(samestr(ext, ".siz")) return 1;
    else return 0;
}

int main(int argc, char* argv [])
{
    FILE *fptr = fopen(argv[1], "r");

    if(fptr == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    if(!check_extension(argv[1]))
    {
        printf("Not a Sizzle file.\n");
        return 1;
    }

    struct Token token_list[1024];
    char buffer[256] = {0};
    int lexemes = 0;
    int line = 1;
    int is_str = 0;

    while(fgets(buffer, 256, fptr) != NULL)
    {
        // For each line
        int i = 0;
        while(buffer[i] != '\0')
        {
            char c = buffer[i];
            if(buffer[i] == '\r' || buffer[i] == '\n')
            {
                lexemes++;
                break;
            }
            else if(c == ' ' && is_str == 0)
            {
                lexemes++;
            }
            else
            {
                if(samestr(token_list[lexemes].lexeme, ""))
                {
                    token_list[lexemes].position = lexemes;
                    token_list[lexemes].line = line;
                }
                if(c == '\"') is_str = !is_str;
                strncat(token_list[lexemes].lexeme, &c, 1);
            }
            i++;
        }
        line++;
    }

    /*
    printf("LEXEMES: %d\n", lexemes);
    printf("[");
    for(int j = 0; j < lexemes; j++)
    {
        printf("%d - ", token_list[j].line);
        printf("%s", token_list[j].lexeme);
        printf(" - %d ", token_list[j].position);
        if(j < lexemes - 1) printf(", ");
    }
    printf("]\n");
    */

    execute(token_list, lexemes);
    
    fclose(fptr); 
    return 0;
}
