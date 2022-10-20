#ifndef SEMESTRALNIPRACE_BOMBER_MENU_H
#define SEMESTRALNIPRACE_BOMBER_MENU_H

#include <string>
#include <vector>

/**
 * Menu class
 */
class Menu {
public:
    /**
     * Method displays the solo / duo menu and waits for enter press
     */
    bool getPlayerCnt ( );

    /**
     * Method displays maps in the map folder and waits for enter press
     */
    std::string getMap ( );

protected:
    /**
     * Method displays solo / duo picking menu
     */
    void displaySolo ( ) const;

    /**
     * Method displays map picking menu
     */
    void displayMaps ( ) const;

    std::string mapSelectMess = "* select map *"; /**< message telling player to select map */
    std::vector<std::string> files; /**< files available in maps */
    size_t maxW = 0; /**< longest file name */
    int selected = 0; /**< currently selected item */
};

#endif //SEMESTRALNIPRACE_BOMBER_MENU_H
