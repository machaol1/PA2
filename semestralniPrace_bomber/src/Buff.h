#ifndef SEMESTRALNIPRACE_BOMBER_BUFF_H
#define SEMESTRALNIPRACE_BOMBER_BUFF_H

#include "Object.h"

/**
 * Buff class
 */
class Buff : public Object {
public:
    /**
     * Buff constructor generates random type and tier
     */
    Buff ( );

    /**
     * Buff constructor generates random tier
     * @param t type
     */
    explicit Buff ( char t );

    /**
     * Buff constructor generates random type
     * @param t tier
     */
    explicit Buff ( uint8_t t );

    /**
     * Buff constructor
     * @param tier tier
     * @param type type
     */
    explicit Buff ( uint8_t tier, char type );

    /**
     * Method decreases buff duration
     * @return returns duration == 0;
     */
    bool tick ( );

    char type;  /**< buff type (R = bomb range, C = max bomb count) */
    uint8_t tier; /**< stores the buff tier */

protected:
    uint8_t duration = 20;  /**< buff duration */
};

#endif //SEMESTRALNIPRACE_BOMBER_BUFF_H
