#include "Bomb.h"

Bomb::Bomb(uint16_t _x, uint16_t _y, size_t dur, size_t p)
: x(_x), y(_y), player(p), duration(dur * baseDuration) {
    if ( (duration - 1) / 2 > 1 )
        charSymbol = 'B';
    else
        charSymbol = std::to_string((duration - 1) / 2)[0];
}

bool Bomb::tick() {
    if ( (--duration - 1) / 2 > 1 )
        charSymbol = 'B';
    else
        charSymbol = std::to_string((duration - 1) / 2)[0];
    return !duration;
}

void Bomb::print() const {
    int BO_COL = 4,
        B0_COL = 8,
        B1_COL = 9;

    if ( charSymbol == '0' ) {
        attron(COLOR_PAIR(B0_COL));
        addch('0');
        attroff(COLOR_PAIR(B0_COL));
        return;
    }

    if ( charSymbol == '1' ) {
        attron(COLOR_PAIR(B1_COL));
        addch('1');
        attroff(COLOR_PAIR(B1_COL));
        return;
    }

    attron(COLOR_PAIR(BO_COL));
    addch(charSymbol);
    attroff(COLOR_PAIR(BO_COL));
}

int Bomb::explode( vector<vector<shared_ptr<Object>>> &map, const uint8_t& r,
                   list<pair<uint16_t, uint16_t>> &hitList, list<pair<uint16_t,
                   uint16_t>> &explosionList) const {
    for ( uint8_t i = 1; i <= r; ++i )
        if ( map[y][x + i] -> charSymbol == ' ' ) {       // blank space
            explosionList . emplace_back ( std::make_pair( x + i, y ) );
            map[y][x + i]->charSymbol = '-';
        }
        else if ( map[y][x + i] -> charSymbol == 'P' )    // player hit
            return 1;
        else {
            hitList . emplace_back ( std::make_pair( x + i, y ) );
            break;                                             // other than blank space hit
        }

    for ( uint8_t i = 1; i <= r; ++i )
        if ( map[y][x - i] -> charSymbol == ' ' ) {
            explosionList.emplace_back(std::make_pair(x - i, y));
            map[y][x - i]->charSymbol = '-';
        } else if ( map[y][x - i] -> charSymbol == 'P' )
            return 1;
        else {
            hitList . emplace_back ( std::make_pair( x - i, y ) );
            break;
        }

    for ( uint8_t i = 1; i <= r; ++i )
        if ( map[y + i][x] -> charSymbol == ' ' ) {
            explosionList.emplace_back(std::make_pair(x, y + i));
            map[y + i][x]->charSymbol = '|';
        } else if ( map[y + i][x] -> charSymbol == 'P' )
            return 1;
        else {
            hitList.emplace_back(std::make_pair(x, y + i));
            break;
        }

    for ( uint8_t i = 1; i <= r; ++i )
        if ( map[y - i][x] -> charSymbol == ' ' ) {
            explosionList.emplace_back(std::make_pair(x, y - i));
            map[y - i][x]->charSymbol = '|';
        } else if ( map[y - i][x] -> charSymbol == 'P' )
            return 1;
        else {
            hitList.emplace_back(std::make_pair(x, y - i));
            break;
        }

    return 0;
}
