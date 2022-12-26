#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <vector>

#if defined(NCURSES_EXT_FUNCS) && NCURSES_EXT_FUNCS >= 20081102
#ifdef HAVE_SET_ESCDELAY_FUNC
set_escdelay(0);
#endif
#endif

#define DELAYSIZE 200

using namespace std;
bool trans = false;
bool filling = false;
bool buck3pressed = false; //third bucket selection detect
bool instantTransfusion = false; //instant water transfusion switcher
int buck1 = 0, buck2 = 0, buck3 = 0; //current size values
int wincond = 0; //victory condition
int buckcont1 = 7, buckcont2 = 3, buckcont3 = 9; //bucket size values
int protocolX = 30; //x axis protocol position
vector<char*>protocol;

void printwincond()
{
    mvprintw(0, 0, "3rd bucket should have: %d amount of water", wincond);
    if (instantTransfusion)
	mvprintw(1, 0, "transfusion is instant ");
    else
	mvprintw(1, 0, "transfusion is animated");
}

void printprotocol()
{
    int size = protocol.size();
    int pos = 1;
    for (int i = 0; i < size; i++)
    {
       if (i % 22 == 0)
          pos = 1;
       mvprintw(pos, protocolX, "%s\n", protocol[i]);
       pos++;
    }
}

void drawbuck()
{
//1
    mvchgat(20, 5, 3, A_NORMAL, 1, NULL);
    for (int i = 1; i <= buckcont1; ++i)
    {
        mvchgat(20 - i, 5, 1, A_NORMAL, 1, NULL);
        mvchgat(20 - i, 7, 1, A_NORMAL, 1, NULL);
    }
    mvprintw(22, 6, "%d", buck1);
    mvprintw(23, 5, "(%d)", buckcont1);
    for (int i = 19; i > 19 - buck1; --i)
    {
        mvchgat(i, 6, 1, A_NORMAL, 2, NULL);
    }
//2
    mvchgat(20, 11, 3, A_NORMAL, 1, NULL);
    for (int i = 1; i <= buckcont2; ++i)
    {
        mvchgat(20 - i, 11, 1, A_NORMAL, 1, NULL);
        mvchgat(20 - i, 13, 1, A_NORMAL, 1, NULL);
    }
    mvprintw(22, 12, "%d", buck2);
    mvprintw(23, 11, "(%d)", buckcont2);
    for (int i = 19; i > 19 - buck2; --i)
    {
        mvchgat(i, 12, 1, A_NORMAL, 2, NULL);
    }
//3
    mvchgat(20, 23, 3, A_NORMAL, 1, NULL);
    for (int i = 1; i <= buckcont3; ++i)
    {
        mvchgat(20 - i, 23, 1, A_NORMAL, 1, NULL);
        mvchgat(20 - i, 25, 1, A_NORMAL, 1, NULL);
    }
    mvprintw(22, 24, "%d", buck3);
    mvprintw(23, 23, "(%d)", buckcont3);
    for (int i = 19; i > 19 - buck3; --i)
    {
        mvchgat(i, 24, 1, A_NORMAL, 2, NULL);
    }
    mvchgat(20 - wincond, 23, 1, A_NORMAL, 3, NULL);
    mvchgat(20 - wincond, 25, 1, A_NORMAL, 3, NULL);
//print
    printwincond();
    printprotocol();
    wrefresh(stdscr);
}

void addwater(int buckind, int buck)
{
    switch (buckind)
    {
    case(0):
    {
        if (buck == buckcont1)
        {
	    protocol.push_back("transfusion from 1 to 3");
            while (buck1 != 0)
            {
                if (buck3 == buckcont3)
                    break;
                buck3++;
                buck1--;
		if (!instantTransfusion)
	    	{
			clear();
	    		drawbuck();
            		usleep(500000);
	    	}
            }
            clear();
	    drawbuck();
            trans = true;
            break;
        }
        for (int i = 7; i < 20 - buck; ++i)
        {
            clear();
            mvchgat(i, 6, 1, A_NORMAL, 2, NULL);
            wrefresh(stdscr);
            drawbuck();
            usleep(100000);
        }
        break;
    }
    case(1):
    {
        if (buck == buckcont2)
        {
	    protocol.push_back("transfusion from 2 to 3");
            while (buck2 != 0)
            {
                if (buck3 == buckcont3)
                    break;
                buck3++;
                buck2--;
	    	if (!instantTransfusion)
	    	{
			clear();
	    		drawbuck();
            		usleep(500000);
	    	}
            }
            clear();
            trans = true;
            drawbuck();
            break;
        }
        for (int i = 7; i < 20 - buck; ++i)
        {
            clear();
            mvchgat(i, 12, 1, A_NORMAL, 2, NULL);
            wrefresh(stdscr);
            drawbuck();
            usleep(100000);
        }
        break;
    }
    case(2):
    {
        if (buck == buckcont3)
        {
            break;
        }
        for (int i = 7; i < 20 - buck; ++i)
        {
            clear();
            mvchgat(i, 24, 1, A_NORMAL, 2, NULL);
            wrefresh(stdscr);
            drawbuck();
            usleep(100000);
        }
        break;
    }
    }
    printwincond();
    printprotocol();
}

void addwaterFrom3(int buckind)
{
    switch (buckind)
    {
    case(0):
    {
        while (buck1 != buckcont1)
        {
            if (buck3 == 0)
                break;
            buck3--;
            buck1++;
	    if (!instantTransfusion)
	    {
		clear();
	    	drawbuck();
            	usleep(500000);
	    }
        }
        clear();
        drawbuck();
        break;
    }
    case(1):
    {
        while (buck2 != buckcont2)
        {
            if (buck3 == 0)
                break;
            buck3--;
            buck2++;
	    if (!instantTransfusion)
	    {
		clear();
	    	drawbuck();
            	usleep(500000);
	    }
        }
        clear();
        drawbuck();
        break;
    }
    }
}

void resetbucket ()
{
    clear();
    drawbuck();
    printwincond();
    printprotocol();
}

short colors[] =
{
    COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
    COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

void refreshfield(void)
{
    napms(DELAYSIZE);
    wmove(stdscr, LINES - 1, COLS - 1);
    refresh();
}

void color(void)
{
    chtype bold = (rand() % 2) ? A_BOLD : A_NORMAL;
    attrset(COLOR_PAIR(rand() % 8) | bold);
}

void explosion(int row, int col)
{
    erase();
    mvaddstr(row, col, "-");
    refreshfield();

    --col;

    color();
    mvaddstr(row - 1, col, " - ");
    mvaddstr(row,     col, "-+-");
    mvaddstr(row + 1, col, " - ");
    refreshfield();

    --col;

    color();
    mvaddstr(row - 2, col, " --- ");
    mvaddstr(row - 1, col, "-+++-");
    mvaddstr(row,     col, "-+#+-");
    mvaddstr(row + 1, col, "-+++-");
    mvaddstr(row + 2, col, " --- ");
    refreshfield();

    color();
    mvaddstr(row - 2, col, " +++ ");
    mvaddstr(row - 1, col, "++#++");
    mvaddstr(row,     col, "+# #+");
    mvaddstr(row + 1, col, "++#++");
    mvaddstr(row + 2, col, " +++ ");
    refreshfield();

    color();
    mvaddstr(row - 2, col, "  #  ");
    mvaddstr(row - 1, col, "## ##");
    mvaddstr(row,     col, "#   #");
    mvaddstr(row + 1, col, "## ##");
    mvaddstr(row + 2, col, "  #  ");
    refreshfield();

    color();
    mvaddstr(row - 2, col, " # # ");
    mvaddstr(row - 1, col, "#   #");
    mvaddstr(row,     col, "     ");
    mvaddstr(row + 1, col, "#   #");
    mvaddstr(row + 2, col, " # # ");
    refreshfield();
}

void fireworks()
{
    time_t seed;
    int start, end, row, diff, flag, direction;
    short i;
    for (i = 0; i < 8; i++)
        init_pair(i, colors[i], COLOR_BLACK);

    seed = time((time_t *)0);
    srand(seed);
    flag = 0;
    
    //while (getch() == ERR)
    for (int i = 0; i < 5; i++)
    {
        do {
            start = rand() % (COLS - 3);
            end = rand() % (COLS - 3);
            start = (start < 2) ? 2 : start;
            end = (end < 2) ? 2 : end;
            direction = (start > end) ? -1 : 1;
            diff = abs(start - end);

        } while (diff < 2 || diff >= LINES - 2);

        attrset(A_NORMAL);

        for (row = 0; row < diff; row++)
        {
            mvaddstr(LINES - row, row * direction + start,
                (direction < 0) ? "\\" : "/");

            if (flag++)
            {
                refreshfield();
                erase();
                flag = 0;
            }
        }

        if (flag++)
        {
            refreshfield();
            flag = 0;
        }

        explosion(LINES - row, diff * direction + start);
        erase();
        refreshfield();
     }
}

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) { return ch; }
    else { return 0; }
}

int main()
{
    int i, n, t = 0, k = 0;
    srand(time(0));
    wincond = 1 + rand() % buckcont3;
    initscr(); scrollok(stdscr, true); nocbreak(); start_color();
    nodelay(stdscr, true);
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    drawbuck();
    while (1)
    {
        if (buck3 == wincond)
        {
            mvprintw(0, 0, "congrats! you won!\n");
            wmove(stdscr, LINES - 1, COLS - 1);
            wrefresh(stdscr);
	    usleep(1000000);
	    fireworks();
            return 0;
        }
        int ch = kbhit();
        if (ch != 0)
        {
            switch (ch)
            {
            case 27:
            {
                return 0;
            }
            case 32: //instant transfusion switcher
            {
		instantTransfusion ? instantTransfusion = false : instantTransfusion = true;
		printwincond();
                break;
            }
            case 49: //1
            {
                if (buck3pressed)
                {
                    if (buck1 == buckcont1) break;
		    protocol.push_back("transfusion from 3 to 1");
                    addwaterFrom3(0);
                    buck3pressed = false;
                    break;
                }
                if (!trans)
		{
		     if (buck1 == buckcont1) 
		     {
			addwater(0, buck1);
			trans = false;
		     }
		     else {
		     protocol.push_back("filling 1");
		     for (int i = buck1; i < buckcont1; i++)
		     {
			addwater(0, buck1);
			buck1++;
		     }
		     drawbuck();
		   }
		}
                else
                    trans = false;
                break;
            }
            case 50: //2
            {
                if (buck3pressed)
                {
                    if (buck2 == buckcont2) break;
		    protocol.push_back("transfusion from 3 to 2");
                    addwaterFrom3(1);
                    buck3pressed = false;
                    break;
                }
                if (!trans)
		{
		     if (buck2 == buckcont2)
		     {
			addwater(1, buck2);
			trans = false;
		     }
		     else {
		     protocol.push_back("filling 2");
		     for (int i = buck2; i < buckcont2; i++)
		     {
			addwater(1, buck2);
			buck2++;
		     }
		     drawbuck();
		   }
		}
                else
                    trans = false;
                break;
            }
            case 51: //3
            {
                buck3pressed = true;
                break;
            }
	    case 101: //e (reset third bucket)
	    {
		protocol.push_back("water released from 3");
    		buck3 = 0;
		resetbucket();
		break;
	    }
	    case 113: //q (reset first bucket)
	    {
		protocol.push_back("water released from 1");
    		buck1 = 0;
		resetbucket();
		break;
	    }
	    case 119: //w (reset second bucket)
	    {
		protocol.push_back("water released from 2");
    		buck2 = 0;
		resetbucket();
		break;
	    }
	  }
        }
    }

    endwin();
    return 0;
}