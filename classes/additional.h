#ifndef ADDITIONAL_H_INCLUDED
#define ADDITIONAL_H_INCLUDED

#include "resources.h"
#include "buildings_vehicles.h"
#include<iostream>
#include <memory>
#include <vector>
#include <list>
#include <fstream>
#include <iomanip>

namespace ADDITIONAL
{
    typedef std::unique_ptr<char[]> Map;

    class Expedition
    {
    private:
        static std::unique_ptr<Map[]> mars_map;
        int map_displaying_starting_width;
        int vehicles_number;

        struct Expedition_info
        {
            std::string place_description;
            int x;
            int y;
            std::string research_conclusions;
            int humans_needed;
            int prize;
            char character_on_map;
            bool state;             // visited/not visited
        };

        struct Vehicle_on_map
        {
            BUILDINGS_VEHICLE::Vehicle v;
            int x;
            int y;
            bool moved_this_turn;
            char character_on_map;
        };

        static std::vector<Expedition_info> expedition_infos;
        std::vector<Vehicle_on_map> vehicles;

        static void set_locations(std::string directory = "descriptions//locations//location");
        int compute_width(int height);

        bool interact(Vehicle_on_map & vehicle);
        bool move_mechanics(char direction, Vehicle_on_map & vehicle);

    public:
        Expedition();

        void set_vehicles_on_map();
        static void set_map();
        bool move_map(char direction);
        void show_map();
        bool show_info(char choice) const;

        void save_expedition(std::string directory1 = "saves//expedition//expedition.txt", std::string directory2 = "saves//vehicles//vehicle");
        void load_expedition(std::list<RESOURCES_BASE_HUMAN::Human> & l,std::string directory1 = "saves//expedition//expedition.txt" ,std::string directory2 = "saves//vehicles//vehicle");

        bool add_vehicle(BUILDINGS_VEHICLE::Vehicle & veh, char sign);
        bool del_vehicle(char choice);
        bool repair_vehicle(char vehicle_sign);
        int get_number_of_vehicles() const {return vehicles.size();}

        bool add_human_to_vehicle(RESOURCES_BASE_HUMAN::Human & h, char vehicle_sign);
        bool discard_human_from_vehicle(std::string name, char vehicle_sign);
        void vital_signs();

        bool oxygen_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_);
        bool food_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_);
        bool water_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_);
        bool building_materials_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, int value_);

        bool oxygen_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_);
        bool food_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_);
        bool water_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_);
        bool building_materials_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, int value_);

        bool start_expedition(char vehicle_sign);
        bool move_vehicle(char vehicle_sign);
        void change_moved_this_turn(char vehicle_sign);

        bool check_if_vehicle_is_on_map(char vehicle_sign);
        bool check_if_vehicle_exists(char vehicle_sign);
        bool check_if_place_exists(char place_sign);

        void show_vehicle(char vehicle_sign) const;
        void show_vehicles() const;
    };
}

#endif // ADDITIONAL_H_INCLUDED
