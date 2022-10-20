#ifndef SEMESTRALNIPRACE_BOMBER_OBJECT_H
#define SEMESTRALNIPRACE_BOMBER_OBJECT_H

#include <ncurses.h>
#include <cstdint>
#include <memory>

/**
 * Class representing game objects such as players, bombs and so on
 */
class Object {
public:
    /**
     * Object constructor
     */
    Object ( );

    /**
     * Method prints given object
     */
    virtual void print ( ) const;

    unsigned char charSymbol; /**< basic char symbol representing blank map space */
};

#endif //SEMESTRALNIPRACE_BOMBER_OBJECT_H
