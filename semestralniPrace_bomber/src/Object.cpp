#include "Object.h"

Object::Object()
: charSymbol(' ') { }

void Object::print() const {
    int EX_COL = 7;
    if ( charSymbol == '-'
        || charSymbol == '|' ) {
        attron(COLOR_PAIR(EX_COL));
        addch(charSymbol);
        attroff(COLOR_PAIR(EX_COL));
        return;
    }
    addch(charSymbol);
}
