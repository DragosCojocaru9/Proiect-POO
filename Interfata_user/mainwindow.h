#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTimeEdit>
#include <QSpacerItem>
#include <QTextEdit>
#include <QStackedWidget>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QTcpSocket>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket *socket;
    void setMenuByCharacter(char menuIndicator);

private slots:
    void togglePasswordVisibility();
    void onLoginButtonClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *showHideButton;
    bool isPasswordVisible;

    QPixmap closedEyeIcon;
    QPixmap openEyeIcon;
    QPixmap usernameIcon;


    void setupMenu();  // Configurarea meniului
    void setupPersonalDataLayout();  // Configurarea layout-ului pentru datele personale
    void setupReportSelectionLayout();  // Configurarea layout-ului pentru selecția raportului
    void setupLaptopReportLayout();  // Configurarea layout-ului pentru raportul laptop
    void setupLeaveReportLayout();  // Configurarea layout-ului pentru raportul permisie
    void setupDormReportLayout();  // Configurarea layout-ului pentru raportul descazarmare
    void setupdeplasareReportLayout();  // Configurarea layout-ului pentru raportul permisie
    void setupinvoireReportLayout();  // Configurarea layout-ului pentru raportul permisie

    void showPersonalData();  // Slot pentru afișarea datelor personale
    void showReportSelection();  // Slot pentru afișarea selecției raportului
    void showLaptopReport();  // Slot pentru raportul pentru laptop
    void showLeaveReport();  // Slot pentru raportul pentru permisie
    void showDormReport();  // Slot pentru raportul pentru descazarmare
    void showdeplasareReport();  // Slot pentru raportul pentru descazarmare
    void showinvoireReport();  // Slot pentru raportul pentru descazarmare

    void onSubmitLaptop();  // Slot pentru trimiterea raportului laptop
    void onSubmitLeave();  // Slot pentru trimiterea raportului pentru permisie
    void onSubmitdeplasare();  // Slot pentru trimiterea raportului pentru descazarmare
    void onSubmitinvoire();  // Slot pentru trimiterea raportului pentru descazarmare
    void onSubmitDorm();  // Slot pentru trimiterea raportului pentru descazarmare

    QStackedWidget *stack;  // Widget pentru comutare între pagini
    QWidget *personalDataWidget;  // Widget pentru datele personale
    QWidget *reportSelectionWidget;  // Widget pentru selecția raportului
    QWidget *laptopReportWidget;  // Widget pentru raportul pentru laptop
    QWidget *deplasareReportWidget;  // Widget pentru raportul pentru laptop
    QWidget *invoireReportWidget;  // Widget pentru raportul pentru laptop
    QWidget *leaveReportWidget;  // Widget pentru raportul pentru permisie
    QWidget *dormReportWidget;  // Widget pentru raportul pentru descazarmare


};

#endif // MAINWINDOW_H
