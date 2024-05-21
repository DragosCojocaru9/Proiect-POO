#pragma once
#include "IBazaDeDate.h"
#include "Utilizator.h"
#include <codecvt>
#include <sqlext.h>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "ExceptionClass.h"
#include "Logger.h"

class BazaDeDate : public IBazaDeDate
{
	// Define handles and variables
	SQLHANDLE sqlConnHandle = nullptr;
	SQLHANDLE sqlStmtHandle = nullptr;
	SQLHANDLE sqlEnvHandle = nullptr;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
public:
	BazaDeDate() = default;
	void connect() {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle)) {
            throw ExceptionClass("Error allocating environment handle");
        }

        if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) {
            throw ExceptionClass("Error setting environment attributes");
        }

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle)) {
            throw ExceptionClass("Error allocating connection handle");
        }

        // Connect to SQL Server
        switch (SQLDriverConnect(sqlConnHandle,
            NULL,
            (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=LAPTOP-6U3DKNPP\\SQLEXPRESS;DATABASE=Gestionare_Documente_Batalion;Trusted_Connection=yes;",
            SQL_NTS,
            retconstring,
            1024,
            NULL,
            SQL_DRIVER_NOPROMPT)) {
        case SQL_SUCCESS:
            break;
        case SQL_SUCCESS_WITH_INFO:
            wcout << L"Successfully connected to SQL Server" << endl;
            break;

        default:
            wcout << L"Could not connect to SQL Server" << endl;
            wcout << retconstring;
        }

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
            throw ExceptionClass("Error allocating statement handle");
        }
	}
    std::vector<std::vector<std::wstring>> loadTableIntoMatrix(const std::wstring& tableName) {
        SQLCloseCursor(sqlStmtHandle);
        std::vector<std::vector<std::wstring>> matrix;

        std::wstring query = L"SELECT * FROM " + tableName;

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare statement.");
            return matrix;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute statement.");
            return matrix;
        }

        SQLSMALLINT numCols;
        SQLNumResultCols(sqlStmtHandle, &numCols);

        while (SQL_SUCCESS == SQLFetch(sqlStmtHandle)) {
            std::vector<std::wstring> row;

            for (SQLSMALLINT i = 1; i <= numCols; ++i) {
                SQLWCHAR buffer[256];
                SQLLEN indicator;

                SQLGetData(sqlStmtHandle, i, SQL_C_WCHAR, buffer, sizeof(buffer), &indicator);

                if (indicator == SQL_NULL_DATA) {
                    row.push_back(L"NULL");
                }
                else {
                    row.push_back(std::wstring(buffer));
                }
            }

            matrix.push_back(row);
        }

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);   //reset handle
        return matrix;
    }
    std::wstring checkCreditentials(const std::wstring username, const std::wstring password, Logger* logger)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Conturi_Utilizatori");
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == username && matrix[i][j + 1] == password) {
                    logger->log("Clientul s-a logat cu succes");
                    return matrix[i][j + 2];
                }
            }
        }
        logger->log("Clientul a esuat in logare la server");
        return L"5";
    }
    std::wstring getIDfromTable(const std::wstring username) {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Conturi_Utilizatori");
        std::wcout << matrix[0][0] << std::endl;
        for (int i = 0; i < matrix.size() - 1; i++)
        {
            for (int j = 0; j < matrix[i].size() - 1; j++)
            {
                if (matrix[i][j] == username) {
                    return matrix[i][j - 1];
                }
            }
        }
    }
    std::wstring sendProtocolRaportPermisie(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        std::wstring id_utilizator = utilizator->getID();
        std::wstring wstring_to_send;
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == id_utilizator) {
                    wstring_to_send += matrix[i][j + 1] + L"#" + matrix[i][j + 2] + L"#" + matrix[i][j + 3] + L"#" + matrix[i][j + 6] + L"#" + matrix[i][j + 7] + L"#";
                    std::wcout << wstring_to_send << std::endl;
                    return wstring_to_send;
                }
            }
        }
    }
    std::wstring sendProtocolRaportInvoire(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        std::wstring id_utilizator = utilizator->getID();
        std::wstring wstring_to_send;
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == id_utilizator) {
                    wstring_to_send += matrix[i][j + 1] + L"#" + matrix[i][j + 2] + L"#" + matrix[i][j + 3] + L"#" + matrix[i][j + 6] + L"#" + matrix[i][j + 7] + L"#";
                    std::wcout << wstring_to_send << std::endl;
                    return wstring_to_send;
                }
            }
        }
    }
    std::wstring sendProtocolRaportDeplasare(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        std::wstring id_utilizator = utilizator->getID();
        std::wstring wstring_to_send;
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == id_utilizator) {
                    wstring_to_send += matrix[i][j + 1] + L"#" + matrix[i][j + 2] + L"#" + matrix[i][j + 3] + L"#" + matrix[i][j + 6] + L"#" + matrix[i][j + 7] + L"#";
                    std::wcout << wstring_to_send << std::endl;
                    std::cout << "Am trimis date" << std::endl;
                    return wstring_to_send;
                }
            }
        }
    }
    std::wstring sendProtocolRaportLaptop(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        std::wstring id_utilizator = utilizator->getID();
        std::wstring wstring_to_send;
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == id_utilizator) {
                    wstring_to_send += matrix[i][j + 1] + L"#" + matrix[i][j + 2] + L"#" + matrix[i][j + 6] + L"#" + matrix[i][j + 7] + L"#";
                    std::wcout << wstring_to_send << std::endl;
                    return wstring_to_send;
                }
            }
        }
    }
    std::wstring sendProtocolRaportDezcazarmare(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        std::wstring id_utilizator = utilizator->getID();
        std::wstring wstring_to_send;
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == id_utilizator) {
                    wstring_to_send += matrix[i][j + 1] + L"#" + matrix[i][j + 2] + L"#" + matrix[i][j + 3] + L"#" + matrix[i][j + 6] + L"#" + matrix[i][j + 7] + L"#" + matrix[i][j + 8] + L"#" + matrix[i][j + 9] + L"#";
                    std::wcout << wstring_to_send << std::endl;
                    return wstring_to_send;
                }
            }
        }
    }
    std::wstring convertToWString(const std::string& input) {
        try {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(input);
        }
        catch (const std::range_error& e) {
            std::cerr << "Range error during conversion: " << e.what() << std::endl;
            throw;
        }
    }
    std::string convertToString(const std::wstring& input) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(input);
    }
    int convertWstringToInt(const std::wstring& input) {
        return std::stoi(input);
    }

    //insert into db
    void insertRaportPermisie(
        int id_student,
        std::wstring username,
        std::wstring localitate,
        std::wstring judet,
        std::wstring data_plecare,
        std::wstring ora_plecare,
        std::wstring data_intoarcere,
        std::wstring ora_intoarcere,
        std::wstring motiv,
        int raspuns_comandant_companie,
        int raspuns_comandant_batalion,
        int raspuns_indrumator_studii
    )
    {
        std::wstring sql = L"INSERT INTO Raport_Permisie (id_student, username, localitate, judet, data_plecare, ora_plecare, data_intoarcere, ora_intoarcere, motiv, raspuns_comandant_companie, raspuns_comandant_batalion,raspuns_indrumator_studii) "
            L"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare statement.");
            return;
        }

        std::cout << id_student << std::endl;

        // Bind the parameters
        if (SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id_student, 0, NULL) == 0) {
            std::cout << "1 is good" << std::endl;
        }
        else {
            std::cout << "1 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)username.c_str(), 0, NULL) == 0) {
            std::cout << "2 is good" << std::endl;
        }
        else {
            std::cout << "2 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)localitate.c_str(), 0, NULL) == 0) {
            std::cout << "3 is good" << std::endl;
        }
        else {
            std::cout << "3 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)judet.c_str(), 0, NULL) == 0) {
            std::cout << "4 is good" << std::endl;
        }
        else {
            std::cout << "4 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLPOINTER)data_plecare.c_str(), 0, NULL) == 0) {
            std::cout << "5 is good" << std::endl;
        }
        else {
            std::cout << "5 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 8, 0, (SQLPOINTER)ora_plecare.c_str(), 0, NULL) == 0) {
            std::cout << "6 is good" << std::endl;
        }
        else {
            std::cout << "6 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLPOINTER)data_intoarcere.c_str(), 0, NULL) == 0) {
            std::cout << "7 is good" << std::endl;
        }
        else {
            std::cout << "7 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 8, 0, (SQLPOINTER)ora_intoarcere.c_str(), 0, NULL) == 0) {
            std::cout << "8 is good" << std::endl;
        }
        else {
            std::cout << "8 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)motiv.c_str(), 0, NULL) == 0) {
            std::cout << "9 is good" << std::endl;
        }
        else {
            std::cout << "9 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_companie, 0, NULL) == SQL_SUCCESS) {
            std::cout << "10 is good" << std::endl;
        }
        else {
            std::cout << "10 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_batalion, 0, NULL) == SQL_SUCCESS) {
            std::cout << "11 is good" << std::endl;
        }
        else {
            std::cout << "11 binding failed" << std::endl;
        }

        if (SQLBindParameter(sqlStmtHandle, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_indrumator_studii, 0, NULL) == SQL_SUCCESS) {
            std::cout << "12 is good" << std::endl;
        }
        else {
            std::cout << "12 binding failed" << std::endl;
        }

        // Execute the query
        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute INSERT statement.");
            return;
        }

        std::wcout << L"Data inserted into Raport_Permisie." << std::endl;
        updateNrRapPermisie(convertToString(username));
        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);  // free handle
    }
    void insertRaportInvoire(
        int id_student,
        std::wstring username,
        std::wstring data_zilei,
        std::wstring ora_plecare,
        std::wstring ora_intoarcere,
        std::wstring motiv,
        int raspuns_comandant_companie,
        int raspuns_comandant_batalion,
        int raspuns_indrumator_studii
    )
    {
        std::wstring sql = L"INSERT INTO Raport_Invoire (id_student, username, data_zilei, ora_plecare, ora_intoarcere, motiv, raspuns_comandant_companie, raspuns_comandant_batalion, raspuns_indrumator_studii) "
            L"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare statement.");
            return;
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id_student, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)username.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)data_zilei.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 8, 0, (SQLPOINTER)ora_plecare.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 8, 0, (SQLPOINTER)ora_intoarcere.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)motiv.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_companie, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_batalion, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_indrumator_studii, 0, NULL);

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute INSERT statement.");
            return;
        }

        std::wcout << L"Data inserted into Raport_Invoire." << std::endl;
        updateNrRapInvoire(convertToString(username));
        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);  //free handle
    }
    void insertRaportDeplasare(
        int id_student,
        std::wstring username,
        std::wstring localitate,
        std::wstring tara,
        std::wstring data_plecare,
        std::wstring data_intoarcere,
        std::wstring adresa,
        std::wstring nume_proprietar,
        int raspuns_comandant_companie,
        int raspuns_comandant_batalion,
        int raspuns_indrumator_studii
    )
    {
        std::wstring sql = L"INSERT INTO Raport_Deplasare (id_student, username, localitate, tara, data_plecare, data_intoarcere, adresa, nume_proprietar, raspuns_comandant_companie, raspuns_comandant_batalion, raspuns_indrumator_studii) "
            L"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare statement.");
            return;
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id_student, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)username.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)localitate.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)tara.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLPOINTER)data_plecare.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLPOINTER)data_intoarcere.c_str(), 0, NULL);  
        SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)adresa.c_str(), 0, NULL);  
        SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)nume_proprietar.c_str(), 0, NULL);  
        SQLBindParameter(sqlStmtHandle, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_companie, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_batalion, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_indrumator_studii, 0, NULL);

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute INSERT statement.");
            return;
        }

        std::wcout << L"Data inserted into Raport_Deplasare." << std::endl;
        updateNrRapDeplasare(convertToString(username));
        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);  //free handle
    }
    void insertRaportLaptop(
        int id_student,
        std::wstring username,
        std::wstring motiv,
        int raspuns_comandant_companie,
        int raspuns_comandant_batalion
    )
    {
        std::wstring sql = L"INSERT INTO Raport_Laptop (id_student, username, motiv, raspuns_comandant_companie, raspuns_comandant_batalion) "
            L"VALUES (?, ?, ?, ?, ?)";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare statement.");
            return;
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id_student, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)username.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 255, 0, (SQLPOINTER)motiv.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_companie, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_batalion, 0, NULL);

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute INSERT statement.");
            return;
        }

        std::wcout << L"Data inserted into Raport_Laptop." << std::endl;
        updateNrRapLaptop(convertToString(username));
        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);  //free handle
    }
    void insertRaportDezcazarmare(
        int id_student,
        std::wstring username,
        std::wstring data_incepere,
        std::wstring adresa,
        std::wstring nume_proprietar,
        std::wstring tel_proprietar,
        int raspuns_comandant_companie,
        int raspuns_comandant_batalion
    )
    {
        std::wstring sql = L"INSERT INTO Raport_Dezcazarmare (id_student, username, data_incepere, adresa, nume_proprietar, tel_proprietar, raspuns_comandant_companie, raspuns_comandant_batalion) "
            L"VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare statement.");
            return;
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id_student, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)username.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 10, 0, (SQLPOINTER)data_incepere.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)adresa.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)nume_proprietar.c_str(), 0, NULL); 
        SQLBindParameter(sqlStmtHandle, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 15, 0, (SQLPOINTER)tel_proprietar.c_str(), 0, NULL); 
        SQLBindParameter(sqlStmtHandle, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_companie, 0, NULL);
        SQLBindParameter(sqlStmtHandle, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &raspuns_comandant_batalion, 0, NULL);

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw("Failed to execute INSERT statement.");
            return;
        }

        std::wcout << L"Data inserted into Raport_Dezcazarmare." << std::endl;
        updateNrRapDezcazarmare(convertToString(username));
        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);  // free handle
    }
    int findNrLinesInTable(const std::wstring& name, const std::wstring& id_utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(name);
        int nr = 0;
        for (int i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][0] == id_utilizator) { nr++; }
        }
        return nr;
    }
    std::string makeProtocolVizualizareRapoarte(Utilizator* utilizator)
    {
        std::string protocol;

        int nr_rapoarte[5] = { 0 }; //perm,inv,depl,laptop,dezc
        nr_rapoarte[0] = findNrLinesInTable(L"Raport_Permisie", utilizator->getID());
        nr_rapoarte[1] = findNrLinesInTable(L"Raport_Invoire", utilizator->getID());
        nr_rapoarte[2] = findNrLinesInTable(L"Raport_Deplasare", utilizator->getID());
        nr_rapoarte[3] = findNrLinesInTable(L"Raport_Laptop", utilizator->getID());
        nr_rapoarte[4] = findNrLinesInTable(L"Raport_Dezcazarmare", utilizator->getID());
        int nr_total_rapoarte = 0;
        for (int i = 0; i <= 4; i++) { nr_total_rapoarte += nr_rapoarte[i]; }
        protocol += std::to_string(nr_total_rapoarte) + "#";
        if (nr_rapoarte[0] != 0)
        {
            std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Raport_Permisie");
            for (int i = 0; i < matrix.size(); i++)
            {
                if (matrix[i][0] == utilizator->getID())
                {
                    protocol += "0#" + convertToString(matrix[i][9]) + convertToString(matrix[i][10]) + convertToString(matrix[i][11]) + "#";
                }
            }
        }
        if (nr_rapoarte[1] != 0)
        {
            std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Raport_Invoire");
            for (int i = 0; i < matrix.size(); i++)
            {
                if (matrix[i][0] == utilizator->getID()) {
                    protocol += "1#" + convertToString(matrix[i][6]) + convertToString(matrix[i][7]) + convertToString(matrix[i][8]) + "#";
                }
            }
        }
        if (nr_rapoarte[2] != 0)
        {
            std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Raport_Deplasare");
            for (int i = 0; i < matrix.size(); i++)
            {
                if (matrix[i][0] == utilizator->getID()) {
                    protocol += "2#" + convertToString(matrix[i][8]) + convertToString(matrix[i][9]) + convertToString(matrix[i][10]) + "#";
                }
            }
        }
        if (nr_rapoarte[3] != 0)
        {
            std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Raport_Laptop");
            for (int i = 0; i < matrix.size(); i++)
            {
                if (matrix[i][0] == utilizator->getID()) {
                    protocol += "3#" + convertToString(matrix[i][3]) + convertToString(matrix[i][4]) + "#";
                }
            }
        }
        if (nr_rapoarte[4] != 0)
        {
            std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
            for (int i = 0; i < matrix.size(); i++)
            {
                if (matrix[i][0] == utilizator->getID()) {
                    protocol += "4#" + convertToString(matrix[i][6]) + convertToString(matrix[i][7]) + "#";
                }
            }
        }
        return protocol;
    }
    std::string makeProtocolVizualizareDatePersonale(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        std::wstring protocol;
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[i].size(); j++)
            {
                if (matrix[i][j] == utilizator->getID())
                {
                    protocol += matrix[i][j + 1] + L"#" + matrix[i][j + 2] + L"#" + matrix[i][j + 3] + L"#" + matrix[i][j + 4] + L"#" + matrix[i][j + 5] + L"#" + matrix[i][j + 6] + L"#" + matrix[i][j + 7] + L"#" + matrix[i][j + 8] + L"#" + matrix[i][j + 9] + L"#" + matrix[i][j + 10] + L"#";
                    std::wcout << protocol << std::endl;
                    return convertToString(protocol);
                }
            }
        }
    }
    void updateParola(int userId, const std::wstring& newParola) {
        std::wstring sql = L"UPDATE Conturi_Utilizatori SET parola_hash = ? WHERE id = ?";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare UPDATE statement.");
            return;
        }

        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 256, 0, (SQLPOINTER)newParola.c_str(), 0, NULL);
        SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userId, 0, NULL);

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute UPDATE statement.");
            return;
        }

        std::wcout << L"Successfully updated parola_hash for user ID: " << userId << std::endl;

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
    }
    void updateSector_Facutt(const char* name, std::wstring nr) {
        if (name == nullptr) {
            throw ExceptionClass("Sector name cannot be null.");
            return;
        }

        std::wstring sql = L"UPDATE Sectoare SET sector_facut =" + nr + L" WHERE nume_sector = ? ";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare SQL statement.");
            return;
        }
        std::cout << name << std::endl;
        std::wstring nume = convertToWString(name);
        SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)nume.c_str(), 0, NULL);

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Failed to execute SQL update.");
            return;
        }

        std::wcout << L"Updated sector_facut to 0 for nume_sector: " << name << std::endl;

        SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
    }
    int getRolFromUsername(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Conturi_Utilizatori");
        for (int i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][1] == utilizator->getUsername()) { return std::stoi(convertToString(matrix[i][3])); }
        }
    }
    ////////////////////////////nr_rapoarte and stuff//////////////////////////////////
    int getNrRapoartePermisieCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        int nr_rapoarte = 0;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][9] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteInvoireCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        int nr_rapoarte = 0;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][6] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteDeplasareCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        int nr_rapoarte = 0;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][8] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteLaptopCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> laptop_matrix = loadTableIntoMatrix(L"Raport_Laptop");
        int nr_rapoarte = 0;
        for (int i = 0; i < laptop_matrix.size(); i++)
        {
            if (laptop_matrix[i][3] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteDezcazarmareCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> dezc_matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
        int nr_rapoarte = 0;
        for (int i = 0; i < dezc_matrix.size(); i++)
        {
            if (dezc_matrix[i][6] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }

    int getNrRapoartePermisieCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        int nr_rapoarte = 0;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][10] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteInvoireCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        int nr_rapoarte = 0;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][7] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteDeplasareCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        int nr_rapoarte = 0;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][9] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteLaptopCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> laptop_matrix = loadTableIntoMatrix(L"Raport_Laptop");
        int nr_rapoarte = 0;
        for (int i = 0; i < laptop_matrix.size(); i++)
        {
            if (laptop_matrix[i][4] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteDezcazarmareCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> dezc_matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
        int nr_rapoarte = 0;
        for (int i = 0; i < dezc_matrix.size(); i++)
        {
            if (dezc_matrix[i][7] == L"0") { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }

    std::wstring getNumeGrupaIndr(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Conturi_Utilizatori");
        for (int i = 0; i < matrix.size(); i++)
        {
            if (matrix[i][4] == utilizator->getUsername()) { return matrix[i][4]; }
        }
    }
    std::wstring getNumeGrupaStudentWithId(std::wstring id_student)
    {
        std::vector<std::vector<std::wstring>> matrix = loadTableIntoMatrix(L"Studenti");
        for (int i = 0; i < matrix.size(); i++)
        {
            if (id_student == matrix[i][0]) { return matrix[i][10]; } //grupa de studii
        }
    }
    int getNrRapoartePermisieIndr(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        int nr_rapoarte = 0;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][11] == L"0" && getNumeGrupaIndr(utilizator) == getNumeGrupaStudentWithId(perm_matrix[i][0])) { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteInvoireIndr(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        int nr_rapoarte = 0;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][8] == L"0" && getNumeGrupaIndr(utilizator) == getNumeGrupaStudentWithId(inv_matrix[i][0])) { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    int getNrRapoarteDeplasareIndr(Utilizator* utilizator)
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        int nr_rapoarte = 0;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][10] == L"0" && getNumeGrupaIndr(utilizator) == getNumeGrupaStudentWithId(depl_matrix[i][0])) { nr_rapoarte++; }
        }
        return nr_rapoarte;
    }
    ////////////////////////////////////////////////////////////////////////
    ///////////////////////////get rapoarte////////////////////////////////  
    std::string getRaportPermisieWithIdStudent(std::wstring id_student)
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        std::wstring protocol;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][0] == id_student)
            {
                protocol += L"#" + perm_matrix[i][1] + L"#" + perm_matrix[i][2] + L"#" +
                    perm_matrix[i][3] + L"#" + perm_matrix[i][4] + L"#" + perm_matrix[i][5] +
                    L"#" + perm_matrix[i][6] + L"#" + perm_matrix[i][7] + L"#" + perm_matrix[i][8] +
                    L"#" + perm_matrix[i][9] + L"#" + perm_matrix[i][10];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    ////////////////////////////////////////////////////////vedem
    std::string getRaportInvoireWithIdStudent(std::wstring id_student)
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        std::wstring protocol;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][0] == id_student)
            {
                protocol += L"#" + inv_matrix[i][1] + L"#" + inv_matrix[i][2] + L"#" +
                    inv_matrix[i][3] + L"#" + inv_matrix[i][4] + L"#" + inv_matrix[i][5] + L"#";
            }
        }
        return convertToString(protocol);
    }
    std::string getRaportDeplasareWithIdStudent(std::wstring id_student)
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        std::wstring protocol;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][0] == id_student)
            {
                protocol += L"#" + depl_matrix[i][1] + L"#" + depl_matrix[i][2] + L"#" +
                    depl_matrix[i][3] + L"#" + depl_matrix[i][4] + L"#" + depl_matrix[i][5] + L"#" +
                    depl_matrix[i][6] + L"#" + depl_matrix[i][7] + L"#";
            }
        }
        return convertToString(protocol);
    }
    std::string getRaportLaptopWithIdStudent(std::wstring id_student)
    {
        std::vector<std::vector<std::wstring>> laptop_matrix = loadTableIntoMatrix(L"Raport_Laptop");
        std::wstring protocol;
        for (int i = 0; i < laptop_matrix.size(); i++)
        {
            protocol += L"#" + laptop_matrix[i][1] + L"#" + laptop_matrix[i][2] + L"#";
        }
        return convertToString(protocol);
    }
    std::string getRaportDezcazarmareWithIdStudent(std::wstring id_student)
    {
        std::vector<std::vector<std::wstring>> dezc_matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
        std::wstring protocol;
        for (int i = 0; i < dezc_matrix.size(); i++)
        {
            if (dezc_matrix[i][0] == id_student)
            {
                protocol += L"#" + dezc_matrix[i][1] + L"#" + dezc_matrix[i][2] + L"#" +
                    dezc_matrix[i][3] + L"#" + dezc_matrix[i][4] + L"#" + dezc_matrix[i][5] + L"#";
            }
        }
        return convertToString(protocol);
    }
    ///////////////////////////////////////////////////////////vedem
    ////////////////send protocol for each raport for each client type:(////////////////////
    std::string getRaportPermisieCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        std::wstring protocol;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][9] == L"0")
            {
                protocol += L"#" + perm_matrix[i][1] + L"#" + perm_matrix[i][2] + L"#" +
                    perm_matrix[i][3] + L"#" + perm_matrix[i][4] + L"#" + perm_matrix[i][5] + L"#" +
                    perm_matrix[i][6] + L"#" + perm_matrix[i][7] + L"#" + perm_matrix[i][8];
            }
        }
        //protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportInvoireCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        std::wstring protocol;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][6] == L"0")
            {
                protocol += L"#" + inv_matrix[i][1] + L"#" + inv_matrix[i][2] + L"#" +
                    inv_matrix[i][3] + L"#" + inv_matrix[i][4] + L"#" + inv_matrix[i][5];
            }
        }
        //protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportDeplasareCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        std::wstring protocol;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][8] == L"0")
            {
                protocol += L"#" + depl_matrix[i][1] + L"#" + depl_matrix[i][2] + L"#" +
                    depl_matrix[i][3] + L"#" + depl_matrix[i][4] + L"#" + depl_matrix[i][5] + L"#" +
                    depl_matrix[i][6] + L"#" + depl_matrix[i][7];
            }
        }
        //protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportLaptopCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> laptop_matrix = loadTableIntoMatrix(L"Raport_Laptop");
        std::wstring protocol;
        for (int i = 0; i < laptop_matrix.size(); i++)
        {
            if (laptop_matrix[i][3] == L"0")
            {
                protocol += L"#" + laptop_matrix[i][1] + L"#" + laptop_matrix[i][2];
            }
        }
        //protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportDezcazarmareCMDCompanie()
    {
        std::vector<std::vector<std::wstring>> dezc_matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
        std::wstring protocol;
        for (int i = 0; i < dezc_matrix.size(); i++)
        {
            if (dezc_matrix[i][6] == L"0")
            {
                protocol += L"#" + dezc_matrix[i][1] + L"#" + dezc_matrix[i][2] + L"#" +
                    dezc_matrix[i][3] + L"#" + dezc_matrix[i][4] + L"#" + dezc_matrix[i][5];
            }
        }
        //protocol += L"#";
        return  convertToString(protocol);
    }

    std::string getRaportPermisieCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        std::wstring protocol;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][10] == L"0")
            {
                protocol += L"#" + perm_matrix[i][1] + L"#" + perm_matrix[i][2] + L"#" +
                    perm_matrix[i][3] + L"#" + perm_matrix[i][4] + L"#" + perm_matrix[i][5] + L"#" +
                    perm_matrix[i][6] + L"#" + perm_matrix[i][7] + L"#" + perm_matrix[i][8];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportInvoireCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        std::wstring protocol;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][7] == L"0")
            {
                protocol += L"#" + inv_matrix[i][1] + L"#" + inv_matrix[i][2] + L"#" +
                    inv_matrix[i][3] + L"#" + inv_matrix[i][4] + L"#" + inv_matrix[i][5];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportDeplasareCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        std::wstring protocol;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][9] == L"0")
            {
                protocol += L"#" + depl_matrix[i][1] + L"#" + depl_matrix[i][2] + L"#" +
                    depl_matrix[i][3] + L"#" + depl_matrix[i][4] + L"#" + depl_matrix[i][5] + L"#" +
                    depl_matrix[i][6] + L"#" + depl_matrix[i][7];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportLaptopCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> laptop_matrix = loadTableIntoMatrix(L"Raport_Laptop");
        std::wstring protocol;
        for (int i = 0; i < laptop_matrix.size(); i++)
        {
            if (laptop_matrix[i][4] == L"0")
            {
                protocol += L"#" + laptop_matrix[i][1] + L"#" + laptop_matrix[i][2];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportDezcazarmareCMDBatalion()
    {
        std::vector<std::vector<std::wstring>> dezc_matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
        std::wstring protocol;
        for (int i = 0; i < dezc_matrix.size(); i++)
        {
            if (dezc_matrix[i][7] == L"0")
            {
                protocol += L"#" + dezc_matrix[i][1] + L"#" + dezc_matrix[i][2] + L"#" +
                    dezc_matrix[i][3] + L"#" + dezc_matrix[i][4] + L"#" + dezc_matrix[i][5];
            }
        }
        protocol += L"#";
        return  convertToString(protocol);
    }

    std::string getRaportPermisieIndr()
    {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        std::wstring protocol;
        for (int i = 0; i < perm_matrix.size(); i++)
        {
            if (perm_matrix[i][11] == L"0")
            {
                protocol += L"#" + perm_matrix[i][1] + L"#" + perm_matrix[i][2] + L"#" +
                    perm_matrix[i][3] + L"#" + perm_matrix[i][4] + L"#" + perm_matrix[i][5] + L"#" +
                    perm_matrix[i][6] + L"#" + perm_matrix[i][7] + L"#" + perm_matrix[i][8];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportInvoireIndr()
    {
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        std::wstring protocol;
        for (int i = 0; i < inv_matrix.size(); i++)
        {
            if (inv_matrix[i][8] == L"0")
            {
                protocol += L"#" + inv_matrix[i][1] + L"#" + inv_matrix[i][2] + L"#" +
                    inv_matrix[i][3] + L"#" + inv_matrix[i][4] + L"#" + inv_matrix[i][5];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    std::string getRaportDeplasareIndr()
    {
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        std::wstring protocol;
        for (int i = 0; i < depl_matrix.size(); i++)
        {
            if (depl_matrix[i][10] == L"0")
            {
                protocol += L"#" + depl_matrix[i][1] + L"#" + depl_matrix[i][2] + L"#" +
                    depl_matrix[i][3] + L"#" + depl_matrix[i][4] + L"#" + depl_matrix[i][5] + L"#" +
                    depl_matrix[i][6] + L"#" + depl_matrix[i][7];
            }
        }
        protocol += L"#";
        return convertToString(protocol);
    }
    //////////////////////////////////////////////////////////////////////////////
    void updateSectorNames(const std::vector<std::string>& names) {

        if (names.size() != 121) {
            throw ExceptionClass("Error: The number of names in the vector must be 121.");
            return;
        }

        std::wstring sql = L"UPDATE Sectoare SET nume_sector = ? WHERE id_student = ?";

        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            throw ExceptionClass("Failed to prepare SQL update statement.");
            return;
        }

        for (int i = 0; i < names.size(); ++i) {
            std::wstring newSectorName = convertToWString(names[i]);

            SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0, (SQLPOINTER)newSectorName.c_str(), 0, NULL);
            int id = i + 1;
            SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &id, 0, NULL);

            if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
                std::wcout << L"Failed to execute SQL update statement for id_student = " << id << std::endl;
                continue;
            }

            std::wcout << L"Successfully updated nume_sector for id_student: " << id << std::endl;
            SQLFreeStmt(sqlStmtHandle, SQL_CLOSE);
        }
    }
    bool isStudentPrezentLaApel(const std::wstring& id_student) {
        std::vector<std::vector<std::wstring>> perm_matrix = loadTableIntoMatrix(L"Raport_Permisie");
        std::vector<std::vector<std::wstring>> inv_matrix = loadTableIntoMatrix(L"Raport_Invoire");
        std::vector<std::vector<std::wstring>> depl_matrix = loadTableIntoMatrix(L"Raport_Deplasare");
        std::vector<std::vector<std::wstring>> dezc_matrix = loadTableIntoMatrix(L"Raport_Dezcazarmare");
        std::tm roll_call_time = {};
        roll_call_time.tm_hour = 21;
        roll_call_time.tm_min = 30;

        for (const auto& row : dezc_matrix) {
            if (row[0] == id_student && row[6] == L"1" && row[7] == L"1") {
                return false;
            }
        }

        for (const auto& row : perm_matrix) {
            if (row[0] == id_student && row[9] == L"1" && row[10] == L"1" && row[11] == L"1") {
                std::tm plecare_time = {};
                std::tm intoarcere_time = {};

                std::istringstream plecare_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[4])); // data_plecare
                std::istringstream intoarcere_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[6])); // data_intoarcere

                std::istringstream ora_plecare_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[5])); // ora_plecare
                std::istringstream ora_intoarcere_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[7])); // ora_intoarcere

                // convert strings to time structures
                plecare_ss >> std::get_time(&plecare_time, "%Y-%m-%d");
                intoarcere_ss >> std::get_time(&intoarcere_time, "%Y-%m-%d");

                ora_plecare_ss >> std::get_time(&plecare_time, "%H:%M");
                ora_intoarcere_ss >> std::get_time(&intoarcere_time, "%H:%M");

                if (std::mktime(&plecare_time) <= std::mktime(&roll_call_time)) {
                    if (std::mktime(&intoarcere_time) >= std::mktime(&roll_call_time)) {
                        return false; //studentul nu este prezent
                    }
                }
            }

            for (const auto& row : inv_matrix) {
                if (row[0] == id_student && row[6] == L"1" && row[7] == L"1" && row[8] == L"1") {
                    std::tm plecare_time = {};
                    std::tm intoarcere_time = {};

                    std::istringstream data_plecare_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[2]));
                    data_plecare_ss >> std::get_time(&plecare_time, "%Y-%m-%d");

                    std::istringstream ora_plecare_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[3]));
                    ora_plecare_ss >> std::get_time(&plecare_time, "%H:%M");

                    std::istringstream ora_intoarcere_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[4]));
                    ora_intoarcere_ss >> std::get_time(&intoarcere_time, "%H:%M");

                    plecare_time.tm_year = intoarcere_time.tm_year;
                    plecare_time.tm_mon = intoarcere_time.tm_mon;
                    plecare_time.tm_mday = intoarcere_time.tm_mday;

                    roll_call_time.tm_year = plecare_time.tm_year;
                    roll_call_time.tm_mon = plecare_time.tm_mon;
                    roll_call_time.tm_mday = plecare_time.tm_mday;

                    time_t plecare_time_t = std::mktime(&plecare_time);
                    time_t intoarcere_time_t = std::mktime(&intoarcere_time);
                    time_t roll_call_time_t = std::mktime(&roll_call_time);

                    if (plecare_time_t <= roll_call_time_t && intoarcere_time_t >= roll_call_time_t) {
                        return false; //student absent
                    }
                }
            }
            for (const auto& row : depl_matrix) {
                if (row[0] == id_student && row[8] == L"1" && row[9] == L"1" && row[10] == L"1") {
                    std::tm plecare_time = {};
                    std::tm intoarcere_time = {};

                    std::istringstream plecare_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[4]));
                    std::istringstream intoarcere_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[5]));

                    plecare_ss >> std::get_time(&plecare_time, "%Y-%m-%d");
                    intoarcere_ss >> std::get_time(&intoarcere_time, "%Y-%m-%d");

                    roll_call_time.tm_year = plecare_time.tm_year;
                    roll_call_time.tm_mon = plecare_time.tm_mon;
                    roll_call_time.tm_mday = plecare_time.tm_mday;

                    std::istringstream ora_plecare_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[6]));
                    std::istringstream ora_intoarcere_ss(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(row[7]));

                    ora_plecare_ss >> std::get_time(&plecare_time, "%H:%M");
                    ora_intoarcere_ss >> std::get_time(&intoarcere_time, "%H:%M");

                    time_t plecare_time_t = std::mktime(&plecare_time);
                    time_t intoarcere_time_t = std::mktime(&intoarcere_time);
                    time_t roll_call_time_t = std::mktime(&roll_call_time);

                    if (plecare_time_t <= roll_call_time_t && intoarcere_time_t >= roll_call_time_t) {
                        return false; // student absent
                    }
                }
            }
            return true; // stud prezent
        }
    }
    void updateTimpLogat(std::string username, int timp_logat)
    {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
            throw ExceptionClass("Error allocating statement handle");
            SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
            SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
            return;
        }

        // Prepare SQL query to update timp_logat
        std::wstring query = L"UPDATE Statistici SET timp_logat = ? WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
            SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
            SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
            return;
        }

        // Bind parameters
        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &timp_logat, 0, NULL)) {
            throw ExceptionClass("Error binding parameter 1");
            SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
            SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
            SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
            return;
        }
        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter 2");
            SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
            SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
            SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
            return;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
            SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
            SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
            return;
        }
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::wcout << L"timp_logat updated successfully for username: " << std::endl;
    }
    void updateNrChangePasswd(std::string username)
    {
        std::wstring query = L"UPDATE Statistici SET nr_change_passwd = nr_change_passwd + 1 WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            return;
        }

        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter");
            return;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            return;
        }
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        //SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);        //vedem
        //SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::wcout << L"nr_change_passwd updated successfully for username: " << std::endl;
    }
    void updateNrRapPermisie(std::string username)
    {
        SQLCloseCursor(sqlStmtHandle);
        std::wstring query = L"UPDATE Statistici SET nr_rap_permisie = nr_rap_permisie + 1 WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            return;
        }

        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter");
            return;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            return;
        }
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        //SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        //SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::cout << "Nr_rap_permisie update" << std::endl;
    }
    void updateNrRapInvoire(std::string username)
    {
        SQLCloseCursor(sqlStmtHandle);
        std::wstring query = L"UPDATE Statistici SET nr_rap_invoire = nr_rap_invoire + 1 WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            return;
        }

        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter");
            return;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            return;
        }
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        //SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        //SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::cout << "Nr_rap_invoire update" << std::endl;
    }
    void updateNrRapDeplasare(std::string username)
    {
        SQLCloseCursor(sqlStmtHandle);
        std::wstring query = L"UPDATE Statistici SET nr_rap_deplasare = nr_rap_deplasare + 1 WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            return;
        }

        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter");
            return;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            return;
        }
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        //SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        //SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::cout << "Nr_rap_deplasare update" << std::endl;
    }
    void updateNrRapLaptop(std::string username)
    {
        SQLCloseCursor(sqlStmtHandle);
        std::wstring query = L"UPDATE Statistici SET nr_rap_laptop = nr_rap_laptop + 1 WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            return;
        }

        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter");
            return;
        }

        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            return;
        }
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        //SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        //SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::cout << "Nr_rap_laptop update" << std::endl;
    }
    void updateNrRapDezcazarmare(std::string username)
    {
        SQLCloseCursor(sqlStmtHandle);
        std::wstring query = L"UPDATE Statistici SET nr_rap_dezcazarmare = nr_rap_dezcazarmare + 1 WHERE username = ?";
        if (SQL_SUCCESS != SQLPrepare(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
            throw ExceptionClass("Error preparing SQL statement");
            return;
        }

        if (SQL_SUCCESS != SQLBindParameter(sqlStmtHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, username.length(), 0, (SQLCHAR*)username.c_str(), username.length(), NULL)) {
            throw ExceptionClass("Error binding parameter");
            return;
        }

        // Execute the query
        if (SQL_SUCCESS != SQLExecute(sqlStmtHandle)) {
            throw ExceptionClass("Error executing query");
            return;
        }
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        //SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        //SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        std::cout << "Nr_rap_dezcazarmare update" << std::endl;
    }
};