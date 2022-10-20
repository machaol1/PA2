#ifndef SEMESTRALNIPRACE_BOMBER_GAME_H
#define SEMESTRALNIPRACE_BOMBER_GAME_H

#include "Player.h"
#include "Map.h"

#include <memory>
#include <list>
#include <ncurses.h>

/**
 * Game representing class
 */
class Game {
public:
    /**
     * Game constructor
     */
    Game ( );

    /**
     * Sets up the game and then starts the main game loop
     */
    void play ( );

    /**
     * Method ends the game
     * @param r end message code
     */
    void end ( const int& r );

    /**
     * Static delay method
     * @param millis how long to wait
     */
    static void wait ( const int32_t& millis );

    /**
     * Static method that converts key code to char (f.e. left arrow => <)
     * @param key key code
     * @return returns key description
     */
    static char keyToChar ( const int& key );

protected:
    /**
     * Protected method that sets up the game
     */
    void setup ( );

    /**
     * Protected method formats input (no duplicates, p1 controls first)
     */
    void formatInput () const;

    /**
     * Method calls map loading tests
     */
    void test();

    bool solo = true;           /**< game option */

    std::shared_ptr<Player> p1; /**< main player */
    std::shared_ptr<Player> p2; /**< optional player */
    std::shared_ptr<Map> map;   /**< game map */

    size_t frame = 0; /**< frame counter */
};

#endif //SEMESTRALNIPRACE_BOMBER_GAME_H
