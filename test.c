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

using namespace std;
bool trans = false;
bool filling = false;
bool buck3pressed = false; //third bucket selection detect
bool instantTransition = false; //instant water transition switcher
int buck1 = 0, buck2 = 0, buck3 = 0; //current size values
int wincond = 0; //victory condition
int buckcont1 = 7, buckcont2 = 3, buckcont3 = 9; //bucket size values
int protocolX = 30; //x axis protocol position
vector<char*>protocol;

void printwincond()
{
    mvprintw(0, 0, "3rd bucket should have: %d amount of water", wincond);
}

void printprotocol()
{
    int size = protocol.size();
    int i = 0;
    if (size > 23) i = 22;
    for (; i < size; i++)
    mvprintw(i+1, protocolX, "%s\n", protocol[i]);
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
		if (!instantTransition)
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
	    	if (!instantTransition)
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
	    if (!instantTransition)
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
	    if (!instantTransition)
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
    drawbuck();
    while (1)
    {
        if (buck3 == wincond)
        {
            mvprintw(0, 0, "congrats! you won!\n");
            wrefresh(stdscr);
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
            case 32:
            {
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
	    case 113: //q (instant transition switcher)
	    {
		instantTransition ? instantTransition = false : instantTransition = true;
		break;
	    }
	  }
        }
    }

    endwin();
    return 0;
}