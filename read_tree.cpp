#include <cstring>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <Commdlg.h>
#include "nassi.h"
#include <string>
using namespace std;


void convertesteSintaxaCorect(char s[])
{
    int i=0;
    while(s[i]==' ' || s[i]=='\t')
    {
        strcpy(s+i,s+i+1);
    }
}

void preiaInstructiuni(lines LINE[], int &nr_instr,char szFile[])
{
    string instruct;
    fstream OpFile;
    OpFile.open(szFile,ios::in);

    while(getline(OpFile,instruct))
    {
        char instr[256];
        strcpy(instr,instruct.c_str());
        convertesteSintaxaCorect(instr);
        if(strlen(instr)==0)
        {
            continue;
        }
        if (strncmp(instr, "{", 1) == 0) //-1
        {
            LINE[nr_instr].instruction_type = OPEN_BLOCK;
        }
        else if (strncmp(instr, "}", 1) == 0)   //-2
        {
            LINE[nr_instr].instruction_type = CLOSE_BLOCK;
        }
        else if (strncmp(instr, "begin", 5) == 0)   ///0
        {
            LINE[nr_instr].instruction_type = BEGIN;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if (strncmp(instr, "if", 2) == 0)   ///2
        {
            LINE[nr_instr].instruction_type = IF;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if (strncmp(instr, "else", 4) == 0)   ///3
        {

            LINE[nr_instr].instruction_type = ELSE;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if (strncmp(instr, "while", 5) == 0)   ///4
        {
            LINE[nr_instr].instruction_type = WHILE;
            strcpy(LINE[nr_instr].loc,instr);
        }
        else if (strncmp(instr, "for", 3) == 0)   ///5
        {
            LINE[nr_instr].instruction_type = FOR;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if (strncmp(instr, "repeat", 6) == 0)   ///6
        {
            LINE[nr_instr].instruction_type = REPEAT;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if (strncmp(instr, "until", 5) == 0)   ///7
        {
            LINE[nr_instr].instruction_type = UNTIL;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if(strncmp(instr, "switch", 6)==0)
        {
            LINE[nr_instr].instruction_type = SWITCH;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if(strncmp(instr, "case", 4)==0)
        {
            LINE[nr_instr].instruction_type = CASE;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if(strncmp(instr, "default", 7)==0)
        {
            LINE[nr_instr].instruction_type = S_DEFAULT;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else if(strncmp(instr, "end", 3)==0)
        {
            LINE[nr_instr].instruction_type = END;
            strcpy(LINE[nr_instr].loc, instr);
        }
        else   ///variabile - atribuiri - 1
        {
            LINE[nr_instr].instruction_type = INSTR_SIMPLA;
            strcpy(LINE[nr_instr].loc, instr);
        }
        nr_instr++;
    }
    OpFile.close();
}


