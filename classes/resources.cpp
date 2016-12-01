#include "resources.h"

namespace RESOURCES_BASE_HUMAN
{

    //----------------------------------------------------------------------------------------------------------------------------
    //class Resources
    //----------------------------------------------------------------------------------------------------------------------------

    Resources::Resources(double ox, double f, double w, int b_m, int hum)
    {
        oxygen = ox;
        food = f;
        water = w;
        building_materials = b_m;
        humans = hum;
    }

    Resources::Resources(const Resources & r)
    {
        oxygen = r.oxygen;
        food = r.food;
        water = r.water;
        building_materials = r.building_materials;
        humans = r.humans;
    }

    Resources & Resources::operator=(const Resources & r)
    {
        if(this == &r)
            return *this;
        oxygen = r.oxygen;
        food = r.food;
        water = r.water;
        building_materials = r.building_materials;
        humans = r.humans;

        return *this;
    }

    void Resources::show_resources() const
    {
        using std::cout;
        using std::endl;
        cout << "Zyjacy ludzie: " << humans << endl
             << "Ilosc tlenu: " << oxygen << "j" << endl
             << "Ilosc jedzenia: " << food << "j" << endl
             << "Ilosc wody: " << water << "j" << endl
             << "Materialy budowlane: " << building_materials << "j" << endl;
    }

    std::ostream & operator<<(std::ostream & os, const Resources & r)
    {
        using std::endl;
        os << "Zyjacy ludzie: " << r.humans << endl
             << "Ilosc tlenu: " << r.oxygen << "j" << endl
             << "Ilosc jedzenia: " << r.food << "j" << endl
             << "Ilosc wody: " << r.water << "j" << endl
             << "Materialy budowlane: " << r.building_materials << "j" << endl;
        return os;
    }

    //----------------------------------------------------------------------------------------------------------------------------
    // class Base
    //----------------------------------------------------------------------------------------------------------------------------
    void Base::base_description(std::string directory) const
    {
        std::ifstream fin;
        std::string temp;
        fin.open(directory, std::fstream::in);
        if(!fin.is_open())
            std::cout << "Brakuje pliku opisowego!" << std::endl << std::endl;

        else
        {
            int i = 0;
            while(getline(fin, temp))
            {
                ++i;
                if(i == base_level+1)
                    break;
            }

            if(temp.size() == 0)
                std::cout << "Plik z opisem prawdopodobnie uszkodzony!" << std::endl << std::endl;

            std::cout << "Opis bazy marsjanskiej: " << std::endl;
            INDEPENDENT::cut_and_show(temp);
            std::cout << std::endl;
        }
        fin.close();
    }

    void Base::update_max_storage()
    {
        max_oxygen *= 2;
        max_food *= 2;
        max_water *= 2;
        max_building_materials *= 2;
        max_humans = max_humans + base_level * 2 + 2;
    }

    Base::Base(const Base & b) : Resources(b)
    {
        base_level = b.base_level;
        max_oxygen = b.max_oxygen;
        max_food = b.max_food;
        max_water = b.max_water;
        max_building_materials = b.max_building_materials;
        max_humans = b.max_humans;
    }

    void Base::save_base(const std::string file_name_)
    {
        std::ofstream fout;

        fout.open(file_name_, std::ios_base::out | std::ios_base::binary);
        if(!fout.is_open())
            std::cout << "Blad, nie mozna otworzyc pliku sluzacego do zapisu!" << std::endl;

        else
        {
            fout.write(reinterpret_cast<char*> (&oxygen), sizeof(oxygen));
            fout.write(reinterpret_cast<char*> (&food), sizeof(food));
            fout.write(reinterpret_cast<char*> (&water), sizeof(water));
            fout.write(reinterpret_cast<char*> (&comfort), sizeof(comfort));
            fout.write(reinterpret_cast<char*> (&building_materials), sizeof(building_materials));
            fout.write(reinterpret_cast<char*> (&humans), sizeof(humans));
            fout.write(reinterpret_cast<char*> (&base_level), sizeof(base_level));
            fout.write(reinterpret_cast<char*> (&max_oxygen), sizeof(max_oxygen));
            fout.write(reinterpret_cast<char*> (&max_food), sizeof(max_food));
            fout.write(reinterpret_cast<char*> (&max_water), sizeof(max_water));
            fout.write(reinterpret_cast<char*> (&max_building_materials), sizeof(max_building_materials));
            fout.write(reinterpret_cast<char*> (&max_humans), sizeof(max_humans));
        }
        fout.close();
    }

    void Base::load_base(const std::string file_name_)
    {
        std::ifstream fin;

        fin.open(file_name_, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
            std::cout << "Brak pliku z zapisana gra." << std::endl;

        else
        {
            fin.read(reinterpret_cast<char*> (&oxygen), sizeof(oxygen));
            fin.read(reinterpret_cast<char*> (&food), sizeof(food));
            fin.read(reinterpret_cast<char*> (&water), sizeof(water));
            fin.read(reinterpret_cast<char*> (&comfort), sizeof(comfort));
            fin.read(reinterpret_cast<char*> (&building_materials), sizeof(building_materials));
            fin.read(reinterpret_cast<char*> (&humans), sizeof(humans));
            fin.read(reinterpret_cast<char*> (&base_level), sizeof(base_level));
            fin.read(reinterpret_cast<char*> (&max_oxygen), sizeof(max_oxygen));
            fin.read(reinterpret_cast<char*> (&max_food), sizeof(max_food));
            fin.read(reinterpret_cast<char*> (&max_water), sizeof(max_water));
            fin.read(reinterpret_cast<char*> (&max_building_materials), sizeof(max_building_materials));
            fin.read(reinterpret_cast<char*> (&max_humans), sizeof(max_humans));
        }
        fin.close();
    }

    bool Base::substract_building_materials(int value_)
    {
        if(building_materials >= value_ && value_ > 0)
        {
            building_materials -= value_;
            return true;
        }
        return false;
    }

    bool Base::substract_oxygen(double value_)
    {
        if(oxygen >= value_ && value_ > 0)
        {
             oxygen -= value_;
             return true;
        }
        return false;
    }

    bool Base::substract_food(double value_)
    {
        if(food >= value_ && value_ > 0)
        {
            food -= value_;
            return true;
        }
        return false;
    }

    bool Base::substract_water(double value_)
    {
        if(water >= value_ && value_ > 0)
        {
            water -= value_;
            return true;
        }
        return false;
    }

    void Base::add_comfort(int value_)
    {
        comfort += value_;
        if(comfort > 100)
            comfort = 100;
    }

    void Base::add_food(double value_)
    {
        food += value_;
        if(food > max_food)
            food = max_food;
    }

    void Base::add_oxygen(double value_)
    {
        oxygen += value_;
        if(oxygen > max_oxygen)
            oxygen = max_oxygen;
    }

    void Base::add_water(double value_)
    {
        water += value_;
        if(water > max_water)
            water = max_water;
    }

    void Base::add_building_materials(int value_)
    {
        building_materials += value_;
        if(building_materials > max_building_materials)
            building_materials = max_building_materials;
    }

    bool Base::expand_base()
    {
        if(base_level >= 8)
        {
            std::cout << "Osiagnales maksymalny poziom bazy!" << std::endl;
            return false;
        }
        else if(substract_building_materials((max_building_materials*7)/10))                     //max_building_materials * 0.7 makes an error (int)
        {
            substract_building_materials((max_building_materials*7)/10);
            update_max_storage();
            base_level++;
            add_comfort(10);
            std::cout << "Baza zostala rozbudowana." << std::endl;
            return true;
        }
        else
        {
            std::cout << "Masz za malo materialow budowlanych! Brakuje ci jeszcze " << (max_building_materials*7)/10 - building_materials << "j "
                      << std::endl << "materialow budowlanych." << std::endl;
            return false;
        }
    }

    void Base::base_statistics() const
    {
        using std::endl;
        std::cout << "Informacje na temat bazy: " << endl
                  << "Ludzie: " << humans << "/" << max_humans << endl
                  << "Tlen: " << oxygen << "j/" << max_oxygen << "j" << endl
                  << "Jedzenie: " << food << "j/" << max_food << "j" << endl
                  << "Woda: " << water << "j/" << max_water << "j" << endl
                  << "Komfort: " << comfort << "%" << endl
                  << "Materialy budowlane: " << building_materials << "j/" << max_building_materials << "j" << endl;
        base_description();
    }

    //----------------------------------------------------------------------------------------------------------------------------
    // class Human
    //----------------------------------------------------------------------------------------------------------------------------
    void Human::become_more_ill()
    {
        int i = health;
        i++;
        if(i == 1)
            health = Sick;
        else if(i == 2)
            health = Very_sick;
        else
            health = Dead;
    }

    void Human::become_healthier()
    {
        int i = health;
        i--;
        if(i == 1)
            health = Sick;
        else if(i<=0)
            health = Healthy;
        else
            health = Dead;
    }

    int Human::calculate_number_of_names()
    {
        int i = 0;
        std::ifstream fin;
        fin.open("saves\\used_names.txt", std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
            std::cout << "Blad, nie mozna otworzyc pliku z zapisanymi uzytymi imionami w funkcji liczacej ilosc imion!" << std::endl;
        else
        {
            char temp[30];
            while(fin.read(reinterpret_cast<char*>(&temp), sizeof(temp)))
                i++;
        }
        fin.close();
        return 345 - i;
    }

    void Human::to_used_names(std::string directory)
    {
        std::ofstream fout;
        fout.open(directory, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
        if(!fout.is_open())
            std::cout << "Blad, nie mozna otworzyc pliku z zapisanymi uzytymi imionami!" << std::endl;
        else
        {
            char temp[30];
            strcpy(temp, name.c_str());
            fout.write(reinterpret_cast<char*>(&temp), sizeof(temp));
        }
        fout.close();
    }

    Human::Human(Base & b, std::string directory, std::string used_names_directory)
    {
        if(b.humans < b.max_humans)
        {
            int names = calculate_number_of_names();
            if(names == 0)
                name = "Jan" + INDEPENDENT::to_string(rand() % 10000);
            else
            {
                char to_write[30] = "                             ";
                std::ifstream fin;
                std::ofstream fout;

                fin.open(directory, std::ios_base::in | std::ios_base::binary);
                fout.open("saves\\temporary.txt", std::ios_base::out | std::ios_base::app | std::ios_base::binary);
                if(!fin.is_open() || !fout.is_open())
                    std::cout << "Blad, nie mozna otworzyc pliku z imionami." << std::endl;
                else
                {
                    int j = rand() % names;
                    int i=0;
                    char temp[30];

                    while(fin.read(reinterpret_cast<char*>(&temp), sizeof(temp)))
                    {
                        if(strcmp(temp,"                             ") == 0)
                        {
                            continue;
                        }

                        else
                        {
                            if(i == j && strcmp(temp,"                             ") != 0)
                            {
                                name = temp;
                                strcpy(temp, to_write);
                            }
                            fout.write(reinterpret_cast<char*>(&temp), sizeof(temp));
                        }
                        i++;
                    }
                    fin.close();
                    fout.close();

                    char * directory_temp = new char[directory.size()+1];
                    strcpy(directory_temp, directory.c_str());
                    std::remove(directory_temp);
                    std::rename("saves\\temporary.txt", directory_temp);
                    delete [] directory_temp;
                    to_used_names(used_names_directory);
                }
                working = 0;
                health = Healthy;
                b.humans += 1;
                in_base = true;
            }
        }
        else
            std::cout << "Wiecej ludzi nie zmiesci sie w bazie!" << std::endl;
    }

    std::string Human::get_health_status()  const
    {
        if(health == Healthy)
            return "Zdrowy";

        else if(health == Sick)
            return "Chory";

        else if(health == Very_sick)
            return "Krytyczny";
        else
            return "Nie zyje";
    }

    std::string Human::calculate_healthness(Base & b)
    {
        int chance_for_better_health, chance_for_worse_health, comf;

        for(comf = 0; comf <= 100; comf+=10)
        {
            if(b.comfort <= comf)
            {
                chance_for_better_health = comf + 50;
                break;
            }
        }

        if(health == Sick)
        {
            chance_for_worse_health = chance_for_better_health - comf * 1.20;
            if(working)
                chance_for_worse_health += 50;
        }

        else if(health == Very_sick)
        {
            chance_for_worse_health = chance_for_better_health - comf;
            if(working)
                chance_for_worse_health += 100;
        }

        else
            chance_for_worse_health = chance_for_better_health - comf * 1.35;

        int no_changes = 450 * (b.comfort / 100.0);
        int result = rand() % (no_changes + chance_for_better_health + chance_for_worse_health) + 1;

        if(result <= chance_for_worse_health && health != Dead)
        {
            become_more_ill();
            if(health == Dead)
                return name + " umarl.";
            else
            return name + " sie rozchorowal.";
        }
        else if(result >= (chance_for_worse_health + no_changes) && result <=  (chance_for_worse_health + no_changes + chance_for_better_health) && health != Healthy)
        {
            become_healthier();
            return name + " wyzdrowial.";
        }
        else
            return "\0";
    }

    void Human::save_human(std::string directory)
    {
        std::ofstream fout;
        fout.open(directory, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
        if(!fout.is_open())
            std::cout << "Blad, nie mozna otworzyc pliku zapisu ludzi!" << std::endl;
        else
        {
            char temp[30];
            strcpy(temp, name.c_str());
            fout.write(reinterpret_cast<char*>(&temp), sizeof(temp));
            fout.write(reinterpret_cast<char*>(&working), sizeof(working));
            fout.write(reinterpret_cast<char*>(&health), sizeof(health));
            fout.write(reinterpret_cast<char*>(&in_base), sizeof(in_base));
        }
        fout.close();
    }

    void Human::load_human(int already_loaded, std::string directory)
    {
        std::ifstream fin;
        fin.open(directory, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
            std::cout << "Blad, nie mozna otworzyc pliku zapisu ludzi!" << std::endl;
        else
        {
            char temp[30];

            int size = (sizeof(bool) + sizeof(Healthness) + sizeof(temp) + sizeof(in_base)) * already_loaded;
            fin.seekg(size, std::ios::beg);

            fin.read(reinterpret_cast<char*>(&temp), sizeof(temp));
            name = temp;
            fin.read(reinterpret_cast<char*>(&working), sizeof(working));
            fin.read(reinterpret_cast<char*>(&health), sizeof(health));
            fin.read(reinterpret_cast<char*>(&in_base), sizeof(in_base));
        }
        fin.close();
    }

    void Human::change_working_status()
    {
        if(working)
            working = false;
        else
            working = true;
    }

    void Human::show_ID() const
    {
        std::cout << "Imie i nazwisko: " << name << std::endl
                  << "Czy pracuje: ";
        if(working)
            std::cout << "Tak" << std::endl;
        else
            std::cout << "Nie" << std::endl;
        std::cout << "Stan zdrowia: ";
        if(health == Healthy)
            std::cout << "Zdrowy" << std::endl;
        else if(health == Sick)
            std::cout << "Chory" << std::endl;
        else if(health == Very_sick)
            std::cout << "Krytyczny" << std::endl;
        else
            std::cout << "Nie zyje" << std::endl;
    }

    bool Human::if_died(Base & b)
    {
        if(health == Dead)
        {
            b.humans--;
            return true;
        }
        return false;
    }

}
