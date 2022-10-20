#ifndef SEMESTRALNIPRACE_BOMBER_BOMB_H
#define SEMESTRALNIPRACE_BOMBER_BOMB_H

#include "Object.h"

#include <vector>
#include <list>

using namespace std;

/**
 * Bomb class
 */
class Bomb : public Object {
public:
    /**
     * Bomb constructor
     * @param x x coord
     * @param y y coord
     * @param duration after how many ticks does the bomb explode
     * @param p player
     */
    Bomb ( uint16_t x, uint16_t y, size_t duration, size_t p );

    /**
     * Method decreases duration
     * @return returns duration == 0 (bomb is supposed to explode in this frame)
     */
    bool tick ( );

    /**
     * Method explodes bomb
     * @param map map where to explode the bomb
     * @param r bomb range
     * @param hitList list to store hit enemies
     * @param exList list of hit spaces
     * @return returns 1 if explosion hits player
     */
    int explode ( vector<vector<shared_ptr<Object>>>& map, const uint8_t& r,
                  list<pair<uint16_t, uint16_t>>& hitList, list<pair<uint16_t,
                  uint16_t>>& exList ) const;

    void print () const override;

    uint16_t x; /**< x location */
    uint16_t y; /**< y location */

    size_t player; /**< player number */

    static constexpr size_t baseDuration = 5; /**< number of frames after which the bomb will explode */
    static constexpr uint8_t baseRange = 2; /**< base range of the bomb */

protected:
    size_t duration; /**< frame where bomb was placed */
};

#endif //SEMESTRALNIPRACE_BOMBER_BOMB_H
