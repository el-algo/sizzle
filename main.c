#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define stack_len 8

int stack[stack_len];
int sp = -1;

void init_stack(int* stack, int len)
{
    for(int i = 0; i < len; i++)
    {
        stack[i] = 0;
    }
}

void prints(int* stack, int len)
{
    //printf("%d [", sp);
    printf("[");
    int i;
    for(i = 0; i < len - 1; i++)
    {
        printf("%d, ", stack[i]);
    }
    printf("%d] -> %d\n", stack[i], stack[sp]);
}

void push(int val)
{
    if(sp == stack_len - 1)
    {
        printf("Overflow...\n");
    }
    else stack[++sp] = val;
}

int pop()
{
    if(sp == -1) printf("Underflow...\n");
    else
    {
        return stack[sp--];
    }
}

int top()
{
    printf("TOP -> %d\n", stack[sp]);
    return stack[sp];
}

void add()
{
    int a = pop();
    int b = pop();
    push(b + a);
}

void sub()
{
    int a = pop();
    int b = pop();
    push(b - a);
}

void mul()
{
    int a = pop();
    int b = pop();
    push(b * a);
}

void divi()
{
    int a = pop();
    int b = pop();
    push(b / a);
    push(b % a);
}

void halt()
{
    getchar();
}

void duplicate()
{
    int a = pop();
    push(a);
    push(a);
}

void ask()
{
	int n;
	scanf("%d", &n);
	push(n);
}

void wait(){halt();}
void give(int n){push(n);}

void say(char* str){printf("%s\n", str);}
void take(){pop();}
void show(){prints(stack, stack_len);};

void copy(){duplicate();}

void hit(){sub();}
void heal(){add();}

void buff(){mul();}
void debuff(){divi();}

struct Token {
    char lexeme[128];
    int position;
};

int samestr(char *a, char *b)
{
	if(strcmp(a, b) == 0) return 1;
	else return 0;
}

char* str_clean(char *str)
{
    int len = strlen(str) - 1;
    if(str[0] == '"' && str[len] == '"')
    {
        str[len] = '\0';
        return str + 1;
    }
    return str;
}

int execute(struct Token list[1024], int lexemes)
{
    for(int i = 0; i < lexemes; i++)
    {
        char *curr = list[i].lexeme;
        char *next = list[i + 1].lexeme;
        
        if(samestr(curr, "ASK")) ask();
        else if(samestr(curr, "WAIT")) wait();
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
            if(next[0] == '"')
            {
                say(str_clean(next));
                i++;
            }
        }
        else if(samestr(curr, "TAKE")) take();
        else if(samestr(curr, "SHOW")) show();
        else if(samestr(curr, "TOP")) top();
        else if(samestr(curr, "COPY")) copy();

        else if(samestr(curr, "HIT")) hit();
        else if(samestr(curr, "HEAL")) heal();

        else if(samestr(curr, "BUFF")) buff();
        else if(samestr(curr, "DEBUFF")) debuff();
        else
        {
            printf("Instruccion %d invalida.\n", i);
            return 1;
        }
    }
    return 0;
}


int main()
{
    init_stack(stack, stack_len);
        
    char test[] = "SAY \"Metele un numero\"\nGIVE 2\nASK\nSHOW\nBUFF\nSHOW\nSAY \"Te lo duplique!\"\nTOP";
    struct Token token_list[1024];
    int lexemes = 0;
    char *c = &test[0];

    int is_str = 0;
    while(*c != '\0')
    {
        if(*c == '\n' || (*c == ' ' && is_str == 0))
        {
           	lexemes++;
		}
        else
        {
            if(samestr(token_list[lexemes].lexeme, ""))
            {
                token_list[lexemes].position = lexemes;
            }
            if(*c == '\"') is_str = !is_str;
            
            strncat(token_list[lexemes].lexeme, c, 1);
        }
        c++;
    }
    
    lexemes++;
    /*
    printf("[");
    int i;
    for(i = 0; i < lexemes; i++)
    {
        printf("%s", token_list[i].lexeme);
		//printf(" - %d ", token_list[i].position);
        if(i < lexemes - 1) printf(", ");
    }
    printf("]\n");
    */

    execute(token_list, lexemes);

    /*
    while(1)
    {
        printf("What to do?\n");
        printf("1. PUSH - 2. POP\n3. prints - 4. TOP\n5. EXIT\n");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Number? ");
                scanf("%d", &num);
                push(num);
                break;
            case 2:
                pop(num);
                break;
            case 3:
                prints(stack, stack_len);
                break;
            case 4:
                top();
                break;
            case 5:
                return 0;
            default:
                printf("Not a valid choice\n");
        }
    }
    */

    return 0;
}
