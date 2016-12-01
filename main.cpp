#include <iostream>
#include "classes/resources.h"
#include "classes/buildings_vehicles.h"
#include "classes/additional.h"
#include "classes/game.h"

int main()
{
    srand(time(NULL));
    GAME::Game game;
    bool close = game.main_menu();
    if(close)
        return 0;
}
