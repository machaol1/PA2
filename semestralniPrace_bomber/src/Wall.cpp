#include "Wall.h"

Wall::Wall ( const bool& soft )
: breakable(soft) {
    charSymbol = soft ? '*' : '#';
}

void Wall::print() const {
    int UW_COL = 5,
        BW_COL = 6;

    if ( charSymbol == '#' ) {
        attron(COLOR_PAIR(UW_COL));
        addch('#');
        attroff(COLOR_PAIR(UW_COL));
        return;
    }

    attron(COLOR_PAIR(BW_COL));
    addch('*');
    attroff(COLOR_PAIR(BW_COL));
}