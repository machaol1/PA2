#include "Enemy.h"
#include "Player.h"

#include <random>
#include <vector>

Enemy::Enemy( uint16_t x, uint16_t y, uint8_t t )
: Movable(x, y) {
    charSymbol = 'E';
    tier = t;
}

void Enemy::move( const std::vector<std::vector<std::shared_ptr<Object>>>& map ) {
    // t1 is stationary
    if ( tier != 2 && tier != 3 ) return;

    // t2 moves randomly
    // t3 tries to get as close to the player as possible (just a simple dist calculation)

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> move(1, 10);

    auto mapH = map . size() - 2, mapW = map[0].size() - 2;

    // don't always move
    if ( ( tier == 2 && move(rng) > 5 )
        || ( tier == 3 && move(rng) > 7 ) )
        return;

    // 1st try to step on player
    if ( x     < mapW && map[y][x + 1] -> charSymbol == 'P' ) {
        ++x;
        return;
    }
    if ( x - 1 > 0    && map[y][x - 1] -> charSymbol == 'P' ) {
        --x;
        return;
    }
    if ( y     < mapH && map[y + 1][x] -> charSymbol == 'P' ) {
        ++y;
        return;
    }
    if ( y - 1 > 0    && map[y - 1][x] -> charSymbol == 'P' ) {
        --y;
        return;
    }

    //then look for blank space
    std::vector<std::pair<int16_t, int16_t>> validLocs;
    if ( x < mapW &&(map[y][x + 1] -> charSymbol == ' '
                  || map[y][x + 1] -> charSymbol == 'C'
                  || map[y][x + 1] -> charSymbol == 'R' ))
        validLocs.emplace_back(x + 1, y);

    if ( x > 1 &&(map[y][x - 1] -> charSymbol == ' '
               || map[y][x - 1] -> charSymbol == 'C'
               || map[y][x - 1] -> charSymbol == 'R' ))
        validLocs.emplace_back(x - 1, y);

    if ( y < mapH &&(map[y + 1][x] -> charSymbol == ' '
                  || map[y + 1][x] -> charSymbol == 'C'
                  || map[y + 1][x] -> charSymbol == 'R' ))
        validLocs.emplace_back(x, y + 1);

    if ( y > 1 &&(map[y - 1][x] -> charSymbol == ' '
               || map[y - 1][x] -> charSymbol == 'C'
               || map[y - 1][x] -> charSymbol == 'R' ))
        validLocs.emplace_back(x, y - 1);

    if ( validLocs.empty() ) return;

    auto moveTo = std::make_pair(0, 0);

    if ( tier == 2 ) { // select random location
        std::uniform_int_distribution<std::mt19937::result_type> loc(0, validLocs.size() - 1);
        moveTo = validLocs[loc(rng)];
    }

    if ( tier == 3 ) {
        // find players
        std::shared_ptr<Player> p1, p2;
        for ( auto& i : map )
            for ( auto& j : i )
                if ( j -> charSymbol == 'P' ) {
                    if ( p1 == nullptr )
                        p1 = std::dynamic_pointer_cast<Player>(j);
                    else p2 = std::dynamic_pointer_cast<Player>(j);
                }

        // calculate distances and find the closest to the player
        std::vector<double> distance;
        double min = mapW * mapH;
        for ( auto [x, y] : validLocs ) {
            auto dist = sqrt(((p1->x - x) * (p1->x - x)) + ((p1->y - y) * (p1->y - y)));
            if ( p2 != nullptr ) {
                double d2 = sqrt(((p2->x - x) * (p2->x - x)) + ((p2->y - y) * (p1->y - y)));
                if ( d2 < dist ) dist = d2;
            }
            if ( dist<min ) min = dist;
            distance.emplace_back ( dist );
        }

        // select the correct location (the lowest distance)
        for ( size_t j = 0; j < distance.size(); ++j )
            if ( distance[j] == min )
                moveTo = std::make_pair(validLocs[j].first, validLocs[j].second);
    }

    x = moveTo.first;
    y = moveTo.second;
}

void Enemy::print() const {
    int EN_COL = 3;
    attron(COLOR_PAIR(EN_COL));
    addch('@');
    attroff(COLOR_PAIR(EN_COL));
}
