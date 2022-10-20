#ifndef SEMESTRALNIPRACE_BOMBER_FIGURE_H
#define SEMESTRALNIPRACE_BOMBER_FIGURE_H

#include "Movable.h"
#include "Buff.h"

#include <cstdint>
#include <list>

/**
 * Player class
 */
class Player : public Movable {
public:
    /**
     * Player constructor
     * @param x x coord
     * @param y y coord
     * @param u up control
     * @param d down control
     * @param l left control
     * @param r right control
     * @param b bomb placement control
     */
    Player ( uint16_t x, uint16_t y, int u, int d, int l, int r, int b );

    /**
     * Method checks if player can have another bomb
     * @param map map where to place bomb
     * @return returns true if bomb can be placed (valid location and player's bombCnt <= bombLimit)
     */
    bool addBomb ( const std::vector<std::vector<std::shared_ptr<Object>>>& map );

    /**
     * Method decrements bombCnt
     */
    void delBomb ( );

    /**
     * Players bomb range getter
     * @return returns players bomb range
     */
    [[nodiscard]] uint8_t range ( ) const;

    /**
     * Players bomb count limit getter
     * @return returns limit of placed bombs by the player
     */
    [[nodiscard]] uint8_t limit ( ) const;

    /**
     * Method ticks all players buffs
     */
    void tickBuffs ( );

    /**
     * Method adds buff to the player
     * @param b buff to be added
     */
    void addBuff ( const std::shared_ptr<Buff>& b );

    /**
     * Method move player based on input
     * @param map map to move on
     */
    void move( const std::vector<std::vector<std::shared_ptr<Object>>>& map ) override;

    void print () const override;

    /**
     * Method prints player controls
     * @param line line to start
     */
    void printControls ( int& line ) const;

    /**
     * Method check whether the input is player control
     * @param c input key
     * @return return true if key is control
     */
    [[nodiscard]] bool isControl ( const int& c ) const;

    const int moveU;       /**< move up    control key */
    const int moveD;       /**< move down  control key */
    const int moveL;       /**< move left  control key */
    const int moveR;       /**< move right control key */
    const int pBomb;       /**< place bomb control key */

    int16_t xDir = 1;      /**< where is player facing (x direction) */
    int16_t yDir = 0;      /**< where is player facing (y direction) */

protected:
    size_t bombCnt = 0;    /**< current bomb count */

    static constexpr size_t bombLimit = 1;  /**< max amount of bombs placed by 1 player on the field at any given time */
    std::list<std::shared_ptr<Buff>> buffs; /**< players buff container */
};

#endif //SEMESTRALNIPRACE_BOMBER_FIGURE_H
