#include "Menu.h"

#include <string>
#include <filesystem>
#include <ncurses.h>

#define OPTION_HIGHLIGHT 69

bool Menu::getPlayerCnt( ) {
    selected = 0;
    int c;
    while ( true ) {
        displaySolo();
        c = getch();
        if ( c == KEY_LEFT ) {
            --selected;
            selected = (selected + 2) % 2;
        }
        if ( c == KEY_RIGHT ) {
            ++selected;
            selected = (selected + 2) % 2;
        }
        if ( c == '\n' ) {
            clear();
            return selected == 0;
        }
    }
}

std::string Menu::getMap( ) {
    selected = 0;
    namespace fs = std::filesystem;
    files . emplace_back ( "generate" );
    for ( const auto& i : fs::directory_iterator("examples") )
        files.push_back(i.path().filename());

    for ( const auto& i : files )
        if ( i.length() > maxW ) maxW = i.length();

    maxW = std::max ( maxW, mapSelectMess.length() );

    maxW += 4; //additional spacing

    int c;
    while ( true ) {
        displayMaps();
        c = getch();
        if (c == KEY_UP) {
            --selected;
            selected = (selected + (int)files.size()) % (int)files.size();
        }
        if ( c == KEY_DOWN ) {
            ++selected;
            selected = (selected + (int)files.size()) % (int)files.size();
        }

        if ( c == '\n' ) {
            clear();
            //selected generate => return no file
            if ( selected == 0 ) return "";
            return "examples/" + files[selected];
        }
    }
}

void Menu::displaySolo ( ) const {
    clear();

    init_pair(OPTION_HIGHLIGHT, COLOR_BLACK, COLOR_WHITE);

    printw("################\n");
    printw("# ** select ** #\n");
    printw("#              #\n");
    printw("#  ");
    if ( selected == 0 )
        attron(COLOR_PAIR(OPTION_HIGHLIGHT));
    printw("solo");
    attroff(COLOR_PAIR(OPTION_HIGHLIGHT));
    printw("   ");
    if ( selected == 1 )
        attron(COLOR_PAIR(OPTION_HIGHLIGHT));
    printw("duo");
    attroff(COLOR_PAIR(OPTION_HIGHLIGHT));
    printw("  #\n");
    printw("#              #\n");
    printw("################\n");

    refresh();
}

void Menu::displayMaps() const {
    clear();
    for ( size_t i = 0; i < maxW; ++i ) printw( "#" );
    printw("\n");
    printw("#");
    mvprintw( 1, (maxW - mapSelectMess.length() + 1) / 2, mapSelectMess.c_str() );
    mvprintw( 1, 0, "#" );
    mvprintw( 2, 0, "#" );
    mvprintw( 1, (int)maxW - 1, "#" );
    mvprintw( 2, (int)maxW - 1, "#" );
    printw("\n");

    for ( size_t i = 0; i < files.size(); ++i ) {
        printw ( "#" );
        if ( (int)i == selected )
            attron(COLOR_PAIR(OPTION_HIGHLIGHT));
        mvprintw ( (int)i + 3, (int)(maxW - files[i].size()) / 2, files[i].c_str() );
        attroff(COLOR_PAIR(OPTION_HIGHLIGHT));
        mvprintw ( (int)i + 3, (int)maxW - 1, "#\n" );
    }

    for ( size_t i = 0; i < maxW; ++i ) printw( i == 0 || i == maxW - 1 ? "#" : " " );
    printw("\n");
    for ( size_t i = 0; i < maxW; ++i ) printw( "#" );
}
