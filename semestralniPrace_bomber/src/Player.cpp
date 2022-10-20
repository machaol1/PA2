#include "Player.h"
#include "Bomb.h"
#include "Game.h"

#include <vector>

Player::Player ( uint16_t x, uint16_t y, int u, int d, int l, int r, int b )
: Movable( x, y ), moveU(u), moveD(d), moveL(l), moveR(r), pBomb(b) {
    charSymbol = 'P';
}

bool Player::addBomb(const std::vector<std::vector<std::shared_ptr<Object>>>& map) {
    if ( map[y + yDir][x + xDir] -> charSymbol != ' ' ) return false;
    uint8_t bonus = 0;
    for ( const auto& i : buffs )
        if ( i -> type == 'C' && i -> tier > bonus )
            bonus = i -> tier;

    if ( ++bombCnt <= bombLimit + bonus ) return true;
    -- bombCnt;
    return false;
}

void Player::delBomb() {
    --bombCnt;
}

uint8_t Player::range() const {
    uint8_t bonus = 0;
    for ( const auto& i : buffs )
        if ( i -> type == 'R' && i -> tier > bonus )
            bonus = i -> tier;

    return Bomb::baseRange + bonus;
}

void Player::tickBuffs() {
    for ( auto i = buffs.begin(); i != buffs.end(); ++i )
        if ( (*i) -> tick() ) {
            auto tmp = i;
            ++i;
            buffs.erase(tmp);
        }
}

void Player::addBuff(const std::shared_ptr<Buff> &b) {
    buffs.emplace_back(b);
}

uint8_t Player::limit() const {
    uint8_t bonus = 0;
    for ( const auto& i : buffs )
        if ( i -> type == 'C' && i -> tier > bonus )
            bonus = i -> tier;

    return bombLimit + bonus;
}

void Player::move( const std::vector<std::vector<std::shared_ptr<Object>>>& map ) {
    int c = getch();
    if ( ! isControl ( c ) || c == pBomb ) {
        ungetch(c);
        return;
    }

    if ( c == moveU ) {
        xDir = 0;
        yDir = -1;
    }
    if ( c == moveD ) {
        xDir = 0;
        yDir = 1;
    }
    if ( c == moveL ) {
        xDir = -1;
        yDir = 0;
    }
    if ( c == moveR ) {
        xDir = 1;
        yDir = 0;
    }

    if ( map[y + yDir][x + xDir] -> charSymbol == 'R'
         || map[y + yDir][x + xDir] -> charSymbol == 'C' ) {
        auto cpy = std::static_pointer_cast<Buff>(map[y + yDir][x + xDir]);
        addBuff ( cpy );

        x += xDir;
        y += yDir;

        return;
    }
    if ( map[y + yDir][x + xDir] -> charSymbol == ' '
        || map[y + yDir][x + xDir] -> charSymbol == 'E'
        || map[y + yDir][x + xDir] -> charSymbol == '-'
        || map[y + yDir][x + xDir] -> charSymbol == '|' ) { //blank spot
        x += xDir;
        y += yDir;

        return;
    }
}

void Player::print() const {
    if ( xDir == 0 )
        if ( yDir == 1 )
            addch('v');
        else
            addch('^');
    else
    if ( xDir == 1 )
        addch('>');
    else
        addch('<');
}

void Player::printControls( int& line ) const {
    mvaddch(line + 1, 3, Game::keyToChar(moveU));
    mvaddch(line + 3, 3, Game::keyToChar(moveD));
    mvaddch(line + 2, 1, Game::keyToChar(moveL));
    mvaddch(line + 2, 5, Game::keyToChar(moveR));
    mvaddch(line + 2, 3, Game::keyToChar(pBomb));
    mvprintw(line + 4, 0, "");
    line += 4;
}

bool Player::isControl(const int &c) const {
    return c == moveU || c == moveD
        || c == moveL || c == moveR
        || c == pBomb;
}
