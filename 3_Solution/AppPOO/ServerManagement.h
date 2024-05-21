#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "IServerManagement.h"
#include <string>
#include <iostream>
#include "Utilizator.h"
#include "BazaDeDate.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Logger.h"
//#include "md5.h"

/////////////////////////PROTOCOLS////////////////////////////
/*
	^  <-- verificare client in baza de date
	0  <-- clientul vrea sa creeze raport de permisie
	1  <-- clientul vrea sa creeza raport de invoire
	2  <-- clientul vrea sa creeze raport de deplasare
	3  <-- clientul vrea sa creeze raport de laptop
	4  <-- clientul vrea sa creeza raport de dezcazarmare
*/

class ServerManagement : public IServerManagement
{
	int delimitator_aparitions = 1;
    //orele pentru masa
    std::vector<std::string> dimineata = { "6","30","7","30" };
    std::vector<std::string> pranz = { "14","15","15","30" };
    std::vector<std::string> seara = { "20","0","21","15" };
public:
	ServerManagement() = default;

    std::string getCurrentTime() {
        // Get the current system time
        auto now = std::chrono::system_clock::now();

        // Add 24 hours (one day) to the current time
        auto one_day_later = now + std::chrono::hours(24);

        // Convert to time_t to get the time in a human-readable format
        std::time_t later_time = std::chrono::system_clock::to_time_t(one_day_later);

        // Local time object
        std::tm* local_time = std::localtime(&later_time);

        // String stream to format the output
        std::ostringstream oss;
        oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");  // Output formatted date-time

        return oss.str();  // Return the formatted string
    }
    std::string getYear(std::string& current_time)
    {
        std::string str;
        str += current_time[0] + current_time[1] + current_time[2] + current_time[3];
        return str;
    }
    std::string getMonth(std::string& current_time)
    {
        std::string str;
        if (current_time[5] == '0') { str += current_time[6]; }
        else { str += current_time[5] + current_time[6]; }
        return str;
    }
    std::string getDay(std::string& current_time)
    {
        std::string str;
        if (current_time[8] == '0') { str += current_time[9]; }
        else { str += current_time[8] + current_time[9]; }
        return str;
    }
    std::string getHour(std::string& current_time)
    {
        std::string str;
        if (current_time[11] == '0') { str += current_time[12]; }
        else { str += current_time[11] + current_time[12]; }
        return str;
    }
    std::string getMinute(std::string& current_time)
    {
        std::string str;
        if (current_time[14] == '0') { str += current_time[15]; }
        else { str += current_time[14] + current_time[15]; }
        return str;
    }

	std::vector<std::string> getUsernameANDPassword(char* buffer, Utilizator* utilizator, BazaDeDate* b, Logger* logger)
	{
        delimitator_aparitions = 1;
		std::string username_string;
		std::string password_string;
		for (int i = 2; delimitator_aparitions < 3; i++)
		{
			if (buffer[i] == '#') { delimitator_aparitions++; i++; }
			if (delimitator_aparitions == 1) { username_string += buffer[i]; }
			else if (delimitator_aparitions == 2) { password_string += buffer[i]; }
		}
		std::vector<std::string> vector;
		vector.push_back(username_string);
		vector.push_back(password_string);

		utilizator->setUsername(b->convertToWString(vector[0]));
		utilizator->setID(b->getIDfromTable(utilizator->getUsername()));

        logger->log("Clientul a incercat sa se conecteze la server");

		return vector;
	}
    std::string sendClientMessageAboutLogin(const std::wstring& string, Logger* logger)
	{
        if (string == L"0") {
            std::cout << "Utilizator student logat cu succes!" << std::endl;
            logger->log("Utilizator student logat cu succes!");
            return "0";
        }
        else if (string == L"1") {
            std::cout << "Utilizator comandant_pluton logat cu succes!" << std::endl;
            logger->log("Utilizator comandant_pluton logat cu succes!");
            return "1";
        }
        else if (string == L"2")
        {
            std::cout << "Utilizator GSC logat cu succes!" << std::endl;
            logger->log("Utilizator GSC logat cu succes!");
            return "2";
        }
        else if (string == L"3")
        {
            std::cout << "Utilizator comandant_companie logat cu succes!" << std::endl;
            logger->log("Utilizator comandant_companie logat cu succes!");
            return "3";
        }
        else if (string == L"4")
        {
            std::cout << "Utilizator comandant_batalion logat cu succes!" << std::endl;
            logger->log("Utilizator comandant_batalion logat cu succes!");
            return "4";
        }
        else if (string == L"6")
        {
            std::cout << "Utilizator indrumator_grupa logat cu succes!" << std::endl;
            logger->log("Utilizator indrumator_grupa logat cu succes!");
            return "6";
        }
        else if (string == L"7")
        {
            std::cout << "Utilizator admin logat cu succes!" << std::endl;
            logger->log("Utilizator admin logat cu succes!");
            return "7";
        }
        else if (string == L"5") {
            std::cout << "Username sau parola incorecta!" << std::endl;
            logger->log("Username sau parola incorecta!");
            return "5";
        }
	}
    void saveRaportFromClient(char* buffer, BazaDeDate* b, Utilizator* utilizator, Logger* logger)
    {
        switch (buffer[1])
        {
        case '0':
        {
            std::cout << "Clientul a renuntat la raport fara sa trimita mai departe" << std::endl;
            logger->log("Clientul a renuntat la raport fara sa trimita mai departe");
            break;
        }
        case '1':
        {
            std::cout << "Clientul a trimis catre bd raport de permisie" << std::endl;
            logger->log("Clientul a trimis catre bd raport de permisie");
            std::string localitate;
            std::string judet;
            std::string data_plecare;
            std::string ora_plecare;
            std::string data_intoarcere;
            std::string ora_intoarcere;
            std::string motiv;
            int delimitator_aparitions = 1;
            for (int i = 3; delimitator_aparitions < 8; i++)
            {
                if (buffer[i] == '#') { delimitator_aparitions++; i++; }
                if (delimitator_aparitions == 1) { localitate += buffer[i]; }
                else if (delimitator_aparitions == 2) { judet += buffer[i]; }
                else if (delimitator_aparitions == 3) { data_plecare += buffer[i]; }
                else if (delimitator_aparitions == 4) { ora_plecare += buffer[i]; }
                else if (delimitator_aparitions == 5) { data_intoarcere += buffer[i]; }
                else if (delimitator_aparitions == 6) { ora_intoarcere += buffer[i]; }
                else { motiv += buffer[i]; }
            }
            std::cout << "ceva" << std::endl;
            //fac converii to wstring ca nu merge altfel sper
            std::wstring wlocalitate = b->convertToWString(localitate);
            std::cout << localitate << std::endl;
            std::wstring wjudet = b->convertToWString(judet);
            std::cout << judet << std::endl;
            std::wstring wdata_plecare = b->convertToWString(data_plecare);
            std::cout << data_plecare << std::endl;
            std::wstring wora_plecare = b->convertToWString(ora_plecare);
            std::cout << ora_plecare << std::endl;
            std::wstring wdata_intoarcere = b->convertToWString(data_intoarcere);
            std::cout << data_intoarcere << std::endl;
            std::wstring wora_intoarcere = b->convertToWString(ora_intoarcere);
            std::cout << data_intoarcere << std::endl;
            std::cout << motiv << std::endl;
            motiv.pop_back();
            std::wstring wmotiv = b->convertToWString(motiv);
            std::cout << motiv << std::endl;
            b->insertRaportPermisie(
                b->convertWstringToInt(utilizator->getID()),
                utilizator->getUsername(),
                wlocalitate,
                wjudet,
                wdata_plecare,
                wora_plecare,
                wdata_intoarcere,
                wora_intoarcere,
                wmotiv,
                0,
                0,
                0
            );
            std::cout << "ceva" << std::endl;
            //b->updateNrRapPermisie(b->convertToString(utilizator->getUsername())); //update
            break;
        }
        case '2':
        {
            std::cout << "Clientul a trimis catre bd raport de invoire" << std::endl;
            logger->log("Clientul a trimis catre bd raport de invoire");
            std::string data_zilei;
            std::string ora_plecare;
            std::string ora_intoarcere;
            std::string motiv;
            int delimitator_aparitions = 1;
            for (int i = 3; delimitator_aparitions < 5; i++)
            {
                if (buffer[i] == '#') { delimitator_aparitions++; i++; }
                if (delimitator_aparitions == 1) { data_zilei += buffer[i]; }
                else if (delimitator_aparitions == 2) { ora_plecare += buffer[i]; }
                else if (delimitator_aparitions == 3) { ora_intoarcere += buffer[i]; }
                else { motiv += buffer[i]; }
            }
            motiv.pop_back();
            std::wstring wdata_zilei = b->convertToWString(data_zilei);
            std::wstring wora_plecare = b->convertToWString(ora_plecare);
            std::wstring wora_intoarcere = b->convertToWString(ora_intoarcere);
            std::wstring wmotiv = b->convertToWString(motiv);
            b->insertRaportInvoire(
                b->convertWstringToInt(utilizator->getID()),
                utilizator->getUsername(),
                wdata_zilei,
                wora_plecare,
                wora_intoarcere,
                wmotiv,
                0,
                0,
                0
            );
            //b->updateNrRapInvoire(b->convertToString(utilizator->getUsername())); //update
            break;
        }
        case '3':
        {
            std::cout << "Clientul a trimis catre bd raport de deplasare" << std::endl;
            logger->log("Clientul a trimis catre bd raport de deplasare");
            std::string localitate;
            std::string tara;
            std::string data_plecare;
            std::string data_intoarcere;
            std::string adresa;
            std::string nume_proprietar;
            int delimitator_aparitions = 1;
            for (int i = 3; delimitator_aparitions < 7; i++)
            {
                if (buffer[i] == '#') { delimitator_aparitions++; i++; }
                if (delimitator_aparitions == 1) { localitate += buffer[i]; }
                else if (delimitator_aparitions == 2) { tara += buffer[i]; }
                else if (delimitator_aparitions == 3) { data_plecare += buffer[i]; }
                else if (delimitator_aparitions == 4) { data_intoarcere += buffer[i]; }
                else if (delimitator_aparitions == 5) { adresa += buffer[i]; }
                else { nume_proprietar += buffer[i]; }
            }
            nume_proprietar.pop_back();
            std::wstring wlocalitate = b->convertToWString(localitate);
            std::wstring wtara = b->convertToWString(tara);
            std::wstring wdata_plecare = b->convertToWString(data_plecare);
            std::wstring wdata_intoarcere = b->convertToWString(data_intoarcere);
            std::wstring wadresa = b->convertToWString(adresa);
            std::wstring wnume_proprietar = b->convertToWString(nume_proprietar);
            b->insertRaportDeplasare(
                b->convertWstringToInt(utilizator->getID()),
                utilizator->getUsername(),
                wlocalitate,
                wtara,
                wdata_plecare,
                wdata_intoarcere,
                wadresa,
                wnume_proprietar,
                0,
                0,
                0
            );
            //b->updateNrRapDeplasare(b->convertToString(utilizator->getUsername())); //update
            break;
        }
        case '4':
        {
            std::cout << "Clientul a trimis catre bd raport de laptop" << std::endl;
            logger->log("Clientul a trimis catre bd raport de laptop");
            std::string motiv;
            int delimitator_aparitions = 1;
            for (int i = 3; delimitator_aparitions < 2; i++)
            {
                if (buffer[i] == '#') { delimitator_aparitions++; i++; }
                if (delimitator_aparitions == 1) { motiv += buffer[i]; }
            }
            motiv.pop_back();
            std::cout << motiv << std::endl;
            std::wstring wmotiv = b->convertToWString(motiv);
            b->insertRaportLaptop(
                b->convertWstringToInt(utilizator->getID()),
                utilizator->getUsername(),
                wmotiv,
                0,
                0
            );
            //b->updateNrRapLaptop(b->convertToString(utilizator->getUsername())); //update
            break;
        }
        case '5':
        {
            std::cout << "Clientul a trimis catre bd raport de dezcazarmare" << std::endl;
            logger->log("Clientul a trimis catre bd raport de dezcazarmare");
            std::string data_incepere;
            std::string adresa;
            std::string nume_proprietar;
            std::string telefon_proprietar;
            int delimitator_aparitions = 1;
            for (int i = 3; delimitator_aparitions < 5; i++)
            {
                if (buffer[i] == '#') { delimitator_aparitions++; i++; }
                if (delimitator_aparitions == 1) { data_incepere += buffer[i]; }
                else if (delimitator_aparitions == 2) { adresa += buffer[i]; }
                else if (delimitator_aparitions == 3) { nume_proprietar += buffer[i]; }
                else { telefon_proprietar += buffer[i]; }
            }
            telefon_proprietar.pop_back();
            std::wstring wdata_incepere = b->convertToWString(data_incepere);
            std::wstring wadresa = b->convertToWString(adresa);
            std::wstring wnume_proprietar = b->convertToWString(nume_proprietar);
            std::wstring wtelefon_proprietar = b->convertToWString(telefon_proprietar);
            b->insertRaportDezcazarmare(
                b->convertWstringToInt(utilizator->getID()),
                utilizator->getUsername(),
                wdata_incepere,
                wadresa,
                wnume_proprietar,
                wtelefon_proprietar,
                0,
                0
            );
            //b->updateNrRapDezcazarmare(b->convertToString(utilizator->getUsername())); //update
            break;
        }
    }
    }
    std::vector<std::string> getOldPasswordANDNewPassword(char* buffer)
    {
        delimitator_aparitions = 1;
        std::string parola_veche;
        std::string parola_noua;
        for (int i = 2; delimitator_aparitions < 3; i++)
        {
            if (buffer[i] == '#') { delimitator_aparitions++; i++; }
            if (delimitator_aparitions == 1) { parola_veche += buffer[i]; }
            else if (delimitator_aparitions == 2) { parola_noua += buffer[i]; }
        }

        std::vector<std::string> vector;
        vector.push_back(parola_veche);
        vector.push_back(parola_noua);

        return vector;
    }
    std::string sendClientMessageAboutOldPassword(std::wstring& string)
    {
        if (string == L"5") {
            std::cout << "Parola veche introdusa incorecta!" << std::endl;
            return "0";
        }
        else {
            std::cout << "Parola veche corecta!" << std::endl;
            return "1";
        }
    }
    int* isClientAlocatedforFood(Utilizator* utilizator, BazaDeDate* b)
    {
        int protocol[3] = { 0 };
        //for raport_invoire
        std::vector<std::vector<std::wstring>> matrix = b->loadTableIntoMatrix(L"Raport_Invoire");
        std::string current_time = getCurrentTime();
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (utilizator->getID() == matrix[i][j] && matrix[i][j + 6] == L"1" && matrix[i][j + 7] == L"1" && matrix[i][j + 8] == L"1")
                {
                    std::wstring date = matrix[i][j + 2];
                    std::string year;
                    std::string month;
                    std::string day;
                    year += date[0] + date[1] + date[2] + date[3];
                    if (date[5] == '0') { month += date[6]; }
                    else { month += date[5] + date[6]; }
                    if (date[8] == '0') { day += date[9]; }
                    else { day += date[8] + date[9]; }
                    if (std::stoi(year) == std::stoi(getYear(current_time)) &&
                        std::stoi(month) == std::stoi(getMonth(current_time)) &&
                        std::stoi(day) == std::stoi(getDay(current_time)))
                    {
                        std::wstring date = matrix[i][j + 3];
                        std::string hour;
                        std::string hour1;
                        if (date[0] == '0') { hour += date[1]; }
                        else { hour += date[0] + date[1]; }
                        if (std::stoi(getHour(current_time)) >= std::stoi(hour))
                        {
                            date = matrix[i][j + 4];
                            if (date[0] == '0') { hour1 += date[1]; }
                            else { hour1 += date[0] + date[1]; }
                            if (std::stoi(getHour(current_time)) <= std::stoi(hour1))
                            {
                                if (std::stoi(hour) >= std::stoi(dimineata[0]) &&
                                    std::stoi(hour1) <= std::stoi(dimineata[2]))
                                {
                                    protocol[0] = 1;
                                }
                                if (std::stoi(hour) >= std::stoi(pranz[0]) &&
                                    std::stoi(hour1) <= std::stoi(pranz[2]))
                                {
                                    protocol[1] = 1;
                                }
                                if (std::stoi(hour) >= std::stoi(seara[0]) &&
                                    std::stoi(hour1) <= std::stoi(seara[2]))
                                {
                                    protocol[2] = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        //for raport_permisie
        matrix = b->loadTableIntoMatrix(L"Raport_Permisie");
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (utilizator->getID() == matrix[i][j] && matrix[i][j + 9] == L"1" && matrix[i][j + 10] == L"1" && matrix[i][j + 11] == L"1")
                {
                    std::wstring date = matrix[i][j + 4];
                    std::string year;
                    std::string month;
                    std::string day;
                    year += date[0] + date[1] + date[2] + date[3];
                    if (date[5] == '0') { month += date[6]; }
                    else { month += date[5] + date[6]; }
                    if (date[8] == '0') { day += date[9]; }
                    else { day += date[8] + date[9]; }
                    if (std::stoi(year) >= std::stoi(getYear(current_time)) &&
                        std::stoi(month) >= std::stoi(getMonth(current_time)) &&
                        std::stoi(day) >= std::stoi(getDay(current_time)))
                    {
                        date = matrix[i][j + 6];
                        b->convertToString(date);
                        year.clear(); month.clear(); day.clear();
                        year += date[0] + date[1] + date[2] + date[3];
                        if (date[5] == '0') { month += date[6]; }
                        else { month += date[5] + date[6]; }
                        if (date[8] == '0') { day += date[9]; }
                        else { day += date[8] + date[9]; }
                        if (std::stoi(year) <= std::stoi(getYear(current_time)) &&
                            std::stoi(month) <= std::stoi(getMonth(current_time)) &&
                            std::stoi(day) <= std::stoi(getDay(current_time)))
                        {
                            protocol[0] = 1; protocol[1] = 1; protocol[2] = 1;
                        }
                    }
                }
            }
        }
        //for raport dezcazarmare
        matrix = b->loadTableIntoMatrix(L"Raport_dezcazarmare");
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (utilizator->getID() == matrix[i][j] && matrix[i][j + 6] == L"1" && matrix[i][j + 7] == L"1")
                {
                    std::wstring date = matrix[i][j + 2];
                    std::string year;
                    std::string month;
                    std::string day;
                    year += date[0] + date[1] + date[2] + date[3];
                    if (date[5] == '0') { month += date[6]; }
                    else { month += date[5] + date[6]; }
                    if (date[8] == '0') { day += date[9]; }
                    else { day += date[8] + date[9]; }
                    if (std::stoi(year) <= std::stoi(getYear(current_time)) &&
                        std::stoi(month) <= std::stoi(getMonth(current_time)) &&
                        std::stoi(day) <= std::stoi(getDay(current_time)))
                    {
                        protocol[0] = 1; protocol[1] = 1; protocol[2] = 1;
                    }
                }
            }
        }
        return protocol;
    }
    void updateSector_Facut(BazaDeDate* b, char* buffer)
    {
        if (buffer[1] == '0') { b->updateSector_Facutt("exterior", L"0"); }
        if (buffer[2] == '0') { b->updateSector_Facutt("WC", L"0"); }
        if (buffer[3] == '0') { b->updateSector_Facutt("hol", L"0"); }
        if (buffer[4] == '0') { b->updateSector_Facutt("camera", L"0"); }
        if (buffer[1] == '1') { b->updateSector_Facutt("exterior", L"1"); }
        if (buffer[2] == '1') { b->updateSector_Facutt("WC", L"1"); }
        if (buffer[3] == '1') { b->updateSector_Facutt("hol", L"1"); }
        if (buffer[4] == '1') { b->updateSector_Facutt("camera", L"1"); }
    }
    std::string sendClientInfoSectoare1(BazaDeDate* b)
    {
        std::vector<std::vector<std::wstring>> matrix = b->loadTableIntoMatrix(L"Sectoare");
        std::wstring protocol;
        //protocol += b->convertToWString(std::to_string(matrix.size()));
        //protocol += L"#";
        for (int i = 0; i < 39; i++)
        {
            protocol += matrix[i][1] + L"#" + matrix[i][2] + L"#" + matrix[i][3] + L"#";
        }
        return b->convertToString(protocol);
    }
    std::string sendClientInfoSectoare2(BazaDeDate* b)
    {
        std::vector<std::vector<std::wstring>> matrix = b->loadTableIntoMatrix(L"Sectoare");
        std::wstring protocol;
        for (int i = 39; i < 79; i++)
        {
            protocol += matrix[i][1] + L"#" + matrix[i][2] + L"#" + matrix[i][3] + L"#";
        }
        return b->convertToString(protocol);
    }
    std::string sendClientInfoSectoare3(BazaDeDate* b)
    {
        std::vector<std::vector<std::wstring>> matrix = b->loadTableIntoMatrix(L"Sectoare");
        std::wstring protocol;
        for (int i = 79; i < matrix.size(); i++)
        {
            protocol += matrix[i][1] + L"#" + matrix[i][2] + L"#" + matrix[i][3] + L"#";
        }
        return b->convertToString(protocol);
    }
    std::string sendClientRapoarteDeAprobatNeaprobat(BazaDeDate* b, Utilizator* utilizator)
    {
        if (b->getRolFromUsername(utilizator) == 3)
        {
            int nr1 = b->getNrRapoartePermisieCMDCompanie();
            int nr2 = b->getNrRapoarteInvoireCMDCompanie();
            int nr3 = b->getNrRapoarteDeplasareCMDCompanie();
            int nr4 = b->getNrRapoarteLaptopCMDCompanie();
            int nr5 = b->getNrRapoarteDezcazarmareCMDCompanie();
            std::string p1 = b->getRaportPermisieCMDCompanie();
            std::string p2 = b->getRaportInvoireCMDCompanie();
            std::string p3 = b->getRaportDeplasareCMDCompanie();
            std::string p4 = b->getRaportLaptopCMDCompanie();
            std::string p5 = b->getRaportDezcazarmareCMDCompanie();
            std::string protocol;
            protocol += std::to_string(nr1) + "#" + std::to_string(nr2) + "#" + std::to_string(nr3) + "#" +
                std::to_string(nr4) + "#" + std::to_string(nr5);
            protocol += p1 + p2 + p3 + p4 + p5;
            std::cout << protocol << std::endl;
            return protocol;
        }
        else if (b->getRolFromUsername(utilizator) == 4)
        {
            int nr1 = b->getNrRapoartePermisieCMDBatalion();
            int nr2 = b->getNrRapoarteInvoireCMDBatalion();
            int nr3 = b->getNrRapoarteDeplasareCMDBatalion();
            int nr4 = b->getNrRapoarteLaptopCMDBatalion();
            int nr5 = b->getNrRapoarteDezcazarmareCMDBatalion();
            std::string p1 = b->getRaportPermisieCMDBatalion();
            std::string p2 = b->getRaportInvoireCMDBatalion();
            std::string p3 = b->getRaportDeplasareCMDBatalion();
            std::string p4 = b->getRaportLaptopCMDBatalion();
            std::string p5 = b->getRaportDezcazarmareCMDBatalion();
            std::string protocol;
            protocol += std::to_string(nr1) + std::to_string(nr2) + std::to_string(nr3) +
                std::to_string(nr4) + std::to_string(nr5);
            protocol += p1 + p2 + p3 + p4 + p5;
            return protocol;
        }
        else if (b->getRolFromUsername(utilizator) == 6)
        {
            int nr1 = b->getNrRapoartePermisieIndr(utilizator);
            int nr2 = b->getNrRapoarteInvoireIndr(utilizator);
            int nr3 = b->getNrRapoarteDeplasareIndr(utilizator);
            std::string p1 = b->getRaportPermisieIndr();
            std::string p2 = b->getRaportInvoireIndr();
            std::string p3 = b->getRaportDeplasareIndr();
            std::string protocol;
            protocol += std::to_string(nr1) + std::to_string(nr2) + std::to_string(nr3);
            protocol += p1 + p2 + p3;
            return protocol;
        }
        else { std::cout << "Error" << std::endl; }
    }
    std::vector<std::string> splitString(const std::string& input, char delimiter) {
        std::vector<std::string> result;
        std::istringstream stream(input);
        std::string item;
        while (std::getline(stream, item, delimiter)) {
            result.push_back(item);
        }
        return result;
    }
    void updateNameSectorStudent(char* buffer, BazaDeDate* b)
    {
        std::string buffer1 = buffer;
        buffer1.substr(1);
        std::vector<std::string> names = splitString(buffer1, '#');
        b->updateSectorNames(names);
        std::cout << "Sectors updates succesfully!" << std::endl;
    }
    std::string sendClientPretentaApel(BazaDeDate* b)
    {
        std::wstring protocol;
        std::vector<std::vector<std::wstring>> matrix = b->loadTableIntoMatrix(L"Studenti");
        for (int i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][6] == L"1" && matrix[i][8] == L"Almasanu Mihai") { protocol += L"1#"; }
            if (matrix[i][6] == L"2" && matrix[i][8] == L"Almasanu Mihai") { protocol += L"2#"; }
            if (matrix[i][6] == L"3" && matrix[i][8] == L"Almasanu Mihai") { protocol += L"3#"; }
            if (matrix[i][6] == L"4" && matrix[i][8] == L"Almasanu Mihai") { protocol += L"4#"; }
            if (matrix[i][6] == L"5" && matrix[i][8] == L"Almasanu Mihai") { protocol += L"5#"; }
            if (b->isStudentPrezentLaApel(matrix[i][0]) == 1)
            {
                protocol += matrix[i][1] + L"#" + matrix[i][2] + L"#";
            }
        }
        return b->convertToString(protocol);
    }
    std::string sendStatistici(BazaDeDate* b, char* buffer)
    {
        std::wstring protocol;
        std::vector<std::vector<std::wstring>> matrix = b->loadTableIntoMatrix(L"Statistici");
        if (buffer[1] == '0')
        {
            std::cout << "Adminul vrea sa vizualizeze statistici studenti" << std::endl;
            for (int i = 0; i < matrix.size(); i++)
            {
                protocol += matrix[i][1] + L"#" + matrix[i][3] + L"#" +
                    matrix[i][4] + L"#" + matrix[i][5] + L"#" + matrix[i][6] + L"#" +
                    matrix[i][7] + L"#" + matrix[i][8] + L"#" + matrix[i][9] + L"#";
            }
        }
        else if (buffer[1] == '1')
        {
            std::cout << "Adminul vrea sa vizualizeze statistici plutoane" << std::endl;
            int timp_totalp1 = 0;
            int nr_rap_totalp1 = 0;
            int timp_totalp2 = 0;
            int nr_rap_totalp2 = 0;
            int timp_totalp3 = 0;
            int nr_rap_totalp3 = 0;
            int timp_totalp4 = 0;
            int nr_rap_totalp4 = 0;
            int timp_totalp5 = 0;
            int nr_rap_totalp5 = 0;
            for (int i = 0; i < matrix.size(); i++)
            {
                if (matrix[i][2] == L"1") { 
                    timp_totalp1 += b->convertWstringToInt(matrix[i][3]);
                    nr_rap_totalp1 += b->convertWstringToInt(matrix[i][5]) +
                        b->convertWstringToInt(matrix[i][6]) +
                        b->convertWstringToInt(matrix[i][7]) +
                        b->convertWstringToInt(matrix[i][8]) +
                        b->convertWstringToInt(matrix[i][9]);
                }
                if (matrix[i][2] == L"2")
                {
                    timp_totalp2 += b->convertWstringToInt(matrix[i][3]);
                    nr_rap_totalp2 += b->convertWstringToInt(matrix[i][5]) +
                        b->convertWstringToInt(matrix[i][6]) +
                        b->convertWstringToInt(matrix[i][7]) +
                        b->convertWstringToInt(matrix[i][8]) +
                        b->convertWstringToInt(matrix[i][9]);
                }
                if (matrix[i][2] == L"3")
                {
                    timp_totalp3 += b->convertWstringToInt(matrix[i][3]);
                    nr_rap_totalp3 += b->convertWstringToInt(matrix[i][5]) +
                        b->convertWstringToInt(matrix[i][6]) +
                        b->convertWstringToInt(matrix[i][7]) +
                        b->convertWstringToInt(matrix[i][8]) +
                        b->convertWstringToInt(matrix[i][9]);
                }
                if (matrix[i][2] == L"4")
                {
                    timp_totalp4 += b->convertWstringToInt(matrix[i][3]);
                    nr_rap_totalp4 += b->convertWstringToInt(matrix[i][5]) +
                        b->convertWstringToInt(matrix[i][6]) +
                        b->convertWstringToInt(matrix[i][7]) +
                        b->convertWstringToInt(matrix[i][8]) +
                        b->convertWstringToInt(matrix[i][9]);
                }
                if (matrix[i][2] == L"5")
                {
                    timp_totalp5 += b->convertWstringToInt(matrix[i][3]);
                    nr_rap_totalp5 += b->convertWstringToInt(matrix[i][5]) +
                        b->convertWstringToInt(matrix[i][6]) +
                        b->convertWstringToInt(matrix[i][7]) +
                        b->convertWstringToInt(matrix[i][8]) +
                        b->convertWstringToInt(matrix[i][9]);
                }
            }
            protocol += b->convertToWString(std::to_string(timp_totalp1)) + L"#" +
                b->convertToWString(std::to_string(nr_rap_totalp1)) + L"#" +
                b->convertToWString(std::to_string(timp_totalp2)) + L"#" +
                b->convertToWString(std::to_string(nr_rap_totalp2)) + L"#" +
                b->convertToWString(std::to_string(timp_totalp3)) + L"#" +
                b->convertToWString(std::to_string(nr_rap_totalp3)) + L"#" +
                b->convertToWString(std::to_string(timp_totalp4)) + L"#" +
                b->convertToWString(std::to_string(nr_rap_totalp4)) + L"#" +
                b->convertToWString(std::to_string(timp_totalp5)) + L"#" +
                b->convertToWString(std::to_string(nr_rap_totalp5)) + L"#";
        }
        else if (buffer[1] == '2')
        {
            std::cout << "Adminul vrea sa vizualizeze statistici companie" << std::endl;
            int timp_total = 0;
            int nr_rap_total = 0;
            for (int i = 0; i < matrix.size(); i++)
            {
                timp_total += b->convertWstringToInt(matrix[i][3]);
                nr_rap_total += b->convertWstringToInt(matrix[i][5]) +
                    b->convertWstringToInt(matrix[i][6]) +
                    b->convertWstringToInt(matrix[i][7]) +
                    b->convertWstringToInt(matrix[i][8]) +
                    b->convertWstringToInt(matrix[i][9]);
            }
            protocol += b->convertToWString(std::to_string(timp_total)) + L"#" +
                b->convertToWString(std::to_string(nr_rap_total)) + L"#";
        }
        return b->convertToString(protocol);
    }

};