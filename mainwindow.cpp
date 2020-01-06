#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTime>

#include <QMessageBox>

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QIntValidator>




const int pojemnosc = 30;
QString db_name = "Amnesty2019";



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::db_open(db_name);

    MainWindow::on_actionGUI_triggered();

    ui->progressBar->setMaximum(pojemnosc);
    MainWindow::update_stat();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::db_open(QString name) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(name);
    if (db.open()) {
        ui->statusBar->showMessage("DB OK! [ODBC: " + name + "]");
        qDebug() << "DB OK! [ODBC: " + name + "]";
    }
    else {
        ui->statusBar->showMessage(db.lastError().text());
        qDebug() << db.lastError().text();
    }
    QApplication::restoreOverrideCursor();
}


QString dane_ucznia(QString KOD) {
    QSqlQuery subquery;

    QString dane = "ID = ";

    subquery.exec("SELECT ID_ucznia FROM [uczniowie] WHERE (kod=" + KOD + ");");
    subquery.first();
    dane.append(subquery.value(0).toString());
    dane += "\n";

    subquery.exec("SELECT imie FROM [uczniowie] WHERE (kod=" + KOD + ");");
    subquery.first();
    dane.append(subquery.value(0).toString());
    dane += " ";

    subquery.exec("SELECT nazwisko FROM [uczniowie] WHERE (kod=" + KOD + ");");
    subquery.first();
    dane.append(subquery.value(0).toString());
    dane += "\n";

    subquery.exec("SELECT klasa FROM [uczniowie] WHERE (kod=" + KOD + ");");
    subquery.first();
    dane.append(subquery.value(0).toString());
    dane += "\nKOD = " + KOD;

    return dane;
}


void MainWindow::update_stat() {
    QSqlQuery subquery;

    QString temp_string;
    int temp_int;


    //obecnie swietlica
    subquery.exec("SELECT COUNT(ID_ucznia) FROM obecnie_swietlica;");
    subquery.first();
    temp_int = subquery.value(0).toInt();
    ui->lcd_swietlica->display(temp_int);
    ui->progressBar->setValue(temp_int);
    if (temp_int > pojemnosc) {
        ui->lcd_swietlica->setStyleSheet("color: #ffffff; background-color: #cc0000");
        ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: #cc0000; width: 1px;} QProgressBar {border: 1px solid grey; border-radius: 0px; text-align: center;}");
    }
    else {
        ui->lcd_swietlica->setStyleSheet("color: #ffffff; background-color: #2d2d2d");
        ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: #06b025; width: 1px;} QProgressBar {border: 1px solid grey; border-radius: 0px; text-align: center;}");
    }


    //obecnie stołówka
    subquery.exec("SELECT COUNT(ID_ucznia) FROM obecnie_stolowka;");
    subquery.first();
    temp_int = subquery.value(0).toInt();
    ui->lcd_stolowka->display(temp_int);

    //obecnie listów
    subquery.exec("SELECT COUNT(ID_listy) FROM listy;");
    subquery.first();
    temp_int = subquery.value(0).toInt();
    ui->lcd_listy->display(temp_int);


    //staty ofiar
    QSqlQueryModel *sql_model = new QSqlQueryModel;
    sql_model->setQuery("SELECT ID_ofiary, nazwa, ilosc FROM stat_ofiara INNER JOIN ofiary ON ofiara = ID_ofiary;");
    sql_model->setHeaderData(0, Qt::Horizontal, tr("ID ofiary"));
    sql_model->setHeaderData(1, Qt::Horizontal, tr("Nazwa"));
    sql_model->setHeaderData(2, Qt::Horizontal, tr("Ilość listów"));

    ui->table_ofiary->setModel(sql_model);

    //staty klasy
    sql_model = new QSqlQueryModel;
    sql_model->setQuery("SELECT * FROM stat_klasa;");
    sql_model->setHeaderData(0, Qt::Horizontal, tr("Klasa"));
    sql_model->setHeaderData(1, Qt::Horizontal, tr("Ilość listów"));

    ui->table_klasy->setModel(sql_model);


    //ustawienie 1 ekranu stat
    ui->stacked_stat->setCurrentIndex(0);
}






void MainWindow::on_btn_wejscie_clicked() {
    int temp;
    int temp2;
    QString ID;

    QSqlQuery query;

    QIntValidator maska(111, 99999999, this);
    int pos = 0;


    query.exec("SELECT * Count(ID_ucznia) FROM obecnie_swietlica;");
    query.first();
    temp = query.value(0).toInt();
    qDebug() << "obecnie świetlica: " << temp;
    if (temp >= pojemnosc) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Świetlica pełna!", "Świetlica jest wypełniona. Wejść mimo to?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            qDebug() << "Świtlica pełna -- uczeń NIE wszedł";
        } else {
            qDebug() << "Świetlica pełna -- uczeń wszedł";
        }
    }

wczytaj_kod:
    bool ok;
    QString KOD = QInputDialog::getText(this,
                                        tr("Wejście"),
                                        tr("Podaj KOD ucznia"),
                                        QLineEdit::Normal,
                                        tr("111"),   &ok);

    if (ok && !KOD.isEmpty() && (KOD.length() == 8 || KOD == "111") && maska.validate(KOD, pos)) {
        qDebug() << "Wczytano KOD = " << KOD;
    }
    else if (ok == false)
        return;
    else
        goto wczytaj_kod;


    query.exec("SELECT kod FROM uczniowie WHERE kod = " + KOD + ";");
    query.first();
    temp = query.value(0).toInt();
    qDebug() << temp;
    if (temp == 0) {
        //dodawanie nowego ucznia
        QDialog nowy(this);
        QFormLayout form(&nowy);

        form.addRow(new QLabel("Dodaj nowego ucznia:"));

        query.exec("SELECT COUNT(ID_ucznia) FROM uczniowie");
        query.first();
        temp = query.value(0).toInt() + 1;

        QList<QLineEdit *> fields;
        QLineEdit *lineEdit = new QLineEdit(&nowy);
        lineEdit->setText(QString::number(temp));
        lineEdit->setReadOnly(true);
        QString label = "ID: ";
        form.addRow(label, lineEdit);

        lineEdit = new QLineEdit(&nowy);
        lineEdit->setObjectName("imie");
        lineEdit->setFocus();
        label = "Imię: ";
        form.addRow(label, lineEdit);

        lineEdit = new QLineEdit(&nowy);
        lineEdit->setObjectName("nazwisko");
        label = "Nazwisko: ";
        form.addRow(label, lineEdit);

        lineEdit = new QLineEdit(&nowy);
        lineEdit->setObjectName("klasa");
        label = "Klasa: ";
        form.addRow(label, lineEdit);

        lineEdit = new QLineEdit(&nowy);
        lineEdit->setObjectName("kod");
        lineEdit->setText(KOD);
        label = "KOD: ";
        form.addRow(label, lineEdit);


        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &nowy);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &nowy, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &nowy, SLOT(reject()));

        if (nowy.exec() == QDialog::Accepted) {
            if (this->findChild<QLineEdit*>("kod")->text() != KOD)
                KOD = this->findChild<QLineEdit*>("kod")->text();

            label = "INSERT INTO uczniowie (imie, nazwisko, klasa, kod) VALUES ('";
            label.append(this->findChild<QLineEdit*>("imie")->text() + "', '");
            label.append(this->findChild<QLineEdit*>("nazwisko")->text() + "', '");
            label.append(this->findChild<QLineEdit*>("klasa")->text() + "', ");
            label.append(KOD + ");");

            query.exec(label);

            ui->statusBar->showMessage("Dodano ucznia [ID = " + QString::number(temp) + " / KOD = " + KOD + "]");
        }
        else
            return;

    }

    query.exec("SELECT ID_ucznia FROM uczniowie WHERE kod = " + KOD + ";");
    query.first();
    temp = query.value(0).toInt();
    qDebug() << temp;
    ID = QString::number(temp);
    query.exec("SELECT ID_ucznia FROM frekwencja WHERE ID_ucznia = " + ID + "And godzina_wy is null;");
    query.first();
    temp2 = query.value(0).toInt();
    qDebug() << temp2;
    if (temp == temp2) {
        QMessageBox error_uczen_wszedl;
        error_uczen_wszedl.setIcon(QMessageBox::Critical);
        error_uczen_wszedl.setWindowTitle("Uczeń już wszedł!");
        error_uczen_wszedl.setText("Ten uczeń już wszedł!");
        error_uczen_wszedl.button(QMessageBox::Abort);
        error_uczen_wszedl.exec();
        return;
    }


    QMessageBox::StandardButton czy_dobry_uczen;
    czy_dobry_uczen = QMessageBox::question(this, "Czy to dobra osoba?", dane_ucznia(KOD),
                                            QMessageBox::Yes|QMessageBox::No);
    if (czy_dobry_uczen == QMessageBox::No) {
        qDebug() << "Potwierdzenie - zła osoba";
        return;
    }
    qDebug() << "Potwierdzenie - dobra osoba";

    query.exec("INSERT INTO frekwencja (ID_ucznia, godzina_we) VALUES (" + ID + ", " + QTime::currentTime().toString("hh:mm:ss") + "');");


    QMessageBox::StandardButton gdzie_pisze;
    gdzie_pisze = QMessageBox::question(this, "Świetlica / stołówka", "W świetlicy czy na stołówce? [TAK - świetlica / NIE - stołówka]",
                                        QMessageBox::Yes|QMessageBox::No);
    if (gdzie_pisze == QMessageBox::Yes) {
        query.exec("INSERT INTO obecnie_swietlica (ID_ucznia) VALUES (" + ID + ");");
        ///ATUALIZACJA LICZBY OBECNYCH
        ui->statusBar->showMessage("Uczeń [ID = " + ID + " / KOD =" + KOD +  "] wszedł do świetlicy");
    }
    else {
        query.exec("INSERT INTO obecnie_swietlica (ID_ucznia) VALUES (" + ID + ");");
        ui->statusBar->showMessage("Uczeń [ID = " + ID + " / KOD =" + KOD +  "] wszedł na stołówkę");
    }

    MainWindow::update_stat();
}


void MainWindow::on_btn_wyjscie_clicked() {
    int temp;
    QString temp2;
    QString ID;

    int pos = 0;
    QIntValidator maska (111, 99999999, this);

    QSqlQuery query;

wczytaj_kod:
    bool ok;
    QString KOD = QInputDialog::getText(this,
                                        tr("Wyjście"),
                                        tr("Podaj KOD ucznia"),
                                        QLineEdit::Normal,
                                        tr("111"),   &ok);

    if (ok && !KOD.isEmpty() && (KOD.length() == 8 || KOD == "111")  && maska.validate(KOD, pos)) {
        qDebug() << "Wczytano KOD = " << KOD;
    }
    else if (ok == false)
        return;
    else
        goto wczytaj_kod;


    query.exec("SELECT kod FROM uczniowie WHERE kod = " + KOD + ";");
    query.first();
    temp = query.value(0).toInt();
    qDebug() << temp;
    if (temp == 0) {
        QMessageBox error_uczen_wszedl;
        error_uczen_wszedl.setIcon(QMessageBox::Critical);
        error_uczen_wszedl.setWindowTitle("Brak rekordu!");
        error_uczen_wszedl.setText("Brak rekordu dla KODU = " + KOD + "!");
        error_uczen_wszedl.button(QMessageBox::Abort);
        error_uczen_wszedl.exec();
        return;
    }

    QMessageBox::StandardButton czy_dobry_uczen;
    czy_dobry_uczen = QMessageBox::question(this, "Czy to dobra osoba?", dane_ucznia(KOD),
                                            QMessageBox::Yes|QMessageBox::No);
    if (czy_dobry_uczen == QMessageBox::No) {
        qDebug() << "Potwierdzenie - zła osoba";
        return;
    }
    qDebug() << "Potwierdzenie - dobra osoba";

    query.exec("SELECT ID_frekwencja FROM frekwencja WHERE ID_ucznia = " + ID + "And godzina_wy is null");
    query.first();
    temp2 = query.value(0).toString();

    query.exec("UPDATE frekwencja SET godzina_wy = '" + QTime::currentTime().toString("hh:mm:ss") + "'  WHERE ID_frekwencja = " + temp2 + ";");

    ui->statusBar->showMessage("Uczeń [ID = " + ID + " / KOD =" + KOD +  "] wyszedł o " + QTime::currentTime().toString("hh:mm:ss"));

    query.exec("SELECT ID_ucznia FROM obecnie_swietlica WHERE ID_ucznia = " + ID + ";");
    query.first();
    temp = query.value(0).toInt();
    if (temp == ID)
        query.exec("DELETE FROM obecnie_swietlica WHERE ID_ucznia = " + ID + ";");
    else
        query.exec("DELETE FROM obecnie_stolowka WHERE ID_ucznia = " + ID + ";");


    MainWindow::update_stat();
}


void MainWindow::on_btn_list_clicked() {
    int temp;
    QString temp2;
    QString ID;

    QSqlQuery query;

    int pos = 0;
    QIntValidator maska (111, 99999999, this);
    QIntValidator maska_ofiara(1, 10, this);

    bool ok;
wczytaj_kod:
    QString KOD = QInputDialog::getText(this,
                                        tr("Wyjście"),
                                        tr("Podaj KOD ucznia"),
                                        QLineEdit::Normal,
                                        tr("111"),   &ok);

    if (ok && !KOD.isEmpty() && (KOD.length() == 8 || KOD == "111")  && maska.validate(KOD, pos)) {
        qDebug() << "Wczytano KOD = " << KOD;
    }
    else if (ok == false)
        return;
    else
        goto wczytaj_kod;


    query.exec("SELECT kod FROM uczniowie WHERE kod = " + KOD + ";");
    query.first();
    temp = query.value(0).toInt();
    qDebug() << temp;
    if (temp == 0) {
        QMessageBox error_uczen_wszedl;
        error_uczen_wszedl.setIcon(QMessageBox::Critical);
        error_uczen_wszedl.setWindowTitle("Brak rekordu!");
        error_uczen_wszedl.setText("Brak rekordu dla KODU = " + KOD + "!");
        error_uczen_wszedl.button(QMessageBox::Abort);
        error_uczen_wszedl.exec();
        return;
    }

wczytaj_ofiare:
    QString ofiara = QInputDialog::getText(this,
                                           tr("Dodaj list"),
                                           tr("Podaj ID bohatera"),
                                           QLineEdit::Normal,

                                           tr(""),   &ok);

    if (ok && !ofiara.isEmpty()  && maska_ofiara.validate(KOD, pos)) {
        qDebug() << "Wczytano ofiarę ID = " << ofiara;
    }
    else if (ok == false)
        return;
    else
        goto wczytaj_ofiare;

    query.exec("SELECT ID_ofiary FROM ofiary WHERE ID_ofiary = " + ofiara + ";");
    query.first();
    temp = query.value(0).toInt();
    qDebug() << temp;
    if (temp == 0) {
        QMessageBox error_uczen_wszedl;
        error_uczen_wszedl.setIcon(QMessageBox::Critical);
        error_uczen_wszedl.setWindowTitle("Brak rekordu!");
        error_uczen_wszedl.setText("Brak ofiary o ID = " + ofiara + "!");
        error_uczen_wszedl.button(QMessageBox::Abort);
        error_uczen_wszedl.exec();
        return;
    }

    query.exec("SELECT nazwa FROM ofiary WHERE ID_ofiary = " + ofiara);
    query.first();
    temp2 = dane_ucznia(KOD) + "\n\nBohater:\nID = " + ofiara + "\n" + query.value(0).toString();

    QMessageBox::StandardButton czy_dobry_uczen_ofiara;
    czy_dobry_uczen_ofiara = QMessageBox::question(this, "Czy to dobra osoba i bohater?", temp2,
                                                   QMessageBox::Yes|QMessageBox::No);
    if (czy_dobry_uczen_ofiara == QMessageBox::No) {
        qDebug() << "Potwierdzenie - zła osoba";
        return;
    }
    qDebug() << "Potwierdzenie - dobra osoba";

wczytaj_ilosc:
    int ilosc = QInputDialog::getInt(this,
                                     tr("Dodaj list"),
                                     tr("Ile listów?"),
                                     1, 1, 100, 1,   &ok);

    if (ok && !KOD.isEmpty()) {
        qDebug() << "Doda się " << ilosc << " listow";
    }
    else if (ok == false)
        return;
    else
        goto wczytaj_ilosc;

    ui->statusBar->showMessage("Uczeń [ID = " + ID + " / KOD =" + KOD +  "] dodał " + QString::number(ilosc) + "list(ów) dla ofiary ID = " + ofiara);

    query.exec("SELECT ID_ucznia FROM uczniowie WHERE kod = " + KOD + ";");
    query.first();
    temp2 = query.value(0).toString();

    for (int i = 0; i < ilosc; i++)
        query.exec("INSERT INTO listy (ID_ucznia, ID_ofiary, godzina) VALUES ('" + temp2 + "','" + ofiara + "','" + QTime::currentTime().toString("hh:mm:ss") + "');");


    query.exec("DELETE FROM stat_klasa");
    query.exec("INSERT INTO stat_klasa ( klasa , ilosc) SELECT uczniowie.klasa, Count(listy.ID_listy) FROM listy INNER JOIN uczniowie ON listy.ID_ucznia = uczniowie.ID_ucznia GROUP BY uczniowie.klasa ORDER BY Count(listy.ID_listy) DESC;");

    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 1) WHERE ofiara = 1;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 2) WHERE ofiara = 2;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 3) WHERE ofiara = 3;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 4) WHERE ofiara = 4;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 5) WHERE ofiara = 5;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 6) WHERE ofiara = 6;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 7) WHERE ofiara = 7;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 8) WHERE ofiara = 8;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 9) WHERE ofiara = 9;");
    query.exec("UPDATE stat_ofiara SET ilosc = (SELECT  COUNT(ID_listy) FROM listy WHERE ID_ofiary = 10) WHERE ofiara = 10;");


    MainWindow::update_stat();
}

void MainWindow::on_actionWej_cie_triggered()
{
    ui->lcd_swietlica->setStyleSheet("color: #ffffff; background-color: #cc0000");
    ui->progressBar->setStyleSheet("QProgressBar::chunk {background-color: #cc0000; width: 1px;} QProgressBar {border: 1px solid grey; border-radius: 0px; text-align: center;}");
    //MainWindow::on_btn_wejscie_clicked();
}

void MainWindow::on_actionWyj_cie_triggered()
{
    MainWindow::on_btn_wyjscie_clicked();
}

void MainWindow::on_actionDodaj_list_triggered()
{
    MainWindow::on_btn_list_clicked();
}

void MainWindow::on_actionOpu_apolikacj_triggered()
{
    QMessageBox::StandardButton czy_exit;
    czy_exit = QMessageBox::question(this, "Wyjść?", "Czy na pewno chcesz zamknąć to okno?",
                                     QMessageBox::Yes|QMessageBox::No);
    if (czy_exit == QMessageBox::Yes)
        QApplication::quit();
    else
        return;
}

void MainWindow::on_radio_ogolne_clicked() {
    ui->stacked_stat->setCurrentIndex(0);
}

void MainWindow::on_radio_bohater_clicked() {
    ui->stacked_stat->setCurrentIndex(1);
}

void MainWindow::on_radio_klasa_clicked() {
    ui->stacked_stat->setCurrentIndex(2);
}


void MainWindow::on_actionTabela_triggered() {

    ui->actionGUI->setChecked(false);


    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("uczniowie");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID ucznia"));
    model->setHeaderData(1, Qt::Horizontal, tr("Imię"));
    model->setHeaderData(2, Qt::Horizontal, tr("Nazwisko"));
    model->setHeaderData(3, Qt::Horizontal, tr("Klasa"));
    model->setHeaderData(4, Qt::Horizontal, tr("KOD"));
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->show();

    model = new QSqlTableModel;
    model->setTable("frekwencja");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID frekwencji"));
    model->setHeaderData(1, Qt::Horizontal, tr("ID ucznia"));
    model->setHeaderData(2, Qt::Horizontal, tr("Godzina wejścia"));
    model->setHeaderData(3, Qt::Horizontal, tr("Godzina wyjścia"));
    ui->tableView_2->setModel(model);
    ui->tableView_2->show();

    model = new QSqlTableModel;
    model->setTable("listy");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID listu"));
    model->setHeaderData(1, Qt::Horizontal, tr("ID ucznia"));
    model->setHeaderData(2, Qt::Horizontal, tr("ID ofiary"));
    model->setHeaderData(3, Qt::Horizontal, tr("Godzina"));
    ui->tableView_3->setModel(model);
    ui->tableView_3->show();

    model = new QSqlTableModel;
    model->setTable("obecnie_swietlica");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("ID ucznia"));
    ui->tableView_4->setModel(model);
    ui->tableView_4->hideColumn(0);
    ui->tableView_4->show();

    model = new QSqlTableModel;
    model->setTable("obecnie_stolowka");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("ID ucznia"));
    ui->tableView_5->setModel(model);
    ui->tableView_5->hideColumn(0);
    ui->tableView_5->show();

    model = new QSqlTableModel;
    model->setTable("stat_klasa");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Klasa"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ilość listów"));
    ui->tableView_6->setModel(model);
    ui->tableView_6->show();

    model = new QSqlTableModel;
    model->setTable("stat_ofiara");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Ofiara"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ilość listów"));
    ui->tableView_7->setModel(model);
    ui->tableView_7->show();

    model = new QSqlTableModel;
    model->setTable("ofiary");
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID ofiary"));
    model->setHeaderData(0, Qt::Horizontal, tr("Nazwa"));
    ui->tableView_8->setModel(model);
    ui->tableView_8->show();

    ui->tab_db->setCurrentIndex(1);

    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    this->setMinimumSize(100, 100);

    ui->stacked_global->setCurrentIndex(1);
}

void MainWindow::on_actionGUI_triggered()
{
    ui->actionTabela->setChecked(false);

    MainWindow::update_stat();

    ui->stacked_global->setCurrentIndex(0);


    this->setFixedSize(730, 420);
}


void MainWindow::on_actionPomoc_triggered() {
    QMessageBox db_pomoc;
    db_pomoc.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    db_pomoc.setWindowTitle("Pomoc");
    db_pomoc.setText("Żeby połączyć się z bazą danych należy poprawnie skonfigurować połączenie ODBC.\n\n\nNazwa: Amnesty2019 (można ją zmienić w Baza danych->Zmień połączenie)\n\nLogin: Amnesty2019 [Autoryzacja SQL Server]\nHasło: Amnesty2019\n\nDomyślna baza danych: Amnesty2019");
    db_pomoc.button(QMessageBox::Ok);
    db_pomoc.exec();
}

void MainWindow::on_actionObecne_po_czenie_triggered() {
    QMessageBox db_obecna_nazwa;
    db_obecna_nazwa.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    db_obecna_nazwa.setWindowTitle("ODBC - obecne połączenie");
    db_obecna_nazwa.setText("Obecne połączenie ODBC: " + db_name);
    db_obecna_nazwa.button(QMessageBox::Ok);
    db_obecna_nazwa.exec();
}

void MainWindow::on_actionZmie_po_czenie_triggered() {
    bool ok;
zmiana:
    QString db_new_name = QInputDialog::getText(this,
                                                tr("ODBC - zmień połączenie"),
                                                tr("Podaj nową nazwę połączenia ODBC z bazą SQL Server"),
                                                QLineEdit::Normal,
                                                db_name,   &ok);

    if (ok && !db_new_name.isEmpty()) {
        db_name = db_new_name;
        ui->statusBar->showMessage("Zmiana połączenia ODBC [ODBC = " + db_name + "]");
        db_open(db_name);
    }
    else if (ok == false)
        return;
    else
        goto zmiana;

    if (ui->stacked_global->currentIndex() == 0)
        MainWindow::update_stat();
    else
        MainWindow::on_actionTabela_triggered();
}

void db_edit() {
    QSqlQuery query;

    QString nazwisko[250] = {"Ostojski", "Jóźwiak", "Cichosz", "Kiełkowski", "Przytuła", "Jeżak", "Chyła", "Drozdowski", "Ławicki", "Maciak", "Knitter", "Krzyżaniak", "Smolarczyk", "Dolna", "Kucia", "Choromański", "Soćko", "Soja", "Leszek", "Pociecha", "Karliński", "Burski", "Jabłonowski", "Jopek", "Kawalec", "Skowronek", "Depta", "Mostowski", "Antos", "Rekowski", "Grzywna", "Matusz", "Bal", "Jamrozik", "Konefał", "Żygadło", "Wasilewski", "Siwik", "Makulski", "Żaczek", "Turkowski", "Łubiński", "Nieć", "Rybus", "Cuber", "Rękawek", "Gwiazda", "Pietraszko", "Antosiewicz", "Staszewski", "Rybacki", "Fic", "Kutyła", "Boguszewski", "Łuszcz", "Zdanowski", "Piszcz", "Skrzypczak", "Wołowski", "Matys", "Kuźniar", "Konarzewski", "Skarżyński", "Podlewski", "Opolski", "Topolski", "Troszczyński", "Sitkiewicz", "Łebek", "Rożek", "Wasiak", "Kielar", "Lachowicz", "Król", "Kożuch", "Wrześniewski", "Antoniewicz", "Kierzek", "Dubiel", "Pyra", "Zemła", "Zoń", "Wardak", "Rębisz", "Dłużniewski", "Stepaniuk", "Korzec", "Rolka", "Kieliszek", "Majchrowicz", "Drapała", "Podsiadło", "Żyliński", "Ścigała", "Kitowski", "Ewertowski", "Chmielewski", "Deska", "Kościelniak", "Zajdel", "Lis", "Kempa", "Skalski", "Kruk", "Kotlarski", "Walicki", "Bućko", "Szybiak", "Mikrut", "Bujok", "Mocarski", "Sobczuk", "Landowski", "Fatyga", "Nosek", "Kozielski", "Jankowiak", "Kozubek", "Kupczak", "Dróżdż", "Rucki", "Sobiesiak", "Kot", "Szczepankiewicz", "Świeboda", "Napierała", "Ćwiek", "Danielak", "Makarewicz", "Niedźwiedzki", "Stefanowski", "Nasiłowski", "Środa", "Radzki", "Ławniczak", "Oracz", "Makuch", "Jurecki", "Furman", "Kryger", "Borczyk", "Stasiak", "Brożyna", "Stępiński", "Nędza", "Dobek", "Flisikowski", "Rosa", "Popek", "Nowek", "Gwiżdż", "Dudka", "Pyka", "Stanek", "Zabrocki", "Maroń", "Zambrzycki", "Jabłoński", "Beczek", "Szczęsny", "Rządkowski", "Dobkowski", "Jędrusik", "Górny", "Borowik", "Klim", "Dziedzic", "Bronk", "Przybyłek", "Sławiński", "Frańczak", "Budek", "Stasik", "Leśniowski", "Bartnicki", "Ignaciuk", "Maćkiewicz", "Łoboda", "Skrzyniarz", "Frank", "Jałocha", "Rynkowski", "Narkiewicz", "Szlęzak", "Przybyłowski", "Marciniec", "Reda", "Fabiański", "Marciszewski", "Klusek", "Grzywacz", "Kwapisz", "Łazarczyk", "Komarnicki", "Możdżeń", "Pączek", "Ciesielski", "Lisiak", "Wika", "Sobótka", "Adamski", "Bargieł", "Michałkiewicz", "Dziuk", "Czerwik", "Białczak", "Kusz", "Nowosad", "Kurzawski", "Pytka", "Ruszczak", "Berent", "Fila", "Polański", "Stachura", "Pomorski", "Sławek", "Matelski", "Sumiński", "Jażdżewski", "Iwan", "Grot", "Kuta", "Bielecki", "Pietryka", "Bochniak", "Spychalski", "Czubak", "Natkaniec", "Gierczak", "Wojtowicz", "Rusinek", "Owczarczyk", "Tkacz", "Paszkiewicz", "Łuczak", "Flak", "Kasprzyk", "Lasoń", "Pytlak", "Majcherek", "Matwiejczuk", "Barski", "Michalik", "Piotrowicz", "Pożoga", "Karolak", "Bem", "Seliga", "Buczkowski"};

    for (int i = 0; i < 250; i++) {
        char litera = 'A';
        litera += rand()%5;
        char cyfra = '1';
        cyfra += rand()%4;

        query.exec("update uczniowie set nazwisko = '" + nazwisko[i] + "' where ID_ucznia = " + QString::number(i));
        query.exec("update uczniowie set klasa = '" + QString(QChar::fromLatin1(cyfra)) + QString(QChar::fromLatin1(litera)) + "' where ID_ucznia = " + QString::number(i));
    }
}
