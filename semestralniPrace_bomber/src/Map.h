#ifndef SEMESTRALNIPRACE_BOMBER_MAP_H
#define SEMESTRALNIPRACE_BOMBER_MAP_H

#include "Player.h"
#include "Enemy.h"
#include "Bomb.h"

#include <cstdint>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <ncurses.h>

/**
 * Map class - represents the game map
 */
class Map {
public:
    /**
     * Score getter
     * @return returns score
     */
    [[nodiscard]] size_t getScore () const;

    /**
     * Loads map from file
     * @param file saved map file
     * @return returns true if loading succeeded
     */
    bool load ( const char* file );

    /**
     * Method prints unbreakable walls
     */
    void printUnbreakable ( ) const;

    /**
     * Method clears the console and prints the map
     */
    void print ( ) const;

    /**
     * Method adds player to the map on given location
     * @param loc string in lt/rt/lb/rb format based on which it places the player on the grid
     */
    std::shared_ptr<Player> addPlayer ( const std::string& loc );

    /**
     * Method adds enemy to the map on given location (if location == [0, 0] finds random location)
     * @param x x coord with base value 0
     * @param y y coord with base value 0
     */
    void addEnemy ( uint16_t x = 0, uint16_t y = 0);

    /**
     * Method generates blank map
     */
    void generate ( );

    /**
     * Method performs tick that could end the game
     * @return return 0 if game should continue, otherwise returns death message code
     */
    int dangTick ( );

    /**
     * Method executes actions that can't end the game (such as player buff ticking)
     */
    void safeTick ( );

    std::shared_ptr<Player> p1;     /**< ptr to p1 */
    std::shared_ptr<Player> p2;     /**< ptr to p2 */

    uint16_t mapW = 0;  /**< map width  - 2 (borders not included aka mapW = 5 => 5 valid horizontal locations + left & right borders) */
    uint16_t mapH = 0;  /**< map height - 2 (borders not included aka mapH = 5 => 5 valid vertical locations + upper & bottom borders) */

protected:
    /**
     * Method iterates thru bomb list and ticks every bomb (eventually explodes them)
     * @return returns true if player got hit
     */
    int tickBombs ( );

    /**
     * Method moves enemy on given location
     * @return returns 1 if player touches bomb, 2 if player touches enemy otherwise returns 0
     */
    int moveMovs ( );

    /**
     * Method adds random buff to a random location on the map
     * @param prob probability of buff spawn attempt (0 = no spawn attempt, 100 = ensured spawn attempt)
     */
    void addBuffRand ( const uint8_t& prob );

    /**
     * Method explodes given bomb
     * @param b bomb to explode
     * @return returns 0 if player was hit, otherwise returns 1
     */
    int explode ( std::shared_ptr<Bomb>& b );

    /**
     * Method deletes hit enemies
     */
    void delHits ( );

    /**
     * Protected method that find blank spot on the map
     * @param x & of x axis coord where the player should get placed
     * @param y & of y axis coord where the player should get placed
     * @param loc accepts lt / rt / lb / rb representing the corner where the object should be placed
     * @return return true if blank spot was found
     */
    bool findBlankSpot ( uint16_t& x, uint16_t& y, const std::string& loc ) const;

    /**
     * Protected method that generates outer and inner walls
     * be careful! it overrides existing objects
     */
    void generateWalls ( );

    /**
     * Method spawn new player on the map
     * @param x x location in the map
     * @param y y location in the map
     * @return ptr to the player for the first two players, and null ptr for players after that
     */
    std::shared_ptr<Player> nextPlayer( const uint16_t& x, const uint16_t& y );

    /**
     * Method places bombs
     */
    void placeBombs ( );

    std::vector<std::vector<std::shared_ptr<Object>>> map;  /**< 2d array of game objects */

    std::list<std::shared_ptr<Movable>> movList;    /**< list of enemies */
    std::list<std::shared_ptr<Bomb>> bombList;   /**< list of bombs for faster ticking */
    std::list<std::pair<uint16_t, uint16_t>> explosionList; /**< list of coords where there is explosion */
    std::list<std::pair<uint16_t, uint16_t>> hitList; /**< list of enemies coords that got hit by the explosion */

    size_t pCnt = 0;    /**< player count */

    size_t score = 0;   /**< players (combined) score */

    static constexpr uint16_t minW = 4;  /**< minimal map width */
    static constexpr uint16_t minH = 4;  /**< minimal map height */
    static constexpr uint16_t maxW = 32; /**< maximal map width */
    static constexpr uint16_t maxH = 32; /**< maximal map height */

    static constexpr uint8_t enemyLimit = 5; /**< percentage of maximum enemy coverage of the map */
};

#endif //SEMESTRALNIPRACE_BOMBER_MAP_H
