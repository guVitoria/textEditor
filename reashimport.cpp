#include "reashimport.h"
#include "ui_reashimport.h"
#include <QtXml>
#include <QMessageBox>
ReashImport::ReashImport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReashImport)
{
    ui->setupUi(this);
    setWindowTitle("Добавление подсветки");
    ui->colors->addItems({"Red", "darkRed", "darkGreen",  "darkBlue", "Blue", "Green", "Cyan", "darkCyan"
                         , "magenta", "darkMagenta", "yellow", "darkYellow", "gray", "darkGray", "lightGray"});
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(ImportNewToXML()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
}

ReashImport::~ReashImport()
{
    delete ui;
}

void ReashImport::ImportNewToXML(){
        QDomDocument doc;
        QFile file("syntax.xml");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::information(this, "Внимание!", "Не удалось нати XML файл!");
            return;
        }
        else {
            if(!doc.setContent(&file)){
                QMessageBox::information(this, "Внимание!", "Не удалось подключить XML файл!");
                return;
            }
            file.close();
        }
        QDomElement root = doc.firstChildElement();

        if(root.tagName()!="syntax-highlight"){
            QMessageBox::information(this, "Внимание!", "Подключенный файл не является файлом XML подсветки!");
        }
        QDomElement root_ = doc.createElement("language");
        QDomElement name_ = doc.createElement("name");
        QDomElement regexpr_ = doc.createElement("regexpr");
        QDomElement color_ = doc.createElement("color");

        QDomText nameText = doc.createTextNode(ui->rash->text());
        QDomText regexprText = doc.createTextNode(ui->regexpr->text());
        QDomText colorText = doc.createTextNode(ui->colors->currentText());

        name_.appendChild(nameText);
        regexpr_.appendChild(regexprText);
        color_.appendChild(colorText);

        root.appendChild(root_);
        root_.appendChild(name_);
        root_.appendChild(regexpr_);
        root_.appendChild(color_);

        //QFile *fileres = new QFile("newfile.txt");
        QFile *fileres = new QFile("syntax.xml");
        bool success = fileres->open(QIODevice::WriteOnly | QIODevice::Truncate);
        if (!success){
            QMessageBox::information(this, "Внимание!", "Не удалось создать файл для сохранения!");
        } else{
            QTextStream out(fileres);//поток для записи текста
            out << doc.toDocument();
            qDebug() << doc.toString();
        }
        //file.remove();
        //fileres->rename("syntax.xml");

    }

void ReashImport::_newRash(){
    show();
}



