#include <windef.h>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <Commdlg.h>
#define NMAX         1000

#define OPEN_BLOCK     -1
#define CLOSE_BLOCK    -2

#define BEGIN           12
#define INSTR_SIMPLA    1
#define IF              2
#define ELSE            3
#define WHILE           4
#define FOR             5
#define REPEAT          6
#define UNTIL           7
#define SWITCH          8
#define CASE            9
#define S_DEFAULT       10
#define END             11

#define INALTIME_TEXT   29
#define KEY_UP          72 * 256
#define KEY_DOWN        80 * 256
#define LATIME_SCROLL   20
#define INDENT          28

#define LIGHTPURPLE     RGB(178,102,255)

struct code
{
    char code_instruction[256];
    int type;
    int nr_children;
    code *next;
    code *else_instr;
    code *children[NMAX];
    code *switch_default;
    int h = 0;
};

struct lines
{
    int instruction_type;
    char loc[256]; //code_instruction
};

void ClearScreen(bool faraBara = false);
int DesenNassi(code* nod, int screenW, int screenH);

void createArbore(code *s, lines LINE[], int nr_instr);
void preiaInstructiuni(lines LINE[], int &nr_instr, char szFile[]);

