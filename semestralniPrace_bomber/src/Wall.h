#ifndef SEMESTRALNIPRACE_BOMBER_WALL_H
#define SEMESTRALNIPRACE_BOMBER_WALL_H

#include "Object.h"

/**
 * Wall class
 */
class Wall : public Object {
public:
    /**
     * Wall constructor
     * @param soft dictates whether the wall will be breakable (base value = true)
     */
    Wall ( const bool& soft = true );

    void print () const override;

protected:
    bool breakable; /**< decides whether the wall is breakable (by explosion) */
};

#endif //SEMESTRALNIPRACE_BOMBER_WALL_H
