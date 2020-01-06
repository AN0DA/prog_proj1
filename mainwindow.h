#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QInputDialog>

#include <QSqlDatabase>
#include <QSqlError>


//Zwykłe funkcje
QString dane_ucznia(QString KOD);
void db_edit();

extern QString db_name;


//funkcje QT
namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
Q_OBJECT

public:
explicit MainWindow(QWidget *parent = nullptr);
~MainWindow();

QSqlDatabase db;



private slots:

void update_stat();
void db_open(QString name);

void on_btn_wejscie_clicked();

void on_btn_wyjscie_clicked();

void on_btn_list_clicked();

void on_actionWej_cie_triggered();

void on_actionWyj_cie_triggered();

void on_actionDodaj_list_triggered();

void on_actionOpu_apolikacj_triggered();

void on_radio_ogolne_clicked();

void on_radio_bohater_clicked();

void on_radio_klasa_clicked();

void on_actionTabela_triggered();

void on_actionGUI_triggered();

void on_actionPomoc_triggered();


void on_actionObecne_po_czenie_triggered();

void on_actionZmie_po_czenie_triggered();

private:
Ui::MainWindow *ui;

};


#endif // MAINWINDOW_H
