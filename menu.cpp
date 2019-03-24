#include "menu.h"
#include <QAction>
#include <QLayout>
#include <QIcon>
#include <QPixmap>
#include <QSizePolicy>
#include <QFont>


#include <QApplication>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    QFont *font = new QFont;
    font->setBold(true);
    font->setItalic(true);
    font->setPointSize(24);
    font->setFamily("Tahoma");

    resize(1280, 960);

    QPixmap pix_1(":/img/prefix1/img/document.png");
    QIcon ButtonIcon_1(pix_1);

    QPixmap pix_2(":/img/prefix1/img/folder.png");
    pix_2.scaled(128, 128, Qt::KeepAspectRatio);
    QIcon ButtonIcon_2(pix_2);


    _open = new QPushButton("Создать новый документ");
    _open->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    _open->setFont(*font);
    _open->setIcon(ButtonIcon_1);
    _open->setIconSize(pix_1.rect().size());
    _open->setFlat(true);
    connect(_open, SIGNAL(clicked()), this, SLOT(fileNew()));

    _create_new = new QPushButton("Открыть старый документ");
    _create_new->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    _create_new->setFont(*font);
    _create_new->setIcon(ButtonIcon_2);
    _create_new->setIconSize(pix_2.rect().size());
    _create_new->setFlat(true);
    connect(_create_new, SIGNAL(clicked()), this, SLOT(fileOpen()));

    QVBoxLayout *currentLayout = new QVBoxLayout();
    currentLayout->setContentsMargins(100,100,100,100);
    currentLayout -> addWidget(_open);
    currentLayout ->addSpacing(100);
    currentLayout ->addWidget(_create_new);
    setLayout(currentLayout);

}
