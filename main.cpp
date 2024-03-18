#include <iostream>
#include <graphics.h>
#include <fstream>
#include "nassi.h"
using namespace std;

char menu_entry[5][30] = {"Load", "Draw", "Open in Browser", "Instructions", "Quit"};
#define COL_ACTIVE_0    RED
#define COL_ACTIVE_1    LIGHTBLUE
#define COL_ACTIVE_2    LIGHTGREEN
#define COL_INACTIVE    WHITE
#define COL_DELETE      BLACK
int y_menu_item;

int F_LATIME, F_LUNGIME;
void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;

   const HWND hDesktop = GetDesktopWindow();
   GetWindowRect(hDesktop, &desktop);
   horizontal = desktop.right;
   vertical = desktop.bottom;
}
char szFile[1000];
void getFile()
{
    OPENFILENAME ofn;
    ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL  ;
	ofn.lpstrFile = szFile ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;
	ofn.nMaxFileTitle = 0 ;
	ofn.lpstrInitialDir=NULL ;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;

	GetOpenFileName( &ofn );
}

void ClearScreen(bool faraBara)
{
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, F_LATIME - (faraBara*LATIME_SCROLL), F_LUNGIME);
}

void citesteSiDeseneaza()
{
    int nr_instr = 0;
    lines LINE[NMAX];
    preiaInstructiuni(LINE, nr_instr, szFile);

    code *instruction = new code;
    instruction->type=OPEN_BLOCK;
    strcpy(instruction->code_instruction, "OPEN_BLOCK");
    createArbore(instruction, LINE, nr_instr);
    instruction=instruction->next;

    for(int i=0; i<=nr_instr; i++)
       cout<<LINE[i].instruction_type<<" ";
    cout<<endl;

    ClearScreen();
    DesenNassi(instruction, F_LATIME, F_LUNGIME);
}

void drawMenuItem(int item, int x, int y, bool active, int f_active)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    bar(F_LATIME/2-100, y-10, F_LATIME/2+ 100, y + 30);
    if(!f_active)
    {
    setcolor(active ? COL_ACTIVE_0 : COL_DELETE);
    setbkcolor(WHITE);
    outtextxy(x, y, menu_entry[item]);

    setcolor(active ? COL_ACTIVE_0 : COL_INACTIVE);
    rectangle(F_LATIME/2-100, y-10, F_LATIME/2+ 100, y + 30);
    }
    else if(f_active == 1)
    {
        setcolor(active ? COL_ACTIVE_1 : COL_DELETE);
    setbkcolor(WHITE);
    outtextxy(x, y, menu_entry[item]);

    setcolor(active ? COL_ACTIVE_1 : COL_INACTIVE);
    rectangle(F_LATIME/2-100, y-10, F_LATIME/2+ 100, y + 30);
    }
    else
    {
         setcolor(active ? COL_ACTIVE_2 : COL_DELETE);
    setbkcolor(WHITE);
    outtextxy(x, y, menu_entry[item]);

    setcolor(active ? COL_ACTIVE_2 : COL_INACTIVE);
    rectangle(F_LATIME/2-100, y-10, F_LATIME/2+ 100, y + 30);
    }
}

void moveActive(int src, int dst, int f_deschis)
{
    drawMenuItem(src, F_LATIME/2, y_menu_item + 50 * src, false, f_deschis);
    drawMenuItem(dst, F_LATIME/2, y_menu_item + 50 * dst, true, f_deschis);
}

void drawMenu(int active,int f_deschis)
{
    readimagefile("background.jpg",0,0,F_LATIME,F_LUNGIME);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    for (int i = 0; i < 5; i++)
    {
        drawMenuItem(i, F_LATIME/2, y_menu_item + 50 * i, i == active, f_deschis);
    }
}

//driver program

void WriteFromFile()
{
    ClearScreen();
    readimagefile("background_clear.jpg", 0, 0, F_LATIME, F_LUNGIME);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setcolor(WHITE);
    ifstream fin("INSTRUCTIUNI_RO.txt");
    char linie_cod[250];
    int k = 1, v = 1;
    while(fin.getline(linie_cod, 250))
    {
        if(k*INALTIME_TEXT < 700)
        {
            setbkcolor(getpixel(350, k*INALTIME_TEXT + 100));
            outtextxy(300, k*INALTIME_TEXT+ 100, linie_cod);
            k++;
        }
        else
        {
            setbkcolor(getpixel(910, v*INALTIME_TEXT + 100));
            outtextxy(900, v*INALTIME_TEXT + 100, linie_cod);
            v++;
        }

    }
    settextjustify(LEFT_TEXT, BOTTOM_TEXT);
    setcolor(YELLOW);
    setbkcolor(getpixel(1700, 925));
    outtextxy(1555, 920, "Press any key to return to menu...");
    fin.close();

    getch();
    ClearScreen();
}

int main()
{
    F_LATIME = 0 ;
    F_LUNGIME = 0;
    GetDesktopResolution(F_LATIME,F_LUNGIME);
    y_menu_item = F_LUNGIME - F_LUNGIME/3;
    initwindow(F_LATIME, F_LUNGIME, "Nassi-Schneiderman Diagrams", 0, 0);
    readimagefile("background.jpg", 0 ,0 , F_LATIME, F_LUNGIME);
    int active = 0;
    int f_deschis = 0;
    drawMenu(active,f_deschis);

    bool ok=false;
    bool ok_drawn = false;
    do
    {
        int ch = getch();
        if (ch == 0)
        {
            ch = getch() * 256;
        }
        switch (ch)
        {
            case KEY_UP:
                if (active > 0)
                {
                    moveActive(active, active-1,f_deschis);
                    active--;
                }
                break;
            case KEY_DOWN:
                if (active < 4)
                {
                    moveActive(active, active + 1,f_deschis);
                    active++;
                }
                break;
            case 'q':
            case VK_ESCAPE:
                active = -1;
                closegraph();
                return 0;
            case VK_RETURN:
                switch (active)
                {
                    case 0:
                        getFile();
                        cout<<szFile<<'\n';
                        ok = szFile[0]!=NULL;
                        f_deschis = 1;
                        drawMenu(active,f_deschis);
                        break;
                    case 1:
                    {
                        if (!ok)
                            showerrorbox("Please select a file containing the code you want to be represented as a diagram.");
                        else
                        {
                            citesteSiDeseneaza();
                            f_deschis = 2;
                            ClearScreen();
                            drawMenu(active,f_deschis);
                        }
                        break;
                    }
                    case 2:
                        if (!ok)
                            showerrorbox("Please select a file containing the code you want to represent as a diagram.");
                        else if(f_deschis==1)
                            showerrorbox("Please draw the diagram before opening it in browser.");
                        else
                            system("image.svg");
                        drawMenu(active, f_deschis);
                        break;
                    case 3:
                        WriteFromFile();
                        drawMenu(active,f_deschis);
                        break;

                    case 4:
                    {
                        closegraph();
                        return 0;
                    }
                }
                break;
        }
    }while (active >= 0);
    closegraph();
}
