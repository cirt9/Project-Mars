#ifndef BUILDINGS_VEHICLES_H_INCLUDED
#define BUILDINGS_VEHICLES_H_INCLUDED

#include "resources.h"
#include <iostream>
#include <iomanip>
#include <list>
#include <fstream>

namespace BUILDINGS_VEHICLE
{
    //----------------------------------------------------------------------------------------------------------------
    //      class B_V_ABC
    //----------------------------------------------------------------------------------------------------------------
    class B_V_ABC
    {
    protected:
        std::string description;
        std::vector<RESOURCES_BASE_HUMAN::Human * > employees;
        int employees_limit;
        int working_people;
    public:
        B_V_ABC() : description("None"), employees_limit(0), working_people(0) {};

        bool add_employee(RESOURCES_BASE_HUMAN::Human & h);                     //bool for UI
        bool discard_employee(std::string name);
        int get_working_people() const {return working_people;}

        virtual void show_cost() const = 0;

        virtual ~B_V_ABC() = 0;
    };
    inline B_V_ABC::~B_V_ABC() {}

    //----------------------------------------------------------------------------------------------------------------
    //      class Buildings
    //----------------------------------------------------------------------------------------------------------------
    class Buildings : public B_V_ABC
    {
    private:
        int building_level;
        int max_building_level;

        double oxygen_cost;
        double food_cost;
        double water_cost;
        int building_materials_cost;

        double oxygen_production;           //for one worker
        double food_production;             //for one worker
        double water_production;            //for one worker
        int building_materials_production;  //for one worker
        int additional_comfort;

        static bool already_builded_on_this_base_level;         //wont work for multiplayer

        void update_production();
        void update_cost();
        bool whether_enough_resources(RESOURCES_BASE_HUMAN::Base & b) const;

    public:
        Buildings();            //testing and loading constructor
        Buildings(int number, std::string directory = "descriptions//buildings//building");
        ~Buildings() {};

        static void change_already_builded();                                //invoke right after Base::expand_base(); if return statement is true
        void check_employees();                                             //invoke with vital signs

        bool build(RESOURCES_BASE_HUMAN::Base & b);
        bool upgrade(RESOURCES_BASE_HUMAN::Base & b);
        void add_production_to_base(RESOURCES_BASE_HUMAN::Base & b);        //invoke once in every week

        static bool get_builded_status() {return already_builded_on_this_base_level;}    //to check whether it is builded before upgrading base
        std::string get_description() const{return description;}

        void save_building(int number, std::string directory = "saves//buildings//building");
        void load_building(int number, std::list<RESOURCES_BASE_HUMAN::Human> & l, std::string directory = "saves//buildings//building");
        static void save_builded_status(std::string directory = "saves//buildings//is_builded.txt");            //invoke with save_building()
        static void load_builded_status(std::string directory = "saves//buildings//is_builded.txt");            //invoke with load_building()

        void show_cost() const;
        void show_production() const;
        void building_details() const;

        friend bool operator==(const Buildings & b1, const Buildings & b2);
    };

    //----------------------------------------------------------------------------------------------------------------
    //      class Vehicle
    //----------------------------------------------------------------------------------------------------------------
    class Vehicle : public B_V_ABC
    {
    private:
        double max_payload;
        double oxygen_cargo;
        double food_cargo;
        double water_cargo;
        int building_materials_cargo;

        int building_materials_cost;

        int speed;
        int durability; //in percents

        bool is_in_use;

        int calculate_repairing_cost() {return building_materials_cost / 100 * (100 - durability);}
        void check_employees();

    public:
        Vehicle();
        Vehicle(int number, std::string directory = "descriptions//vehicles//vehicle");
        ~Vehicle() {};

        void save_vehicle(int number, std::string directory = "saves//vehicles//vehicle");
        void load_vehicle(int number, std::list<RESOURCES_BASE_HUMAN::Human> & l, std::string directory = "saves//vehicles//vehicle");

        bool get_is_in_use() {return is_in_use;}
        int get_working_people() {return working_people;}
        int get_durability() {return durability;}
        int get_speed() {return speed;}
        void change_is_in_use();

        void take_oxygen_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_);
        void take_food_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_);
        void take_water_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_);
        void take_building_materials_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, int value_);

        void take_oxygen_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_);
        void take_food_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_);
        void take_water_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_);
        void take_building_materials_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, int value_);
        int add_building_materials(int value_);

        bool substract_oxygen(double value_);       //for human vital sings
        bool substract_food(double value_);
        bool substract_water(double value_);

        void human_vital_signs();

        bool build(RESOURCES_BASE_HUMAN::Base & b);
        void repair();
        void decrease_durability();
        void destroy_vehicle();

        void show_cost() const {std::cout << std::left << std::setw(40) << "Koszt budowy pojazdu: " << building_materials_cost << " j. materialow budowlanych.";}
        void show_repairing_cost() {std::cout << std::left << std::setw(50) << "Koszt pelnej naprawy pojazdu: " << calculate_repairing_cost() << " j. materialow budowlanych.";}
        void vehicle_details() const;
        void properties() const;

    };
}

#endif // BUILDINGS_VEHICLES_H_INCLUDED
