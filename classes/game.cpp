#include "game.h"

namespace GAME
{
    void Game::introduction(std::string directory)
    {
        std::ifstream  fin(directory);
        if(!fin.is_open())
            std::cout << "Blad przy otwieraniu pliku wprowadzenia." << std::endl;
        else
        {
            std::string text;
            getline(fin, text);
            INDEPENDENT::cut_and_show(text);
            set_nickname();
            getline(fin, text);
            INDEPENDENT::cut_and_show(text);
            int crash = rand() % 10000;
            if(crash == 6134)
            {
                std::cout << "Ladowanie sie nie udalo." << std::endl;
                getline(fin,text);
                INDEPENDENT::cut_and_show(text);
                reset_game();
                game_over();
                std::cout << "Nacisnij enter, aby powrocic do glownego menu." << std::endl;
                std::cin.get(); std::cin.get();
                main_menu();
            }
            else
                std::cout << "Ladowanie przebieglo pomyslnie, mozemy rozpoczac kolonizacje!\nOto twoj panel sterowania: \n\n";
        }
        fin.close();
    }

    void Game::ending_story(std::string directory)
    {
        if(check_for_win())
        {
            if(died_humans < 15)
                directory += "good_ending.txt";
            else
                directory += "bad_ending.txt";
        }

        std::ifstream  fin(directory);
        if(!fin.is_open())
            std::cout << "Blad przy otwieraniu pliku z koncowa fabula." << std::endl;
        else
        {
            std::string text;
            getline(fin, text);
            INDEPENDENT::cut_and_show(text);
        }
    }

    void Game::set_nickname()
    {
        std::cin.get();
        getline(std::cin, nickname);
    }

    void Game::reset_game()
    {
        Game new_game;
        *this = new_game;
    }

    void Game::prepare_game()
    {
        std::remove("saves//game.txt");
        std::remove("saves//names.txt");
        std::ofstream  fout("saves//used_names.txt", std::ios::binary);         //clear used_names
        fout.close();
        std::remove("saves//humans_save.txt");
        std::remove("saves//base_resources_save.txt");
        std::remove("saves//buildings//is_builded.txt");
        std::remove("saves//expedition//expedition.txt");
        for(size_t i=0; i<10; i++)
        {
            std::string rem_directory = "saves//vehicles//vehicle";
            rem_directory += INDEPENDENT::to_string(i) + ".txt";
            const char * rem_directory_char = rem_directory.c_str();
            std::remove(rem_directory_char);

            if(i>=0 && i<=7)
            {
                std::string rem_directory = "saves//buildings//building";
                rem_directory += INDEPENDENT::to_string(i) + ".txt";
                const char * rem_directory_char = rem_directory.c_str();
                std::remove(rem_directory_char);
            }
        }

        std::ifstream  src("descriptions//names.txt", std::ios::binary);
        std::ofstream  dst("saves/names.txt",   std::ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
    }

    void Game::results_to_file(std::string directory)
    {
        std::ifstream fin(directory, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
        {
            std::ofstream fout(directory, std::ios_base::out | std::ios_base::binary);
            int string_length = nickname.size();
            fout.write(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            fout.write(reinterpret_cast<char*>(&nickname[0]), string_length);
            fout.write(reinterpret_cast<char*>(&week), sizeof(week));
            fout.write(reinterpret_cast<char*>(&died_humans), sizeof(died_humans));
            fout.close();
        }
        else
        {
            int string_length, read_week, read_died;
            std::string read_nickname;
            fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            read_nickname.resize(string_length);
            fin.read(reinterpret_cast<char*>(&read_nickname[0]), string_length);
            fin.read(reinterpret_cast<char*>(&read_week), sizeof(read_week));
            fin.read(reinterpret_cast<char*>(&read_died), sizeof(read_died));
            fin.close();

            int score1 = week * (-1) - died_humans;
            int score2 = read_week * (-1) - read_died;

            if(score1 > score2)
            {
                std::ofstream fout(directory, std::ios_base::out | std::ios_base::binary);
                int string_length = nickname.size();
                fout.write(reinterpret_cast<char*>(&string_length), sizeof(string_length));
                fout.write(reinterpret_cast<char*>(&nickname[0]), string_length);
                fout.write(reinterpret_cast<char*>(&week), sizeof(week));
                fout.write(reinterpret_cast<char*>(&died_humans), sizeof(died_humans));
                fout.close();
            }
        }
    }

    void Game::show_tips(std::string directory)
    {
        std::ifstream  fin(directory);
        if(!fin.is_open())
            std::cout << "Blad przy otwieraniu pliku z podpowiedziami." << std::endl;
        else
        {
            std::string text;
            while(getline(fin, text))
                INDEPENDENT::cut_and_show(text);
        }
    }

    void Game::show_humans() const
    {
        std::cout << std::string(300, '\n');
        int i=1;
        for(auto list_part : humans)
        {
            list_part.show_ID();
            std::cout << "\n";
            if(i%15 == 0)
            {
                std::cout << "\nNacisnij enter, aby przejsc dalej." << std::endl;
                std::cin.get(); std::cin.get();
            }
            i++;
        }
    }

    void Game::show_not_working_humans_names() const
    {
        for(auto list_part : humans)
        {
            if(list_part.get_working_status() == false)
                std::cout << list_part.get_name() << std::endl;
        }
    }

    void Game::display_not_builded()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\tInfrastruktura do zbudowania" << std::endl;

        std::vector<BUILDINGS_VEHICLE::Buildings> display;
        for(int i=1; i<=7; i++)
        {
            bool validate = true;
            BUILDINGS_VEHICLE::Buildings building(i);
            for(auto vector_part : buildings)
            {
                if(vector_part == building)
                {
                    validate = false;
                    break;
                }
            }
            if(validate)
                display.push_back(building);
        }

        if(display.size() == 0)
            std::cout << "Wszystko zostalo juz budowane" << std::endl;
        else
        {
            int i=1;
            for(auto vector_part : display)
            {
                std::cout << i << "." << std::endl;
                INDEPENDENT::cut_and_show(vector_part.get_description());
                vector_part.show_production();
                std::cout << std::endl;
                vector_part.show_cost();
                std::cout << "\n\n";
                i++;
            }
            std::cout << "Gdy wpiszesz numer budowli zostanie podjeta proba zbudowania jej, \nw przeciwnym razie wrocisz do poprzedniego menu.\n" << std::endl;
            char ch = INDEPENDENT::get_validated_char();
            size_t choice = static_cast<size_t>(ch) - '0';
            if(choice > 0 && choice <= display.size())
                build_building(display[choice-1]);
        }
        std::cout << "\nNacisnij enter, aby wrocic." << std::endl;
        std::cin.get(); std::cin.get();
        infrastructure_controls();
    }

    void Game::display_builded()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\tIstniejaca infrastruktura\n" << std::endl;

        if(buildings.size() == 0)
            std::cout << "Zadna infrastruktura jeszcze nie powstala." << std::endl;
        else
        {
            for(auto vector_part : buildings)
            {
                vector_part.building_details();
                std::cout << "\n\n";
            }
        }

        std::cout << "Nacisnij enter, aby wrocic." << std::endl;
        std::cin.get(); std::cin.get();
        infrastructure_controls();
    }

    void Game::upgrade_building()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\tUsprawnianie infrastruktury\n" << std::endl;

        if(buildings.size() == 0)
            std::cout << "Zadna infrastruktura jeszcze nie powstala." << std::endl;
        else
        {
            int i=1;
            for(auto vector_part : buildings)
            {
                std::cout << i << "." << std::endl;
                vector_part.building_details();
                std::cout << "\n\n";
                i++;
            }

            std::cout << "Gdy wpiszesz numer budowli zostanie podjeta proba ulepszenia jej, \nw przeciwnym razie wrocisz do poprzedniego menu.\n" << std::endl;
            char ch = INDEPENDENT::get_validated_char();
            size_t choice = (size_t) ch - '0';
            if(choice > 0 && choice <= buildings.size())
                buildings[choice-1].upgrade(base);
        }
        std::cout << "Nacisnij enter, aby wrocic." << std::endl;
        std::cin.get(); std::cin.get();
        infrastructure_controls();
    }

    void Game::build_building(BUILDINGS_VEHICLE::Buildings & b)
    {
        if(b.build(base))
            buildings.push_back(b);
    }

    void Game::workers_controls(BUILDINGS_VEHICLE::Buildings & b)
    {
        show_not_working_humans_names();
        std::cout << "\n\n\t1.Dodaj pracownika" << std::endl
                  << "\t2.Zwolnij pracownika" << std::endl
                  << "\t3.Powrot" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();
            std::string name;
            if(choice == '1' || choice == '2')
            {
                std::cout << "\n\nPodaj imie i nazwisko czlowieka: ";
                std::cin.get();
                getline(std::cin, name);
            }
            switch(choice)
            {
            case '1':
            {
                bool human_exist = false;
                for(auto & list_part : humans)
                {
                    if(list_part.get_name() == name)
                    {
                        b.add_employee(list_part);
                        human_exist = true;
                        break;
                    }
                }
                if(!human_exist)
                    std::cout << "Nie ma takiego czlowieka." << std::endl;
                std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                std::cin.get();
                buildings_management();
                validate = false; break;
            }
            case '2':
            {
                b.discard_employee(name);
                std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                std::cin.get();
                buildings_management();
                validate = false; break;
            }
            case '3': std::cout << std::string(300, '\n'); buildings_management(); validate = false; break;
            default: std::cout << "Podaj liczbe od 1 do 3." << std::endl; break;
            }
        }
    }

    void Game::infrastructure_controls()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\tInfrastruktura w bazie\n" << std::endl
                  << "\t1.Istniejaca infrastruktura" << std::endl
                  << "\t2.Infrastruktura do zbudowania" << std::endl
                  << "\t3.Usprawnianie infrastruktury" << std::endl
                  << "\t4.Powrot" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case '1':
            {
                display_builded();
                validate = false; break;
            }
            case '2':
            {
                display_not_builded();
                validate = false; break;
            }
            case '3':
            {
                upgrade_building();
                validate = false; break;
            }
            case '4': std::cout << std::string(300, '\n'); buildings_controls(); validate = false; break;
            default: std::cout << "Podaj liczbe od 1 do 4." << std::endl; break;
            }
        }
    }

    void Game::buildings_management()
    {
        std::cout << "\t\t\tZarzadzanie pracownikami\n" << std::endl;
        if(buildings.size() == 0)
        {
            std::cout << "Brak budynkow, w ktorych mozna zarzadzac ludzmi." << std::endl;
            std::cout << "Nacisnij enter, aby wrocic do poprzedniego menu." << std::endl;
            std::cin.get(); std::cin.get();
            buildings_controls();
        }

        else
        {
            int i=1;
            for(auto vector_part : buildings)
            {
                std::cout << i << ".\n";
                vector_part.building_details();
                i++;
                std::cout << "\n\n";
            }
            std::cout << "Wpisz numer budynku, ktorym chcesz zarzadzac. Jesli chcesz wrocic\ndo poprzedniego menu wpisz znak inny niz numer budynku." << std::endl;
            char ch = INDEPENDENT::get_validated_char();
            size_t choice = (size_t) ch - '0';
            if(choice > 0 && choice <= buildings.size())
                workers_controls(buildings[choice-1]);
            else
                buildings_controls();
        }
    }

    bool Game::check_whether_expeditions_building_exists()
    {
        BUILDINGS_VEHICLE::Buildings expeditions_building(6);
        for(auto vector_part : buildings)
        {
            if(vector_part == expeditions_building && vector_part.get_working_people() > 0)
                return true;
        }
        return false;
    }

    void Game::map_management()
    {
        std::cout << "Wpisz A lub D przegladajac mape, aby przesunac mape w danym kierunku." << std::endl
                  << "Wpisz oznaczenie miejsca z mapy, aby wyswietlic jego opis." << std::endl
                  << "Wpisz oznaczenie pojazdu z mapy, aby zaczac go kontrolowac." << std::endl
                  << "Aby poruszac sie pojazdem uzywaj znakow W, A, S, lub D." << std::endl
                  << "Wroc do poprzedniego menu wpisujac znak nie zwiazany z zadna czynnoscia." << std::endl
                  << "Nacisnij enter, aby przejsc dalej." << std::endl;
        std::cin.get();std::cin.get();
        std::cout << "\t\t\t\t   Mars" << std::endl;
        while(true)
        {
            expeditions.show_map();
            std::cout << "Wpisz jakis znak: ";
            char choice = INDEPENDENT::get_validated_char();
            int choice_int = static_cast<int>(choice) - '0';
            if(choice == 'A' || choice == 'a' || choice == 'D' || choice == 'd')
                expeditions.move_map(choice);
            else if(choice_int > 0 && choice_int <= 9)
            {
                if(expeditions.check_if_vehicle_is_on_map(choice))
                {
                    std::cin.get();
                    if(expeditions.move_vehicle(choice))
                        std::cout << "Ruch sie zakonczyl." << std::endl;
                    else
                        std::cout << "Ten pojazd nie jest w uzyciu, nie posiada kierowcow lub zrobil\njuz ruch w tym tygodniu." << std::endl;
                    std::cout << "Nacisnij enter, aby przejsc dalej." << std::endl;
                    std::cin.get();
                }
                else
                {
                    std::cout << "Pojazd o tym oznaczeniu nie znajduje sie na mapie." << std::endl
                              << "Nacisnij enter, aby przejsc dalej." << std::endl;
                    std::cin.get(); std::cin.get();
                }

            }
            else if(expeditions.check_if_place_exists(choice))
            {
                std::cout << std::endl;
                expeditions.show_info(choice);
                std::cout << "Nacisnij enter, aby przejsc dalej." << std::endl;
                std::cin.get(); std::cin.get();
            }
            else
                break;
        }
        expeditions_controls();
    }

    void Game::display_vehicles() const
    {
        if(expeditions.get_number_of_vehicles() > 0)
            expeditions.show_vehicles();
        else
            std::cout << "Zaden pojazd nie zostal jeszcze zbudowany." << std::endl;
    }

    void Game::build_vehicle()
    {
        std::cout << std::string(300, '\n');
        if(expeditions.get_number_of_vehicles() >= 9)
            std::cout << "Osiagnales maksymalna liczbe pojazdow." << std::endl;
        else
        {
            int number_of_variants = 3;
            BUILDINGS_VEHICLE::Vehicle vehicle_variants[number_of_variants] = {BUILDINGS_VEHICLE::Vehicle(1), BUILDINGS_VEHICLE::Vehicle(2), BUILDINGS_VEHICLE::Vehicle(3)};
            for(int i=0; i < number_of_variants; i++)
            {
                std::cout << i+1 << "." << std::endl;
                vehicle_variants[i].properties();
                std::cout << "\n\n";
            }
            std::cout << "Wpisz numer pojazdu, ktory chcesz zbudowac. Aby wrocic do poprzedniego\nmenu wpisz cokolwiek innego od numeru pojazdu." << std::endl;
            char choice = INDEPENDENT::get_validated_char();
            int choice_int = static_cast<int>(choice) - '0';
            if(choice_int > 0 && choice_int <= number_of_variants)
            {
                if(vehicle_variants[choice_int-1].build(base))
                {
                    char sign_choice;
                    while(true)
                    {
                        std::cout << "\nPodaj znak jaki chcesz nadac pojazdowi(1-9): ";
                        sign_choice = INDEPENDENT::get_validated_char();
                        if(expeditions.add_vehicle(vehicle_variants[choice_int-1], sign_choice))
                        {
                            std::cout << "Pojazd zostal zbudowany.\n" << std::endl;
                            break;
                        }
                        else
                            std::cout << "Ten znak jest juz zajety lub niepoprawny.\n" << std::endl;
                    }
                }
            }
        }

        std::cout << "Nacisnij enter, aby wrocic do poprzednego menu." << std::endl;
        std::cin.get();std::cin.get();
        expeditions_controls();
    }

    void Game::destroy_vehicle()
    {
        std::cout << std::string(300, '\n');
        if(expeditions.get_number_of_vehicles() == 0)
            std::cout << "Zaden pojazd nie zostal jeszcze zbudowany." << std::endl;
        else
        {
            std::cout << "Wpisz znak pojazdu, ktory chcesz zniszczyc\n(pamietaj, ze jesli jest on poza baza to zaloga zginie)." << std::endl;
            char sign = INDEPENDENT::get_validated_char();
            if(expeditions.del_vehicle(sign))
                std::cout << "Pojazd zostal zniszczony." << std::endl;
            else
                std::cout << "Pojazd o takim znaku nie istnieje." << std::endl;
        }
        std::cout << "\nNacisnij enter, aby wrocic do poprzednego menu." << std::endl;
        std::cin.get();std::cin.get();
        expeditions_controls();
    }

    void Game::vehicle_controls()
    {
        std::cout << std::string(300, '\n');
        expeditions.show_vehicles();
        std::cout << "\n\n\t1.Wyslij pojazd na misje" << std::endl
                  << "\t2.Napraw pojazd" << std::endl
                  << "\t3.Zarzadzaj zaloga" << std::endl
                  << "\t4.Przetransportuj zasoby do pojazdu" << std::endl
                  << "\t5.Przetransportuj zasoby do bazy" << std::endl
                  << "\t6.Powrot" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();
            switch(choice)
            {
            case '1':
            {
                start_expedition_controls();
                validate = false; break;
            }
            case '2':
            {
                repair_vehicle_controls();
                validate = false; break;
            }
            case '3':
            {
                crew_controls();
                validate = false; break;
            }
            case '4':
            {
                transport_to_vehicle_controls();
                validate = false; break;
            }
            case '5':
            {
                transport_to_base_controls();
                validate = false; break;
            }
            case '6': std::cout << std::string(300, '\n'); expeditions_controls(); validate = false; break;
            default: std::cout << "Podaj liczbe od 1 do 6." << std::endl; break;
            }
        }
    }

    void Game::start_expedition_controls()
    {
        std::cout << std::string(300, '\n');
        std::cout << "Podaj znak pojazdu, ktory ma rozpoczac misje.\nJesli wpiszesz cokolwiek innego to powrocisz do poprzedniego menu." << std::endl;
        char sign = INDEPENDENT::get_validated_char();
        if(expeditions.check_if_vehicle_exists(sign))
        {
            if(expeditions.start_expedition(sign))
                std::cout << "\nPojazd opuscil baze." << std::endl;
            else
                std::cout << "\nNie udalo sie wyslac tego pojazdu na misje. Przyczyna moze byc to," << std::endl
                          << "ze nie nadaje sie do uzytku, nie ma nikogo" << std::endl
                          << "na pokladzie, badz juz zostal wyslany na misje." << std::endl;
        }
        else
            std::cout << "Pojazd prawdopodobnie nie istnieje." << std::endl;
        std::cout << "\nNacisnij enter, aby wrocic do poprzednego menu." << std::endl;
        std::cin.get();std::cin.get();
        vehicle_controls();
    }

    void Game::repair_vehicle_controls()
    {
        std::cout << std::string(300, '\n');
        std::cout << "Podaj znak pojazdu, ktory chcesz naprawic(pamietaj ze do naprawy\nuzywane sa surowce z pojazdu).Jesli wpiszesz cokolwiek innego to\npowrocisz do poprzedniego menu." <<std::endl;
        char sign = INDEPENDENT::get_validated_char();
        if(expeditions.check_if_vehicle_exists(sign))
        {
            expeditions.repair_vehicle(sign);
        }
        else
            std::cout << "Pojazd prawdopodobnie nie istnieje." << std::endl;
        std::cout << "\nNacisnij enter, aby wrocic do poprzednego menu." << std::endl;
        std::cin.get();std::cin.get();
        vehicle_controls();
    }

    void Game::crew_controls()
    {
        std::cout << std::string(300, '\n');
        expeditions.show_vehicles();
        std::cout << "Wpisz znak pojazdu, w ktorym chcesz zarzadzac zaloga.\nJesli wpiszesz nieprawidlowy znak, powrocisz do poprzedniego menu." << std::endl;
        char sign = INDEPENDENT::get_validated_char();
        if(!expeditions.check_if_vehicle_exists(sign))
            vehicle_controls();
        else
        {
            show_not_working_humans_names();
            std::cout << "\n\n\t1.Przydziel zaloganta do pojazdu" << std::endl
                      << "\t2.Oddeleguj zaloganta do bazy" << std::endl
                      << "\t3.Powrot" << std::endl;
            bool validate = true;
            while(validate)
            {
                char choice = INDEPENDENT::get_validated_char();
                std::string name;
                if(choice == '1' || choice == '2')
                {
                    std::cout << "\n\nPodaj imie i nazwisko czlowieka: ";
                    std::cin.get();
                    getline(std::cin, name);
                }
                switch(choice)
                {
                case '1':
                {
                    bool human_exist = false;
                    for(auto & list_part : humans)
                    {
                        if(list_part.get_name() == name)
                        {
                            expeditions.add_human_to_vehicle(list_part, sign);
                            human_exist = true;
                            break;
                        }
                    }
                    if(!human_exist)
                        std::cout << "Nie ma takiego czlowieka." << std::endl;
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    crew_controls();
                    validate = false; break;
                }
                case '2':
                {
                    expeditions.discard_human_from_vehicle(name, sign);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    crew_controls();
                    validate = false; break;
                }
                case '3': std::cout << std::string(300, '\n'); vehicle_controls(); validate = false; break;
                default: std::cout << "Podaj liczbe od 1 do 3." << std::endl; break;
                }
            }
        }
    }

    void Game::transport_to_vehicle_controls()
    {
        std::cout << std::string(300, '\n');
        expeditions.show_vehicles();
        std::cout << "\n\nWpisz znak pojazdu, w ktorym chcesz zarzadzac zasobami.\nJesli wpiszesz nieprawidlowy znak, powrocisz do poprzedniego menu." << std::endl;
        char sign = INDEPENDENT::get_validated_char();
        if(!expeditions.check_if_vehicle_exists(sign))
            vehicle_controls();
        else
        {
            std::cout << "\n\n\t1.Przenies tlen do pojazdu" << std::endl
                      << "\t2.Przenies jedzenie do pojazdu" << std::endl
                      << "\t3.Przenies wode do pojazdu" << std::endl
                      << "\t4.Przenies materialy budowlane do pojazdu" << std::endl
                      << "\t5.Powrot" << std::endl;
            bool validate = true;
            while(validate)
            {
                char choice = INDEPENDENT::get_validated_char();
                double amount;
                if(choice == '1' || choice == '2' || choice == '3' || choice == '4')
                {
                    std::cout << "Podaj ilosc zasobu, ktora chcesz przetransportowac do pojazdu." << std::endl;
                    std::cin.get();
                    amount = INDEPENDENT::get_validated_number();
                }
                switch(choice)
                {
                case '1':
                {
                    expeditions.oxygen_to_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_vehicle_controls();
                    validate = false; break;
                }
                case '2':
                {
                    expeditions.food_to_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_vehicle_controls();
                    validate = false; break;
                }
                case '3':
                {
                    expeditions.water_to_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_vehicle_controls();
                    validate = false; break;
                }
                case '4':
                {
                    expeditions.building_materials_to_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_vehicle_controls();
                    validate = false; break;
                }
                case '5': std::cout << std::string(300, '\n'); vehicle_controls(); validate = false; break;
                default: std::cout << "Podaj liczbe od 1 do 5." << std::endl; break;
                }
            }
        }
    }

    void Game::transport_to_base_controls()
    {
        std::cout << std::string(300, '\n');
        expeditions.show_vehicles();
        std::cout << "\n\nWpisz znak pojazdu, w ktorym chcesz zarzadzac zasobami.\nJesli wpiszesz nieprawidlowy znak, powrocisz do poprzedniego menu." << std::endl;
        char sign = INDEPENDENT::get_validated_char();
        if(!expeditions.check_if_vehicle_exists(sign))
            vehicle_controls();
        else
        {
            std::cout << "\n\n\t1.Przenies tlen do bazy" << std::endl
                      << "\t2.Przenies jedzenie do bazy" << std::endl
                      << "\t3.Przenies wode do bazy" << std::endl
                      << "\t4.Przenies materialy budowlane do bazy" << std::endl
                      << "\t5.Powrot" << std::endl;
            bool validate = true;
            while(validate)
            {
                char choice = INDEPENDENT::get_validated_char();
                double amount;
                if(choice == '1' || choice == '2' || choice == '3' || choice == '4')
                {
                    std::cout << "Podaj ilosc zasobu, ktora chcesz przetransportowac do pojazdu." << std::endl;
                    std::cin.get();
                    amount = INDEPENDENT::get_validated_number();
                }
                switch(choice)
                {
                case '1':
                {
                    expeditions.oxygen_from_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_base_controls();
                    validate = false; break;
                }
                case '2':
                {
                    expeditions.food_from_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_base_controls();
                    validate = false; break;
                }
                case '3':
                {
                    expeditions.water_from_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_base_controls();
                    validate = false; break;
                }
                case '4':
                {
                    expeditions.building_materials_from_vehicle(sign, base, amount);
                    std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                    std::cin.get();
                    transport_to_base_controls();
                    validate = false; break;
                }
                case '5': std::cout << std::string(300, '\n'); vehicle_controls(); validate = false; break;
                default: std::cout << "Podaj liczbe od 1 do 5." << std::endl; break;
                }
            }
        }
    }

    bool Game::check_for_win()
    {
        bool validate = false;
        for(auto vector_part : buildings)
        {
            if(vector_part.get_description() == "Dwanascie pierscieni nadprzewodnikowych budowanych i zabezpieczanych przez flote autonomicznych robotow")
            {
                validate = true;
                break;
            }
        }

        if(humans.size() == 74 && validate)
            return true;
        else
            return false;
    }

    bool Game::check_for_lose()
    {
        if(humans.size() == 0)
            return true;
        else
            return false;
    }

    void Game::gifts()
    {
        if(week == 1)
        {
            base.add_building_materials(100);
            base.add_food(30.0);
            base.add_oxygen(20.0);
            base.add_water(20.0);
            RESOURCES_BASE_HUMAN::Human hum1(base), hum2(base);
            humans.push_front(hum1);
            humans.push_front(hum2);
        }
        if(week == 3)
        {
            std::cout << "Na Marsie udalo sie wyladowac naszemu drugiemu statkowi kosmicznemu.\nStatek przywiozl troche surowcow";
            if(base.get_max_humans() - humans.size() >= 2)
            {
                std::cout << " i dwojke ludzi." << std::endl;
                RESOURCES_BASE_HUMAN::Human hum1(base), hum2(base);
                humans.push_front(hum1);
                humans.push_front(hum2);
            }
            else
                std::cout << "." << std::endl;
            base.add_building_materials(200);
            base.add_food(30.0);
            base.add_oxygen(30.0);
            base.add_water(30.0);
        }
        if(week == 12)
        {
            int number = rand() % 2;
            if(number == 1)
            {
                std::cout << "Na Marsie udalo sie wyladowac naszemu trzeciemu statkowi kosmicznemu.\nStatek przywiozl troche surowcow";
                int places_left = base.get_max_humans() - humans.size();
                if(places_left > 0)
                {
                    std::cout << " i " << places_left << " ludzi." << std::endl;
                    for(int i=0; i<places_left; i++)
                    {
                        RESOURCES_BASE_HUMAN::Human hum(base);
                        humans.push_front(hum);
                    }
                }
                else
                    std::cout << "." << std::endl;
                base.add_building_materials(300);
                base.add_food(15.0);
                base.add_oxygen(15.0);
                base.add_water(15.0);
            }
            else
            {
                std::cout << "Nasz 3 statek kosmiczny niestety rozbil sie niedaleko bazy. Jedyne co udalo\nnam sie zrobic to odzyskac niewielka ilosc surowcow." << std::endl;
                base.add_building_materials(50);
            }
        }
    }

    void Game::born_human()
    {
        if(base.get_base_level() >= 3 && static_cast<size_t>(base.get_max_humans()) > humans.size())
        {
            int random = rand() % 11 - base.get_base_level();
            if(random == 2)
            {
                RESOURCES_BASE_HUMAN::Human hum(base);
                humans.push_front(hum);
                std::cout << "Urodzil sie nowy czlowiek!" << std::endl;
            }
        }
    }

    void Game::check_humans()
    {
        for(auto & vector_part : buildings)
            vector_part.check_employees();

        bool deleted = true;
        while(deleted != false)
        {
            if(humans.size() == 0)
                break;

            std::list<RESOURCES_BASE_HUMAN::Human>::iterator it;
            for (it = humans.begin(); it != humans.end(); it++)
            {
                if((*it).if_died(base))
                {
                    humans.erase(it);
                    died_humans++;
                    deleted = true;
                    break;
                }
                deleted = false;
            }
        }
    }

    void Game::humans_vital_signs()
    {
        expeditions.vital_signs();
        for(auto & list_part : humans)
        {
            if(list_part.get_in_base())
            {
                list_part.breathe(base);
                list_part.eat(base);
                list_part.drink(base);
            }
        }
    }

    void Game::humans_health_calculation()
    {
        for(auto & list_part : humans)
        {
            std::string statement;
            statement = list_part.calculate_healthness(base);
            if(statement != "\0")
                humans_statements.push_back(statement);
        }
    }

    void Game::events()
    {
        std::cout << std::endl;
        if(week % 30 == 0)
            std::cout << "Rozpoczela sie burza pylowa. Niedaleko bazy mozna zobaczyc wiry pylowe." << std::endl;
        if(week % 100 == 0)
            std::cout << "Pojawil sie rozblysk sloneczny!" << std::endl;
        if(week == 14)
            std::cout << "Podobno niedlugo ma byc testowany nowy naped EmDrive.\nMa on przywiezc nam surowce w najblizszym czasie." << std::endl;
        if(week == 19)
            std::cout << "Sytuacja na Ziemi nie jest stabilna. Rosnie napiecie\npomiedzy najwiekszymi mocarstwami." << std::endl;
        if(week == 24)
            std::cout << "Podobno na Ziemi rozpoczela sie wojna! To dla nas tragiczna\nwiadomosc. Na pewno nie zdazyli jeszcze wyslac EmSpaceship z\nsurowcami. Wyglada na to"
                      << " ze bedziemy musieli sobie radzic sami." << std::endl;
        if(week == 35)
            std::cout << "Na Ziemi sytuacja jest goraca. Wojna rozprzestrzenila sie\njuz na kilkanascie panstw. To jest trzecia wojna swiatowa." << std::endl;
        if(week == 46)
            std::cout << "Na Ziemi wojna zbiera swoje zniwa. Zginela juz masa ludzi.\nNarazie nie widac przewagi zadnej ze stron." << std::endl;
        if(week == 57)
            std::cout << "Jedno z mocarstw zyskalo znaczaca przewage!\nByc moze wojna zbliza sie ku koncowi." << std::endl;
        if(week == 64)
            std::cout << "Wyglada na to ze wojna zakonczy sie w ciagu kilku najblizszych\ntygodni. Cala Ziemia pograzona jest w chaosie. Nie mozemy w najblizszym\nczasie liczyc na jakakolwiek pomoc"
                      << " nawet po zakonczeniu wojny." << std::endl;
        if(week == 72)
            std::cout << "Mimo, ze sprzyjajace nam mocarstwo praktycznie zwyciezylo\nto doszli tam do wladzy ludzie nam nie przychylni." << std::endl;
        if(week == 80)
            std::cout << "Przegrywajace mocarstwo uzylo broni atomowej!\nTo moze calkowicie zmienic Ziemie." << std::endl;
        if(week == 85)
            std::cout << "Ziemia jest kompletnie zniszczona! Trwa tam wojna nuklearna!\nWszystkie najwieksze osrodki zostaly zniszczone." << std::endl;
        if(week == 92)
            std::cout << "Wojna sie zakonczyla. Na Ziemi zapanowal nowy porzadek swiata. Dostalismy informacje,\nze ostatni wladcy przegranej strony uciekli z Ziemi i\nprawdopodobnie beda chcieli"
                      << "sie do nas dostac." << std::endl;
        if(week == 97)
            std::cout << "Dostalismy informacje z Ziemi, ze niedlugo rozpocznie sie potezna\nburza slonecza. Uciekinierzy z Ziemi najprawdopodobniej zgina.\nZabije ich promieniowanie."
                      << "Nie musimy sie juz nimi przejmowac." << std::endl;
        if(week == 105)
            std::cout << "Ludzie zaczeli juz odbudowe Ziemi. Zajmie im to na\npewno bardzo dlugo, poniewaz jest calkowicie zniszczona.\nJednak juz pojawiaja sie plany lotow na Marsa." << std::endl;
    }

    Game::Game()
    {
        base = RESOURCES_BASE_HUMAN::Base();
        expeditions = ADDITIONAL::Expedition();
        ADDITIONAL::Expedition::set_map();
        nickname = "None";
        week = 1;
        died_humans = 0;
    }

    bool Game::main_menu()
    {
        std::cout << std::string(300, '\n');
        std::cout << std::setw(44) << std::right << "PROJECT M.A.R.S.\n\n" << std::endl
              << "\t1. Nowa gra" << std::endl
              << "\t2. Wczytaj gre" << std::endl
              << "\t3. Najlepszy wynik" << std::endl
              << "\t4. Wyjdz z gry\n\n\n\n\n\n\n\n\n\n" << std::endl;
        bool validate = true;

        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case '1': new_game(); validate = false; break;
            case '2':
            {
                std::cout << std::string(300, '\n');
                load_game();
                validate = false; break;
            }
            case '3':
            {
                display_scores();
                std::cout << "Nacisnij enter, aby powrocic do glownego menu." << std::endl;
                std::cin.get(); std::cin.get();
                main_menu();
                validate = false; break;
            }
            case '4': return true;
            default: std::cout << "Podaj liczbe od 1 do 4." << std::endl; break;
            }
        }
        return false;
    }

    void Game::new_game()
    {
        std::cout << "Czy na pewno chcesz rozpoczac nowa gre? Jesli gra byla wczesniej \nzapisana, spowoduje to usuniecie zapisu gry." << std::endl
                  << "T - tak" << std::endl
                  << "N - nie" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case 'T':
            case 't':
            {
                std::cout << std::string(300, '\n');
                prepare_game();
                gifts();
                introduction();
                game_menu();
                validate = false; break;
            }
            case 'N':
            case 'n': main_menu(); validate = false; break;
            default: std::cout << "Wpisz T lub N." << std::endl; break;
            }
        }
    }

    void Game::game_over()
    {
        if(check_for_win())
        {
            std::cout << "Udalo sie rozwinac samowystarczalna kolonie." << std::endl;
            results_to_file();
        }
        else
            std::cout << "Wszyscy umarli. Misja nie powiodla sie." << std::endl;
        show_result();
        prepare_game();
        reset_game();
    }

    void Game::save_game(std::string directory)
    {
        std::ofstream fout;
        fout.open(directory, std::ios_base::out | std::ios_base::binary);
        if(!fout.is_open())
            std::cout << "Blad, nie udalo sie otworzyc pliku zapisu informacji na temat gry." << std::endl;
        else
        {
            int string_length = nickname.size();
            int number_of_buildings = buildings.size();
            fout.write(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            fout.write(reinterpret_cast<char*>(&nickname[0]), string_length);
            fout.write(reinterpret_cast<char*>(&week), sizeof(week));
            fout.write(reinterpret_cast<char*>(&died_humans), sizeof(died_humans));
            fout.write(reinterpret_cast<char*>(&number_of_buildings), sizeof(number_of_buildings));

            base.save_base();

            std::ofstream fout2;
            fout2.open("saves\\humans_save.txt", std::ios_base::out | std::ios_base::binary);           //clear humans save
            fout2.close();
            for(auto list_part : humans)
                list_part.save_human();

            int i=0;
            for(auto vector_part : buildings)
            {
                vector_part.save_building(i);
                i++;
            }
            BUILDINGS_VEHICLE::Buildings::save_builded_status();
            expeditions.save_expedition();
        }
        fout.close();
    }

    void Game::load_game(std::string directory)
    {
        std::ifstream fin;
        fin.open(directory, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
        {
            std::cout << "Nie istnieje zaden zapis gry. Nacisnij enter, aby kontynuowac." << std::endl;
            std::cin.get(); std::cin.get();
            main_menu();
        }

        else
        {
            int string_length;
            int number_of_buildings;

            fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            nickname.resize(string_length);
            fin.read(reinterpret_cast<char*>(&nickname[0]), string_length);
            fin.read(reinterpret_cast<char*>(&week), sizeof(week));
            fin.read(reinterpret_cast<char*>(&died_humans), sizeof(died_humans));
            fin.read(reinterpret_cast<char*>(&number_of_buildings), sizeof(number_of_buildings));

            base.load_base();
            for(int i=0; i<base.get_humans(); i++)
            {
                RESOURCES_BASE_HUMAN::Human load;
                load.load_human(i);
                humans.push_front(load);
            }

            for(int i=0; i<number_of_buildings; i++)
            {
                BUILDINGS_VEHICLE::Buildings load;
                load.load_building(i, humans);
                buildings.push_back(load);
            }
            BUILDINGS_VEHICLE::Buildings::load_builded_status();
            expeditions.load_expedition(humans);
            game_menu();
        }
        fin.close();
    }

    void Game::game_menu()
    {
        std::cout << "\t1.Centrum Dowodzenia\t\t2.Baza" << std::endl
                  << "\t3.Ludzie\t\t\t4.Zarzadzanie infrastruktura" << std::endl
                  << "\t5.Ekspedycje badawcze\t\t6.Nastepny tydzien" << std::endl
                  << "\t7.Zapisz gre\t\t\t8.Menu glowne" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case '1': command_center_controls(); validate = false; break;
            case '2': base_controls(); validate = false; break;
            case '3': humans_controls(); validate = false; break;
            case '4': buildings_controls(); validate = false; break;
            case '5': expeditions_controls(); validate = false; break;
            case '6':
            {
                if(skip_week())
                {
                    game_over();
                    std::cout << "Nacisnij enter, aby powrocic do glownego menu." << std::endl;
                    std::cin.get(); std::cin.get();
                    main_menu();
                }
                else
                    game_menu();
                validate = false; break;
            }
            case '7':
            {
                save_game();
                std::cout << std::string(300, '\n');
                game_menu();
                validate = false; break;
            }
            case '8':
            {
                std::cout << "Czy na pewno chcesz wysjc do glownego menu?" << std::endl
                          << "T - tak" << std::endl
                          << "N - nie" << std::endl;
                bool validate_confirm = true;
                while(validate_confirm)
                {
                    char confirm = INDEPENDENT::get_validated_char();

                    switch(confirm)
                    {
                    case 'T':
                    case 't':
                    {
                        reset_game();
                        main_menu();
                        validate_confirm = false; break;
                    }
                    case 'N':
                    case 'n':
                    {
                        std::cout << std::string(300, '\n');
                        game_menu();
                        validate_confirm = false; break;
                    }
                    default: std::cout << "Wpisz T lub N." << std::endl; break;
                    }
                }
                validate = false; break;
            }
            default: std::cout << "Podaj liczbe od 1 do 8." << std::endl; break;
            }
        }
    }

    void Game::display_scores(std::string directory)
    {
        std::cout << std::string(300, '\n');
        std::ifstream fin(directory, std::ios_base::in | std::ios_base::binary);
        if(!fin.is_open())
            std::cout << "Nikt jeszcze nie zdolal ukonczyc gry." << std::endl;
        else
        {
            int string_length, read_week, read_died;
            std::string read_nickname;
            fin.read(reinterpret_cast<char*>(&string_length), sizeof(string_length));
            read_nickname.resize(string_length);
            fin.read(reinterpret_cast<char*>(&read_nickname[0]), string_length);
            fin.read(reinterpret_cast<char*>(&read_week), sizeof(read_week));
            fin.read(reinterpret_cast<char*>(&read_died), sizeof(read_died));
            std::cout << "Najlepszy wynik:\n" << read_nickname << " przetrwal " << read_week << " tygodni ze skutkiem " << read_died << " martwych osob.\n" << std::endl;
            fin.close();
        }
    }

    void Game::base_controls()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\t\tBaza\n" << std::endl
                  << "\t1.Informacje na temat bazy" << std::endl
                  << "\t2.Rozbuduj baze" << std::endl
                  << "\t3.Powrot" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case '1':
            {
                std::cout << std::string(300, '\n');
                base.base_statistics();
                std::cout << "Nacisnij enter, aby powrocic do menu bazy." << std::endl;
                std::cin.get(); std::cin.get();
                base_controls();
                validate = false; break;
            }
            case '2':
            {
                std::cout << std::string(300, '\n');
                if((base.get_base_level() == 0 || base.get_base_level() == 7 || base.get_base_level() == 8) || BUILDINGS_VEHICLE::Buildings::get_builded_status())
                {
                    if(base.expand_base())
                    {
                        if(base.get_base_level() != 1)
                            BUILDINGS_VEHICLE::Buildings::change_already_builded();
                    }

                }
                else
                    std::cout << "Najpierw musisz zbudowac jakis budynek." << std::endl;
                std::cout << "Nacisnij enter, aby powrocic do menu bazy." << std::endl;
                std::cin.get(); std::cin.get();
                base_controls();
                validate = false; break;
            }
            case '3': std::cout << std::string(300, '\n'); game_menu(); validate = false; break;
            default: std::cout << "Podaj liczbe od 1 do 3." << std::endl; break;
            }
        }
    }
    void Game::command_center_controls()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\tCentrum dowodzenia\n" << std::endl
                  << "\t\t\t    Tydzien " << week << std::endl << std::endl
                  << "\t1.Informacje" << std::endl
                  << "\t2.Podpowiedzi" << std::endl
                  << "\t3.Powrot" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case '1':
            {
                std::cout << std::string(300, '\n');
                std::cout << "Komandorze " << nickname << ", naszym celem jest zbudowanie\ninfrastruktury umozliwiajacej zbudowanie kolonii, ktora pozwoli na\nprzeksztalcenie"
                          << "Marsa w druga Ziemie i osiedlenie sie tutaj na stale.\nPodczas tej misji musimy osiagnac maksymalna liczbe ludzi,\nktora zmiesci sie w bazie"
                          << "i doprowadzic do komfortowej\nrozbudowy kolejnych baz." << std::endl;
                std::cout << "\n\t\t\tLiczba zmarlych osob: " << died_humans << std::endl << std::endl;
                if(died_humans < 15)
                    std::cout << "Komandorze " << nickname << ", poki co idzie nam calkiem niezle!" << std::endl;
                else if(died_humans >= 15 && died_humans < 30)
                    std::cout << "Komandorze " << nickname << ", zbyt duzo naszych ludzi umiera.\nLudzie na Ziemi zaczynaja sie niepokoic." << std::endl;
                else
                    std::cout << "Komandorze " << nickname << "Jak tak dalej pojdzie wszyscy umrzemy!" << std::endl;
                std::cout << "\nNacisnij enter, aby wrocic do poprzedniego menu." << std::endl;
                std::cin.get(); std::cin.get();
                command_center_controls();
                validate = false; break;
            }
            case '2':
            {
                std::cout << std::string(300, '\n');
                show_tips();
                std::cout << "\nNacisnij enter, aby wrocic do poprzedniego menu." << std::endl;
                std::cin.get(); std::cin.get();
                command_center_controls();
                validate = false; break;
            }
            case '3': std::cout << std::string(300, '\n'); game_menu(); validate = false; break;
            default: std::cout << "Podaj liczbe od 1 do 3." << std::endl; break;
            }
        }
    }

    void Game::humans_controls()
    {
        std::cout << std::string(300, '\n');
        std::cout << "\t\t\tLudzie\n" << std::endl
                  << "\t1.Lista osob" << std::endl
                  << "\t2.Komunikaty" << std::endl
                  << "\t3.Powrot" << std::endl;

        bool validate = true;
        while(validate)
        {
            char choice = INDEPENDENT::get_validated_char();

            switch(choice)
            {
            case '1':
            {
                if(humans.size() == 0)
                    std::cout << "Brak ludzi w bazie!" << std::endl;
                else
                    show_humans();
                std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                std::cin.get(); std::cin.get();
                humans_controls();
                validate = false; break;
            }
            case '2':
            {
                std::cout << std::string(300, '\n');
                std::cout << "\t\tKomunikaty z biezacego tygodnia\n" << std::endl;
                for(auto vector_part : humans_statements)
                    std::cout << vector_part << std::endl << std::endl;
                if(humans_statements.size() == 0)
                    std::cout << "Brak komunikatow." << std::endl;
                std::cout << "Nacisnij enter, aby wrocic." << std::endl;
                std::cin.get(); std::cin.get();
                humans_controls();
                validate = false; break;
            }
            case '3': std::cout << std::string(300, '\n'); game_menu(); validate = false; break;
            default: std::cout << "Podaj liczbe od 1 do 3." << std::endl; break;
            }
        }
    }

    void Game::buildings_controls()
    {
        if(base.get_base_level() == 0)
        {
            std::cout << "Najpierw musisz rozwinac baze. Nacisnij enter, aby wrocic." << std::endl;
            std::cin.get(); std::cin.get();
            std::cout << std::string(300, '\n');
            game_menu();
        }
        else
        {
            std::cout << std::string(300, '\n');
            std::cout << "\t\t\tZarzadzanie infrastruktura\n" << std::endl
                      << "\t1.Infrastruktura w bazie" << std::endl
                      << "\t2.Zarzadzanie pracownikami" << std::endl
                      << "\t3.Powrot" << std::endl;

            bool validate = true;
            while(validate)
            {
                char choice = INDEPENDENT::get_validated_char();

                switch(choice)
                {
                case '1':
                {
                    std::cout << std::string(300, '\n');
                    infrastructure_controls();
                    validate = false; break;
                }
                case '2':
                {
                    std::cout << std::string(300, '\n');
                    buildings_management();
                    validate = false; break;
                }
                case '3': std::cout << std::string(300, '\n'); game_menu(); validate = false; break;
                default: std::cout << "Podaj liczbe od 1 do 3." << std::endl; break;
                }
            }
        }
    }

    void Game::expeditions_controls()
    {
        std::cout << std::string(300, '\n');
        if(!check_whether_expeditions_building_exists())
        {
            std::cout << "Musisz zbudowac budynek z pojazdami i miec w nim pracownikow!\nAby powrocic do poprzedniego menu nacisnij enter." << std::endl;
            std::cin.get(); std::cin.get();
            std::cout << std::string(300, '\n');
            game_menu();
        }
        else
        {
            std::cout << "\t\t\tEkspedycje\n" << std::endl
                      << "\t1.Mapa Marsa" << std::endl
                      << "\t2.Pojazdy" << std::endl
                      << "\t3.Zbuduj pojazd" << std::endl
                      << "\t4.Zniszcz pojazd" << std::endl
                      << "\t5.Zarzadzanie pojazdami" << std::endl
                      << "\t6.Powrot" << std::endl;

            bool validate = true;
            while(validate)
            {
                char choice = INDEPENDENT::get_validated_char();

                switch(choice)
                {
                case '1':
                {
                    std::cout << std::string(300, '\n');
                    map_management();
                    validate = false; break;
                }
                case '2':
                {
                    std::cout << std::string(300, '\n');
                    display_vehicles();
                    std::cout << "Aby powrocic do poprzedniego menu nacisnij enter." << std::endl;
                    std::cin.get(); std::cin.get();
                    expeditions_controls();
                    validate = false; break;
                }
                case '3':
                {
                    build_vehicle();
                    validate = false; break;
                }
                case '4':
                {
                    destroy_vehicle();
                    validate = false; break;
                }
                case '5':
                {
                    vehicle_controls();
                    validate = false; break;
                }
                case '6': std::cout << std::string(300, '\n'); game_menu(); validate = false; break;
                default: std::cout << "Podaj liczbe od 1 do 6." << std::endl; break;
                }
            }
        }
    }

    bool Game::skip_week()
    {
        std::cout << std::string(300, '\n');
        week++;
        if(week % 3 == 0)
            humans_statements.clear();                      //clear statements once per 3 weeks

        humans_health_calculation();

        for(auto & vector_part : buildings)
            vector_part.add_production_to_base(base);       //add all produced resources

        humans_vital_signs();
        check_humans();                                     //check humans state and if someone died delete him

        if(check_for_win())
        {
            ending_story();
            std::cout << "\nNacisnij enter, aby przejsc dalej." << std::endl;
            std::cin.get(); std::cin.get();
            return true;
        }                                                   //check for lose/win
        else if(check_for_lose())
            return true;

        gifts();                                            //add starting gifts (humans and resources)
        born_human();                                       //chance for new human appear
        events();                                           // info about story&weather events
        save_game();

        std::cout << "\n\t\t\tRozpoczal sie " << week << " tydzien." << std::endl
                  << "\nNacisnij enter, aby przejsc dalej." << std::endl;
        std::cin.get(); std::cin.get();
        std::cout << std::string(300, '\n');
        return false;
    }
}

