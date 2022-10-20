#include "Game.h"
#include "Menu.h"

#include <set>
#include <fstream>
#include <ncurses.h>
#include <cassert>
#include <algorithm>

#define FPS 2

#define DEBUG 0
#define KEY_DEBUG 0

#define P1_COL 1
#define P2_COL 2
#define EN_COL 3
#define BO_COL 4
#define UW_COL 5
#define BW_COL 6
#define EX_COL 7
#define B0_COL 8
#define B1_COL 9

Game::Game ( ) {
    map = std::make_shared<Map>();
}

void Game::play ( ) {
    setup( );

    int ret;
    while ( true ) {
        ++frame;

        // spawn in new enemy every 5 sec
        if ( frame % (5 * FPS) == 0 ) map -> addEnemy ( );

        formatInput();

        if ( ( ret = map -> dangTick() ) ) break;

        map -> safeTick();

        map -> print ( );

        wait( 1000 / FPS );
    }

    end ( ret );
}

void Game::end ( const int& r ) {
    // screen blink
    for ( uint8_t i = 0; i < 4; ++i ) {
        if ( i != 0 ) {
            clear();
            refresh();
            wait(500);
        }

        clear();
        map -> printUnbreakable();
        map -> print ();

        wait( 500 );
    }

    clear();
    std::string msg = "YOU DIED ";

    if ( r == 1 ) msg += "(ENEMY CONTACT)";
    if ( r == 2 ) msg += "(EXPLOSION)";

    size_t sc = map -> getScore(), time = frame / FPS;
    std::ifstream rec ( "assets/record.txt" );
    size_t recTime, recScore;
    rec >> recTime >> recScore;
    if ( sc > recScore
        || ( sc == recScore && time > recTime ) ) {
        std::string newRecMsg = "!! NEW RECORD !!";
        mvprintw( 1, std::max(map -> mapW - (uint16_t)newRecMsg.size() / 2 + 1, 0), newRecMsg.c_str() );
        rec.close();
        std::ofstream newRec;
        newRec.open("assets/record.txt", std::ofstream::out | std::ofstream::trunc);
        newRec << time << ' ' << sc << std::endl;
        newRec.close();
    } else
        rec.close();

    mvprintw(std::max(map -> mapH / 2, 0), std::max(map -> mapW - (uint16_t)msg.size() / 2 + 1, 0), msg.c_str());
    msg = "YOU SURVIVED FOR " + std::to_string(time) + " s";
    mvprintw(std::max(map -> mapH / 2 + 2, 0), std::max(map -> mapW - (uint16_t)msg.size() / 2 + 1, 0), msg.c_str());
    msg = "AND ACHIEVED SCORE OF " + std::to_string( map -> getScore() );
    mvprintw(std::max(map -> mapH / 2 + 3, 0), std::max(map -> mapW - (uint16_t)msg.size() / 2 + 1, 0), msg.c_str());
    refresh();
    wait( 2500 );

    flushinp();
    endwin();
}

void Game::setup( ) {
    //clear log content
    std::ofstream ofs;
    ofs.open("assets/log.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

#if !KEY_DEBUG
    noecho();
#endif

    Menu menu;
    solo = menu . getPlayerCnt ( );
    std::string f = menu . getMap ( );

    nodelay(stdscr, TRUE);

    if ( ! map -> load ( f . c_str() ) )
        map -> generate();

    p1 = map -> addPlayer ( "lt" );

    if ( ! solo )
        p2 = map -> addPlayer ( "rb" );

    test ();

    init_pair(P1_COL, COLOR_GREEN,   COLOR_BLACK); // p1
    init_pair(P2_COL, COLOR_BLUE,    COLOR_BLACK); // p2
    init_pair(EN_COL, COLOR_RED,     COLOR_BLACK); // enemy
    init_pair(BO_COL, COLOR_MAGENTA, COLOR_BLACK); // bomb
    init_pair(UW_COL, COLOR_BLACK,   COLOR_WHITE); // unbreakable wall
    init_pair(BW_COL, COLOR_CYAN,    COLOR_BLACK); // breakable wall
    init_pair(EX_COL, COLOR_YELLOW,  COLOR_BLACK); // explosion
    init_pair(B0_COL, COLOR_RED,       COLOR_RED); // bomb at 0 sec till explosion
    init_pair(B1_COL, COLOR_YELLOW, COLOR_YELLOW); // bomb at 1 sec till explosion

    string intro = "Your goal is to survive as long as possible\n"
                   "and kill as many enemies as you can\n"
                   "You can kill enemies with your bombs\n"
                   "You can also collect bonuses\n"
                   "   C for increased bomb count or\n"
                   "   R for increased bomb range\n\n";

    printw ( intro.c_str() );

    int curLine = (int)std::count(intro.begin(), intro.end(), '\n');

    attron(COLOR_PAIR(P1_COL));
    printw("player 1 control:");
    p1 -> printControls ( curLine );
    attroff(COLOR_PAIR(P1_COL));

    if ( ! solo ) {
        attron(COLOR_PAIR(P2_COL));
        printw("player 2 control:");
        p2 -> printControls ( curLine );
        attroff(COLOR_PAIR(P2_COL));
    }

    refresh();

    printw("press any key to start");
    refresh();
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);

    map -> addEnemy ( );

    clear();
    map -> printUnbreakable ( );
    map -> print ( );
}

void Game::wait(const int32_t& millis) {
    clock_t time_end;
    time_end = clock() + millis * CLOCKS_PER_SEC/1000;
    while (clock() < time_end);
}

char Game::keyToChar(const int& key) {
    char ret;
    switch (key) {
        case ' ':
            ret = '_';
            break;

        case KEY_UP:
            ret = '^';
            break;

        case KEY_DOWN:
            ret = 'v';
            break;

        case KEY_LEFT:
            ret = '<';
            break;

        case KEY_RIGHT:
            ret = '>';
            break;

        default:
            ret = (char)key;
            break;
    }

    return ret;
}

void Game::formatInput() const {
    int c;
    int p1M = 0, p2M = 0, // player movements
        p1B = 0, p2B = 0; // player bomb placement
    while ((c = getch()) != ERR) {
        if ( p1M == 0 && p1 -> isControl( c ) ) {
            if ( c == p1 -> pBomb )
                p1B = c;
            else
                p1M = c;
        } else if ( p2 != nullptr
            && p2M == 0 && p2 -> isControl ( c ) ) {
            if ( c == p2 -> pBomb )
                p2B = c;
            else
                p2M = c;
        }
    }
    if ( ! solo && p2B != 0 ) ungetch ( p2B );
    if ( p1B != 0 ) ungetch ( p1B );
    if ( ! solo && p2M != 0 ) ungetch ( p2M );
    if ( p1M != 0 ) ungetch ( p1M );
}

void Game::test () {
#if DEBUG
    if ( f == "examples/mapTest01" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line == "illegal map size" );
        std::getline( ctrl, line );
        assert ( line == "0 : 0" );
    }
    if ( f == "examples/mapTest02" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line . empty () );
    }
    if ( f == "examples/mapTest03" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line . empty () );
    }
    if ( f == "examples/mapTest04" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line == "illegal map size" );
        std::getline( ctrl, line );
        assert ( line == "0 : 0" );
    }
    if ( f == "examples/mapTest05" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line . empty () );
    }
    if ( f == "examples/mapTest06" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line . empty () );
    }
    if ( f == "examples/mapTest07" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line == "illegal map size" );
        std::getline( ctrl, line );
        assert ( line == "3 : 3" );
    }
    if ( f == "examples/mapTest08" ) {
        std::ifstream ctrl ( "assets/log.txt" );
        if ( ! ctrl . is_open ( ) ) break;
        std::string line;
        std::getline( ctrl, line );
        assert ( line == "illegal map size" );
        std::getline( ctrl, line );
        assert ( line == "33 : 33" );
    }

    if ( solo ) assert ( p2 == nullptr );
#endif
}
