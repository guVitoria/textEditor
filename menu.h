#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include "mainwindow.h"

class Menu : public QWidget
{
    Q_OBJECT

private:
    QPushButton *_open;
    QPushButton *_create_new;
public:
    MainWindow *mainWind;
    explicit Menu(QWidget *parent = nullptr);
signals:

public slots:
    void fileOpen(){
        mainWind = new MainWindow;
        mainWind->fileOpen();
        mainWind->show();
        close();
    }
    void fileNew(){
        mainWind = new MainWindow;
        mainWind->fileNew();
        mainWind->show();
        close();
    }
};

#endif // MENU_H
