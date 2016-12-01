#include "additional.h"

namespace ADDITIONAL
{
    std::unique_ptr<Map[]> Expedition::mars_map;
    std::vector<Expedition::Expedition_info> Expedition::expedition_infos;

    void Expedition::set_locations(std::string directory)
    {
        std::ifstream fin;
        std::string full_directory = directory;

        for(int i=0; i<7; i++)
        {
            full_directory += INDEPENDENT::to_string(i) + ".txt";
            fin.open(full_directory, std::ios_base::in | std::ios_base::binary);

            if(!fin.is_open())
                std::cout << "Blad, nie udalo sie otworzyc pliku opisu lokacji." << std::endl;
            else
            {
                int string_length;
                Expedition_info temp;

                fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
                temp.place_description.resize(string_length);
                fin.read(reinterpret_cast<char*>(&temp.place_description[0]), string_length);
                fin.read(reinterpret_cast<char*>(&temp.x), sizeof(temp.x));
                fin.read(reinterpret_cast<char*>(&temp.y), sizeof(temp.y));
                fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
                temp.research_conclusions.resize(string_length);
                fin.read(reinterpret_cast<char*>(&temp.research_conclusions[0]), string_length);
                fin.read(reinterpret_cast<char*>(&temp.humans_needed), sizeof(temp.humans_needed));
                fin.read(reinterpret_cast<char*>(&temp.prize), sizeof(temp.prize));
                fin.read(reinterpret_cast<char*>(&temp.character_on_map), sizeof(temp.character_on_map));
                temp.state = false;

                expedition_infos.push_back(temp);
            }
            full_directory = directory;
            fin.close();
        }
        for(auto vector_part : expedition_infos)
            mars_map[vector_part.x][vector_part.y] = vector_part.character_on_map;
    }

    int Expedition::compute_width(int height)
    {
        int width = 52;

        for(int i=1; i<21; i++)
        {
            if(i>height)
                break;
            if(i<=3)
                width += 4;
            else if(i<11)
                width += 2;
            else if(i<18)
                width -= 2;
            else
                width -=4;
        }
        return width;
    }

    Expedition::Expedition()
    {
        map_displaying_starting_width = 0;
        vehicles_number = 0;
    }

    void Expedition::set_vehicles_on_map()
    {
        for(auto vector_part : vehicles)
        {
            if(vector_part.v.get_is_in_use())
                mars_map[vector_part.x][vector_part.y] = vector_part.character_on_map;
        }
    }

    void Expedition::set_map()
    {
        size_t height = 21, width = 52, first_addition = 4, second_addition = 2;

        mars_map = std::unique_ptr<Map[]>(new Map[height]);
        for(size_t i=0; i<height; ++i)
        {
            mars_map[i] = Map(new char[width]);
            if(i < 3)
                width += first_addition;

            else if(i <= 9)
                width += second_addition;

            else if(i <= 16)
                width -= second_addition;

            else if(i > 16 && i < 20)
                width -= first_addition;
        }

        for(size_t i=0; i<height; ++i)
        {
            for(size_t j=0; j<width; ++j)
                mars_map[i][j] = ' ';
            if(i < 3)
                width += first_addition;

            else if(i >= 3 && i <= 9)
                width += second_addition;

            else if(i > 9 && i <= 16)
                width -= second_addition;

            else if(i > 16 && i < 20)
                width -= first_addition;
        }
        set_locations();
    }

    void Expedition::show_map()
    {
        size_t height = 21, width = 22, first_addition = 4, second_addition = 2, line_filling_size = 26;
        std::cout << "/////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
        for(size_t i=0; i<height; ++i)
        {
            if(map_displaying_starting_width == 78)
                map_displaying_starting_width = 0;

            size_t background_fill = 0;
            if(i < 11)
            {
                while(background_fill != line_filling_size)
                {
                    std::cout << "/";
                    background_fill++;
                }
            }
            else
            {
                while(background_fill != line_filling_size)
                {
                    std::cout << "\\";
                    background_fill++;
                }
            }

            if(map_displaying_starting_width<31)
            {
                for(size_t j=map_displaying_starting_width; j<width+map_displaying_starting_width; ++j)
                        std::cout << mars_map[i][j];
            }
            else
            {
                size_t test = 0;
                for(size_t j = map_displaying_starting_width; j<width+30; j++)
                {
                    std::cout << mars_map[i][j];
                    test++;
                }
                for(size_t j = 0; j<width-test; j++)
                    std::cout << mars_map[i][j];
            }

            background_fill = 0;
            if(i < 11)
            {
                while(background_fill != line_filling_size)
                {
                    std::cout << "\\";
                    background_fill++;
                }
            }
            else
            {
                while(background_fill != line_filling_size)
                {
                    std::cout << "/";
                    background_fill++;
                }
            }

            if(i < 3)
            {
                width += first_addition;
                line_filling_size -= (first_addition/2);
            }
            else if(i >= 3 && i <= 9)
            {
                width += second_addition;
                line_filling_size -= (second_addition/2);
            }
            else if(i > 9 && i <= 16)
            {
                width -= second_addition;
                line_filling_size += (second_addition/2);
            }
            else if(i > 16 && i < 20)
            {
                width -= first_addition;
                line_filling_size += (first_addition/2);
            }
            std::cout << std::endl;
        }
        std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/////////////////////////////////////" << std::endl;
    }

    bool Expedition::move_map(char direction)
    {
        switch(direction)
        {
            case 'D':
            case 'd': map_displaying_starting_width++; return true;
            case 'A':
            case 'a':
                    if(map_displaying_starting_width == 0)
                        map_displaying_starting_width = 77;
                    else
                        map_displaying_starting_width--;
                    return true;
            default: std::cout << "Wpisales zly znak." << std::endl; return false;
        }
    }

    bool Expedition::show_info(char choice) const
    {
        choice = toupper(choice);

        for(auto vector_part : expedition_infos)
        {
            if(choice == vector_part.character_on_map)
            {
                std::cout << "Oznaczenie na mapie: " << vector_part.character_on_map << std::endl;
                std::cout << "Opis: " << std::endl;
                INDEPENDENT::cut_and_show(vector_part.place_description);
                if(choice != 'B')
                {
                    std::cout << "Minimalna potrzebna ilosc ludzi do zbadania tego miejsca: " << vector_part.humans_needed << std::endl;

                    std::cout << "Odwiedzone: ";
                    if(vector_part.state)
                        std::cout << "Tak" << std::endl;
                    else
                        std::cout << "Nie" << std::endl;
                }
                return true;
            }
        }
        return false;
    }

    void Expedition::save_expedition(std::string directory1, std::string directory2)
    {
        std::ofstream fout;

        fout.open(directory1, std::ios_base::out | std::ios_base::binary);
        if(!fout.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu informacji na temat ekspedycji." << std::endl;
        else
        {
            fout.write(reinterpret_cast<char*>(&vehicles_number), sizeof(vehicles_number));
            for(auto vector_part : expedition_infos)
                fout.write(reinterpret_cast<char*>(&vector_part.state), sizeof(vector_part.state));
        }
        fout.close();

        for(unsigned int i=0; i<vehicles.size(); i++)
        {
            std::string temp_directory = directory2;
            vehicles[i].v.save_vehicle(i, temp_directory);
            temp_directory += INDEPENDENT::to_string(i) + ".txt";

            fout.open(temp_directory, std::ios_base::out | std::ios_base::binary | std::ios_base::app);

            if(!fout.is_open())
                std::cout << "Blad, nie udalo sie otworzyc pliku zapisu szczegolowych informacji na temat pojazdow." << std::endl;
            else
            {
                fout.write(reinterpret_cast<char*>(&vehicles[i].x), sizeof(vehicles[i].x));
                fout.write(reinterpret_cast<char*>(&vehicles[i].y), sizeof(vehicles[i].y));
                fout.write(reinterpret_cast<char*>(&vehicles[i].moved_this_turn), sizeof(vehicles[i].moved_this_turn));
                fout.write(reinterpret_cast<char*>(&vehicles[i].character_on_map), sizeof(vehicles[i].character_on_map));
            }
            fout.close();
        }
    }

    void Expedition::load_expedition(std::list<RESOURCES_BASE_HUMAN::Human> & l, std::string directory1, std::string directory2)
    {
        std::ifstream fin;

        fin.open(directory1, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu informacji na temat ekspedycji." << std::endl;
        else
        {
            fin.read(reinterpret_cast<char*>(&vehicles_number), sizeof(vehicles_number));
            for(auto vector_part : expedition_infos)
                fin.read(reinterpret_cast<char*>(&vector_part.state), sizeof(vector_part.state));
        }
        fin.close();

        for(int i=0; i<vehicles_number; i++)
        {
            std::string temp_directory = directory2;
            temp_directory += INDEPENDENT::to_string(i) + ".txt";

            Vehicle_on_map vehicle;
            BUILDINGS_VEHICLE::Vehicle veh;
            veh.load_vehicle(i,l, directory2);
            vehicle.v = veh;

            fin.open(temp_directory, std::ios_base::in | std::ios_base::binary);

            if(!fin.is_open())
                std::cout << "Blad, nie udalo sie otworzyc pliku zapisu szczegolowych informacji na temat pojazdow." << std::endl;
            else
            {
                fin.seekg(INDEPENDENT::get_file_length(temp_directory)-10, std::ios::beg);                      //ten is the size of this part of file
                fin.read(reinterpret_cast<char*>(&vehicle.x), sizeof(vehicle.x));
                fin.read(reinterpret_cast<char*>(&vehicle.y), sizeof(vehicle.y));
                fin.read(reinterpret_cast<char*>(&vehicle.moved_this_turn), sizeof(vehicle.moved_this_turn));
                fin.read(reinterpret_cast<char*>(&vehicle.character_on_map), sizeof(vehicle.character_on_map));
            }
            fin.close();
            vehicles.push_back(vehicle);
        }
    }

    bool Expedition::add_vehicle(BUILDINGS_VEHICLE::Vehicle & veh, char sign)
    {
        int sign_int = static_cast<int>(sign) - '0';
        if(sign_int <= 0 || sign_int > 9)
            return false;

        bool not_used = true;
        for(auto vector_part : vehicles)
        {
            if(vector_part.character_on_map == sign)
            {
                not_used = false;
                break;
            }
        }
        if(not_used)
        {
            Vehicle_on_map vehicle;
            vehicle.v = veh;
            vehicle.x = expedition_infos[0].x;
            vehicle.y = expedition_infos[0].y;
            vehicle.moved_this_turn = false;
            vehicle.character_on_map = sign;
            vehicles.push_back(vehicle);
            vehicles_number++;
            return true;
        }
        return false;
    }

    bool Expedition::del_vehicle(char choice)
    {
        bool found = false;
        int i = 0;
        for(auto vector_part : vehicles)
        {
            if(vector_part.character_on_map == choice)
            {
                vector_part.v.destroy_vehicle();
                found = true;
                break;
            }
            i++;
        }
        if(found)
        {
            vehicles.erase(vehicles.begin() + i);
            vehicles_number--;
        }
        return found;
    }

    bool Expedition::repair_vehicle(char vehicle_sign)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.repair();
                return true;
            }
        }
        return false;
    }

    bool Expedition::add_human_to_vehicle(RESOURCES_BASE_HUMAN::Human & h, char vehicle_sign)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                h.set_in_base(false);
                bool added = vector_part.v.add_employee(h);
                return added;
            }
        }
        return false;
    }

    bool Expedition::discard_human_from_vehicle(std::string name, char vehicle_sign)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                bool discarded = vector_part.v.discard_employee(name);
                return discarded;
            }
        }
        return false;
    }

    void Expedition::vital_signs()
    {
        for(auto & vector_part : vehicles)
            vector_part.v.human_vital_signs();
    }

    bool Expedition::oxygen_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_oxygen_to_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::food_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_food_to_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::water_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_water_to_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::building_materials_to_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, int value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_building_materials_to_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::oxygen_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_oxygen_from_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::food_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_food_from_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::water_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, double value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_water_from_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::building_materials_from_vehicle(char vehicle_sign, RESOURCES_BASE_HUMAN::Base & b, int value_)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.take_building_materials_from_vehicle(b, value_);
                return true;
            }
        }
        return false;
    }

    bool Expedition::start_expedition(char vehicle_sign)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                if(vector_part.v.get_is_in_use() == false && vector_part.v.get_working_people() > 0 && vector_part.v.get_durability() > 0)
                {
                    vector_part.v.change_is_in_use();
                    vector_part.y -= 1;
                    mars_map[vector_part.x][vector_part.y] = vector_part.character_on_map;
                    return true;
                }
                break;
            }
        }
        return false;
    }

    bool Expedition::interact(Vehicle_on_map & vehicle)
    {
        for(auto & vector_part : expedition_infos)
        {
            if(vector_part.x == vehicle.x && vector_part.y == vehicle.y)
            {
                if(vector_part.character_on_map == 'B')
                {
                    std::cout << vector_part.research_conclusions << std::endl;
                    vehicle.v.change_is_in_use();
                    mars_map[vector_part.x][vector_part.y] = vector_part.character_on_map;
                }

                else
                {
                    if(vector_part.state)
                    {
                        std::cout << "To miejsce jest juz zbadane. Wnioski: " << std::endl;
                        INDEPENDENT::cut_and_show(vector_part.research_conclusions);
                    }

                    else
                    {
                        if(vector_part.humans_needed > vehicle.v.get_working_people())
                            std::cout << "Wymagana liczba ludzi do zbadania tego miejsca: " << vector_part.humans_needed << std::endl;
                        else
                        {
                            std::cout << "Miejsce: " << std::endl;
                            INDEPENDENT::cut_and_show(vector_part.place_description);
                            std::cout << "Oznaczenie na mapie: " << vector_part.character_on_map << std::endl;
                            std::cout << "Wnioski: " << std::endl;
                            INDEPENDENT::cut_and_show(vector_part.research_conclusions);
                            std::cout << "Dodatkowo udalo nam sie wydobyc " << vehicle.v.add_building_materials(vector_part.prize) << " jednostek materialow budowlanych," << std::endl
                                      << "ktore zaladowalismy juz do pojazdu." << std::endl;
                            vector_part.state = true;
                        }
                    }
                }
                std::cin.get();
                return true;
            }
        }
        return false;
    }

    bool Expedition::move_mechanics(char direction, Vehicle_on_map & vehicle)
    {
        switch(direction)
        {
        case 'W':
        case 'w':
            if(vehicle.x <= 3)
                vehicle.y -= 2;
            else if(vehicle.x < 11)
                vehicle.y -= 1;
            else if(vehicle.x > 17)
                vehicle.y += 2;
            else
                vehicle.y += 1;

            vehicle.x -= 1; break;
        case 'S':
        case 's':
            if(vehicle.x <= 3)
                vehicle.y += 2;
            else if(vehicle.x < 10)
                vehicle.y += 1;
            else if(vehicle.x > 16)
                vehicle.y -= 2;
            else
                vehicle.y -= 1;

            vehicle.x += 1; break;
        case 'A':
        case 'a':
            vehicle.y -= 1; break;
        case 'D':
        case 'd':
            vehicle.y += 1; break;
        default:
            std::cout << "Nie podales znaku kierunkowego. Ruch zostaje przerwany." << std::endl;
            return false;
        }
        if(vehicle.y < 0)
            vehicle.y = compute_width(vehicle.x);
        if(vehicle.y > compute_width(vehicle.x))
            vehicle.y = 0;

        if(vehicle.x < 0)
        {
            vehicle.x = 0;
            vehicle.y += 26;
            if(vehicle.y > compute_width(vehicle.x))
                vehicle.y = 0 + (vehicle.y - compute_width(vehicle.x));
        }

        if(vehicle.x > 20)
        {
            vehicle.x = 20;
            vehicle.y += 26;
            if(vehicle.y > compute_width(vehicle.x))
                vehicle.y = 0 + (vehicle.y - compute_width(vehicle.x));
        }
        return true;
    }

    bool Expedition::move_vehicle(char vehicle_sign)
    {
        for(auto & vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign && vector_part.v.get_is_in_use() == true && vector_part.moved_this_turn == false && vector_part.v.get_working_people() > 0)
            {
                char direction, previous_char = ' ';
                for(auto vector_part_info : expedition_infos)
                {
                    if(vector_part_info.x == vector_part.x && vector_part_info.y == vector_part.y)
                    {
                        previous_char = vector_part_info.character_on_map;
                        break;
                    }
                }
                for(auto vector_part_vehicles : vehicles)
                {
                    if(vector_part_vehicles.x == vector_part.x && vector_part_vehicles.y == vector_part.y
                        && vector_part_vehicles.character_on_map != vector_part.character_on_map && vector_part_vehicles.v.get_is_in_use())
                    {
                        previous_char = vector_part_vehicles.character_on_map;
                        break;
                    }
                }

                for(int i = vector_part.v.get_speed(); i>0; i--)
                {
                    std::cout << std::string(300, '\n') << std::endl;
                    Expedition::show_map();
                    std::cout << "Podaj kierunek: " << std::endl;
                    std::cin.get(direction).get();
                    mars_map[vector_part.x][vector_part.y] = previous_char;

                    if(!move_mechanics(direction, vector_part))
                        i=0;
                    previous_char = mars_map[vector_part.x][vector_part.y];
                    mars_map[vector_part.x][vector_part.y] = vector_part.character_on_map;
                    if(interact(vector_part))
                        break;
                }
                vector_part.moved_this_turn = true;
                vector_part.v.decrease_durability();
                return true;
            }
        }
        return false;
    }

    void Expedition::change_moved_this_turn(char vehicle_sign)
    {
        for(auto vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                if(vector_part.moved_this_turn)
                    vector_part.moved_this_turn = false;
                else
                    vector_part.moved_this_turn = true;
            }
        }
    }

    bool Expedition::check_if_vehicle_is_on_map(char vehicle_sign)
    {
        for(auto vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign && vector_part.v.get_is_in_use())
                return true;
        }
        return false;
    }

    bool Expedition::check_if_vehicle_exists(char vehicle_sign)
    {
        for(auto vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
                return true;
        }
        return false;
    }

    bool Expedition::check_if_place_exists(char place_sign)
    {
        place_sign = toupper(place_sign);
        for(auto vector_part : expedition_infos)
        {
            if(vector_part.character_on_map == place_sign)
                return true;
        }
        return false;
    }

    void Expedition::show_vehicle(char vehicle_sign) const
    {
        for(auto vector_part : vehicles)
        {
            if(vector_part.character_on_map == vehicle_sign)
            {
                vector_part.v.vehicle_details();
                std::cout << std::setw(51) << "\nWykonal ruch w tym tygodniu: ";
                if(vector_part.moved_this_turn)
                    std::cout << "Tak" << std::endl;
                else
                    std::cout << "Nie" << std::endl;
                std::cout << std::setw(50) << "Znak na mapie: " << vector_part.character_on_map << std::endl;
                break;
            }
        }
    }

    void Expedition::show_vehicles() const
    {
        std::cout << std::setw(42) << "Liczba pojazdow: " << vehicles_number << std::endl << std::endl;
        for(auto vector_part : vehicles)
        {
            vector_part.v.vehicle_details();
            std::cout << std::setw(51) << "\nWykonal ruch w tym tygodniu: ";
            if(vector_part.moved_this_turn)
                std::cout << "Tak" << std::endl;
            else
                std::cout << "Nie" << std::endl;
            std::cout << std::setw(50) << "Znak na mapie: " << vector_part.character_on_map << "\n\n\n";
        }
    }
}
