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
#include <QString>
#include <QList>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isPasswordVisible(false), socket(new QTcpSocket(this)){

    this->resize(700, 700);

    QWidget *centralWidget = new QWidget(this);

    QPalette palette;
    QPixmap bgPixmap("C:/Users/ThinkPad/Desktop/WhatsApp Image 2024-05-02 at 15.10.01_d38496f0.jpg");
    palette.setBrush(QPalette::Window, QBrush(bgPixmap));
    centralWidget->setAutoFillBackground(true);
    centralWidget->setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QSpacerItem *spacerTop = new QSpacerItem(0, 50, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addSpacerItem(spacerTop);

    usernameIcon = QPixmap("C:/Users/ThinkPad/Desktop/WhatsApp Image 2024-05-02 at 15.10.01_9a3b4aa9.jpg");
    QPushButton *iconButton = new QPushButton(this);
    iconButton->setIcon(QIcon(usernameIcon));
    iconButton->setIconSize(QSize(20, 20));

    QLabel *usernameLabel = new QLabel("       ", this);
    usernameEdit = new QLineEdit(this);
    usernameEdit->setMinimumWidth(200);
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addStretch();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);
    usernameLayout->addWidget(iconButton);
    usernameLayout->addStretch();

    layout->addLayout(usernameLayout);

    // Schimbă culoarea text box-urilor
    QPalette editPalette = usernameEdit->palette();
    editPalette.setColor(QPalette::Base, QColor(144, 238, 144));
    usernameEdit->setPalette(editPalette);

    QLabel *passwordLabel = new QLabel("       ", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumWidth(200);

    closedEyeIcon = QPixmap("C:/Users/ThinkPad/Desktop/WhatsApp Image 2024-05-02 at 15.10.01_cacdbf7a.jpg");
    openEyeIcon = QPixmap("C:/Users/ThinkPad/Desktop/WhatsApp Image 2024-05-02 at 15.10.01_8d855ecd.jpg");
    showHideButton = new QPushButton(this);
    showHideButton->setIcon(closedEyeIcon);

    connect(showHideButton, &QPushButton::clicked, this, &MainWindow::togglePasswordVisibility);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addStretch();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(showHideButton);
    passwordLayout->addStretch();

    layout->addLayout(passwordLayout);

    QPalette editPalette1;
    editPalette1.setColor(QPalette::Base, QColor(144, 238, 144));

    passwordEdit->setPalette(editPalette1);

    passwordLabel->setAutoFillBackground(true);
    passwordLabel->setPalette(editPalette1);

    showHideButton->setAutoFillBackground(true);
    showHideButton->setPalette(editPalette);

    loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
    layout->addWidget(loginButton, 0, Qt::AlignCenter);

    QSpacerItem *spacerBottom = new QSpacerItem(0, 50, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addSpacerItem(spacerBottom);

    setCentralWidget(centralWidget);

    usernameLabel->setAutoFillBackground(false);
    passwordLabel->setAutoFillBackground(false);

}

MainWindow::~MainWindow() {}

void MainWindow::onLoginButtonClicked() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    socket->connectToHost("25.33.200.233", 12345);

    if (!socket->waitForConnected(5000))
    {
        qDebug() << "Connection failed:" << socket->errorString();
        return;
    }

    qDebug() << "Connected to server";

    QString formattedData = QString("^#%1#%2#").arg(username).arg(password);
    QByteArray dataToSend = formattedData.toUtf8();

    socket->write(dataToSend);


    if (socket->waitForReadyRead(5000000)) {
        QByteArray receivedData = socket->readAll();
        if (!receivedData.isEmpty()) {
            char menuIndicator = receivedData[0];
            qDebug() << "Received from server:" << receivedData[0];
            setMenuByCharacter(menuIndicator);
        } else {
            qDebug() << "No data received from server";
        }
    } else {
        qDebug() << "Server did not send data or took too long";
    }
}

void MainWindow::togglePasswordVisibility() {
    if (isPasswordVisible) {
        passwordEdit->setEchoMode(QLineEdit::Password);
        isPasswordVisible = false;
        showHideButton->setIcon(closedEyeIcon);
    } else {
        passwordEdit->setEchoMode(QLineEdit::Normal);
        isPasswordVisible = true;
        showHideButton->setIcon(openEyeIcon);
    }
}

QPixmap drawTextOnImage(const QString& imagePath, QString* text, int x, int y) {
    QPixmap pixmap(imagePath);

    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::red));
    painter.setFont(QFont("Times New Roman", 16));
    painter.drawText(x, y, *text);
    painter.end();

    return pixmap;
}

bool isValidPassword(const QString& password) {
    bool hasMinLength = password.length() >= 8;
    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;

    for (const QChar& character : password) {
        if (character.isUpper()) {
            hasUppercase = true;
        } else if (character.isLower()) {
            hasLowercase = true;
        } else if (character.isDigit()) {
            hasDigit = true;
        } else {
            if (!character.isLetter() && !character.isDigit() && !character.isSpace()) {
                hasSpecialChar = true;
                break;
            }
        }
    }

    return hasMinLength && hasUppercase && hasLowercase && hasDigit && hasSpecialChar;
}



void MainWindow::setMenuByCharacter(char menuIndicator) {

    QWidget* currentCentralWidget = centralWidget();
    if (currentCentralWidget) {
        currentCentralWidget->deleteLater();
    }

    switch (menuIndicator) {

    case '0':
    {
        QWidget* newCentralWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);

        QPushButton* personalDetailsButton = new QPushButton("Afișează detalii personale", this);
        QPushButton* createReportButton = new QPushButton("Creează raport", this);
        QPushButton* resetPassword = new QPushButton("Reseteaza parola", this);
        QPushButton* statusRaport = new QPushButton("Vizualizare status raport", this);
        QPushButton* alocareMasa = new QPushButton("Vizualizare alocare masa", this);

        layout->addWidget(personalDetailsButton);
        layout->addWidget(createReportButton);
        layout->addWidget(resetPassword);
        layout->addWidget(statusRaport);
        layout->addWidget(alocareMasa);

        newCentralWidget->setLayout(layout);
        setCentralWidget(newCentralWidget);

        // Slot pentru butonul de vizualizare alocare masa
        connect(alocareMasa, &QPushButton::clicked, [this]() {
            socket->write("%");

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll();
            qDebug()<<receivedData;

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true);

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
            socket->write("@");

            if (!socket->waitForReadyRead(5000)) {
                qDebug() << "Error: Timed out waiting for server response";
                return;
            }

            QByteArray receivedData = socket->readAll();
            qDebug() << "Received from server:" << receivedData;
            QList<QByteArray> dataParts = receivedData.split('#');

            if (dataParts.isEmpty()) {
                qDebug() << "Error: Received data is empty";
                return;
            }

            bool conversionOk = false;
            qint16 num = dataParts[0].toUShort(&conversionOk);
            if (!conversionOk || num + 1 > dataParts.size()) {
                qDebug() << "Error: Invalid number of reports or dataParts size mismatch";
                return;
            }

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);
            personalDetailsWidget->setLayout(newlayout);

            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true);

            for (qint16 i = 1; i <= num; i++) {
                if (i * 2 >= dataParts.size()) {
                    qDebug() << "Error: Index out of bounds";
                    break;
                }

                qint16 x = dataParts[i * 2 - 1].toUShort(&conversionOk);
                if (!conversionOk) {
                    qDebug() << "Error: Failed to convert x from dataParts";
                    continue;
                }

                QByteArray detailPart = dataParts[i * 2];
                if (x >= 0 && x <= 2 && detailPart.size() < 3) {
                    qDebug() << "Error: Invalid detail part size for report type" << x;
                    continue;
                } else if ((x == 3 || x == 4) && detailPart.size() < 2) {
                    qDebug() << "Error: Invalid detail part size for report type" << x;
                    continue;
                }

                switch (x) {
                case 0:
                    newlayout->addWidget(new QLabel("\nRaport permisie\n", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("\nRaport invoire\n", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("\nRaport deplasare\n", this));
                    break;
                case 3:
                    newlayout->addWidget(new QLabel("\nRaport laptop\n", this));
                    break;
                case 4:
                    newlayout->addWidget(new QLabel("\nRaport descazarmare\n", this));
                    break;
                default:
                    qDebug() << "Error: Unknown report type";
                    continue;
                }

                if (x >= 0 && x <= 2) {
                    for (int j = 0; j < 3; j++) {
                        QLabel* label = nullptr;
                        switch (detailPart[j]) {
                        case '0':
                            label = new QLabel("IN ASTEPTARE", this);
                            break;
                        case '1':
                            label = new QLabel("APROBAT", this);
                            break;
                        case '2':
                            label = new QLabel("RESPINS", this);
                            break;
                        default:
                            qDebug() << "Error: Invalid status code";
                            break;
                        }
                        if (label) {
                            switch (j) {
                            case 0:
                                label->setText("Comandant batalion: " + label->text());
                                break;
                            case 1:
                                label->setText("Comandant companie: " + label->text());
                                break;
                            case 2:
                                label->setText("Indrumator de grupa: " + label->text());
                                break;
                            }
                            newlayout->addWidget(label);
                        }
                    }
                } else if (x == 3 || x == 4) {
                    for (int j = 0; j < 2; j++) {
                        QLabel* label = nullptr;
                        switch (detailPart[j]) {
                        case '0':
                            label = new QLabel("IN ASTEPTARE", this);
                            break;
                        case '1':
                            label = new QLabel("APROBAT", this);
                            break;
                        case '2':
                            label = new QLabel("RESPINS", this);
                            break;
                        default:
                            qDebug() << "Error: Invalid status code";
                            break;
                        }
                        if (label) {
                            switch (j) {
                            case 0:
                                label->setText("Comandant batalion: " + label->text());
                                break;
                            case 1:
                                label->setText("Comandant companie: " + label->text());
                                break;
                            }
                            newlayout->addWidget(label);
                        }
                    }
                }
            }

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('0');
            });
            newlayout->addWidget(backButton);

            setCentralWidget(scrollArea);
        });


        // Slot pentru butonul de afișare detalii personale
        connect(personalDetailsButton, &QPushButton::clicked, [this]() {
            socket->write("$");

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll();
            QList<QByteArray> dataParts = receivedData.split('#');

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

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('0');
            });
            newlayout->addWidget(backButton);

            personalDetailsWidget->setLayout(newlayout);
            setCentralWidget(personalDetailsWidget);

            personalDetailsWidget->setLayout(newlayout);
            setCentralWidget(personalDetailsWidget);

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

                setMenuByCharacter('0');
            });
        });

    }
        break;
    case '1':
    {
        setMenuByCharacter('0');
    }
        break;
    case '2':
    {
        QWidget* newCentralWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);

        QPushButton* personalDetailsButton = new QPushButton("Afișează detalii personale", this);
        QPushButton* createReportButton = new QPushButton("Creează raport", this);
        QPushButton* resetPassword = new QPushButton("Reseteaza parola", this);
        QPushButton* statusRaport = new QPushButton("Vizualizare status raport", this);
        QPushButton* alocareMasa = new QPushButton("Vizualizare alocare masa", this);
        QPushButton* prezentaApel = new QPushButton("Vizualizare prezenta apel", this);


        layout->addWidget(personalDetailsButton);
        layout->addWidget(createReportButton);
        layout->addWidget(resetPassword);
        layout->addWidget(statusRaport);
        layout->addWidget(alocareMasa);
        layout->addWidget(prezentaApel);

        newCentralWidget->setLayout(layout);
        setCentralWidget(newCentralWidget);

        // Slot pentru butonul de vizualizare alocare masa
        connect(alocareMasa, &QPushButton::clicked, [this]() {
            socket->write("%");

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll();
            qDebug()<<receivedData;
            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true);

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
                setMenuByCharacter('2');
            });
            newlayout->addWidget(backButton);
            newlayout->update();

            personalDetailsWidget->setLayout(newlayout);
            scrollArea->setWidget(personalDetailsWidget);
            setCentralWidget(scrollArea);

        });

        // Slot pentru butonul de vizualizare detalii raport
        connect(statusRaport, &QPushButton::clicked, [this]() {
            socket->write("@");

            if (!socket->waitForReadyRead(5000)) {
                qDebug() << "Error: Timed out waiting for server response";
                return;
            }

            QByteArray receivedData = socket->readAll();
            qDebug() << "Received from server:" << receivedData;
            QList<QByteArray> dataParts = receivedData.split('#');

            if (dataParts.isEmpty()) {
                qDebug() << "Error: Received data is empty";
                return;
            }

            bool conversionOk = false;
            qint16 num = dataParts[0].toUShort(&conversionOk);
            if (!conversionOk || num + 1 > dataParts.size()) {
                qDebug() << "Error: Invalid number of reports or dataParts size mismatch";
                return;
            }

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);
            personalDetailsWidget->setLayout(newlayout);

            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true);

            for (qint16 i = 1; i <= num; i++) {
                if (i * 2 >= dataParts.size()) {
                    qDebug() << "Error: Index out of bounds";
                    break;
                }

                qint16 x = dataParts[i * 2 - 1].toUShort(&conversionOk);
                if (!conversionOk) {
                    qDebug() << "Error: Failed to convert x from dataParts";
                    continue;
                }

                QByteArray detailPart = dataParts[i * 2];
                if (x >= 0 && x <= 2 && detailPart.size() < 3) {
                    qDebug() << "Error: Invalid detail part size for report type" << x;
                    continue;
                } else if ((x == 3 || x == 4) && detailPart.size() < 2) {
                    qDebug() << "Error: Invalid detail part size for report type" << x;
                    continue;
                }

                switch (x) {
                case 0:
                    newlayout->addWidget(new QLabel("\nRaport permisie\n", this));
                    break;
                case 1:
                    newlayout->addWidget(new QLabel("\nRaport invoire\n", this));
                    break;
                case 2:
                    newlayout->addWidget(new QLabel("\nRaport deplasare\n", this));
                    break;
                case 3:
                    newlayout->addWidget(new QLabel("\nRaport laptop\n", this));
                    break;
                case 4:
                    newlayout->addWidget(new QLabel("\nRaport descazarmare\n", this));
                    break;
                default:
                    qDebug() << "Error: Unknown report type";
                    continue;
                }

                if (x >= 0 && x <= 2) {
                    for (int j = 0; j < 3; j++) {
                        QLabel* label = nullptr;
                        switch (detailPart[j]) {
                        case '0':
                            label = new QLabel("IN ASTEPTARE", this);
                            break;
                        case '1':
                            label = new QLabel("APROBAT", this);
                            break;
                        case '2':
                            label = new QLabel("RESPINS", this);
                            break;
                        default:
                            qDebug() << "Error: Invalid status code";
                            break;
                        }
                        if (label) {
                            switch (j) {
                            case 0:
                                label->setText("Comandant batalion: " + label->text());
                                break;
                            case 1:
                                label->setText("Comandant companie: " + label->text());
                                break;
                            case 2:
                                label->setText("Indrumator de grupa: " + label->text());
                                break;
                            }
                            newlayout->addWidget(label);
                        }
                    }
                } else if (x == 3 || x == 4) {
                    for (int j = 0; j < 2; j++) {
                        QLabel* label = nullptr;
                        switch (detailPart[j]) {
                        case '0':
                            label = new QLabel("IN ASTEPTARE", this);
                            break;
                        case '1':
                            label = new QLabel("APROBAT", this);
                            break;
                        case '2':
                            label = new QLabel("RESPINS", this);
                            break;
                        default:
                            qDebug() << "Error: Invalid status code";
                            break;
                        }
                        if (label) {
                            switch (j) {
                            case 0:
                                label->setText("Comandant batalion: " + label->text());
                                break;
                            case 1:
                                label->setText("Comandant companie: " + label->text());
                                break;
                            }
                            newlayout->addWidget(label);
                        }
                    }
                }
            }

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('2');
            });
            newlayout->addWidget(backButton);

            setCentralWidget(scrollArea);
        });

        // Slot pentru butonul de afișare detalii personale
        connect(personalDetailsButton, &QPushButton::clicked, [this]() {
            socket->write("$");

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll();
            QList<QByteArray> dataParts = receivedData.split('#');

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

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('2');
            });
            newlayout->addWidget(backButton);

            personalDetailsWidget->setLayout(newlayout);
            setCentralWidget(personalDetailsWidget);

            personalDetailsWidget->setLayout(newlayout);
            setCentralWidget(personalDetailsWidget);

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
            setMenuByCharacter('7');
        });
        // Slot pentru buton resetare parola
        connect(resetPassword, &QPushButton::clicked, [this]() {
            QWidget* resetPasswordWidget = new QWidget(this);
            QVBoxLayout* resetLayout = new QVBoxLayout(resetPasswordWidget);

            QLabel* oldPasswordLabel = new QLabel("Parola veche:", this);
            QLineEdit* oldPasswordInput = new QLineEdit(this);
            oldPasswordInput->setEchoMode(QLineEdit::EchoMode::Password);

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

                QByteArray data = "*#" + oldPassword.toUtf8() + "#" + newPassword.toUtf8() + "#";
                socket->write(data);

                socket->waitForReadyRead(5000000);
                QByteArray receivedData = socket->readAll();
                if(receivedData[0]=='0')
                    QMessageBox::warning(this, "Schimbarea parolei a esuat", "Parola veche este incorecta.");
                else
                    QMessageBox::warning(this, "Schimbarea parolei a reusit", "Parola a fost schimbata cu succes.");

                oldPasswordInput->clear();
                newPasswordInput->clear();
                confirmPasswordInput->clear();

                setMenuByCharacter('2');
            });
        });

        // Slot pentru buton prezenta apel
        connect(prezentaApel, &QPushButton::clicked, [this]() {
            socket->write("W");

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll();
            qDebug() << receivedData;
            QList<QByteArray> dataParts = receivedData.split('#');

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true);

            QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);

            for (int i = 0; i < dataParts.size() - 1; i += 2) {
                QString nume = dataParts[i];
                QString prenume = dataParts[i + 1];
                qDebug() << nume << " " << i << " " << prenume << " " << (i + 1) << " \n";
                newlayout->addWidget(new QLabel(nume + " " + prenume, this));
            }

            newlayout->update();
            socket->waitForReadyRead(5000000);
            receivedData = socket->readAll();
            dataParts = receivedData.split('#');
            for (int i = 0; i < dataParts.size() - 1; i += 2) {
                QString nume = dataParts[i];
                QString prenume = dataParts[i + 1];
                qDebug() << nume << " " << i << " " << prenume << " " << (i + 1) << " \n";
                newlayout->addWidget(new QLabel(nume + " " + prenume, this));
            }

            newlayout->update();

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);

            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('2');
            });

            newlayout->addWidget(backButton);
            newlayout->update();

            personalDetailsWidget->setLayout(newlayout);
            scrollArea->setWidget(personalDetailsWidget);
            setCentralWidget(scrollArea);
        });


    }
        break;
    case '3':
    {
        QWidget* newCentralWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);

        QPushButton* resetPassword = new QPushButton("Reseteaza parola", this);
        QPushButton* vizualizareSectoare = new QPushButton("Vizualizare sectoare batalion", this);
        QPushButton* verificareSectoare = new QPushButton("Verificare sectoare batalion", this);
        QPushButton* vizualizareRapoarte = new QPushButton("Vizualizare rapoarte", this);

        layout->addWidget(resetPassword);
        layout->addWidget(vizualizareSectoare);
        layout->addWidget(verificareSectoare);
        layout->addWidget(vizualizareRapoarte);


        newCentralWidget->setLayout(layout);
        setCentralWidget(newCentralWidget);  // Setează widgetul ca central

        // Slot pentru butonul de vizualizare sectoare
        connect(vizualizareSectoare, &QPushButton::clicked, [this]() {
            socket->write("?"); // Solicită datele personale de la server

            socket->waitForReadyRead(5000000);
            QList<QPair<QString, QString>> sectorData;
            QByteArray receivedData = socket->readAll(); // Obține răspunsul de la server
            qDebug()<<"PARTEA 1\n"<<receivedData;
            QList<QByteArray> dataParts = receivedData.split('#');
            // QString pentru stocarea datelor personale
            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true); // Faceți ca QScrollArea să redimensioneze conținutul automat

            QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);

            for(qint16 i=0;i<=116;i+=3)
            {
                QString nume = dataParts[i];
                QString prenume = dataParts[i+1];
                QString sector = dataParts[i+2];
                qDebug()<<nume<<" "<< i <<" "<<prenume<< i+1 <<" "<<sector<<" " << i+2<<" \n";
                newlayout->addWidget(new QLabel(nume + " " + prenume + ": Sector " + sector + "\n", this));
                QLabel* modificareSector = new QLabel("Modificare sector:", this);
                newlayout->addWidget(modificareSector);
                sectorData.append(qMakePair(nume + " " + prenume, sector));

                QLineEdit* sectorEdit = new QLineEdit(this);
                newlayout->addWidget(sectorEdit);
            }
            socket->waitForReadyRead(5000000);
            receivedData = socket->readAll(); // Obține răspunsul de la server
            qDebug()<<"PARTEA 2\n"<<receivedData;
            dataParts = receivedData.split('#');
            for(qint16 i=0;i<=119;i+=3)
            {
                QString nume = dataParts[i];
                QString prenume = dataParts[i+1];
                QString sector = dataParts[i+2];
                qDebug()<<nume<<" "<<prenume<<" "<<sector<<" \n";
                newlayout->addWidget(new QLabel(nume + " " + prenume + ": Sector " + sector + "\n", this));
                sectorData.append(qMakePair(nume + " " + prenume, sector));

                QLineEdit* sectorEdit = new QLineEdit(this);
                newlayout->addWidget(sectorEdit);
            }

            socket->waitForReadyRead(5000000);
            receivedData = socket->readAll(); // Obține răspunsul de la server
            qDebug()<<"PARTEA 3\n"<<receivedData;
            dataParts = receivedData.split('#');
            for(qint16 i=0;i<=119;i+=3)
            {
                QString nume = dataParts[i];
                QString prenume = dataParts[i+1];
                QString sector = dataParts[i+2];
                qDebug()<<nume<<" "<<prenume<<" "<<sector<<" \n";
                newlayout->addWidget(new QLabel(nume + " " + prenume + ": Sector " + sector + "\n", this));
                sectorData.append(qMakePair(nume + " " + prenume, sector));

                QLineEdit* sectorEdit = new QLineEdit(this);
                newlayout->addWidget(sectorEdit);
            }
            newlayout->update();
            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            QPushButton* salvareModificari = new QPushButton("Salvati modificarile sectoarelor", this);

            for (const QPair<QString, QString>& sectorInfo : sectorData) {
                newlayout->addWidget(new QLabel(sectorInfo.first + ": Sector " + sectorInfo.second + "\n", this));
                QLineEdit* sectorEdit = new QLineEdit(sectorInfo.second, this); // Pre-populate QLineEdit with current sector
                newlayout->addWidget(sectorEdit);
            }
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('3');
            });

            newlayout->addWidget(backButton);
            newlayout->addWidget(salvareModificari);
            newlayout->update();

            personalDetailsWidget->setLayout(newlayout);
            scrollArea->setWidget(personalDetailsWidget);
            setCentralWidget(scrollArea);

        });

        //Slot pentru butonul de verificare sectoare
        connect(verificareSectoare, &QPushButton::clicked, [this]() {
            // Crează o nouă fereastră de derulare și un widget pentru detalii personale
            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);
            personalDetailsWidget->setLayout(newlayout);

            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true);

            // Adaugă câmpurile pentru fiecare sector și combobox-uri pentru starea lor
            QStringList sectorLabels = {"Exterior:", "Toalete:", "Hol:", "Camera:"};
            QStringList statusOptions = {"FACUT", "NEFACUT"};

            for (const QString &label : sectorLabels) {
                // Adaugă eticheta pentru sector
                newlayout->addWidget(new QLabel(label, this));

                // Adaugă un combobox pentru a alege starea
                QComboBox* comboBox = new QComboBox(this);
                comboBox->addItems(statusOptions);
                newlayout->addWidget(comboBox);
            }

            // Adaugă butoanele pentru navigare și salvare
            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            QPushButton* salvareModificari = new QPushButton("Salvati modificarile sectoarelor", this);

            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('3');
            });

            connect(salvareModificari, &QPushButton::clicked, [this, newlayout]() {
                QStringList sectorLabels = {"Exterior:", "Toalete:", "Hol:", "Camera:"};
                QStringList statusOptions = {"FACUT", "NEFACUT"};

                QString status = "!";

                // Parcurgem toate elementele din layout
                for (int i = 0; i < newlayout->count(); ++i) {
                    // Verificăm dacă elementul este un combobox
                    if (QComboBox *comboBox = qobject_cast<QComboBox*>(newlayout->itemAt(i)->widget())) {
                        // Obținem indicele corect din sectorLabels
                        int sectorIndex = i / 2;
                        if (comboBox->currentText() == "FACUT")
                            status += '1';
                        else
                            status += '0';
                    }
                }

                qDebug() << status;
                // Trimite statusul către server
                QByteArray dataToSend = status.toUtf8();
                socket->write(dataToSend);
                qDebug() << "Sent to server:" << dataToSend;
                QMessageBox::information(this, "Succes", "Modificarile au fost salvate.");
                setMenuByCharacter('3');

            });

            // Adaugă butoanele la layout
            newlayout->addWidget(backButton);
            newlayout->addWidget(salvareModificari);
            newlayout->update();

            // Setează layout-ul în widget și widget-ul în fereastra de derulare
            personalDetailsWidget->setLayout(newlayout);
            scrollArea->setWidget(personalDetailsWidget);
            setCentralWidget(scrollArea);
        });

        connect(vizualizareRapoarte, &QPushButton::clicked, [this]() {
            socket->write("Q"); // Solicită datele personale de la server

            socket->waitForReadyRead(5000000);
            QByteArray receivedData = socket->readAll(); // Obține răspunsul de la server
            qDebug()<<receivedData<<"\n";

            QList<QByteArray> dataParts = receivedData.split('#');
            qint16 nr_rapoarte0 =  dataParts[0].toUInt();
            qint16 nr_rapoarte1 =  dataParts[1].toUInt();
            qint16 nr_rapoarte2 =  dataParts[2].toUInt();
            qint16 nr_rapoarte3 =  dataParts[3].toUInt();
            qint16 nr_rapoarte4 =  dataParts[4].toUInt();

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* personalDetailsWidget = new QWidget(this);
            scrollArea->setWidget(personalDetailsWidget);
            scrollArea->setWidgetResizable(true); // Faceți ca QScrollArea să redimensioneze conținutul automat
            QVBoxLayout* newlayout = new QVBoxLayout(personalDetailsWidget);



            qint16 k=5;
            for(qint16 i=0;i<nr_rapoarte0;i++)
            {
                qDebug() << "Iterația nr_rapoarte0, indice: " << k << "\n";

                QString* user = new QString(dataParts[k]);
                QString* localitate = new QString(dataParts[k+1]);
                QString* judet = new QString(dataParts[k+2]);
                QString* data_plecare = new QString(dataParts[k+3]);
                QString* ora_plecare = new QString(dataParts[k+4]);
                QString* data_intoarcere = new QString(dataParts[k+5]);
                QString* ora_intoarcere = new QString(dataParts[k+6]);
                QString* motiv = new QString(dataParts[k+7]);
                k+=8;
                newlayout->addWidget(new QLabel("Student: " + *user, this));
                newlayout->addWidget(new QLabel("Localitate: " + *localitate, this));
                newlayout->addWidget(new QLabel("Judet: " + *judet, this));
                newlayout->addWidget(new QLabel("Data plecare: " + *data_plecare, this));
                newlayout->addWidget(new QLabel("Ora plecare: " + *ora_plecare, this));
                newlayout->addWidget(new QLabel("Data intoarcere: " + *ora_intoarcere, this));
                newlayout->addWidget(new QLabel("Motiv: " + *motiv, this));
                delete user;
                delete localitate;
                delete judet;
                delete data_plecare;
                delete ora_plecare;
                delete data_intoarcere;
                delete ora_intoarcere;
                delete motiv;
            }


            for(qint16 i=0;i<nr_rapoarte1;i++)
            {
                qDebug() << "Iterația nr_rapoarte0, indice: " << k << "\n";

                QString* user = new QString(dataParts[k]);
                QString* data = new QString(dataParts[k+1]);
                QString* ora_plecare = new QString(dataParts[k+2]);
                QString* ora_intoarcere = new QString(dataParts[k+3]);
                QString* motiv = new QString(dataParts[k+4]);
                k+=5;
                newlayout->addWidget(new QLabel("Student: " + *user, this));
                newlayout->addWidget(new QLabel("Ora plecare: " + *ora_plecare, this));
                newlayout->addWidget(new QLabel("Data intoarcere: " + *ora_intoarcere, this));
                newlayout->addWidget(new QLabel("Motiv: " + *motiv, this));
                delete user;
                delete ora_plecare;
                delete ora_intoarcere;
                delete motiv;
            }


            for(qint16 i=0;i<nr_rapoarte2;i++)
            {
                qDebug() << "Iterația nr_rapoarte0, indice: " << k << "\n";

                QString* user = new QString(dataParts[k]);
                QString* localitate = new QString(dataParts[k+1]);
                QString* tara = new QString(dataParts[k+2]);
                QString* data_plecare = new QString(dataParts[k+3]);
                QString* data_intoarcere = new QString(dataParts[k+4]);
                QString* adresa = new QString(dataParts[k+5]);
                QString* proprietar = new QString(dataParts[k+6]);
                k+=7;
                newlayout->addWidget(new QLabel("Student: " + *user, this));
                newlayout->addWidget(new QLabel("Localitate: " + *localitate, this));
                newlayout->addWidget(new QLabel("Tara: " + *tara, this));
                newlayout->addWidget(new QLabel("Data plecare: " + *data_plecare, this));
                newlayout->addWidget(new QLabel("Adresa: " + *adresa, this));
                newlayout->addWidget(new QLabel("Nume proprietar: " + *proprietar, this));
                delete user;
                delete localitate;
                delete tara;
                delete data_plecare;
                delete adresa;
                delete data_intoarcere;
                delete proprietar;
            }

            for(qint16 i=0;i<nr_rapoarte3;i++)
            {
                qDebug() << "Iterația nr_rapoarte0, indice: " << k << "\n";

                QString* user = new QString(dataParts[k]);
                QString* motiv = new QString(dataParts[k+1]);
                k+=2;
                newlayout->addWidget(new QLabel("Student: " + *user, this));
                newlayout->addWidget(new QLabel("Motiv: " + *motiv, this));
                delete user;
                delete motiv;
            }

            for(qint16 i=0;i<nr_rapoarte4;i++)
            {
                qDebug() << "Iterația nr_rapoarte0, indice: " << k << "\n";

                QString* user = new QString(dataParts[k]);
                QString* data = new QString(dataParts[k+1]);
                QString* adresa = new QString(dataParts[k+2]);
                QString* proprietar = new QString(dataParts[k+3]);
                QString* telproprietar = new QString(dataParts[k+4]);
                k+=5;
                newlayout->addWidget(new QLabel("Student: " + *user, this));
                newlayout->addWidget(new QLabel("Data inceperii descazarmarii: " + *data, this));
                newlayout->addWidget(new QLabel("Adresa: " + *adresa, this));
                newlayout->addWidget(new QLabel("Nume proprietar: " + *proprietar, this));
                newlayout->addWidget(new QLabel("Numar telefon proprietar: " + *telproprietar, this));
                delete user;
                delete data;
                delete adresa;
                delete proprietar;
                delete telproprietar;
            }

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('3');
            });


            newlayout->addWidget(backButton);
            personalDetailsWidget->setLayout(newlayout);
            scrollArea->setWidget(personalDetailsWidget);
            setCentralWidget(scrollArea);

        });


    }
        break;
    case '5':
    {
        QWidget* newCentralWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);

        QLabel* menuLabel = new QLabel("Eroare de conectare: nume utilizator sau parola incorecta.\n Va rugam incercati mai tarziu.", this);
        layout->addWidget(menuLabel);

        QPushButton* retryButton = new QPushButton("Incercati din nou mai tarziu", this);
        layout->addWidget(retryButton);

        connect(retryButton, &QPushButton::clicked, [this]() {
            return;
        });


        newCentralWidget->setLayout(layout);
        setCentralWidget(newCentralWidget);

    }
        break;
    case '8':
    {
        QWidget* newCentralWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);


        QPushButton* statisticiStudenti = new QPushButton("Statistici studenti", this);
        QPushButton* statisticiPlutoane = new QPushButton("Statistici plutoane", this);
        QPushButton* statisticiCompanie = new QPushButton("Statistici companie", this);

        layout->addWidget(statisticiStudenti);
        layout->addWidget(statisticiPlutoane);
        layout->addWidget(statisticiCompanie);


        newCentralWidget->setLayout(layout);
        setCentralWidget(newCentralWidget);

        connect(statisticiStudenti, &QPushButton::clicked, [this]() {
            socket->write("E0");

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* detailsWidget = new QWidget(this);
            QVBoxLayout* newlayout = new QVBoxLayout(detailsWidget);

            for (qint16 j = 0; j < 3; j++) {
                // Read data in a loop until we have received all the expected data
                QByteArray receivedData;
                while (socket->waitForReadyRead(5000)) {  // Adjust timeout as necessary
                    receivedData.append(socket->readAll());
                }
                qDebug() << "Received data:" << receivedData << "\n\n\n";

                QList<QByteArray> dataParts = receivedData.split('#');


                for (int i = 0; i < dataParts.size(); i += 9) {
                    if (i + 8 >= dataParts.size()) {
                        qDebug() << "Skipping incomplete data chunk";
                        continue;
                    }
                    QString username = dataParts[i];
                    QString pluton = dataParts[i + 1];
                    QString timpLogat = dataParts[i + 2];
                    QString nrChangePasswd = dataParts[i + 3];
                    QString nrRapPerm = dataParts[i + 4];
                    QString nrRapInv = dataParts[i + 5];
                    QString nrRapDepl = dataParts[i + 6];
                    QString nrRapLap = dataParts[i + 7];
                    QString nrRapDezc = dataParts[i + 8];

                    newlayout->addWidget(new QLabel("Username: " + username, this));
                    newlayout->addWidget(new QLabel("Pluton: " + pluton, this));
                    newlayout->addWidget(new QLabel("Timp logat: " + timpLogat, this));
                    newlayout->addWidget(new QLabel("Numar schimbari parola: " + nrChangePasswd, this));
                    newlayout->addWidget(new QLabel("Numar rapoarte permisiune: " + nrRapPerm, this));
                    newlayout->addWidget(new QLabel("Numar rapoarte invoire: " + nrRapInv, this));
                    newlayout->addWidget(new QLabel("Numar rapoarte deplasare: " + nrRapDepl, this));
                    newlayout->addWidget(new QLabel("Numar rapoarte lap: " + nrRapLap, this));
                    newlayout->addWidget(new QLabel("Numar rapoarte descazarmare: " + nrRapDezc, this));
                    newlayout->addWidget(new QLabel("\n", this));
                }
            }

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('8');
            });
            newlayout->addWidget(backButton);

            detailsWidget->setLayout(newlayout);
            scrollArea->setWidget(detailsWidget);
            scrollArea->setWidgetResizable(true);
            setCentralWidget(scrollArea);
        });

        connect(statisticiPlutoane, &QPushButton::clicked, [this]() {
            socket->write("E1");

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* detailsWidget = new QWidget(this);
            QVBoxLayout* newlayout = new QVBoxLayout(detailsWidget);

                QByteArray receivedData;
                while (socket->waitForReadyRead(5000)) {
                    receivedData.append(socket->readAll());
                }
                qDebug() << "Received data:" << receivedData << "\n\n\n";

                QList<QByteArray> dataParts = receivedData.split('#');

                const int fieldsPerPluton = 2;
                const int plutoaneCount = 5;

                for (int i = 0; i < dataParts.size(); i += fieldsPerPluton * plutoaneCount) {
                    if (i + (fieldsPerPluton * plutoaneCount - 1) >= dataParts.size()) {
                        qDebug() << "Skipping incomplete data chunk";
                        continue;
                    }

                    for (int k = 0; k < plutoaneCount; ++k) {
                        int baseIndex = i + k * fieldsPerPluton;
                        QString timpTotalLogat = dataParts[baseIndex];
                        QString nrRapoarte = dataParts[baseIndex + 1];
                        QString pluton = QString::number(k + 1);

                        newlayout->addWidget(new QLabel("Pluton: " + pluton, this));
                        newlayout->addWidget(new QLabel("Timp total logat: " + timpTotalLogat, this));
                        newlayout->addWidget(new QLabel("Numar rapoarte: " + nrRapoarte, this));
                        newlayout->addWidget(new QLabel("\n", this));
                    }
                }

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('8');
            });
            newlayout->addWidget(backButton);

            detailsWidget->setLayout(newlayout);
            scrollArea->setWidget(detailsWidget);
            scrollArea->setWidgetResizable(true);
            setCentralWidget(scrollArea);
        });

        connect(statisticiCompanie, &QPushButton::clicked, [this]() {
            socket->write("E2");

            QScrollArea* scrollArea = new QScrollArea(this);
            QWidget* detailsWidget = new QWidget(this);
            QVBoxLayout* newlayout = new QVBoxLayout(detailsWidget);

            QByteArray receivedData;
            while (socket->waitForReadyRead(5000)) {
                receivedData.append(socket->readAll());
            }
            qDebug() << "Received data:" << receivedData << "\n\n\n";

            QList<QByteArray> dataParts = receivedData.split('#');

            if (dataParts.size() >= 2) {
                QString timpTotal = dataParts[0];
                QString nrRapoarteTotal = dataParts[1];

                newlayout->addWidget(new QLabel("Timp total logat companie: " + timpTotal, this));
                newlayout->addWidget(new QLabel("Numar total rapoarte companie: " + nrRapoarteTotal, this));
            } else {
                qDebug() << "Incomplete data received";
            }

            QPushButton* backButton = new QPushButton("Inapoi la meniul principal", this);
            connect(backButton, &QPushButton::clicked, [this]() {
                setMenuByCharacter('8');
            });
            newlayout->addWidget(backButton);

            detailsWidget->setLayout(newlayout);
            scrollArea->setWidget(detailsWidget);
            scrollArea->setWidgetResizable(true);
            setCentralWidget(scrollArea);
        });


    }
        break;
    case '7':
    {
        bool isLaptopReportButtonPressed = false;
        bool isdeplasareReportButtonPressed = false;
        bool isdescazarmareReportButtonPressed = false;
        bool isinvoireReportButtonPressed = false;
        bool ispermisieReportButtonPressed = false;


        QWidget* newCentralWidget = new QWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(newCentralWidget);

        QLabel* menuLabel = new QLabel("Selectati tipul de raport", this);
        layout->addWidget(menuLabel);

        QPushButton* laptopReportButton = new QPushButton("Raport pentru laptop", this);
        QPushButton* deplasareReportButton = new QPushButton("Raport pentru deplasare", this);
        QPushButton* descazarmareReportButton = new QPushButton("Raport pentru descazarmare", this);
        QPushButton* invoireReportButton = new QPushButton("Raport pentru invoire", this);
        QPushButton* permisieReportButton = new QPushButton("Raport pentru permisie", this);

        layout->addWidget(invoireReportButton);
        layout->addWidget(permisieReportButton);
        layout->addWidget(deplasareReportButton);
        layout->addWidget(descazarmareReportButton);
        layout->addWidget(laptopReportButton);

        newCentralWidget->setLayout(layout);
        setCentralWidget(newCentralWidget);

        // Slot pentru raportul laptop
        connect(laptopReportButton, &QPushButton::clicked, [this, &isLaptopReportButtonPressed]() {
            isLaptopReportButtonPressed = true;
            if (isLaptopReportButtonPressed) {
                QWidget* laptopWidget = new QWidget(this);
                QVBoxLayout* layout = new QVBoxLayout(laptopWidget);

                socket->write("3");
                socket->waitForReadyRead(5000000);
                QByteArray receivedData = socket->readAll();
                QList<QByteArray> dataParts = receivedData.split('#');
                QString* nume =new QString(dataParts[0]);
                QString* prenume =new QString(dataParts[1]);
                QString* pluton = new QString(dataParts[3]);
                QString* companie = new QString(dataParts[4]);
                qDebug()<<nume<<prenume<<pluton<<companie;

                QString imagePath = "C:/Users/ThinkPad/Desktop/Raport permisiee.PNG";
                QPixmap pixmap(imagePath);
                QPainter painter(&pixmap);
                painter.setPen(QPen(Qt::red));
                painter.setFont(QFont("Times New Roman", 16));
                painter.drawText(150, 700, *nume);
                delete nume;
                painter.drawText(150, 710, *prenume);
                delete prenume;
                painter.drawText(150,770,*pluton);
                delete pluton;
                painter.drawText(150,760,*companie);
                delete companie;
                painter.end();



                QLabel* imageLabel = new QLabel(this);
                imageLabel->setPixmap(pixmap);
                layout->addWidget(imageLabel);

                QPushButton* continueButton = new QPushButton("Continuă", this);
                layout->addWidget(continueButton);


                // Slot pentru continuare și introducerea motivului pentru raportul laptop
                connect(continueButton, &QPushButton::clicked, [this]() {
                    QWidget* inputWidget = new QWidget(this);
                    QVBoxLayout* layout = new QVBoxLayout(inputWidget);

                    QLabel* reasonLabel = new QLabel("Introduceți motivul cererii pentru permisie:", this);
                    layout->addWidget(reasonLabel);

                    QTextEdit* reasonEdit = new QTextEdit(this);
                    layout->addWidget(reasonEdit);

                    QPushButton* submitButton = new QPushButton("Trimite", this);
                    layout->addWidget(submitButton);


                    // Slot pentru trimiterea raportului laptop
                    connect(submitButton, &QPushButton::clicked, [this, reasonEdit]() {
                        QString reason = reasonEdit->toPlainText();
                        if (reason.isEmpty()) {
                            QMessageBox::warning(this, "Eroare", "Motivul este obligatoriu.");
                        } else {
                            QString message = QString("&4#%1#").arg(reason);
                            QByteArray dataToSend(message.toUtf8());
                            socket->write(dataToSend);

                            QMessageBox::information(this, "Succes", "Raportul a fost trimis cu succes.");
                            setMenuByCharacter('0');
                        }
                    });

                    inputWidget->setLayout(layout);
                    setCentralWidget(inputWidget);
                });

                laptopWidget->setLayout(layout);
                setCentralWidget(laptopWidget);
            }
        });

        // Slot pentru raportul deplasare
        connect(deplasareReportButton, &QPushButton::clicked, [this, &isdeplasareReportButtonPressed]() {
            isdeplasareReportButtonPressed = true;
            if (isdeplasareReportButtonPressed) {
                QWidget* leaveWidget = new QWidget(this);
                QVBoxLayout* layout = new QVBoxLayout(leaveWidget);

                socket->write("2");
                socket->waitForReadyRead(5000000);


                QByteArray receivedData = socket->readAll();
                QList<QByteArray> dataParts = receivedData.split('#');
                QString* nume =new QString(dataParts[0]);
                QString* prenume =new QString(dataParts[1]);
                QString* telefon = new QString(dataParts[2]);
                QString* pluton = new QString(dataParts[3]);
                QString* companie = new QString(dataParts[4]);
                qDebug()<<nume<<prenume<<telefon<<pluton<<companie;

                QString imagePath = "C:/Users/ThinkPad/Desktop/Raport permisiee.PNG";
                QPixmap pixmap(imagePath);
                QPainter painter(&pixmap);
                painter.setPen(QPen(Qt::red));
                painter.setFont(QFont("Times New Roman", 16));
                painter.drawText(150, 700, *nume);
                delete nume;
                painter.drawText(150, 710, *prenume);
                delete prenume;
                painter.drawText(150, 720, *telefon);
                delete telefon;
                painter.drawText(150,770,*pluton);
                delete pluton;
                painter.drawText(150,760,*companie);
                delete companie;
                painter.end();

                QLabel* imageLabel = new QLabel(this);
                imageLabel->setPixmap(pixmap);
                layout->addWidget(imageLabel);

                QPushButton* continueButton = new QPushButton("Continuă", this);
                layout->addWidget(continueButton);

                connect(continueButton, &QPushButton::clicked, [this]() {


                    QWidget* inputWidget = new QWidget(this);
                    QVBoxLayout* layout = new QVBoxLayout(inputWidget);


                    QLabel* localitateLabel = new QLabel("Localitate:", this);
                    layout->addWidget(localitateLabel);


                    QLineEdit* localitateEdit = new QLineEdit(this);
                    layout->addWidget(localitateEdit);


                    QLabel* taraLabel = new QLabel("Tara:", this);
                    layout->addWidget(taraLabel);

                    QLineEdit* taraEdit = new QLineEdit(this);
                    layout->addWidget(taraEdit);

                    QLabel* plecareLabel = new QLabel("Data plecării:", this);
                    layout->addWidget(plecareLabel);

                    QDateEdit* plecareDateEdit = new QDateEdit(QDate::currentDate(), this);
                    layout->addWidget(plecareDateEdit);

                    QLabel* intoarcereLabel = new QLabel("Data întoarcerii:", this);
                    layout->addWidget(intoarcereLabel);

                    QDateEdit* intoarcereDateEdit = new QDateEdit(QDate::currentDate().addDays(2), this);
                    layout->addWidget(intoarcereDateEdit);

                    QLabel* adresaLabel = new QLabel("Adresa:", this);
                    layout->addWidget(adresaLabel);

                    QLineEdit* adresaEdit = new QLineEdit(this);
                    layout->addWidget(adresaEdit);

                    QLabel* proprietarLabel = new QLabel("Nume proprietar:", this);
                    layout->addWidget(proprietarLabel);

                    QLineEdit* proprietarEdit = new QLineEdit(this);
                    layout->addWidget(proprietarEdit);

                    QPushButton* submitButton = new QPushButton("Trimite", this);
                    layout->addWidget(submitButton);

                    // Slot pentru trimiterea raportului pentru deplasare
                    connect(submitButton, &QPushButton::clicked, [this,localitateEdit, taraEdit, plecareDateEdit, intoarcereDateEdit, adresaEdit, proprietarEdit]() {
                        QString localitate = localitateEdit->text();
                        QString tara = taraEdit->text();
                        QDate plecareDate = plecareDateEdit->date();
                        QDate intoarcereDate = intoarcereDateEdit->date();
                        QString adresa = adresaEdit->text();
                        QString proprietar = proprietarEdit->text();

                        if (tara.isEmpty()) {
                            QMessageBox::warning(this, "Eroare", "Tara este obligatorie.");
                        } else {
                            QString message = QString("&3#%1#%2#%3#%4#%5#%6#").arg(localitate, tara, plecareDate.toString("yyyy-MM-dd"), intoarcereDate.toString("yyyy-MM-dd"), adresa, proprietar);
                            QByteArray dataToSend(message.toUtf8());
                            socket->write(dataToSend);

                            QMessageBox::information(this, "Succes", "Raportul a fost trimis cu succes.");
                            setMenuByCharacter('0');
                        }
                    });

                    inputWidget->setLayout(layout);
                    setCentralWidget(inputWidget);
                });


                leaveWidget->setLayout(layout);
                setCentralWidget(leaveWidget);
            }
        });

        // Slot pentru raportul descazarmare
        connect(descazarmareReportButton, &QPushButton::clicked, [this, &isdescazarmareReportButtonPressed]() {
            isdescazarmareReportButtonPressed = true;
            if (isdescazarmareReportButtonPressed) {
                QWidget* descazarmareWidget = new QWidget(this);
                QVBoxLayout* layout = new QVBoxLayout(descazarmareWidget);

                socket->write("0");
                socket->waitForReadyRead(5000000);

                QByteArray receivedData = socket->readAll();
                QList<QByteArray> dataParts = receivedData.split('#');
                QString* nume =new QString(dataParts[0]);
                QString* prenume =new QString(dataParts[1]);
                QString* telefon = new QString(dataParts[2]);
                QString* pluton = new QString(dataParts[3]);
                QString* companie = new QString(dataParts[4]);
                qDebug()<<nume<<prenume<<telefon<<pluton<<companie;

                QString imagePath = "C:/Users/ThinkPad/Desktop/Raport permisiee.PNG";
                QPixmap pixmap(imagePath);
                QPainter painter(&pixmap);
                painter.setPen(QPen(Qt::red));
                painter.setFont(QFont("Times New Roman", 16));
                painter.drawText(150, 700, *nume);
                delete nume;
                painter.drawText(150, 710, *prenume);
                delete prenume;
                painter.drawText(150, 720, *telefon);
                delete telefon;
                painter.drawText(150,770,*pluton);
                delete pluton;
                painter.drawText(150,760,*companie);
                delete companie;
                painter.end();


                QLabel* imageLabel = new QLabel(this);
                imageLabel->setPixmap(pixmap);
                layout->addWidget(imageLabel);

                QPushButton* continueButton = new QPushButton("Continuă", this);
                layout->addWidget(continueButton);

                connect(continueButton, &QPushButton::clicked, [this]() {


                    QWidget* inputWidget = new QWidget(this);
                    QVBoxLayout* layout = new QVBoxLayout(inputWidget);


                    QLabel* plecareLabel = new QLabel("Data inceperii descazarmarii:", this);
                    layout->addWidget(plecareLabel);

                    QDateEdit* plecareDateEdit = new QDateEdit(QDate::currentDate(), this);
                    layout->addWidget(plecareDateEdit);

                    QLabel* adresaLabel = new QLabel("Adresa:", this);
                    layout->addWidget(adresaLabel);

                    QTextEdit* adresaEdit = new QTextEdit(this);
                    layout->addWidget(adresaEdit);

                    QLabel* proprietarLabel = new QLabel("Introduceți numele proprietarului:", this);
                    layout->addWidget(proprietarLabel);

                    QTextEdit* proprietarEdit = new QTextEdit(this);
                    layout->addWidget(proprietarEdit);

                    QLabel* proprietartelLabel = new QLabel("Introduceți numarul de telefon al proprietarului:", this);
                    layout->addWidget(proprietartelLabel);

                    QTextEdit* proprietartelEdit = new QTextEdit(this);
                    layout->addWidget(proprietartelEdit);

                    QPushButton* submitButton = new QPushButton("Trimite", this);
                    layout->addWidget(submitButton);

                    // Slot pentru trimiterea raportului pentru permisie
                    connect(submitButton, &QPushButton::clicked, [this,plecareDateEdit, adresaEdit, proprietarEdit, proprietartelEdit]() {

                        QDate plecareDate = plecareDateEdit->date();
                        QString adresa = adresaEdit->toPlainText();
                        QString proprietar = proprietarEdit->toPlainText();
                        QString proprietartel = proprietartelEdit->toPlainText();

                        if (adresa.isEmpty()) {
                            QMessageBox::warning(this, "Eroare", "Adresa este obligatorie.");
                        } else {
                            QString message = QString("&5#%1#%2#%3#%4#").arg(plecareDate.toString("yyyy-MM-dd"),adresa, proprietar, proprietartel);
                            QByteArray dataToSend(message.toUtf8());
                            socket->write(dataToSend);

                            QMessageBox::information(this, "Succes", "Raportul a fost trimis cu succes.");
                            setMenuByCharacter('0');
                        }
                    });

                    inputWidget->setLayout(layout);
                    setCentralWidget(inputWidget);
                });

                descazarmareWidget->setLayout(layout);
                setCentralWidget(descazarmareWidget);
            }
        });

        // Slot pentru raportul invoire
        connect(invoireReportButton, &QPushButton::clicked, [this, &isinvoireReportButtonPressed]() {
            isinvoireReportButtonPressed = true;
            if (isinvoireReportButtonPressed) {
                QWidget* invoireWidget = new QWidget(this);
                QVBoxLayout* layout = new QVBoxLayout(invoireWidget);

                socket->write("1");
                socket->waitForReadyRead(5000000);

                QByteArray receivedData = socket->readAll();
                QList<QByteArray> dataParts = receivedData.split('#');
                QString* nume =new QString(dataParts[0]);
                QString* prenume =new QString(dataParts[1]);
                QString* telefon = new QString(dataParts[2]);
                QString* pluton = new QString(dataParts[3]);
                QString* companie = new QString(dataParts[4]);
                qDebug()<<nume<<prenume<<telefon<<pluton<<companie;

                QString imagePath = "C:/Users/ThinkPad/Desktop/Raport laptop.PNG";
                QPixmap pixmap(imagePath);
                QPainter painter(&pixmap);
                painter.setPen(QPen(Qt::red));
                painter.setFont(QFont("Times New Roman", 16));
                painter.drawText(150, 700, *nume);
                delete nume;
                painter.drawText(150, 710, *prenume);
                delete prenume;
                painter.drawText(150, 720, *telefon);
                delete telefon;
                painter.drawText(150,770,*pluton);
                delete pluton;
                painter.drawText(150,760,*companie);
                delete companie;
                painter.end();


                QLabel* imageLabel = new QLabel(this);
                imageLabel->setPixmap(pixmap);
                layout->addWidget(imageLabel);

                QPushButton* continueButton = new QPushButton("Continuă", this);
                layout->addWidget(continueButton);

                connect(continueButton, &QPushButton::clicked, [this]() {


                    QWidget* inputWidget = new QWidget(this);
                    QVBoxLayout* layout = new QVBoxLayout(inputWidget);



                    QLabel* plecareLabel = new QLabel("Data:", this);
                    layout->addWidget(plecareLabel);

                    QDateEdit* plecareDateEdit = new QDateEdit(QDate::currentDate(), this);
                    layout->addWidget(plecareDateEdit);

                    QLabel* plecareTimeLabel = new QLabel("Ora plecării:", this);
                    layout->addWidget(plecareTimeLabel);

                    QTimeEdit* plecareTimeEdit = new QTimeEdit(QTime::currentTime(), this);
                    layout->addWidget(plecareTimeEdit);

                    QLabel* intoarcereTimeLabel = new QLabel("Ora întoarcerii:", this);
                    layout->addWidget(intoarcereTimeLabel);

                    QTimeEdit* intoarcereTimeEdit = new QTimeEdit(QTime::currentTime(), this);
                    layout->addWidget(intoarcereTimeEdit);

                    QLabel* reasonLabel = new QLabel("Introduceți motivul cererii pentru învoire:", this);
                    layout->addWidget(reasonLabel);

                    QTextEdit* reasonEdit = new QTextEdit(this);
                    layout->addWidget(reasonEdit);

                    QPushButton* submitButton = new QPushButton("Trimite", this);
                    layout->addWidget(submitButton);

                    // Slot pentru trimiterea raportului pentru permisie
                    connect(submitButton, &QPushButton::clicked, [this, plecareDateEdit, plecareTimeEdit,intoarcereTimeEdit, reasonEdit]() {
                        QDate plecareDate = plecareDateEdit->date();
                        QTime plecareTime = plecareTimeEdit->time();
                        QTime intoarcereTime = intoarcereTimeEdit->time();
                        QString reason = reasonEdit->toPlainText();

                        if (reason.isEmpty()) {
                            QMessageBox::warning(this, "Eroare", "Motivul este obligatoriu.");
                        } else {
                            QString message = QString("&2#%1#%2#%3#%4#").arg(plecareDate.toString("yyyy-MM-dd"),  plecareTime.toString("hh:mm"), intoarcereTime.toString("hh:mm"), reason);
                            QByteArray dataToSend(message.toUtf8());
                            socket->write(dataToSend);
                            qDebug()<<dataToSend;
                            QMessageBox::information(this, "Succes", "Raportul a fost trimis cu succes.");
                            setMenuByCharacter('0');
                        }
                    });

                    inputWidget->setLayout(layout);
                    setCentralWidget(inputWidget);
                });

                invoireWidget->setLayout(layout);
                setCentralWidget(invoireWidget);
            }
        });

        // Slot pentru raportul permisieee
        connect(permisieReportButton, &QPushButton::clicked, [this, &ispermisieReportButtonPressed]() {
            ispermisieReportButtonPressed = true;
            if (ispermisieReportButtonPressed) {
                QWidget* permisieWidget = new QWidget(this);
                QVBoxLayout* layout = new QVBoxLayout(permisieWidget);

                socket->write("0");
                socket->waitForReadyRead(5000000);

                QByteArray receivedData = socket->readAll();
                QList<QByteArray> dataParts = receivedData.split('#');
                QString* nume =new QString(dataParts[0]);
                QString* prenume =new QString(dataParts[1]);
                QString* telefon = new QString(dataParts[2]);
                QString* pluton = new QString(dataParts[3]);
                QString* companie = new QString(dataParts[4]);
                qDebug()<<nume<<prenume<<telefon<<pluton<<companie;

                QString imagePath = "C:/Users/ThinkPad/Desktop/Raport permisiee.PNG";
                QPixmap pixmap(imagePath);
                QPainter painter(&pixmap);
                painter.setPen(QPen(Qt::red));
                painter.setFont(QFont("Times New Roman", 16));
                painter.drawText(150, 700, *nume);
                delete nume;
                painter.drawText(150, 710, *prenume);
                delete prenume;
                painter.drawText(150, 720, *telefon);
                delete telefon;
                painter.drawText(150,770,*pluton);
                delete pluton;
                painter.drawText(150,760,*companie);
                delete companie;
                painter.end();


                QLabel* imageLabel = new QLabel(this);
                imageLabel->setPixmap(pixmap);
                layout->addWidget(imageLabel);

                QPushButton* continueButton = new QPushButton("Continuă", this);
                layout->addWidget(continueButton);

                connect(continueButton, &QPushButton::clicked, [this]() {


                    QWidget* inputWidget = new QWidget(this);
                    QVBoxLayout* layout = new QVBoxLayout(inputWidget);


                    QLabel* localitateLabel = new QLabel("Localitate:", this);
                    layout->addWidget(localitateLabel);


                    QLineEdit* localitateEdit = new QLineEdit(this);
                    layout->addWidget(localitateEdit);


                    QLabel* judetLabel = new QLabel("Judet:", this);
                    layout->addWidget(judetLabel);

                    QLineEdit* judetEdit = new QLineEdit(this);
                    layout->addWidget(judetEdit);

                    QLabel* plecareLabel = new QLabel("Data plecării:", this);
                    layout->addWidget(plecareLabel);

                    QDateEdit* plecareDateEdit = new QDateEdit(QDate::currentDate(), this);
                    layout->addWidget(plecareDateEdit);

                    QLabel* intoarcereLabel = new QLabel("Data întoarcerii:", this);
                    layout->addWidget(intoarcereLabel);

                    QDateEdit* intoarcereDateEdit = new QDateEdit(QDate::currentDate().addDays(2), this); // Set default return date 2 days from now
                    layout->addWidget(intoarcereDateEdit);

                    QLabel* plecareTimeLabel = new QLabel("Ora plecării:", this);
                    layout->addWidget(plecareTimeLabel);

                    QTimeEdit* plecareTimeEdit = new QTimeEdit(QTime::currentTime(), this);
                    layout->addWidget(plecareTimeEdit);

                    QLabel* intoarcereTimeLabel = new QLabel("Ora plecării:", this);
                    layout->addWidget(intoarcereTimeLabel);

                    QTimeEdit* intoarcereTimeEdit = new QTimeEdit(QTime::currentTime(), this);
                    layout->addWidget(intoarcereTimeEdit);

                    QLabel* reasonLabel = new QLabel("Introduceți motivul cererii pentru permisie:", this);
                    layout->addWidget(reasonLabel);

                    QTextEdit* reasonEdit = new QTextEdit(this);
                    layout->addWidget(reasonEdit);

                    QPushButton* submitButton = new QPushButton("Trimite", this);
                    layout->addWidget(submitButton);

                    // Slot pentru trimiterea raportului pentru permisie
                    connect(submitButton, &QPushButton::clicked, [this,localitateEdit, judetEdit, plecareDateEdit, plecareTimeEdit, intoarcereDateEdit,intoarcereTimeEdit, reasonEdit]() {
                        QString localitate = localitateEdit->text();
                        QString judet = judetEdit->text();
                        QDate plecareDate = plecareDateEdit->date();
                        QTime plecareTime = plecareTimeEdit->time();
                        QDate intoarcereDate = intoarcereDateEdit->date();
                        QTime intoarcereTime = intoarcereTimeEdit->time();
                        QString reason = reasonEdit->toPlainText();

                        if (reason.isEmpty()) {
                            QMessageBox::warning(this, "Eroare", "Motivul este obligatoriu.");
                        } else {
                            QString message = QString("&1#%1#%2#%3#%4#%5#%6#%7#").arg(localitate, judet, plecareDate.toString("yyyy-MM-dd"),  plecareTime.toString("hh:mm"), intoarcereDate.toString("yyyy-MM-dd"), intoarcereTime.toString("hh:mm"), reason);
                            QByteArray dataToSend(message.toUtf8());
                            socket->write(dataToSend);

                            QMessageBox::information(this, "Succes", "Raportul a fost trimis cu succes.");
                            setMenuByCharacter('0');
                        }
                    });

                    inputWidget->setLayout(layout);
                    setCentralWidget(inputWidget);
                });

                permisieWidget->setLayout(layout);
                setCentralWidget(permisieWidget);
            }
        });
    }
        break;
    default:
        QMessageBox::warning(this, "Eroare", "Cod de meniu necunoscut.");
        break;
    }
}
