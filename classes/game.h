#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include "resources.h"
#include "buildings_vehicles.h"
#include "additional.h"
#include "../functions/independent_functions.h"
#include <list>
#include <iomanip>

namespace GAME
{
    class Game
    {
    private:
        RESOURCES_BASE_HUMAN::Base base;
        std::list<RESOURCES_BASE_HUMAN::Human> humans;
        std::vector<BUILDINGS_VEHICLE::Buildings> buildings;
        ADDITIONAL::Expedition expeditions;
        std::vector<std::string> humans_statements;
        std::string nickname;
        int week;
        int died_humans;

        void introduction(std::string directory = "descriptions\\story\\introduction.txt");
        void ending_story(std::string directory = "descriptions\\story\\");
        void set_nickname();

        void reset_game();
        void prepare_game();
        void show_result() const {std::cout << "Przetrwales " << week << " tygodni. Liczba ofiar: " <<  died_humans << std::endl;}
        void results_to_file(std::string directory = "descriptions\\scores\\scores.txt");

        void show_tips(std::string directory = "descriptions\\tips.txt");

        void show_humans() const;
        void show_not_working_humans_names() const;

        void display_not_builded();
        void display_builded();
        void upgrade_building();
        void build_building(BUILDINGS_VEHICLE::Buildings & b);
        void workers_controls(BUILDINGS_VEHICLE::Buildings & b);
        void infrastructure_controls();
        void buildings_management();

        bool check_whether_expeditions_building_exists();
        void map_management();
        void display_vehicles() const;
        void build_vehicle();
        void destroy_vehicle();
        void vehicle_controls();
        void start_expedition_controls();
        void repair_vehicle_controls();
        void crew_controls();
        void transport_to_vehicle_controls();
        void transport_to_base_controls();

        bool check_for_win();
        bool check_for_lose();

        void gifts();
        void born_human();
        void check_humans();
        void humans_vital_signs();
        void humans_health_calculation();
        void events();

    public:
        Game();
        ~Game() {};

        bool main_menu();
        void new_game();
        void game_over();
        void game_menu();

        void save_game(std::string directory = "saves\\game.txt");
        void load_game(std::string directory = "saves\\game.txt");

        void display_scores(std::string directory = "descriptions\\scores\\scores.txt");

        void command_center_controls();
        void base_controls();
        void humans_controls();
        void buildings_controls();
        void expeditions_controls();
        bool skip_week();               //true == gameover | false == continue
    };
}

#endif // GAME_H_INCLUDED
