#include "Map.h"
#include "Wall.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <random>

#define P1_COL 1
#define P2_COL 2
#define UW_COL 5

size_t Map::getScore() const {
    return score;
}

bool Map::load ( const char *file ) {
    std::ifstream myFile ( file );

    if ( myFile . is_open() ) {
        std::string line;

        // read 1st line
        myFile >> mapW >> mapH;
        getline( myFile, line );

        if ( mapW < minW || mapW > maxW
            || mapH < minH || mapH > maxH ) {
            std::ofstream err ( "assets/log.txt" );
            if ( err . is_open() ) {
                err << "illegal map size" << std::endl;
                err << mapW << " : " << mapH << std::endl;
                err . close();
            }
            myFile . close();
            return false;
        }

        // resize map vector and lines to given W and H (+2 for borders)
        map.resize( mapH + 2);
        for ( auto& i : map )
            i.resize( mapW + 2 );

        generateWalls ();

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<std::mt19937::result_type> tier(1, 3);

        // read the map
        size_t loadedLines = 0;
        while ( getline( myFile, line ) && loadedLines++ < mapH ) {
            size_t i = 1;
            for ( ; i <= line.length() && (int32_t)i < mapW + 1; ++i)
                switch (line[i - 1]) {
                    case '#':
                        map[loadedLines][i] = std::make_shared<Wall>();
                        break;

                    case '*':
                        map[loadedLines][i] = std::make_shared<Wall>(true);
                        break;

                    case 'E':
                        map[loadedLines][i] = std::make_shared<Enemy>(loadedLines, i, tier(rng));
                        break;

                    case 'B':
                        map[loadedLines][i] = std::make_shared<Bomb>(loadedLines, i, 1, 0);
                        break;

                    case '1':
                        map[loadedLines][i] = std::make_shared<Buff>(tier(rng), 'R');
                        break;

                    case '2':
                        map[loadedLines][i] = std::make_shared<Buff>(tier(rng), 'C');
                        break;

                    default:
                        if ( map[loadedLines][i] == nullptr )
                            map[loadedLines][i] = std::make_shared<Object>();
                        break;
                }
            // fill in blank spaces
            for ( ; (int32_t) i < mapW + 1; ++i )
                if ( map[loadedLines][i] == nullptr )
                    map[loadedLines][i] = std::make_shared<Object>(Object());
        }

        while ( loadedLines++ < mapH )
            for ( size_t i = 1; i <= mapW; ++i )
                if ( map[loadedLines][i] == nullptr )
                    map[loadedLines][i] = std::make_shared<Object>(Object());

    } else {
        std::ofstream err ( "assets/log.txt" );
        if ( err . is_open() ) {
            err << "unable to open file" << std::endl;
            err . close ( );
        }
        myFile . close ( );
        return false;
    }

    myFile . close ( );
    return true;
}

void Map::printUnbreakable ( ) const {
    attron(COLOR_PAIR(UW_COL));
    for ( int y = 0; y < (int) map . size(); ++y )
        for ( int x = 0; x < (int) map[y] . size(); ++x )
            if ( map[y][x] -> charSymbol == '#' )
                mvaddch(y, x, '#');
    attroff(COLOR_PAIR(UW_COL));
}

void Map::print ( ) const {
    // print map
    for ( int y = 1; y < (int) map . size() - 1; ++y ) {
        move ( y, 1 );
        for ( int x = 1; x < (int) map[y] . size() - 1; ++x )
            if ( map[y][x] -> charSymbol == 'P' ) {
                if ( map[y][x] == p1 )
                    attron(COLOR_PAIR(P1_COL));
                if ( map[y][x] == p2 )
                    attron(COLOR_PAIR(P2_COL));
                map[y][x] -> print ();
                attroff(COLOR_PAIR(P1_COL));
                attroff(COLOR_PAIR(P2_COL));
            } else map[y][x]->print();
    }

    // print current player stats
    if ( p2 != nullptr ) {
        mvprintw( mapH / 2 - 1, mapW + 3, "p1 range: ");
        printw( std::to_string(p1 -> range()) . c_str() );
        mvprintw( mapH / 2, mapW + 3, "p1 bomb limit: ");
        printw( std::to_string(p1 -> limit()) . c_str() );

        mvprintw( mapH / 2 + 2, mapW + 3, "p2 range: ");
        printw( std::to_string(p2 -> range()) . c_str() );
        mvprintw( mapH / 2 + 3, mapW + 3, "p2 bomb limit: ");
        printw( std::to_string(p2 -> limit()) . c_str() );
    } else {
        mvprintw( mapH / 2, mapW + 3, "p range: ");
        printw( std::to_string(p1 -> range()) . c_str() );
        mvprintw( mapH / 2 + 1, mapW + 3, "p bomb limit: ");
        printw( std::to_string(p1 -> limit()) . c_str() );
    }

    // print score
    std::string sc = "score: " + std::to_string ( score );
    mvprintw ( mapH + 2, std::max((mapW - (int)sc.length() + 1) / 2, 0), sc . c_str() );

    refresh();
}

std::shared_ptr<Player> Map::addPlayer( const std::string& loc ) {
    uint16_t x = 0, y = 0;

    if ( ! findBlankSpot( x, y, loc ) ) return nullptr;

    auto toAdd = nextPlayer( x, y );
    map[y][x] = toAdd;
    // make sure p1 is first in the list
    if ( !movList.empty() && (*movList.begin()) -> charSymbol == 'P' ) {
        auto tmp = *movList.begin();
        movList.erase(movList.begin());
        movList . push_front(toAdd);
        movList . push_front(tmp);
    } else
        movList . push_front(toAdd);
    return toAdd;
}

void Map::addEnemy(uint16_t x, uint16_t y) {
    if ( movList . size() >= (size_t)(mapW * mapH * (enemyLimit / 100.0)) ) return;

    double_t MIN_DIST = 3.0f;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> xRan(1, mapW);
    std::uniform_int_distribution<std::mt19937::result_type> yRan(1, mapH);
    size_t itCnt = 0;
    for ( ; itCnt < 5; ++itCnt) {
        x = xRan(rng);
        y = yRan(rng);
        auto dist = sqrt(((p1->x - x) * (p1->x - x)) + ((p1->y - y) * (p1->y - y))); // dist = sqrt(xDist^2 + yDist^2)
        if ( p2 != nullptr ) {
            auto d2 = sqrt(((p2->x - x) * (p2->x - x)) + ((p2->y - y) * (p2->y - y)));
            if ( d2 < dist ) dist = d2;
        }
        if ( map[y][x]->charSymbol == ' ' && dist > MIN_DIST ) break;
    }

    if ( itCnt >= 5 ) return;
    std::uniform_int_distribution<std::mt19937::result_type> tier(1, 10);
    uint8_t t = tier(rng);
    if ( t >= 5 && t <= 8 )  t = 2;
    else if ( t >= 9 && t <= 10 ) t = 3;
    else t = 1;

    auto toAdd = std::make_shared<Enemy>(x, y, t);
    map[y][x] = toAdd;
    movList.emplace_back(toAdd);
}

void Map::addBuffRand(const uint8_t& prob) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> spawn(0, 99);
    std::uniform_int_distribution<std::mt19937::result_type> xRan(1, mapW);
    std::uniform_int_distribution<std::mt19937::result_type> yRan(1, mapH);
    if ( spawn(rng) < prob ) {
        uint16_t x = xRan(rng);
        uint16_t y = yRan(rng);
        if ( map[y][x]->charSymbol == ' ' ) {
            std::uniform_int_distribution<std::mt19937::result_type> tier(1, 100);
            uint8_t t = tier(rng);
            if ( t < 50 ) t = 1;
            else if ( t < 90 ) t = 2;
            else t = 3;
            std::uniform_int_distribution<std::mt19937::result_type> type(0, 1);
            if ( type(rng) /*== 1*/ )
                map[y][x] = std::make_shared<Buff>(t, 'C');
            else /* == 0 */
                map[y][x] = std::make_shared<Buff>(t, 'R');
        }
    }
}

void Map::generate() {
    mapW = 15; mapH = 7;

    // resize map vector and lines to given W and H (+2 for borders)
    map.resize( mapH + 2);
    for ( auto& i : map )
        i.resize( mapW + 2 );

    for ( size_t y = 1; y <= mapH; ++y )
        for ( size_t x = 1; x <= mapW; ++x )
            if ( ! ( x % 2 == y % 2 && x % 2 == 0 ) )
                map[y][x] = std::make_shared<Object>(Object ());

    generateWalls ();
}

void Map::placeBombs ( ) {
    for ( int c = getch(); c != ERR; c = getch () ) {
        std::shared_ptr<Player> p;
        size_t pl = 0;
        if ( c == p1 -> pBomb ) { pl = 1; p = p1; }
        if ( p2 != nullptr
            && c == p2 -> pBomb ) { pl = 2; p = p2; }
        if ( p == nullptr ) return;

        if ( p -> addBomb( map ) ) {
            auto bomb = std::make_shared<Bomb>(p -> x + p -> xDir, p -> y + p -> yDir, 1, pl);
            map [p -> y + p -> yDir][p -> x + p -> xDir] = bomb;
            bombList . emplace_back ( bomb );
        }
    }
}

int  Map::tickBombs ( ) {
    for ( auto [x, y] : explosionList )
        map[y][x] = std::make_shared<Object>();
    explosionList.clear();

    for ( auto j = bombList.begin(); j != bombList.end(); ++j) {
        auto i = *j;
        if ( i -> tick() ) { //explode?
            if ( i -> player == 1 )
                p1 -> delBomb();

            if ( i -> player == 2 )
                p2 -> delBomb();

            if ( explode ( i ) == 1 ) // hit player
                return 2;

            map[i -> y][i -> x] = std::make_shared<Object>();
            auto tmp = j;
            ++j;
            bombList.erase( tmp );
        }
    }

    delHits();

    return false;
}

int  Map::dangTick() {
    int ret;
    if ( (ret = tickBombs()) || (ret = moveMovs()) ) return ret;
    return ret;
}

void Map::safeTick() {
    placeBombs();
    addBuffRand( 10 );
    p1 -> tickBuffs();
    if ( p2 != nullptr) p2 -> tickBuffs();
}

int  Map::explode( std::shared_ptr<Bomb>& b ) {
    uint8_t r = 0;
    if ( b -> player == 1 ) r = p1 -> range();
    if ( b -> player == 2 ) r = p2 -> range();

    return b -> explode ( map, r, hitList, explosionList );
}

int  Map::moveMovs ( ) {
    for ( const auto& i : movList ) {
        uint8_t oldX = i -> x, oldY = i -> y;

        i -> move( map );

        uint8_t newX = i -> x, newY = i -> y;
        if ( oldX == newX && oldY == newY ) continue;
        if ( ( map[oldY][oldX] -> charSymbol == 'E' ) && map[newY][newX] -> charSymbol == 'P' ) {
            map[newY][newX] = map[oldY][oldX];
            map[oldY][oldX] = std::make_shared<Object>();
            return 1;
        }
        if ( ( map[oldY][oldX] -> charSymbol == 'P' ) && map[newY][newX] -> charSymbol == 'E' ) {
            map[oldY][oldX] = std::make_shared<Object>();
            return 1;
        }
        if ( map[oldY][oldX] -> charSymbol == 'P'
            && ( map[newY][newX] -> charSymbol == '-'
                 || map[newY][newX] -> charSymbol == '|' ) ) {
            map[newY][newX] = map[oldY][oldX];
            map[oldY][oldX] = std::make_shared<Object>();
            return 2;
        }
        if ( map[newY][newX] -> charSymbol == 'C'
            || map[newY][newX] -> charSymbol == 'R'
            || map[newY][newX] -> charSymbol == ' ') {
            map[newY][newX] = map[oldY][oldX];
            map[oldY][oldX] = std::make_shared<Object>();
        }
    }

    return 0;
}

void Map::delHits ( ) {
    for ( auto [x, y] : hitList )
        if ( map[y][x] -> charSymbol == 'E' ) {
            ++ score;
            for (auto j = movList.begin(); j != movList.end(); ++j) {
                auto k = *j;
                if (k->x == x && k->y == y) {
                    map[y][x] = std::make_shared<Object>();
                    auto tmp = j;
                    ++j;
                    movList.erase(tmp);
                }
            }
        }
        else if ( map[y][x] -> charSymbol == '*' )
            map[y][x] = std::make_shared<Object>();
    hitList.clear();
}

bool Map::findBlankSpot( uint16_t& x, uint16_t& y, const std::string& loc) const {
    uint16_t xBase;
    int8_t xDir = 0, yDir = 0;

    if ( loc.length() != 2 ) return false;

    switch (loc[0]) {
        case 'l':
            x = 1;
            xDir = 1;
            break;

        case 'r':
            x = mapW;
            xDir = -1;
            break;
    }

    switch (loc[1]) {
        case 't':
            y = 1;
            yDir = 1;
            break;

        case 'b':
            y = mapH;
            yDir = -1;
            break;
    }

    xBase = x;

    if ( x < 1 || x > mapW || y < 1 || y > mapH )
        return false;

    while ( map[y][x] -> charSymbol != ' ' && y < mapH + 1 && y > 0 ) {
        while ( map[y][x] -> charSymbol != ' ' && x < mapW + 1 && x > 0 )
            x += xDir;

        if ( map[y][x] -> charSymbol == ' ' ) return true;
        x = xBase; //reset x
        y += yDir;
    }

    return map[y][x] -> charSymbol == ' ';
}

void Map::generateWalls() {
    // setup borders
    //up & down
    for ( int32_t i = 0; i <= mapW + 1; ++i ) {
        map[0]       [i] = std::make_shared<Object>(Wall (false)); // upper border
        map[mapH + 1][i] = std::make_shared<Object>(Wall (false)); // bottom border
    }

    //left & right
    for ( int32_t i = 0; i <= mapH + 1; ++i ) {
        map[i]       [0] = std::make_shared<Object>(Wall (false)); // left border
        map[i][mapW + 1] = std::make_shared<Object>(Wall (false)); // right border
    }

    //middle walls
    for ( int32_t y = 2; y <= mapH; y += 2 )
        for ( size_t x = 2; x <= mapW; x += 2 )
            map[y][x] = std::make_shared<Object>(Wall ( false ));
}

std::shared_ptr<Player> Map::nextPlayer( const uint16_t& x, const uint16_t& y ) {
    ++pCnt;

    if ( pCnt == 1 )//1st player
        return p1 = std::make_shared<Player>(x, y, 'w', 's', 'a', 'd', ' ');

    if ( pCnt == 2 ) { //2nd player
        p2 = std::make_shared<Player>(x, y, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, '0');
        p2 -> xDir = -1;
        return p2;
    }

    std::ofstream err ( "assets/log.txt" );
    if ( err . is_open() ) {
        err << "too many players\n";
        err.close();
    }
    return nullptr;
}
