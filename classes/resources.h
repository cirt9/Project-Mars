#ifndef RESOURCES_H_INCLUDED
#define RESOURCES_H_INCLUDED

#include <iostream>
#include <fstream>
#include <cstring>
#include "../functions/independent_functions.h"
#include <algorithm>

namespace RESOURCES_BASE_HUMAN
{
    //----------------------------------------------------------------------------------------------------------------------------
    //class Resources
    //----------------------------------------------------------------------------------------------------------------------------
    class Resources
    {
    protected:
        double oxygen;
        double food;
        double water;
        int building_materials;
        int humans;
    public:
        Resources(double ox = 0.0, double f = 0.0, double w = 0.0, int b_m = 0, int hum = 0);
        Resources(const Resources & r);
        Resources & operator=(const Resources & r);
        virtual ~Resources() {};

        double get_oxygen() const {return oxygen;}
        double get_food() const {return food;}
        double get_water() const {return water;}
        double get_building_materials() const {return building_materials;}

        virtual void add_building_materials(int value_) {building_materials += value_;}
        void show_resources() const;
        friend std::ostream & operator<<(std::ostream & os, const Resources & r);
    };

    //----------------------------------------------------------------------------------------------------------------------------
    // class Human
    //----------------------------------------------------------------------------------------------------------------------------

    class Human;

    //----------------------------------------------------------------------------------------------------------------------------
    // class Base
    //----------------------------------------------------------------------------------------------------------------------------
    class Base : public Resources
    {
    private:
        int base_level;
        int comfort;
        double max_oxygen;
        double max_food;
        double max_water;
        int max_building_materials;
        int max_humans;

        friend class Human;

        void base_description(std::string directory = "descriptions\\base.txt") const;
        void update_max_storage();

    public:
        Base() : Resources(), base_level(0), comfort(0), max_oxygen(100.0), max_food(100.0), max_water(100.0), max_building_materials(100), max_humans(2) {};
        Base(const Base & b);
        virtual ~Base() {};

        void save_base(const std::string file_name_ = "saves\\base_resources_save.txt");
        void load_base(const std::string file_name_ = "saves\\base_resources_save.txt");

        int get_base_level() const {return base_level;}
        int get_max_humans() const {return max_humans;}
        int get_humans() const {return humans;}
        bool substract_building_materials(int value_);
        bool substract_oxygen(double value_);
        bool substract_food(double value_);
        bool substract_water(double value_);

        void add_comfort(int value_);
        void add_food(double value_);
        void add_oxygen(double value_);
        void add_water(double value_);
        void add_building_materials(int value_);

        bool expand_base();

        void base_statistics() const;
    };

    //----------------------------------------------------------------------------------------------------------------------------
    // class Human
    //----------------------------------------------------------------------------------------------------------------------------
    class Human
    {
    private:
        std::string name;
        bool working;
        enum Healthness{Healthy, Sick, Very_sick, Dead};
        Healthness health;
        bool in_base;

        void become_more_ill();
        void become_healthier();
        int calculate_number_of_names();
        void to_used_names(std::string directory); //if lost/win remember to erase names

    public:
        Human() : name("None"), working(false), health(Very_sick), in_base(true) {}; //constructor to loading
        Human(Base & b, std::string directory = "saves\\names.txt", std::string used_names_directory = "saves\\used_names.txt");
        ~Human() {};

        std::string get_name()  const {return name;}
        std::string get_health_status()  const;
        bool get_working_status() const {return working;}
        bool get_in_base() {return in_base;}

        std::string calculate_healthness(Base & b);
        void save_human(std::string directory = "saves\\humans_save.txt");
        void load_human(int already_loaded, std::string directory = "saves\\humans_save.txt");

        template <typename T>
        void drink(T & t, double value_ = 0.2)                                              //invoke every week only with Base or Vehicle earlier there was 0.1 so check if something isnt ok
        {
            if(t.substract_water(value_) && health != Dead);                                //if there will be less water than value_ people wont drink it
            else
                become_more_ill();
        }

        template <typename T>
        void eat(T & t, double value_ = 0.2)                                                 //invoke every week only with Base or Vehicle
        {
            if(t.substract_food(value_) && health != Dead);
            else
                become_more_ill();
        }

        template <typename T>
        void breathe(T & t, double value_ = 0.2)                                            //invoke every week only with Base or Vehicle
        {
            if(t.substract_oxygen(value_) && health != Dead);
            else
                become_more_ill();
        }

        void change_working_status();
        void set_in_base(bool is) {in_base = is;}
        void kill_human() {health = Dead;}
        bool if_died(Base & b);                                                 //remember to delete this human and save after that

        void show_ID() const;
    };
}

#endif // RESOURCES_H_INCLUDED
