#ifndef SEMESTRALNIPRACE_BOMBER_ENEMY_H
#define SEMESTRALNIPRACE_BOMBER_ENEMY_H

#include "Movable.h"

/**
 * Enemy class
 */
class Enemy : public Movable {
public:
    /**
     * Enemy constructor
     * @param x x coord
     * @param y y coord
     * @param t enemy tier (base value = 1)
     */
    explicit Enemy ( uint16_t x, uint16_t y, uint8_t t = 1 );

    /**
     * Method moves enemy based on their tier
     * @param m map to move on
     */
    void move ( const std::vector<std::vector<std::shared_ptr<Object>>>& m ) override;

    void print () const override;

    uint8_t tier; /**< represents enemy tier level (higher = smarter) */
};

#endif //SEMESTRALNIPRACE_BOMBER_ENEMY_H
