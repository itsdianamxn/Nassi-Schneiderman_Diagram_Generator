#include <graphics.h>
#include <iostream>
#include <fstream>
#include "nassi.h"

using namespace std;
ofstream* fout = NULL;

int processHeight(code* nod);
int Desenare(code* nod, int x, int y, int w);

void openSVG(const char* name, int width, int height)
{
    fout = new ofstream(name);

    *fout << "<?xml version=\"1.0\" standalone=\"yes\"?>" <<endl;
    *fout << "<svg width=\"" <<width/2 <<"\" height=\"" <<height <<"\" x=\"0\" y=\"0\" "
          "xmlns=\"http://www.w3.org/2000/svg\">"<<endl;

    *fout<<"<style>" <<endl
         <<"    .bar {fill: "<<"white" <<";pointer-events: all;}"<<endl
         <<"    .bar:hover { fill: yellow;}"<<endl
         <<"    .if {fill: "<<"lightpink" <<";pointer-events: all;}"<<endl
         <<"    .if:hover { fill: yellow;}"<<endl
         <<"    .while {fill: "<<"lightgreen" <<";pointer-events: all;}"<<endl
         <<"    .while:hover { fill: yellow;}"<<endl
         <<"    .for {fill: "<<"#F5B041" <<";pointer-events: all;}"<<endl
         <<"    .for:hover { fill: yellow;}"<<endl
         <<"    .repeat {fill: "<<"lightblue" <<";pointer-events: all;}"<<endl
         <<"    .repeat:hover { fill: yellow;}"<<endl
         <<"    .switch {fill: "<<"lightcyan" <<";pointer-events: all;}"<<endl
         <<"    .switch:hover { fill: yellow;}"<<endl
         <<"</style>"<<endl<<endl;
}
void closeSVG()
{
    *fout<<"</svg>";
    fout->close();
    fout = NULL;
}
void lineSVG(int x1, int y1, int x2, int y2, const char* color)
{
    if (fout!=NULL)
        *fout<<"   <line x1=\"" <<x1/2 <<"\" y1=\"" <<y1 <<"\" x2=\"" <<x2/2 <<"\" y2=\"" <<y2 <<"\" stroke=\""<<color <<"\" />" <<endl;
}

void textSVG(int x, int y, const char* text, const char* color = "black")
{
    if (fout==NULL)
        return;

    // inlocuim <, > cu &lt; si &gt; respectiv
    char buff[300];
    int len = strlen(text), lenfinal=0, i;
    for(i=0; i<=len; i++)
    {
        if(text[i]=='<')
        {
            buff[lenfinal]='&';
            buff[lenfinal+1]='l';
            buff[lenfinal+2]='t';
            buff[lenfinal+3]=';';

            lenfinal+=4;
        }
        else if (text[i]=='>')
        {
            buff[lenfinal]='&';
            buff[lenfinal+1]='g';
            buff[lenfinal+2]='t';
            buff[lenfinal+3]=';';
            lenfinal+=4;

        }
        else
        {
            buff[lenfinal]=text[i];
            lenfinal++;
        }
    }
    buff[lenfinal]='\0';
    *fout<<"   <text style=\"font-family:'Lucida Bright'\" x=\"" << x/2 << "\" y=\"" << y <<
         "\" text-anchor=\"middle\" alignment-baseline=\"center\" " <<
         " fill=\"" << color << "\">" << buff << "</text>"<<endl;
}
void rectangleSVG(int x, int y, int w, int h, const char* clasa, const char* borderColor)
{
    if (fout!=NULL)
    {
        *fout<<"   <rect class=\"" <<clasa <<"\" x=\"" <<x/2 <<"\" y=\"" <<y <<"\" width=\"" <<w/2 <<"\" height=\"" <<h<<"\" stroke=\""
             <<borderColor <<"\" stroke-width=\"1\"/> "<<endl;
    }
}

int processHeightSimplu(code* instr)
{
    return instr->h = INALTIME_TEXT;
}

int desenSimplu(code* instr, int x, int y, int w)
{
    setfillstyle(SOLID_FILL, WHITE);
    bar(x, y, x+w, y+INALTIME_TEXT);

    rectangleSVG(x, y, w, INALTIME_TEXT, "bar", "black");
    setbkcolor(WHITE);
    setcolor(BLACK);
    outtextxy(x + w/2, y + INALTIME_TEXT- 5, instr->code_instruction);
    textSVG(x + w/2, y + INALTIME_TEXT- 5, instr->code_instruction, "black");
    rectangle(x, y, x + w, y + INALTIME_TEXT);
    return INALTIME_TEXT;
}

int processHeightIf(code* instr)
{
    int hDaca = 0;
    int hAltfel = 0;
    for (int i = 0; i < instr->nr_children; i++)
    {
        hDaca += processHeight(instr->children[i]);
    }
    if (instr->else_instr!=NULL)
        for(int i=0; i<instr->else_instr->nr_children; i++)
            hAltfel += processHeight(instr->else_instr->children[i]);

    return instr->h = max(hDaca, hAltfel) + 2*INALTIME_TEXT;
}

int desenIf(code* instr, int x, int y, int w)
{
    setfillstyle(SOLID_FILL, LIGHTMAGENTA);
    bar(x, y, x + w, y + instr->h);
    rectangleSVG(x, y, w, instr->h, "if", "magenta");

    setcolor(BLACK);
    setbkcolor(LIGHTMAGENTA);
    outtextxy(x + w/2, y + INALTIME_TEXT, instr->code_instruction);
    outtextxy(x + 20, y + 2*INALTIME_TEXT-5, "DA");
    outtextxy(x + w-20, y + 2*INALTIME_TEXT-5, "NU");

    setcolor(BROWN);
    line(x, y, x+w/2, y + 2*INALTIME_TEXT);
    line(x + w/2, y + 2*INALTIME_TEXT, x+w, y);
    rectangle(x, y, x + w, y + 2*INALTIME_TEXT);

    int hDaca = 0;
    int hAltfel = 0;
    for (int i = 0; i < instr->nr_children; i++)
    {
        hDaca += Desenare(instr->children[i], x, y + 2*INALTIME_TEXT + hDaca, (w+1)/2);
    }
    if (instr->else_instr!=NULL)
        for(int i=0; i<instr->else_instr->nr_children; i++)
            hAltfel += Desenare(instr->else_instr->children[i], x+(w+1)/2, y+2*INALTIME_TEXT + hAltfel, (w+1)/2);
    int maxh = max(hDaca, hAltfel);

    setcolor(BROWN);
    line(x, y + 2*INALTIME_TEXT, x+w, y + 2*INALTIME_TEXT);
    line(x+(w+1)/2, y + 2*INALTIME_TEXT, x+(w+1)/2, y + 2*INALTIME_TEXT+maxh);
    rectangle(x, y, x + w, y + 2*INALTIME_TEXT + maxh);

    lineSVG(x, y, x+w/2, y + 2*INALTIME_TEXT, "magenta");
    lineSVG(x + w/2, y + 2*INALTIME_TEXT, x+w, y, "magenta");
    lineSVG(x, y + 2*INALTIME_TEXT, x+w, y + 2*INALTIME_TEXT, "magenta");
    textSVG(x + w-30, y + 2*INALTIME_TEXT-5, "NU", "black");
    textSVG(x + 30, y + 2*INALTIME_TEXT-5, "DA", "black");
    textSVG(x + w/2, y + INALTIME_TEXT, instr->code_instruction, "black");

    return 2*INALTIME_TEXT + maxh;
}

int processHeightWhile(code* instr)
{
    int heightWhile = 0;
    for (int i = 0; i < instr->nr_children; i++)
        heightWhile += processHeight(instr->children[i]);
    return instr->h = heightWhile + INALTIME_TEXT;
}

int desenWhile(code* instr, int x, int y, int w)
{
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(x, y, x + w, y + instr->h);
    setbkcolor(LIGHTGREEN);
    setcolor(BLACK);
    outtextxy(x + w/2, y + INALTIME_TEXT-5, instr->code_instruction);

    rectangleSVG(x, y, w, instr->h, "while", "green");
    textSVG(x + w/2, y + INALTIME_TEXT-5, instr->code_instruction);

    int lengthWhile = 0;
    for(int i = 0; i < instr->nr_children; i++)
        lengthWhile += Desenare(instr->children[i], x + INDENT, y + INALTIME_TEXT + lengthWhile, w - INDENT);

    setcolor(GREEN);
    rectangle(x, y, x+w, y+lengthWhile+INALTIME_TEXT);
    setcolor(BLACK);
    line(x, y, x+w, y);
    line(x + w, y, x + w, y + INALTIME_TEXT);
    line(x + INDENT , y + INALTIME_TEXT , x + w, y + INALTIME_TEXT);
    line(x , y , x, y + INALTIME_TEXT + lengthWhile);
    line(x , y + INALTIME_TEXT + lengthWhile, x + INDENT, y + INALTIME_TEXT + lengthWhile);
    line(x + INDENT , y + INALTIME_TEXT , x + INDENT, y + INALTIME_TEXT + lengthWhile);
    return INALTIME_TEXT + lengthWhile;
}

int processHeightFor(code* instr)
{
    int heightFor = 0;
    for (int i = 0; i < instr->nr_children; i++)
        heightFor += processHeight(instr->children[i]);
    return instr->h = heightFor + INALTIME_TEXT;
}

int desenFor(code* instr, int x, int y, int w)
{
    setfillstyle(SOLID_FILL, LIGHTRED);
    bar(x, y, x + w, y + instr->h);
    setbkcolor(LIGHTRED);
    setcolor(BLACK);
    outtextxy(x + w/2, y + INALTIME_TEXT-5, instr->code_instruction);

    rectangleSVG(x, y, w, instr->h, "for", "red");
    textSVG(x + w/2, y + INALTIME_TEXT-5, instr->code_instruction);

    int lengthFor = 0;
    for(int i = 0; i < instr->nr_children; i++)
        lengthFor += Desenare(instr->children[i], x + INDENT, y + INALTIME_TEXT + lengthFor, w - INDENT);
    setcolor(BLACK);
    line(x, y, x+w, y);
    line(x + w, y, x + w, y + INALTIME_TEXT);
    line(x + INDENT , y + INALTIME_TEXT , x + w, y + INALTIME_TEXT);
    line(x , y , x, y + INALTIME_TEXT + lengthFor);
    line(x , y + INALTIME_TEXT + lengthFor, x + INDENT, y + INALTIME_TEXT + lengthFor);
    line(x + INDENT , y + INALTIME_TEXT , x + INDENT, y + INALTIME_TEXT + lengthFor);
    return INALTIME_TEXT + lengthFor;
}

int processHeightRepeat(code* instr)
{
    int heightRepeat = 0;
    for (int i = 0; i < instr->nr_children; i++)
        heightRepeat += processHeight(instr->children[i]);
    return instr->h = heightRepeat + INALTIME_TEXT;
}

int desenRepeat(code* instr, int x, int y, int w)
{
    rectangleSVG(x, y, w, instr->h, "repeat", "blue");
    textSVG(x + w/2, y + instr->h -5, instr->code_instruction);

    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(x, y, x + w, y +instr->h);

    setbkcolor(LIGHTBLUE);
    setcolor(BLACK);
    outtextxy(x + w/2, y +instr->h - 5, instr->code_instruction);

    int lengthR = 0;
    for(int i = 0; i < instr->nr_children; i++)
        lengthR  += Desenare(instr->children[i], x + INDENT, y + lengthR, w - INDENT);

    setcolor(BLUE);
    rectangle(x, y, x+w, y+lengthR+INALTIME_TEXT);

    return INALTIME_TEXT + lengthR;
}

int processHeightSwitch(code *instr)
{
    int hCazuri = (instr->nr_children+1)*INALTIME_TEXT;
    int lengthSwitchCases = 0;
    int lengthActual[instr->nr_children+3];
    for (int i=0; i<instr->nr_children; i++)
    {
        lengthActual[i]=0;
        for(int j=0; j<instr->children[i]->nr_children; j++)
            lengthActual[i] += processHeight(instr->children[i]->children[j]);
        lengthSwitchCases = max(lengthActual[i], lengthSwitchCases);
    }
    int lengthSwitchDefault=0;
    lengthActual[instr->nr_children] = 0;
    if(instr->switch_default!=NULL)
    {
        for(int i=0; i<instr->switch_default->nr_children; i++)
            lengthSwitchDefault += processHeight(instr->switch_default->children[i]);
        lengthActual[instr->nr_children] = lengthSwitchDefault;
    }
    return instr->h = hCazuri + max(lengthSwitchCases,lengthSwitchDefault);
}

int desenSwitch(code *instr, int x, int y, int w)
{
    rectangleSVG(x, y, w, instr->h, "switch", "cyan");

    int hCazuri = (instr->nr_children+1)*INALTIME_TEXT;
    setfillstyle(SOLID_FILL, LIGHTPURPLE);
    bar(x, y, x + w, y + hCazuri);
    setbkcolor(LIGHTPURPLE);
    setcolor(MAGENTA);
    rectangle(x, y, x + w, y + hCazuri);
    int x_unit = w / (instr->nr_children + 1);
    int y_unit = hCazuri/ instr->nr_children;
    line(x, y, x + w - x_unit, y+hCazuri);
    lineSVG(x, y, x + w - x_unit, y+hCazuri, "cyan");
    line(x + w - x_unit, y+hCazuri, x + w, y);
    lineSVG(x + w - x_unit, y+hCazuri, x + w, y, "cyan");
    setcolor(BLACK);
    outtextxy(x + w - x_unit, y + INALTIME_TEXT-5, instr->code_instruction);
    textSVG(x + w - x_unit, y + INALTIME_TEXT-5, instr->code_instruction);

    int lengthSwitchCases = 0;
    int lengthActual[instr->nr_children+3];
    for(int i=0; i<instr->nr_children; i++)
    {
        lengthActual[i]=0;
        setbkcolor(LIGHTPURPLE);
        setcolor(BLACK);
        outtextxy(x + i*x_unit + textwidth(instr->children[i]->code_instruction)/2, y+hCazuri - 5, instr->children[i]->code_instruction);
        textSVG(x + i*x_unit + textwidth(instr->children[i]->code_instruction), y+hCazuri - 5, instr->children[i]->code_instruction);
        setcolor(MAGENTA);
        line(x + i*x_unit, y+hCazuri, x + i*x_unit, y+i*y_unit);
        lineSVG(x + i*x_unit, y+hCazuri, x + i*x_unit, y+i*y_unit, "cyan");
        for(int j=0; j<instr->children[i]->nr_children; j++)
            lengthActual[i] +=Desenare(instr->children[i]->children[j], x + i*x_unit, y + hCazuri + lengthActual[i], x_unit);
        lengthSwitchCases = max(lengthActual[i], lengthSwitchCases);
    }
    int lengthSwitchDefault=0;
    lengthActual[instr->nr_children] = 0;
    setbkcolor(LIGHTPURPLE);
    setcolor(BLACK);
    outtextxy(x + w - x_unit/2 + 35, y + hCazuri - 5, "default:");
    textSVG(x + w - x_unit/2 + 45, y + hCazuri - 5, "default:");
    if(instr->switch_default!=NULL)
    {
        for(int i=0; i<instr->switch_default->nr_children; i++)
            lengthSwitchDefault += Desenare(instr->switch_default->children[i], x + w - x_unit, y + hCazuri + lengthSwitchDefault, x_unit);
        lengthActual[instr->nr_children] = lengthSwitchDefault;
    }
    for(int i=0; i<instr->nr_children; i++)
    {
        setfillstyle(SOLID_FILL,LIGHTPURPLE);
        bar(x + i*x_unit, y  + hCazuri + max(lengthSwitchCases,lengthSwitchDefault), x + (i+1)*x_unit, y + hCazuri + lengthActual[i]);
        setcolor(MAGENTA);
        rectangle(x + i*x_unit, y  + hCazuri + max(lengthSwitchCases,lengthSwitchDefault), x + (i+1)*x_unit, y + hCazuri + lengthActual[i]);
    }
    setfillstyle(SOLID_FILL,LIGHTPURPLE);
    bar(x + w - x_unit, y  + hCazuri + max(lengthSwitchCases,lengthSwitchDefault), x + w, y + hCazuri + lengthActual[instr->nr_children]);
    setcolor(MAGENTA);
    rectangle(x + w - x_unit, y  + hCazuri + max(lengthSwitchCases,lengthSwitchDefault), x + w, y + hCazuri + lengthActual[instr->nr_children]);
    //setcolor(BLACK);
    //rectangle(x, y , x + w, y + hCazuri + max(lengthSwitchCases,lengthSwitchDefault));
    return hCazuri + max(lengthSwitchCases,lengthSwitchDefault);
}

int processHeight(code* nod)
{
    int h = 0;
    switch (nod->type)
    {
    case BEGIN:
        for (int i = 0; i < nod->nr_children; i++)
            h += processHeight(nod->children[i]);
        break;
    case INSTR_SIMPLA:
        h += processHeightSimplu(nod);
        break;
    case IF:
        h += processHeightIf(nod);
        break;
    case WHILE:
        h += processHeightWhile(nod);
        break;
    case REPEAT:
        h += processHeightRepeat(nod);
        break;
    case FOR:
        h += processHeightFor(nod);
        break;
    case SWITCH:
        h += processHeightSwitch(nod);
        break;
    case END:
        break;
    }
    return h;
}

int Desenare(code* nod, int x, int y, int w)
{
    cout << nod->type << "\t" << nod->code_instruction << "\t" << nod->nr_children << endl;
    int h = 0;
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    switch (nod->type)
    {
    case BEGIN:
        for (int i = 0; i < nod->nr_children; i++)
            h += Desenare(nod->children[i], x, y + h, w);
        break;
    case INSTR_SIMPLA:
        h += desenSimplu(nod, x, y + h, w);
        break;
    case IF:
        h += desenIf(nod, x, y + h, w);
        break;
    case WHILE:
        h += desenWhile(nod, x, y + h, w);
        break;
    case REPEAT:
        h += desenRepeat(nod, x, y + h, w);
        break;
    case FOR:
        h += desenFor(nod, x, y + h, w);
        break;
    case SWITCH:
        h += desenSwitch(nod, x, y + h, w);
        break;
    case END:
        break;
    }
    return h;
}

int getHeightRadacina(code* nod)
{
    int h = 0;
    while (nod)
    {
        h += processHeight(nod);
        nod = nod->next;
    }
    return h;
}

void DesenareRadacina(code* nod, int x, int y, int w)
{
    int h = 0;
    while (nod)
    {
        h += Desenare(nod, x, y + h, w);
        nod = nod->next;
    }
}

int DesenNassi(code* nod, int screenW, int screenH)
{
    int totalH = getHeightRadacina(nod);

    if (totalH < screenH)
    {


        openSVG("image.svg", screenW, totalH);
        DesenareRadacina(nod, 0, 0, screenW - 1);
        closeSVG();
        setbkcolor(RGB(0xe0, 0xff, 0x15));
        setcolor(BLACK);
        outtextxy(screenW-200, screenH-3*INALTIME_TEXT, "Press ESC to return to menu.");

        int ch = getch();
        while(ch!=VK_ESCAPE)
            ch = getch();
        return totalH;
    }
    int yOffset = 0;
    int thumbh=(screenH*screenH) / totalH;
    int allStepCount=(totalH-screenH)/INALTIME_TEXT;
    int scrollStepHeight = (screenH - thumbh) / allStepCount;
    int currentStep = 0;

    openSVG("image.svg", screenW-LATIME_SCROLL, totalH);
    DesenareRadacina(nod, 0, yOffset, screenW-LATIME_SCROLL);
    closeSVG();
    setcolor(BLACK);
    setbkcolor(RGB(0xe0, 0xff, 0x15));
    outtextxy(screenW-300, screenH-4*INALTIME_TEXT, "Press ESC to return to menu.");
    outtextxy(screenW-338, screenH-3*INALTIME_TEXT, "Use the UP and DOWN keys to scroll.");

    do {
        setfillstyle(1, LIGHTGRAY);
        bar3d(screenW-LATIME_SCROLL, 0, screenW, totalH, LATIME_SCROLL, 1);
        setfillstyle(1, DARKGRAY);
        bar(screenW-LATIME_SCROLL+1, currentStep*scrollStepHeight, screenW-1, currentStep*scrollStepHeight+thumbh);

        int ch = getch();
        if (ch == 0)
        {
            ch = getch() * 256;
        }
        switch (ch)
        {
            case KEY_UP:
                if (yOffset < 0)
                {
                    yOffset += INALTIME_TEXT;
                    DesenareRadacina(nod, 0, yOffset, screenW-LATIME_SCROLL);
                    currentStep--;
                }
                break;
            case KEY_DOWN:
                if (totalH + yOffset >= screenH)
                {
                    yOffset -= INALTIME_TEXT;
                    DesenareRadacina(nod, 0, yOffset, screenW-LATIME_SCROLL);
                    setbkcolor(BLACK);
                    setfillstyle(XHATCH_FILL, WHITE);
                    bar(0, totalH+yOffset+1, screenW-LATIME_SCROLL, totalH+yOffset+INALTIME_TEXT);
                    currentStep++;
                }
                break;
            case 'q':
            case VK_ESCAPE:
                return totalH;
        }
        cout << ch << " " << yOffset << endl;

    } while (true);
    return totalH;
}
