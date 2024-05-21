#ifndef MENIU_STUDENTI_H
#define MENIU_STUDENTI_H
#include "mainwindow.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStackedWidget>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QPen>
#include <QColor>
#include <QScrollArea>

void afisare_meniu_student()
{
    QWidget* currentCentralWidget = centralWidget();
    if (currentCentralWidget) {
        currentCentralWidget->deleteLater();  // Curăță resursele vechi
    }

    QWidget* newCentralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);

    QPushButton* personalDetailsButton = new QPushButton("Afișează detalii personale", this);
    QPushButton* createReportButton = new QPushButton("Creează raport", this);
    QPushButton* resetPassword = new QPushButton("Reseteaza parola", this);
    QPushButton* statusRaport = new QPushButton("Vizualizare status raport", this);
    QPushButton* alocareMasa = new QPushButton("Vizualizare alocare masa", this);
    QPushButton* vizualizareSectoare = new QPushButton("Vizualizare sectoare batalion", this);

    layout->addWidget(personalDetailsButton);
    layout->addWidget(createReportButton);
    layout->addWidget(resetPassword);
    layout->addWidget(statusRaport);
    layout->addWidget(alocareMasa);
    layout->addWidget(vizualizareSectoare);

    newCentralWidget->setLayout(layout);
    setCentralWidget(newCentralWidget);  // Setează widgetul ca central

    // Slot pentru butonul de vizualizare sectoare
    connect(vizualizareSectoare, &QPushButton::clicked, [this]() {
        socket->write("?"); // Solicită datele personale de la server

        socket->waitForReadyRead(5000000);
        QByteArray receivedData = socket->readAll(); // Obține răspunsul de la server
        qDebug()<<receivedData;
        QList<QByteArray> dataParts = receivedData.split('#');
        // QString pentru stocarea datelor personale
        QScrollArea* scrollArea = new QScrollArea(this);
        QWidget* personalDetailsWidget = new QWidget(this);
        scrollArea->setWidget(personalDetailsWidget);
        scrollArea->setWidgetResizable(true); // Faceți ca QScrollArea să redimensioneze conținutul automat

        QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);

        qint16 num = dataParts[0].toUInt();
        for(qint16 i=1;i<=num;i++)
        {
            newlayout->addWidget(new QLabel(dataParts[i] + dataParts[i+1] + dataParts[i+2] + "\n", this));
            i=i+3;
        }
        newlayout->update();
        QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
        connect(backButton, &QPushButton::clicked, [this]() {
            setMenuByCharacter('0');
        });
        newlayout->addWidget(backButton);
        newlayout->update();

        personalDetailsWidget->setLayout(newlayout);
        scrollArea->setWidget(personalDetailsWidget);
        setCentralWidget(scrollArea);

    });


    // Slot pentru butonul de vizualizare alocare masa
    connect(alocareMasa, &QPushButton::clicked, [this]() {
        socket->write("%"); // Solicită datele personale de la server

        socket->waitForReadyRead(5000000);
        QByteArray receivedData = socket->readAll(); // Obține răspunsul de la server
        qDebug()<<receivedData;
        // QString pentru stocarea datelor personale
        QScrollArea* scrollArea = new QScrollArea(this);
        QWidget* personalDetailsWidget = new QWidget(this);
        scrollArea->setWidget(personalDetailsWidget);
        scrollArea->setWidgetResizable(true); // Faceți ca QScrollArea să redimensioneze conținutul automat

        QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);
        if(receivedData[2]==0)
            newlayout->addWidget(new QLabel("Masa de dimineata: NEALOCAT\n", this));
        else
            newlayout->addWidget(new QLabel("Masa de dimineata: ALOCAT\n", this));

        if(receivedData[1]==0)
            newlayout->addWidget(new QLabel("\nMasa de pranz: NEALOCAT\n", this));
        else
            newlayout->addWidget(new QLabel("\nMasa de pranz: ALOCAT\n", this));

        if(receivedData[0]==0)
            newlayout->addWidget(new QLabel("\nMasa de seara: NEALOCAT\n", this));
        else
            newlayout->addWidget(new QLabel("\nMasa de seara: ALOCAT\n", this));


        QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
        connect(backButton, &QPushButton::clicked, [this]() {
            setMenuByCharacter('0');
        });
        newlayout->addWidget(backButton);
        newlayout->update();

        personalDetailsWidget->setLayout(newlayout);
        scrollArea->setWidget(personalDetailsWidget);
        setCentralWidget(scrollArea);

    });

    // Slot pentru butonul de vizualizare detalii raport
    connect(statusRaport, &QPushButton::clicked, [this]() {
        socket->write("@"); // Solicită datele personale de la server

        socket->waitForReadyRead(5000000);
        QByteArray receivedData = socket->readAll(); // Obține răspunsul de la server
        qDebug()<<receivedData;
        QList<QByteArray> dataParts = receivedData.split('#');
        // QString pentru stocarea datelor personale
        QScrollArea* scrollArea = new QScrollArea(this);
        QWidget* personalDetailsWidget = new QWidget(this);
        scrollArea->setWidget(personalDetailsWidget);
        scrollArea->setWidgetResizable(true); // Faceți ca QScrollArea să redimensioneze conținutul automat

        QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);

        qint16 num = dataParts[0].toUInt();
        qint16 x;
        for(qint16 i=1;i<=num;i++)
        {
            x=dataParts[i].toUInt();
            switch(x)
            {
            case 0:
            {
                newlayout->addWidget(new QLabel("\nRaport permisie\n", this));
                newlayout->update();

                switch((dataParts[i+1].toUInt())/100)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: RESPINS", this));
                    break;
                }
                switch(((dataParts[i+1].toUInt())/10)%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant companie: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant companie: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant companie: RESPINS", this));
                    break;
                }
                switch((dataParts[i+1].toUInt())%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant batalion: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant batalion: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant batalion: RESPINS", this));
                    break;
                }
            }
            break;
            case 1:
            {
                newlayout->addWidget(new QLabel("\nRaport invoire\n", this));
                newlayout->update();

                switch((dataParts[i+1].toUInt())/100)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: RESPINS", this));
                    break;
                }
                switch(((dataParts[i+1].toUInt())/10)%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant companie: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant companie: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant companie: RESPINS", this));
                    break;
                }
                switch((dataParts[i+1].toUInt())%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant batalion: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant batalion: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant batalion: RESPINS", this));
                    break;
                }
            }
            break;
            case 2:
            {
                newlayout->addWidget(new QLabel("\nRaport deplasare\n", this));

                newlayout->update();
                switch((dataParts[i+1].toUInt())/100)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: RESPINS", this));
                    break;
                }
                switch(((dataParts[i+1].toUInt())/10)%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant companie: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant companie: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant companie: RESPINS", this));
                    break;
                }
                switch((dataParts[i+1].toUInt())%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant batalion: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant batalion: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant batalion: RESPINS", this));
                    break;
                }
            }
            break;
            case 3:
            {

                newlayout->addWidget(new QLabel("\nRaport laptop\n", this));
                newlayout->update();

                switch((dataParts[i+1].toUInt())/100)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: RESPINS", this));
                    break;
                }
                switch(((dataParts[i+1].toUInt())/10)%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant companie: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant companie: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant companie: RESPINS", this));
                    break;
                }
                switch((dataParts[i+1].toUInt())%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant batalion: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant batalion: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant batalion: RESPINS", this));
                    break;
                }
            }
            break;
            case 4:
            {
                newlayout->addWidget(new QLabel("\nRaport descazarmare\n", this));
                newlayout->update();

                switch((dataParts[i+1].toUInt())/100)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Indrumator de grupa: RESPINS", this));
                    break;
                }
                switch(((dataParts[i+1].toUInt())/10)%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant companie: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant companie: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant companie: RESPINS", this));
                    break;
                }
                switch((dataParts[i+1].toUInt())%10)
                {
                case 0:
                    newlayout->addWidget(new QLabel("Comandant batalion: IN ASTEPTARE", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("Comandant batalion: APROBAT", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("Comandant batalion: RESPINS", this));
                    break;
                }
            }
            break;
            }

        }
        QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
        connect(backButton, &QPushButton::clicked, [this]() {
            setMenuByCharacter('0');
        });
        newlayout->addWidget(backButton);
        newlayout->update();

        personalDetailsWidget->setLayout(newlayout);
        scrollArea->setWidget(personalDetailsWidget);
        setCentralWidget(scrollArea);

    });

    // Slot pentru butonul de afișare detalii personale
    connect(personalDetailsButton, &QPushButton::clicked, [this]() {
        socket->write("$"); // Solicită datele personale de la server

        socket->waitForReadyRead(5000000);
        QByteArray receivedData = socket->readAll(); // Obține răspunsul de la server
        QList<QByteArray> dataParts = receivedData.split('#');
        // QString pentru stocarea datelor personale
        QString* nume = new QString(dataParts[0]);
        QString* prenume = new QString(dataParts[1]);
        QString* telefon = new QString(dataParts[2]);
        QString* grad = new QString(dataParts[3]);
        QString* sector = new QString(dataParts[4]);
        QString* pluton = new QString(dataParts[5]);
        QString* companie = new QString(dataParts[6]);
        QString* std_alerta = new QString(dataParts[7]);
        QString* tel_std_alerta = new QString(dataParts[8]);
        QString* grupa_studii = new QString(dataParts[9]);

        QWidget* personalDetailsWidget = new QWidget(this);
        QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);

        // Etichete pentru afișarea datelor personale
        newlayout->addWidget(new QLabel("Nume: " + *nume, this));
        newlayout->addWidget(new QLabel("Prenume: " + *prenume, this));
        newlayout->addWidget(new QLabel("Telefon: " + *telefon, this));
        newlayout->addWidget(new QLabel("Grad: " + *grad, this));
        newlayout->addWidget(new QLabel("Sector: " + *sector, this));
        newlayout->addWidget(new QLabel("Pluton: " + *pluton, this));
        newlayout->addWidget(new QLabel("Companie: " + *companie, this));
        newlayout->addWidget(new QLabel("Standard Alerta: " + *std_alerta, this));
        newlayout->addWidget(new QLabel("Telefon Standard Alerta: " + *tel_std_alerta, this));
        newlayout->addWidget(new QLabel("Grupa Studii: " + *grupa_studii, this));

        // ** Add back button here **
        QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
        connect(backButton, &QPushButton::clicked, [this]() {
            setMenuByCharacter('0'); // Go back to menu 0 (main menu)
        });
        newlayout->addWidget(backButton);

        personalDetailsWidget->setLayout(newlayout);
        setCentralWidget(personalDetailsWidget);

        personalDetailsWidget->setLayout(newlayout);
        setCentralWidget(personalDetailsWidget);

        // Eliberarea memoriei alocate QStringurilor
        delete nume;
        delete prenume;
        delete telefon;
        delete grad;
        delete sector;
        delete pluton;
        delete companie;
        delete std_alerta;
        delete tel_std_alerta;
        delete grupa_studii;
    });

    // Slot pentru butonul de creare raport
    connect(createReportButton, &QPushButton::clicked, [this]() {
        // Afișează meniul de selecție a raportului existent deja
        setMenuByCharacter('7');
    });
    // Slot pentru buton resetare parola
    connect(resetPassword, &QPushButton::clicked, [this]() {
        QWidget* resetPasswordWidget = new QWidget(this);
        QVBoxLayout* resetLayout = new QVBoxLayout(resetPasswordWidget);

        QLabel* oldPasswordLabel = new QLabel("Parola veche:", this);
        QLineEdit* oldPasswordInput = new QLineEdit(this);
        oldPasswordInput->setEchoMode(QLineEdit::EchoMode::Password); // Hide characters as they are typed

        QLabel* newPasswordLabel = new QLabel("Parola noua:", this);
        QLineEdit* newPasswordInput = new QLineEdit(this);
        newPasswordInput->setEchoMode(QLineEdit::EchoMode::Password);

        QLabel* confirmPasswordLabel = new QLabel("Confirma parola noua:", this);
        QLineEdit* confirmPasswordInput = new QLineEdit(this);
        confirmPasswordInput->setEchoMode(QLineEdit::EchoMode::Password);

        QPushButton* resetButton = new QPushButton("Reseteaza", this);

        resetLayout->addWidget(oldPasswordLabel);
        resetLayout->addWidget(oldPasswordInput);
        resetLayout->addWidget(newPasswordLabel);
        resetLayout->addWidget(newPasswordInput);
        resetLayout->addWidget(confirmPasswordLabel);
        resetLayout->addWidget(confirmPasswordInput);
        resetLayout->addWidget(resetButton);

        resetPasswordWidget->setLayout(resetLayout);
        setCentralWidget(resetPasswordWidget);

        connect(resetButton, &QPushButton::clicked, [this, oldPasswordInput, newPasswordInput, confirmPasswordInput]() {
            QString oldPassword = oldPasswordInput->text();
            QString newPassword = newPasswordInput->text();
            QString confirmPassword = confirmPasswordInput->text();

            if (!isValidPassword(newPassword)) {
                QMessageBox::warning(this, "Parola invalida", "Parola trebuie sa aiba minim 8 caractere, sa contina litere mari, litere mici, cifre si un caracter special.");
                return;
            }

            if (newPassword != confirmPassword) {
                QMessageBox::warning(this, "Parola invalida", "Parola si parola de confirmare nu se potrivesc.");
                return;
            }

            // Send old and new password to server separated by "#" using socket->write()
            QByteArray data = "*#" + oldPassword.toUtf8() + "#" + newPassword.toUtf8() + "#";
            socket->write(data);

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll();
            if(receivedData[0]=='0')
                QMessageBox::warning(this, "Schimbarea parolei a esuat", "Parola veche este incorecta.");
            else
                QMessageBox::warning(this, "Schimbarea parolei a reusit", "Parola a fost schimbata cu succes.");

            // Clear password fields after reset attempt
            oldPasswordInput->clear();
            newPasswordInput->clear();
            confirmPasswordInput->clear();

            // Optionally, return to main menu after successful reset
            setMenuByCharacter('0');
        });
    });

}
#endif // MENIU_STUDENTI_H
