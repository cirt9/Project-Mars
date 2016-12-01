#include "buildings_vehicles.h"

namespace BUILDINGS_VEHICLE
{
    //----------------------------------------------------------------------------------------------------------------
    //      class B_V_ABC
    //----------------------------------------------------------------------------------------------------------------
    bool B_V_ABC::add_employee(RESOURCES_BASE_HUMAN::Human & h)
    {
        if(working_people < employees_limit && h.get_health_status() != "Nie zyje")
        {
            if(h.get_working_status())
            {
                std::cout << "Ten pracownik juz pracuje." << std::endl;
                return false;
            }

            working_people++;
            employees.push_back(&h);
            employees[working_people-1]->change_working_status();
            std::cout << "Pracownik zaczal pracowac." << std::endl;
            return true;
        }
        std::cout << "Nie udalo sie przydzielic pracownikowi pracy.\nPrawdopodobnie wszystkie miejsca sa zajete." << std::endl;
        return false;
    }

    bool B_V_ABC::discard_employee(std::string name)
    {
        if(working_people <= 0)
        {
            std::cout << "Nikt nie pracuje." << std::endl;
            return false;
        }

        for(int i=0; i<working_people; i++)
        {
            if(employees[i]->get_name() == name)
            {
                employees[i]->change_working_status();
                employees[i]->set_in_base(true);
                employees[i] = nullptr;
                employees.erase(employees.begin() + i);
                working_people -= 1;
                std::cout << "Pracownik przestal pracowac." << std::endl;
                return true;
            }
        }

        std::cout << "Nie ma takiej osoby wsrod pracownikow." << std::endl;
        return false;
    }

    //----------------------------------------------------------------------------------------------------------------
    //      class Buildings
    //----------------------------------------------------------------------------------------------------------------
    void Buildings::update_production()
    {
        oxygen_production *= 7;
        food_production *= 7;
        water_production *= 7;
        building_materials_production *= 7;
        additional_comfort *= 2;
    }

    void Buildings::update_cost()
    {
        oxygen_cost *= 15;
        food_cost *= 15;
        water_cost *= 15;
        building_materials_cost *= 15;
    }

    bool Buildings::whether_enough_resources(RESOURCES_BASE_HUMAN::Base & b) const
    {
        if(b.get_oxygen() >= oxygen_cost && b.get_food() >= food_cost && b.get_water() >= water_cost && b.get_building_materials() >= building_materials_cost)
            return true;
        return false;
    }

    bool Buildings::already_builded_on_this_base_level = false;

    void Buildings::change_already_builded()
    {
        if(already_builded_on_this_base_level)
            already_builded_on_this_base_level = false;
        else
            already_builded_on_this_base_level = true;
    }

    void Buildings::check_employees()
    {
        for(auto vector_part : employees)
        {
            if(vector_part->get_health_status() == "Nie zyje")
                discard_employee(vector_part->get_name());
        }
    }

    Buildings::Buildings()
    {
        description = "Puste miejsce";
        building_level = 0;
        max_building_level = 2;
        employees_limit = 3;
        working_people = 0;

        oxygen_cost = 1.0;
        food_cost = 1.0;
        water_cost = 1.0;
        building_materials_cost = 1;

        oxygen_production = 5.0;
        food_production = 5.0;
        water_production = 5.0;
        building_materials_production = 5;
        additional_comfort = 5;
    }

    Buildings::Buildings(int number, std::string directory)
    {
        directory += INDEPENDENT::to_string(number) + ".txt";
        std::ifstream fin;
        fin.open(directory, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku budowli." << std::endl;
        else
        {
            int string_size;
            fin.read(reinterpret_cast<char*> (&string_size), sizeof(string_size));
            description.resize(string_size);
            fin.read(reinterpret_cast<char*> (&description[0]), string_size);
            building_level = 0;
            fin.read(reinterpret_cast<char*> (&max_building_level), sizeof(max_building_level));
            working_people = 0;
            fin.read(reinterpret_cast<char*> (&employees_limit), sizeof(employees_limit));
            fin.read(reinterpret_cast<char*> (&oxygen_cost), sizeof(oxygen_cost));
            fin.read(reinterpret_cast<char*> (&food_cost), sizeof(food_cost));
            fin.read(reinterpret_cast<char*> (&water_cost), sizeof(water_cost));
            fin.read(reinterpret_cast<char*> (&building_materials_cost), sizeof(building_materials_cost));
            fin.read(reinterpret_cast<char*> (&oxygen_production), sizeof(oxygen_production));
            fin.read(reinterpret_cast<char*> (&food_production), sizeof(food_production));
            fin.read(reinterpret_cast<char*> (&water_production), sizeof(water_production));
            fin.read(reinterpret_cast<char*> (&building_materials_production), sizeof(building_materials_production));
            fin.read(reinterpret_cast<char*> (&additional_comfort), sizeof(additional_comfort));
        }
        fin.close();

    }

    bool Buildings::build(RESOURCES_BASE_HUMAN::Base & b)
    {
        if(building_level != 0)
        {
            std::cout << "Budynek jest juz zbudowany." << std::endl;
            return false;
        }
        else if(b.get_base_level() >= 0 && !get_builded_status())
        {
            if(whether_enough_resources(b))
            {
                b.substract_oxygen(oxygen_cost);
                b.substract_food(food_cost);
                b.substract_water(water_cost);
                b.substract_building_materials(building_materials_cost);

                std::cout << "Budynek zostal zbudowany." << std::endl;
                building_level++;
                b.add_comfort(additional_comfort);
                update_cost();
                change_already_builded();
                return true;
            }

            std::cout << "Masz za malo surowcow." << std::endl;
            return false;
        }
        std::cout << "Budynek nie zostal zbudowany. Prawdopodobnie na tym poziomie\nbazy zbudowales juz budynek." << std::endl;
        return false;
    }

    bool Buildings::upgrade(RESOURCES_BASE_HUMAN::Base & b)
    {
        if(max_building_level == building_level)
        {
            std::cout << "Osiagnales maksymalny poziom budynku." << std::endl;
            return false;
        }

        else if(building_level == 0)
        {
            std::cout << "Musisz najpierw wybudowac budynek." << std::endl;
            return false;
        }

        else if(whether_enough_resources(b))
        {
            b.substract_oxygen(oxygen_cost);
            b.substract_food(food_cost);
            b.substract_water(water_cost);
            b.substract_building_materials(building_materials_cost);

            building_level++;
            employees_limit++;
            update_cost();
            update_production();
            b.add_comfort(additional_comfort);
            std::cout << "Budynek zostal ulepszony." << std::endl;
            return true;
        }

        std::cout << "Masz za malo surowcow." << std::endl;
        return false;
    }

    void Buildings::add_production_to_base(RESOURCES_BASE_HUMAN::Base & b)
    {
        b.add_oxygen(oxygen_production * working_people);
        b.add_food(food_production * working_people);
        b.add_water(water_production * working_people);
        b.add_building_materials(building_materials_production * working_people);
    }

    void Buildings::save_building(int number, std::string directory)
    {
        directory += INDEPENDENT::to_string(number) + ".txt";
        std::ofstream fout;
        fout.open(directory, std::ios_base::out | std::ios_base::binary);

        if(!fout.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu budowli." << std::endl;
        else
        {
            int string_length = description.size();
            fout.write(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            fout.write(reinterpret_cast<char*>(&description[0]), string_length);

            fout.write(reinterpret_cast<char*>(&building_level), sizeof(building_level));
            fout.write(reinterpret_cast<char*>(&max_building_level), sizeof(max_building_level));
            fout.write(reinterpret_cast<char*>(&employees_limit), sizeof(employees_limit));
            fout.write(reinterpret_cast<char*>(&working_people), sizeof(working_people));

            for(auto vector_part : employees)
            {
                int length = vector_part->get_name().size();
                fout.write(reinterpret_cast<char*>(&length), sizeof(length));
                fout.write(reinterpret_cast<char*>(&vector_part->get_name()[0]), length);
            }

            fout.write(reinterpret_cast<char*>(&oxygen_cost), sizeof(oxygen_cost));
            fout.write(reinterpret_cast<char*>(&food_cost), sizeof(food_cost));
            fout.write(reinterpret_cast<char*>(&water_cost), sizeof(water_cost));
            fout.write(reinterpret_cast<char*>(&building_materials_cost), sizeof(building_materials_cost));

            fout.write(reinterpret_cast<char*>(&oxygen_production), sizeof(oxygen_production));
            fout.write(reinterpret_cast<char*>(&food_production), sizeof(food_production));
            fout.write(reinterpret_cast<char*>(&water_production), sizeof(water_production));
            fout.write(reinterpret_cast<char*>(&building_materials_production), sizeof(building_materials_production));
            fout.write(reinterpret_cast<char*>(&additional_comfort), sizeof(additional_comfort));
        }
        fout.close();
    }

    void Buildings::load_building(int number, std::list<RESOURCES_BASE_HUMAN::Human> & l, std::string directory)
    {
        directory += INDEPENDENT::to_string(number) + ".txt";
        std::ifstream fin;
        fin.open(directory, std::ios_base::in | std::ios_base::binary);

        if(!fin.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu budowli." << std::endl;
        else
        {
            int string_length;
            fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            description.resize(string_length);
            fin.read(reinterpret_cast<char*>(&description[0]), string_length);

            fin.read(reinterpret_cast<char*>(&building_level), sizeof(building_level));
            fin.read(reinterpret_cast<char*>(&max_building_level), sizeof(max_building_level));
            fin.read(reinterpret_cast<char*>(&employees_limit), sizeof(employees_limit));
            fin.read(reinterpret_cast<char*>(&working_people), sizeof(working_people));

            for(int i=0; i<working_people; i++)
            {
                int name_length;
                std::string name;
                fin.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
                name.resize(name_length);
                fin.read(reinterpret_cast<char*>(&name[0]), name_length);

                for(auto & list_part : l)
                {
                    if(list_part.get_name() == name)
                    {
                        employees.push_back(&list_part);
                        break;
                    }
                }
            }
            fin.read(reinterpret_cast<char*>(&oxygen_cost), sizeof(oxygen_cost));
            fin.read(reinterpret_cast<char*>(&food_cost), sizeof(food_cost));
            fin.read(reinterpret_cast<char*>(&water_cost), sizeof(water_cost));
            fin.read(reinterpret_cast<char*>(&building_materials_cost), sizeof(building_materials_cost));

            fin.read(reinterpret_cast<char*>(&oxygen_production), sizeof(oxygen_production));
            fin.read(reinterpret_cast<char*>(&food_production), sizeof(food_production));
            fin.read(reinterpret_cast<char*>(&water_production), sizeof(water_production));
            fin.read(reinterpret_cast<char*>(&building_materials_production), sizeof(building_materials_production));
            fin.read(reinterpret_cast<char*>(&additional_comfort), sizeof(additional_comfort));
        }
        fin.close();
    }

    void Buildings::save_builded_status(std::string directory)
    {
        std::ofstream fout;
        fout.open(directory, std::ios_base::out | std::ios_base::binary);

        if(!fout.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu stanu budowania" << std::endl;
        else
            fout.write(reinterpret_cast<char*>(&already_builded_on_this_base_level), sizeof(already_builded_on_this_base_level));

        fout.close();
    }

    void Buildings::load_builded_status(std::string directory)
    {
        std::ifstream fin;
        fin.open(directory, std::ios_base::in | std::ios_base::binary);

        if(!fin.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu stanu budowania" << std::endl;
        else
            fin.read(reinterpret_cast<char*>(&already_builded_on_this_base_level), sizeof(already_builded_on_this_base_level));

        fin.close();
    }


    void Buildings::show_cost() const
    {
        if(building_level == 0)
            std::cout << "Koszt budowli: " << std::endl;
        else
            std::cout << "Koszt ulepszenia budowli: " << std::endl;
        std::cout << std::left << std::setw(25) << "Tlen: " << oxygen_cost << "j" << std::endl
                  << std::setw(25) << "Jedzenie: " << food_cost << "j" << std::endl
                  << std::setw(25) << "Woda: " << water_cost << "j" << std::endl
                  << std::setw(25) << "Materialy budowlane: " << building_materials_cost << "j" << std::endl;
    }

    void Buildings::show_production() const
    {
        std::cout << std::left << std::setw(67) << "Produkowana ilosc tlenu przez jednego pracownika: " << oxygen_production << "j" << std::endl
                  << std::setw(67) << "Produkowana ilosc jedzenia przez jednego pracownika: " << food_production << "j" << std::endl
                  << std::setw(67) << "Produkowana ilosc wody przez jednego pracownika: " << water_production << "j" << std::endl
                  << std::setw(67) << "Produkowana ilosc materialow budowlanych przez jednego pracownika: " << building_materials_production << "j" << std::endl
                  << std::setw(67) << "Dodatkowy komfort: " << additional_comfort << "%" << std::endl;
    }

    void Buildings::building_details() const
    {
        INDEPENDENT::cut_and_show(description);
        std::cout << "\n";
        std::cout << std::left << std::setw(19) << "Poziom budowli: " << building_level << "/" << max_building_level << std::endl;
        std::cout << std::setw(15) << "Ilosc pracownikow: " << working_people << "/" << employees_limit << std::endl << std::endl;

        show_production();
        std::cout << "\n";

        if(employees.size() > 0)
        {
            std::cout << std::right << "Pracownicy:" << std::endl;
            std::cout << "Imie i nazwisko" << std::setw(33) << "Stan zdrowia\n" << std::endl;

            for(auto vector_part : employees)
                std::cout << std::setw(35) << std::left << vector_part->get_name() << std::setw(25) << vector_part->get_health_status() << std::endl;
        }
    }

    bool operator==(const Buildings & b1, const Buildings & b2)
    {
        if(b1.get_description() == b2.get_description())
            return true;
        else
            return false;
    }

    //----------------------------------------------------------------------------------------------------------------
    //      class Vehicle
    //----------------------------------------------------------------------------------------------------------------

    void Vehicle::check_employees()
    {
        bool deleted = true;
        while(deleted != false)
        {
            if(employees.size() == 0)
                break;

            for(unsigned int i=0; i<employees.size(); i++)
            {
                if(employees[i] == nullptr)
                {
                    employees.erase(employees.begin() + i);
                    deleted = true;
                    break;
                }
                deleted = false;
            }
        }
    }

    Vehicle::Vehicle()
    {
        description = "None";
        employees_limit = 3;
        working_people = 0;
        max_payload = 400.0;
        oxygen_cargo = 0.0;
        food_cargo = 0.0;
        water_cargo = 0.0;
        building_materials_cargo = 0;
        building_materials_cost = 100;
        speed = 1;
        durability = 50;
        is_in_use = false;
    }

    Vehicle::Vehicle(int number, std::string directory)
    {
        std::ifstream fin;
        directory += INDEPENDENT::to_string(number) + ".txt";
        fin.open(directory, std::ios_base::in | std::ios_base::binary);

        if(!fin.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku opisu pojazdow." << std::endl;
        else
        {
            int string_size;
            fin.read(reinterpret_cast<char*> (&string_size), sizeof(string_size));
            description.resize(string_size);
            fin.read(reinterpret_cast<char*> (&description[0]), string_size);
            fin.read(reinterpret_cast<char*> (&employees_limit), sizeof(employees_limit));
            fin.read(reinterpret_cast<char*> (&max_payload), sizeof(max_payload));
            fin.read(reinterpret_cast<char*> (&building_materials_cost), sizeof(building_materials_cost));
            fin.read(reinterpret_cast<char*> (&speed), sizeof(speed));

            working_people = 0;
            oxygen_cargo = 0;
            food_cargo = 0;
            water_cargo = 0;
            building_materials_cargo = 0;
            durability = 100;
            is_in_use = false;
        }
        fin.close();
    }

    void Vehicle::save_vehicle(int number, std::string directory)
    {
        directory += INDEPENDENT::to_string(number) + ".txt";
        std::ofstream fout;
        fout.open(directory, std::ios_base::out | std::ios_base::binary);

        if(!fout.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu pojazdow." << std::endl;
        else
        {
            int string_length = description.size();
            fout.write(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            fout.write(reinterpret_cast<char*>(&description[0]), string_length);
            fout.write(reinterpret_cast<char*>(&employees_limit), sizeof(employees_limit));
            fout.write(reinterpret_cast<char*>(&working_people), sizeof(working_people));

            for(auto vector_part : employees)
            {
                int length = vector_part->get_name().size();
                fout.write(reinterpret_cast<char*>(&length), sizeof(length));
                fout.write(reinterpret_cast<char*>(&vector_part->get_name()[0]), length);
            }
            fout.write(reinterpret_cast<char*>(&max_payload), sizeof(max_payload));
            fout.write(reinterpret_cast<char*>(&oxygen_cargo), sizeof(oxygen_cargo));
            fout.write(reinterpret_cast<char*>(&food_cargo), sizeof(food_cargo));
            fout.write(reinterpret_cast<char*>(&water_cargo), sizeof(water_cargo));
            fout.write(reinterpret_cast<char*>(&building_materials_cargo), sizeof(building_materials_cargo));
            fout.write(reinterpret_cast<char*>(&building_materials_cost), sizeof(building_materials_cost));
            fout.write(reinterpret_cast<char*>(&speed), sizeof(speed));
            fout.write(reinterpret_cast<char*>(&durability), sizeof(durability));
            fout.write(reinterpret_cast<char*>(&is_in_use), sizeof(is_in_use));
        }
        fout.close();
    }

    void Vehicle::load_vehicle(int number, std::list<RESOURCES_BASE_HUMAN::Human> & l, std::string directory)
    {
        directory += INDEPENDENT::to_string(number) + ".txt";
        std::ifstream fin;
        fin.open(directory, std::ios_base::in | std::ios_base::binary);

        if(!fin.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu pojazdow." << std::endl;
        else
        {
            int string_length;
            fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            description.resize(string_length);
            fin.read(reinterpret_cast<char*>(&description[0]), string_length);
            fin.read(reinterpret_cast<char*>(&employees_limit), sizeof(employees_limit));
            fin.read(reinterpret_cast<char*>(&working_people), sizeof(working_people));

            for(int i=0; i<working_people; i++)
            {
                int name_length;
                std::string name;
                fin.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
                name.resize(name_length);
                fin.read(reinterpret_cast<char*>(&name[0]), name_length);

                for(auto & list_part : l)
                {
                    if(list_part.get_name() == name)
                    {
                        employees.push_back(&list_part);
                        break;
                    }
                }
            }
            fin.read(reinterpret_cast<char*>(&max_payload), sizeof(max_payload));
            fin.read(reinterpret_cast<char*>(&oxygen_cargo), sizeof(oxygen_cargo));
            fin.read(reinterpret_cast<char*>(&food_cargo), sizeof(food_cargo));
            fin.read(reinterpret_cast<char*>(&water_cargo), sizeof(water_cargo));
            fin.read(reinterpret_cast<char*>(&building_materials_cargo), sizeof(building_materials_cargo));
            fin.read(reinterpret_cast<char*>(&building_materials_cost), sizeof(building_materials_cost));
            fin.read(reinterpret_cast<char*>(&speed), sizeof(speed));
            fin.read(reinterpret_cast<char*>(&durability), sizeof(durability));
            fin.read(reinterpret_cast<char*>(&is_in_use), sizeof(is_in_use));
        }
        fin.close();
    }

    void Vehicle::change_is_in_use()
    {
        if(is_in_use)
            is_in_use = false;
        else
            is_in_use = true;
    }

    void Vehicle::take_oxygen_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;

        else if(oxygen_cargo + food_cargo + water_cargo + building_materials_cargo == max_payload)
            std::cout << "Pojazd jest pelny." << std::endl;
        else
        {
            if(b.substract_oxygen(value_))
            {
                double amount_ = (oxygen_cargo + food_cargo + water_cargo + building_materials_cargo + value_) - max_payload;
                oxygen_cargo += value_;
                std::cout << "Tlen zostal przetransportowany do pojazdu." << std::endl;
                if(amount_ > 0)
                {
                    oxygen_cargo -= amount_;
                    b.add_oxygen(amount_);
                    std::cout << amount_ << "j. tlenu nie zmiescilo sie w pojezdzie." << std::endl;
                }
            }
            else
                std::cout << "W bazie nie ma wystarczajacej ilosci tlenu." << std::endl;
        }
    }

    void Vehicle::take_food_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;

        else if(oxygen_cargo + food_cargo + water_cargo + building_materials_cargo == max_payload)
            std::cout << "Pojazd jest pelny." << std::endl;
        else
        {
            if(b.substract_food(value_))
            {
                double amount_ = (oxygen_cargo + food_cargo + water_cargo + building_materials_cargo + value_) - max_payload;
                food_cargo += value_;
                std::cout << "Jedzenie zostalo przetransportowane do pojazdu." << std::endl;
                if(amount_ > 0)
                {
                    food_cargo -= amount_;
                    b.add_food(amount_);
                    std::cout << amount_ << "j. jedzenia nie zmiescilo sie w pojezdzie." << std::endl;
                }
            }
            else
                std::cout << "W bazie nie ma wystarczajacej ilosci jedzenia." << std::endl;
        }
    }

    void Vehicle::take_water_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;

        else if(oxygen_cargo + food_cargo + water_cargo + building_materials_cargo == max_payload)
            std::cout << "Pojazd jest pelny." << std::endl;
        else
        {
            if(b.substract_water(value_))
            {
                double amount_ = (oxygen_cargo + food_cargo + water_cargo + building_materials_cargo + value_) - max_payload;
                water_cargo += value_;
                std::cout << "Woda zostala przetransportowana do pojazdu." << std::endl;
                if(amount_ > 0)
                {
                    water_cargo -= amount_;
                    b.add_water(amount_);
                    std::cout << amount_ << "j. wody nie zmiescilo sie w pojezdzie." << std::endl;
                }
            }
            else
                std::cout << "W bazie nie ma wystarczajacej ilosci wody." << std::endl;
        }
    }

    void Vehicle::take_building_materials_to_vehicle(RESOURCES_BASE_HUMAN::Base & b, int value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;

        else if(oxygen_cargo + food_cargo + water_cargo + building_materials_cargo == max_payload)
            std::cout << "Pojazd jest pelny." << std::endl;
        else
        {
            if(b.substract_building_materials(value_))
            {
                double amount_ = (oxygen_cargo + food_cargo + water_cargo + building_materials_cargo + value_) - max_payload;
                building_materials_cargo += value_;
                std::cout << "Materialy budowlane zostaly przetransportowane do pojazdu." << std::endl;
                if(amount_ > 0)
                {
                    building_materials_cargo -= amount_;
                    b.add_building_materials(amount_);
                    std::cout << amount_ << "j. materialow budowlanych nie zmiescilo sie w pojezdzie." << std::endl;
                }
            }
            else
                std::cout << "W bazie nie ma wystarczajacej ilosci materialow budowlanych." << std::endl;
        }
    }

    void Vehicle::take_oxygen_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;
        else if(oxygen_cargo < value_)
            std::cout << "W pojezdzie nie ma takiej ilosci tlenu." << std::endl;
        else if(value_ > 0)
        {
            b.add_oxygen(value_);
            oxygen_cargo -= value_;
            std::cout << "Tlen zostal przetransportowany do bazy." << std::endl;
        }
        else
            std::cout << "Podales zbyt mala ilosc tlenu." << std::endl;
    }

    void Vehicle::take_food_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;
        else if(food_cargo < value_)
            std::cout << "W pojezdzie nie ma takiej ilosci jedzenia." << std::endl;
        else if(value_ > 0)
        {
            b.add_food(value_);
            food_cargo -= value_;
            std::cout << "Jedzenie zostalo przetransportowane do bazy." << std::endl;
        }
        else
            std::cout << "Podales zbyt mala ilosc jedzenia." << std::endl;
    }

    void Vehicle::take_water_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;
        else if(water_cargo < value_)
            std::cout << "W pojezdzie nie ma takiej ilosci wody." << std::endl;
        else if(value_ > 0)
        {
            b.add_water(value_);
            water_cargo -= value_;
            std::cout << "Woda zostala przetransportowana do bazy." << std::endl;
        }
        else
            std::cout << "Podales zbyt mala ilosc wody." << std::endl;
    }

    void Vehicle::take_building_materials_from_vehicle(RESOURCES_BASE_HUMAN::Base & b, int value_)
    {
        if(is_in_use)
            std::cout << "Pojazd jest poza baza." << std::endl;
        else if(building_materials_cargo < value_)
            std::cout << "W pojezdzie nie ma takiej ilosci materialow budowlanych." << std::endl;
        else if(value_ > 0)
        {
            b.add_building_materials(value_);
            building_materials_cargo -= value_;
            std::cout << "Materialy budowlane zostaly przetransportowane do bazy." << std::endl;
        }
        else
            std::cout << "Podales zbyt mala ilosc materialow budowlanych." << std::endl;
    }

    int Vehicle::add_building_materials(int value_)
    {
        int possible_additional_bm = max_payload - oxygen_cargo - food_cargo - water_cargo - building_materials_cargo;
        if(possible_additional_bm >= value_)
        {
            building_materials_cargo += value_;
            return value_;
        }
        building_materials_cargo += possible_additional_bm;
        return possible_additional_bm;
    }

    bool Vehicle::substract_oxygen(double value_)
    {
        if(oxygen_cargo >= value_)
        {
            oxygen_cargo -= value_;
            return true;
        }
        return false;
    }

    bool Vehicle::substract_food(double value_)
    {
        if(food_cargo >= value_)
        {
            food_cargo -= value_;
            return true;
        }
        return false;
    }

    bool Vehicle::substract_water(double value_)
    {
        if(water_cargo >= value_)
        {
            water_cargo -= value_;
            return true;
        }
        return false;
    }

    void Vehicle::human_vital_signs()
    {
        for(auto & vector_part : employees)
        {
            vector_part->breathe(*this);
            vector_part->eat(*this);
            vector_part->drink(*this);
            if(vector_part->get_health_status() == "Nie zyje")
            {
                std::cout << vector_part->get_name() << " nie zyje." << std::endl;
                vector_part->change_working_status();
                working_people -= 1;
                vector_part = nullptr;
            }
        }
        check_employees();
    }

    bool Vehicle::build(RESOURCES_BASE_HUMAN::Base & b)
    {
        if(b.substract_building_materials(building_materials_cost))
            return true;
        else
        {
            std::cout << "Masz za malo materialow budowlanych." << std::endl;
            return false;
        }
    }

    void Vehicle::repair()
    {
        if(durability == 100)
            std::cout << "Pojazd jest w pelni sprawny." << std::endl;

        else if(calculate_repairing_cost() < building_materials_cargo)
        {
            building_materials_cargo -= calculate_repairing_cost();
            durability = 100;
            std::cout << "Pojazd zostal naprawiony." << std::endl;
        }
        else
        {
            int percents = building_materials_cargo / (building_materials_cost / 100);
            building_materials_cargo -= (percents * (building_materials_cost / 100));
            durability += percents;
            std::cout << "Zabraklo materialow na pelna naprawe pojazdu!\nPojazd zostal naprawiony o " << percents << "%." << std::endl;
        }
    }

    void Vehicle::decrease_durability()
    {
        durability -= 2;
        if(durability <= 0)
        {
            durability = 0;
            is_in_use = false;
            destroy_vehicle();
        }
    }

    void Vehicle::destroy_vehicle()
    {
        if(is_in_use)
        {
            for(auto & vector_part : employees)
            {
                vector_part->change_working_status();
                vector_part->kill_human();
                std::cout << vector_part->get_name() << " nie zyje." << std::endl;
                working_people -= 1;
                vector_part = nullptr;
            }
            check_employees();
        }
        else
        {
            for(auto & vector_part : employees)
                discard_employee(vector_part->get_name());
        }
    }

    void Vehicle::vehicle_details() const
    {
        std::cout << description << std::endl;
        std::cout << std::left << std::setw(50) << "Maksymalna ladownosc zasobow: " << max_payload << "j" << std::endl
                  << std::setw(50) << "Ilosc tlenu na pokladzie: " << oxygen_cargo << "j" << std::endl
                  << std::setw(50) << "Ilosc jedzenia na pokladzie: " << food_cargo << "j" << std::endl
                  << std::setw(50) << "Ilosc wody na pokladzie: " << water_cargo << "j" << std::endl
                  << std::setw(50) << "Ilosc materialow budowlanych na pokladzie: " << building_materials_cargo << "j" << std::endl
                  << std::setw(50) << "Predkosc: " << speed << std::endl
                  << std::setw(50) << "Maksymalna ilosc osob w pojezdzie: " << employees_limit << std::endl
                  << std::setw(50) << "Wytrzymalosc: " << durability << "%" << std::endl;
        std::cout << std::setw(50) << "Aktualnie w uzyciu: ";
        if(is_in_use)
            std::cout << "Tak" << std::endl;
        else
            std::cout << "Nie" << std::endl;

        std::cout << std::setw(50) << "Ilosc osob w pojezdzie: " << working_people << std::endl;
        if(employees.size() > 0)
        {
            std::cout << std::right << "Zaloga: \n" << std::endl
                      << "Imie i nazwisko" << std::setw(33) << "Stan zdrowia\n" << std::endl;

            for(auto vector_part : employees)
                std::cout << std::setw(35) << std::left << vector_part->get_name() << std::setw(25) << vector_part->get_health_status() << std::endl;
        }
    }

    void Vehicle::properties() const
    {
        std::cout << description << std::endl;
        std::cout << std::left << std::setw(40) << "Maksymalna ladownosc zasobow: " << max_payload << "j" << std::endl
                << std::setw(40) << "Predkosc: " << speed << std::endl
                << std::setw(40) << "Maksymalna ilosc osob w pojezdzie: " << employees_limit << std::endl;
        show_cost();
    }


}

