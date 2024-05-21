//#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include "BazaDeDate.h"
#include "md5.h"
#include "IUtilizator.h"
#include "Utilizator.h"
#include <math.h>
#include "ServerManagement.h"
#include <chrono>
#include <thread>
#include <Windows.h>
#include "ExceptionClass.h"
#include "Logger.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

using namespace std::chrono;

time_point<steady_clock> start_time;
time_point<steady_clock> end_time;

void start_clock() {
    start_time = steady_clock::now();
}

void stop_clock() {
    end_time = steady_clock::now();
}

int get_elapsed_time() {
    return duration_cast<seconds>(end_time - start_time).count();
}

int main() {
    Logger* logger = new Logger();

    WSADATA wsaData;
    int iResult;

    // Initializare Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in coming_addr;

    // Structura sockaddr_in e folosita pentru a specifica adresa IP
    // si portul pe care ascultam
    struct sockaddr_in saServer;
    saServer.sin_family = AF_INET;
    // 0.0.0.0 inseamna orice IP 
    inet_pton(AF_INET, "0.0.0.0", &saServer.sin_addr);
    saServer.sin_port = htons(12345);

    bind(listen_sock, (sockaddr*)&saServer, sizeof(sockaddr_in));
    listen(listen_sock, SOMAXCONN);

    int size = sizeof(coming_addr);
    SOCKET client_sock = accept(listen_sock,(sockaddr*) &coming_addr, &size);
    if (client_sock == INVALID_SOCKET)
    {
		printf("connect function failed with error: %ld\n", WSAGetLastError());
        return -1;
    }
    else {
        char buffer[36];
        inet_ntop(AF_INET, &coming_addr.sin_addr.s_addr, buffer, 36);
        printf("%s connected!\n", buffer);
        std::cout << "Ceasul a pornit" << std::endl;
        start_clock();
    }
    //////////////////conectare baza de date + initializare utilizator + SM///////////////////
    BazaDeDate* b = new BazaDeDate();
    try {
        b->connect();
    }
    catch (const ExceptionClass& e) {
        std::cout << e.what() << std::endl;
    }
    char buffer[1024];
    Utilizator* utilizator = new Utilizator();
    ServerManagement* SM = new ServerManagement();
    /////////////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        try {
            printf("%s\n", buffer);
            int recv_result = recv(client_sock, buffer, 1024, 0);
            if (recv_result == 0) {
                std::cout << "Clientul s-a deconectat" << std::endl;
                logger->log("Clientul s-a deconectat");
                stop_clock();
                int elapsed_time = get_elapsed_time();
                cout << "Client stayed on the server for: " << elapsed_time << " seconds." << endl;
                b->updateTimpLogat(b->convertToString(utilizator->getUsername()), elapsed_time);
                return 0;
            }
            switch (buffer[0]) //for client login verification
            {
            case '^':
            {
                std::vector<std::string> vector = SM->getUsernameANDPassword(buffer, utilizator, b, logger);
                std::wstring string = b->checkCreditentials(b->convertToWString(vector[0]), b->convertToWString(md5(vector[1])), logger);
                std::string message = SM->sendClientMessageAboutLogin(string, logger);
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '0': { //aici sunt trimise info despre rapoartele cerute (0-4)
                std::cout << "Utilizatorul vrea sa creeze raport permisie" << std::endl;
                logger->log("Utilizatorul vrea sa creeze raport permisie");
                std::string message = b->convertToString(b->sendProtocolRaportPermisie(utilizator));
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '1':
            {
                std::cout << "Utilizatorul vrea sa creeze raport invoire" << std::endl;
                logger->log("Utilizatorul vrea sa creeze raport invoire");
                std::string message = b->convertToString(b->sendProtocolRaportInvoire(utilizator));
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '2':
            {
                std::cout << "Utilizatorul vrea sa creeze raport deplasare" << std::endl;
                logger->log("Utilizatorul vrea sa creeze raport deplasare");
                std::string message = b->convertToString(b->sendProtocolRaportDeplasare(utilizator));
                std::cout << message << std::endl;
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '3':
            {
                std::cout << "Utilizatorul vrea sa creeze raport laptop" << std::endl;
                logger->log("Utilizatorul vrea sa creeze raport laptop");
                std::string message = b->convertToString(b->sendProtocolRaportLaptop(utilizator));
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '4':
            {
                std::cout << "Utilizatorul vrea sa creeze raport dezcazarmare" << std::endl;
                logger->log("Utilizatorul vrea sa creeze raport dezcazarmare");
                std::string message = b->convertToString(b->sendProtocolRaportDezcazarmare(utilizator));
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '&': {  //aici incepem sa primim buffere si sa salvam in bd
                SM->saveRaportFromClient(buffer, b, utilizator, logger);
                break;
            }
            case '@': //vizualizare rapoarte personale
            {
                logger->log("Clientul vrea sa vizualizeze date personale");
                std::string protocol = b->makeProtocolVizualizareRapoarte(utilizator);
                std::cout << protocol << std::endl;
                send(client_sock, protocol.c_str(), protocol.length(), 0);
                break;
            }
            case '$': //vizualizare date personale
            {
                logger->log("Clientul a vizualizat date personale");
                std::string protocol = b->makeProtocolVizualizareDatePersonale(utilizator);
                std::cout << protocol << std::endl;
                send(client_sock, protocol.c_str(), protocol.length(), 0);
                break;
            }
            case '*': //pt schimnare parola
            {
                logger->log("Clientul vrea sa schimbe parola");
                std::vector<std::string> vector = SM->getOldPasswordANDNewPassword(buffer);
                std::cout << vector[1] << std::endl;
                std::wstring string = b->checkCreditentials(utilizator->getUsername(), b->convertToWString(md5(vector[0])), logger);
                b->updateParola(b->convertWstringToInt(utilizator->getID()), b->convertToWString(md5(vector[1])));
                std::string protocol = SM->sendClientMessageAboutOldPassword(string);
                send(client_sock, protocol.c_str(), protocol.length(), 0);
                //b->updateNrChangePasswd(b->convertToString(utilizator->getUsername())); //update
                break;
            }
            case '%': //pt a vedea daca esti sau nu alocat la masa (send 0 sau 1)
            {
                logger->log("Clientul vrae sa vada daca este alocat la masa");
                int* protocol = SM->isClientAlocatedforFood(utilizator, b);
                std::string message;
                message += std::to_string(protocol[0]) + std::to_string(protocol[1]) + std::to_string(protocol[2]);
                std::cout << message << std::endl;
                send(client_sock, message.c_str(), message.length(), 0);
                break;
            }
            case '!':
            {
                logger->log("Comandantul de companie a verificat sectoarele!");
                std::cout << "Comandantul de companie a verificat sectoarele!" << std::endl;
                SM->updateSector_Facut(b, buffer);
                break;
            }
            case '?':
            {
                logger->log("Comandantul de companie vrea sa modifice sectoare:(");
                std::cout << "Comandantul de companie vrea sa modifice sectoare:(" << std::endl;
                std::string m1, m2, m3;
                m1 = SM->sendClientInfoSectoare1(b);
                m2 = SM->sendClientInfoSectoare2(b);
                m3 = SM->sendClientInfoSectoare3(b);
                std::cout << m1 << '@' << std::endl;
                std::cout << m2 << '@' << std::endl;
                std::cout << m3 << '@' << std::endl;
                send(client_sock, m1.c_str(), m1.length(), 0);
                Sleep(1000);
                send(client_sock, m2.c_str(), m2.length(), 0);
                Sleep(1000);
                send(client_sock, m3.c_str(), m3.length(), 0);
                break;
            }
            case '+':
            {
                logger->log("Comandantul de companie a modificat sectoarele!");
                std::cout << "Comandantul de companie a modificat sectoarele!" << std::endl;
                SM->updateNameSectorStudent(buffer, b);
                //sa vedem ce o sa primesc...
                break;
            }
            case 'Q':
            {
                logger->log("Clientul intentioneaza sa aprobe/neaprobe rapoarte");
                std::cout << "Clientul intentioneaza sa aprobe/neaprobe rapoarte" << std::endl;
                std::string message = SM->sendClientRapoarteDeAprobatNeaprobat(b, utilizator);
                send(client_sock, message.c_str(), message.length(), 0); // sa vedem daca merge!!!
                break;
            }
            case 'W': // vizualizare prezenta apel GSC
            {
                logger->log("GSC-ul vrea sa vizualizeze prezenta la apel");
                std::cout << "GSC-ul vrea sa vizualizeze prezenta la apel" << std::endl;
                std::string message = SM->sendClientPretentaApel(b);
                send(client_sock, message.c_str(), message.length(), 0); // sa vedem daca merge!!!
                break;
            }
            case 'E':
            {
                logger->log("Adminul vrea sa vizualizeze statistici!");
                std::cout << "Adminul vrea sa vizualizeze statistici!" << std::endl;
                std::string message = SM->sendStatistici(b, buffer);
                send(client_sock, message.c_str(), message.length(), 0); // sa vedem daca merge!!!
                break;
            }
            }
            strcpy(buffer, "");
        }
        catch (ExceptionClass* e)
        {
            std::cout << e->what() << std::endl;
        }
    }    //!!!!!!!!!!!!!!!!!!!!!sa nu trimitem 2 rapoarte de acelasi fel pt un client!!!!!!!
    logger->~Logger();
    return 0;
}