#ifndef SEMESTRALNIPRACE_BOMBER_MOVABLE_H
#define SEMESTRALNIPRACE_BOMBER_MOVABLE_H

#include "Object.h"

#include <vector>

/**
 * Class representing movable objects
 */
class Movable : public Object {
public:
    /**
     * Movable constructor
     * @param x x coord
     * @param y y coord
     */
    Movable ( uint16_t x, uint16_t y );

    /**
     * Method moves object
     * @param map map to move on
     */
    virtual void move ( const std::vector<std::vector<std::shared_ptr<Object>>>& map ) = 0;

    uint16_t x;     /**< x location in map */
    uint16_t y;     /**< y location in map */
};

#endif //SEMESTRALNIPRACE_BOMBER_MOVABLE_H
