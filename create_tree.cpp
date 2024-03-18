#include "nassi.h"
code* init_nod(char inf[],int instr_type)
{
    code* p = new code;
    strcpy(p->code_instruction, inf);
    p->nr_children = 0;
    p->type = instr_type;
    p->else_instr = NULL;
    p->next = NULL;
    p->switch_default = NULL;
    for(int i=0; i < NMAX; i++)
        p->children[i]=NULL;
    return p;
}
void addNodeChild(code *parent, lines LINE)
{
    code *p;
    p = init_nod(LINE.loc, LINE.instruction_type);
    parent->children[parent->nr_children] = p;
    parent->nr_children ++;
}
void addNodeElse(code *parinte, lines LINE)
{
    code *p=new code;
    p = init_nod(LINE.loc,LINE.instruction_type);
    parinte->else_instr=p;
}
void addNodeNext(code *parinte, lines LINE)
{
    code *p=new code;
    p = init_nod(LINE.loc,LINE.instruction_type);
    parinte->next=p;
}
void addNodeSwitchDefault(code *&parinte, lines LINE)
{
    code *p=new code;
    init_nod(LINE.loc,LINE.instruction_type);
    parinte->switch_default=p;
}
bool getNrDescendants(lines LINE[], int &i, int nr_instr, code *curent)
{
    i+= 2;
    int par=1;
    while(i<=nr_instr&&par)
    {
        if(LINE[i].instruction_type == OPEN_BLOCK)
        {
            i--;
            getNrDescendants(LINE,i, nr_instr, curent->children[curent->nr_children - 1]);
            if(LINE[i].instruction_type == UNTIL)
            {
                strcat(curent->children[curent->nr_children-1]->code_instruction," ");
                strcat(curent->children[curent->nr_children-1]->code_instruction, LINE[i].loc);
                i++;
            }
            par++;
        }
        else if(LINE[i].instruction_type == CLOSE_BLOCK)
        {
            par--;
            i++;
            break;
        }
         else if(LINE[i].instruction_type==S_DEFAULT)
        {
            addNodeSwitchDefault(curent,LINE[i]);
            getNrDescendants(LINE,i,nr_instr,curent->switch_default);
        }
        else
        {
            addNodeChild(curent, LINE[i]);
            i++;
        }

    }
    if(LINE[i].instruction_type == UNTIL)
    {
        strcat(curent->code_instruction," ");
        strcat(curent->code_instruction, LINE[i].loc);
        i++;
    }
    if(LINE[i].instruction_type==ELSE)
    {
        addNodeElse(curent,LINE[i]);
        getNrDescendants(LINE,i,nr_instr,curent->else_instr);
        return 1;
    }
    return 0;
    }

void createArbore(code *s, lines LINE[], int nr_instr)
{
    code *p = s;
    int i=0;
    while(i <= nr_instr)
    {
        switch(LINE[i].instruction_type)
        {
        case BEGIN:
        {
            addNodeNext(s, LINE[i]);
            s = s->next;
            i++;
            break;
        }
        case INSTR_SIMPLA:
        {

            addNodeNext(s, LINE[i]);
            s = s->next;
            i++;
            break;
        }
        case OPEN_BLOCK:
        {
            i++;
            break;
        }
        case CLOSE_BLOCK:
        {
            i++;
            break;
        }
        case UNTIL:
        {
            addNodeNext(s, LINE[i]);
            s = s->next;
            i++;
            break;
        }
        case END:
        {
            addNodeNext(s, LINE[i]);
            i++;
            break;
        }
        default:
        {
            addNodeNext(s, LINE[i]);
            s = s->next;
            bool ok = getNrDescendants(LINE, i, nr_instr,s);
            break;
        }
        }
    }
    s = p;
}
