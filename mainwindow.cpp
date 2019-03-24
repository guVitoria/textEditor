#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabBar>
#include <QColor>
#include <QGridLayout>
#include <QToolBar>
#include "xmlsyntaxhig.h"
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QSyntaxHighlighter>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QListView>
#include <QHBoxLayout>
#include <QMap>
#include "myhighlighter.h"
#include <QTableWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QListIterator>
#include <QLabel>
#include <QDockWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QModelIndex>
#include <QList>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include "linenumberarea.h"
#include <QAbstractScrollArea>
#include <QRect>
#include <QPainter>
#include <QTextBlock>
#include "textedit.h"
#include <QPushButton>
#include <QFileInfo>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    resize(1280, 960);
    /*--------------------------------------------\
   |                      Реализация меню                         |
    \--------------------------------------------*/
    setupFileActions();
    setupEditActions();
    setupViewActions();
    county = 1;

    _tab = new QTabWidget(this);
    currentTextEdit = new TextEdit(this);
    currentTextEdit->hide();
    setCentralWidget(_tab);
    setWindowTitle("Текстовый редактор");


    connect(currentTextEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(currentTextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));



    setCurrentFileName(QString());

    //lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

//    updateLineNumberAreaWidth(0);
//    highlightCurrentLine();



}

MainWindow::~MainWindow()
{
    delete layout();
}

void MainWindow::setupFileActions(){
    QMenu *file = new QMenu(tr("&Файл"));
    menuBar()->addMenu(file);//Создаем меню бар и помещаяем в него первый пункт меню file

    tb = new QToolBar(this);
    //tb->setWindowTitle(tr("Файл"));
    addToolBar(tb);

    QAction *newFile = new QAction(QIcon(":/img/prefix1/img/filenew.png"), tr("Новый файл"), this); // Создаем экшн для нашего меню
    QAction *open = new QAction(QIcon(":/img/prefix1/img/fileopen.png"), tr("Открыть"), this);
    actionSave = new QAction(QIcon(":/img/prefix1/img/filesave.png"),tr("Сохранить"), this);
    QAction *saveAs = new QAction(tr("Сохранить как"), this);
    QAction *saveAll = new QAction(tr("Сохранить все"), this);
    actionClose = new QAction(QIcon(":/img/prefix1/close") ,tr("Закрыть"), this);
    QAction *closeAll = new QAction(tr("Закрыть все"), this);
    QAction *exit = new QAction(tr("Выход"), this);
    QAction *newRash = new QAction(QIcon(":/img/prefix1/img/plus-512.png") ,tr("Новая подсветка"), this);


    file->addAction(newFile); // Привязка экшена к меню File
    tb->addAction(newFile);
    file->addSeparator();
    connect(newFile, SIGNAL(triggered()), this, SLOT(fileNew()));
    //connect(newFile, SIGNAL(triggered()), this, SLOT(onFind()));


    file->addAction(open);
    tb->addAction(open);
    file->addSeparator();
    connect(open, SIGNAL(triggered()), this, SLOT(fileOpen()));
    //connect(open, SIGNAL(triggered()), this, SLOT(onFind()));


    file->addAction(actionSave);
    tb->addAction(actionSave);
    file->addSeparator();
    connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));

    file->addAction(saveAs);
    file->addSeparator();
    connect(saveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));

    file->addAction(saveAll);
    //tb->addAction(saveAll);
    connect(saveAll, SIGNAL(triggered()), this, SLOT(fileSaveAll()));
    file->addSeparator();

    file->addAction(actionClose);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(close()));
    file->addSeparator();

    file->addAction(closeAll);
    connect(closeAll, SIGNAL(triggered()), this, SLOT(closeAll()));
    file->addSeparator();

    file->addAction(exit);
    file->addSeparator();
    connect(exit, &QAction::triggered, qApp, QApplication::quit); //Конектим выход с завершением приложения

    file->addAction(newRash);
    file->addSeparator();
    tb->addAction(newRash);
    imp = new ReashImport;
    connect(newRash, SIGNAL(triggered()), imp, SLOT(_newRash()));
}

void MainWindow::setupEditActions(){
     QMenu *edit = menuBar()->addMenu("Правка"); // Добавляем в наш меню бар дополнительный пункт edit

     actionCut = new QAction(tr("Вырезать"), this);
     actionCopy = new QAction(tr("Копировать"), this);
     actionPaste = new QAction(tr("Вставить"), this);
     actionDel = new QAction(tr("Удалить"), this);
     actionSelectAll = new QAction(tr("Выбрать все"), this);

     //Нужны ли нам горячие клавиши?
     actionCut->setShortcut(Qt::CTRL + Qt::Key_S);
     actionCopy->setShortcut(Qt::CTRL + Qt::Key_C);
     actionPaste->setShortcut(Qt::CTRL + Qt::Key_V);
     actionSelectAll->setShortcut(Qt::CTRL + Qt::Key_A);

     edit->addAction(actionCut); // Привязка экшена к меню edit
     edit->addSeparator();
     edit->addAction(actionCopy);
     edit->addSeparator();
     edit->addAction(actionPaste);
     edit->addSeparator();
     edit->addAction(actionSelectAll);
     edit->addSeparator();
     edit->addAction(actionDel);
     edit->addSeparator();

     actionUndo = new QAction(QIcon::fromTheme("edit-undo", QIcon(":/img/prefix1/img/editundo.png")),
                                                    tr("&Undo"), this);
     actionUndo->setShortcut(QKeySequence::Undo);
     tb->addAction(actionUndo);


     actionRedo = new QAction(QIcon::fromTheme("edit-redo", QIcon(":/img/prefix1/img/editredo.png")),
                                                    tr("&Redo"), this);
     actionRedo->setPriority(QAction::LowPriority);
     actionRedo->setShortcut(QKeySequence::Redo);
     tb->addAction(actionRedo);
}

void MainWindow::setupViewActions(){
    QMenu *view = menuBar()->addMenu("Вид"); // Добавляем в наш меню бар дополнительный пункт view

    QAction *observer = new QAction(QIcon(":/img/prefix1/img/folder_photos.png"), tr("Обозреватель"), this);
    QAction *conductor = new QAction(QIcon(":/img/prefix1/img/box_open.png"), tr("Проводник"), this);
    connect(conductor, SIGNAL(triggered()), this, SLOT(conductor()));
    connect(observer, SIGNAL(triggered()), this, SLOT(observer()));

    view->addAction(observer); // Привязка экшена к меню view
    view->addSeparator();
    view->addAction(conductor);
    view->addSeparator();
}

bool MainWindow::load(const QString &fileName){

    if (!QFile::exists(fileName))
        return false;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return false;



    QByteArray data = file.readAll();
    QString str;
    str = QString::fromLocal8Bit(data);
    currentTextEdit->setPlainText(str);

    setCurrentFileName(fileName);

    TextEdit *ptr = currentTextEdit;
    NewTab(ptr);
    QFileInfo inf(file);
    QString ptr_1 = inf.completeSuffix();
    LoadXML(inf.completeSuffix());


    return true;
}


bool MainWindow::maybeSave(){
    int letsCount =0;
    for(int i =0; i<_tab->count(); ++i){
        if(textEdits[i]->document()->isModified()){
            letsCount ++;
        }
    }
    if (letsCount == 0)
        return true;

    QTableWidget *table = new QTableWidget;
    table->insertColumn(0);
    table->insertColumn(1);
    QDialog dlg(this);
    dlg.setFixedSize(400, 200);
    dlg.setWindowTitle(tr("Внимание!"));

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::No);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    int rowNumb = 0;
    QFormLayout *layout = new QFormLayout();
    for(int i =0; i<_tab->count(); ++i){
        if(textEdits[i]->document()->isModified()){
            QTableWidgetItem *item = new QTableWidgetItem;
            QTableWidgetItem *item_2 = new QTableWidgetItem;

            QFileInfo *inf = new QFileInfo(_tab->tabText(i));
            item->setText(inf->fileName());
            item_2->setText(inf->filePath());
            if (item->text() == item_2->text()){
                table->insertRow(rowNumb);
                table->setItem(rowNumb, 0, item);
                item_2->setText("___nope___");
                table->setItem(rowNumb, 1, item_2);
            } else {
                table->insertRow(rowNumb);
                table->setItem(rowNumb, 0, item);
                table->setItem(rowNumb, 1, item_2);

            }
            rowNumb++;
        }
    }
    QStringList name_list;
    name_list.push_back("Название файла");
    name_list.push_back("Путь файла");
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setHorizontalHeaderLabels(name_list);

    QLabel *str = new QLabel(&dlg);
    str->setText("Некоторые документы были изменениы, сохранить их?");
    layout->addWidget(str);
    layout->addWidget(table);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    // В случае, если пользователь нажал "Ok".
    if(dlg.exec() == QDialog::Accepted) {
        for(int i =0; i<_tab->count(); ++i){
            if(textEdits[i]->document()->isModified()){
                if (_tab->tabText(i).startsWith("Новый")){
                    QString fn = QFileDialog::getSaveFileName(this, tr("Save as...") + _tab->tabText(i),
                                                              QString(), tr("All Files (*);;Py Files (*.p *.pyc *.pyo);;TXT files (*.txt);; ODT files (*.odt);;"
                                                                            "HTML-Files (*.htm *.html);;СPP-Files(*.cpp);;"
                                                                            "H-Files(*.h *.hpp);;JAVA-Files(*.JAVA);;Javascript-Files(*.JS);;"
                                                                            "Ruby-Files(*.rb);; PHP-Files(*.php);; SCALA-Files(*.SCALA);;"
                                                                            "Go-Files(*.go);; Swift-Files(*.swift);; Haskell-Files(*.hs *.lhs);;"));
//                    if (fn.isEmpty())
//                        qDebug() << "Error no file name";
                    //if (! (fn.endsWith(".txt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)
                    //      || fn.endsWith(".h", Qt::CaseInsensitive) || fn.endsWith(".cpp", Qt::CaseInsensitive) || fn.endsWith(".hpp", Qt::CaseInsensitive)) )
                     //   fn += ".txt"; // по умолчанию
                    //QTextDocumentWriter writer(fn);
                    TextEdit *trry = textEdits.value(i);
                    QFile *file = new QFile(fileName);

                    bool success = file->open(QIODevice::WriteOnly | QIODevice::Text);
                    if (!success){
                        QMessageBox::information(this, "Внимание!", "Не удалось создать файл для сохранения!");
                    } else{
                        QTextStream out(file);//поток для записи текста
                        out << trry->toPlainText(); //в зависимости от того нужна ли разметка или нет
                        textEdits[i]->document()->setModified(false);
                        _tab->setTabText(i, fileName);
                    }
                    //bool success = writer.write(trry->document());
                }
                QTextDocumentWriter writer(_tab->tabText(i));
                TextEdit *trry = textEdits[i];
                bool success = writer.write(trry->document());
            }
        }
        return true;
    } else {
        return true;
    }
}

void MainWindow::fileNew(){

    NewTab();
    currentTextEdit->clear();
    setCurrentFileName(QString());
}

void MainWindow::fileOpen(){

    QString fn = QFileDialog::getOpenFileName(this, tr("Открыть файл..."),
                                              QString(), tr("All Files (*);;Py Files (*.p *.pyc *.pyo);;TXT files (*.txt);; ODT files (*.odt);;"
                                                            "HTML-Files (*.htm *.html);;СPP-Files(*.cpp);;"
                                                            "H-Files(*.h *.hpp);;JAVA-Files(*.JAVA);;Javascript-Files(*.JS);;"
                                                            "Ruby-Files(*.rb);; PHP-Files(*.php);; SCALA-Files(*.SCALA);;"
                                                            "Go-Files(*.go);; Swift-Files(*.swift);; Haskell-Files(*.hs *.lhs);;"));

    if (!fn.isEmpty())
        load(fn);
}

bool MainWindow::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    //QTextDocumentWriter writer;
    //int c = _tab->currentIndex();
    TextEdit *trry = textEdits[_tab->currentIndex()];

//    QFileInfo inf(fileName);
//    QString ptr_1 = inf.completeSuffix();
    QFile *file = new QFile(fileName);

    bool success = file->open(QIODevice::WriteOnly | QIODevice::Text);
    if (!success){
        QMessageBox::information(this, "Внимание!", "Не удалось создать файл для сохранения!");
    } else{
        QTextStream out(file);//поток для записи текста
        out << trry->toPlainText(); //в зависимости от того нужна ли разметка или нет
        textEdits[_tab->currentIndex()]->document()->setModified(false);
        _tab->setTabText(_tab->currentIndex(), fileName);
    }


    return success;
}

bool MainWindow::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("All Files (*);;Py Files (*.p *.pyc *.pyo);;TXT files (*.txt);; ODT files (*.odt);;"
                                                            "HTML-Files (*.htm *.html);;СPP-Files(*.cpp);;"
                                                            "H-Files(*.h *.hpp);;JAVA-Files(*.JAVA);;Javascript-Files(*.JS);;"
                                                            "Ruby-Files(*.rb);; PHP-Files(*.php);; SCALA-Files(*.SCALA);;"
                                                            "Go-Files(*.go);; Swift-Files(*.swift);; Haskell-Files(*.hs *.lhs);;"));
    if (fn.isEmpty())
        return false;

    setCurrentFileName(fn);

    QFileInfo inf(fn);
    QString ptr_1 = inf.completeSuffix();
    LoadXML(inf.completeSuffix());
    //_tab->setTabText(_tab->currentIndex(), fn);
    return fileSave();
}

void MainWindow::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    currentTextEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "Безымянный.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowModified(false);
}


//Доделать
void MainWindow::fileSaveAll(){
        for(int i =0; i<_tab->count(); ++i){
            if(textEdits[i]->document()->isModified()){
                if (_tab->tabText(i).startsWith("Новый")){
                    QString fn = QFileDialog::getSaveFileName(this, tr("Save as...")+ _tab->tabText(i),
                                                              QString(), tr("All Files (*);;Py Files (*.p *.pyc *.pyo);;TXT files (*.txt);; ODT files (*.odt);;"
                                                                            "HTML-Files (*.htm *.html);;СPP-Files(*.cpp);;"
                                                                            "H-Files(*.h *.hpp);;JAVA-Files(*.JAVA);;Javascript-Files(*.JS);;"
                                                                            "Ruby-Files(*.rb);; PHP-Files(*.php);; SCALA-Files(*.SCALA);;"
                                                                            "Go-Files(*.go);; Swift-Files(*.swift);; Haskell-Files(*.hs *.lhs);;"));

                    QFileInfo inf(fn);
                    QFile *file = new QFile(fn);
                    TextEdit *trry = textEdits[i];
                    bool success = file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::NewOnly);
                    if (!success){
                        QMessageBox::information(this, "Внимание!", "Не удалось создать файл для сохранения!");
                    } else{
                        QTextStream out(file);//поток для записи текста
                        out << trry->toPlainText(); //в зависимости от того нужна ли разметка или нет
                        textEdits[_tab->currentIndex()]->document()->setModified(false);
                        _tab->setTabText(_tab->currentIndex(), fn);
                        LoadXML(inf.completeSuffix());
                    }

                } else {
                QFile *file = new QFile(_tab->tabText(i));
                TextEdit *trry = textEdits[i];
                bool success = file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::NewOnly);
                if (!success){
                    QMessageBox::information(this, "Внимание!", "Не удалось создать файл для сохранения!");
                } else{
                    QTextStream out(file);//поток для записи текста
                    out << trry->toPlainText(); //в зависимости от того нужна ли разметка или нет
                    textEdits[_tab->currentIndex()]->document()->setModified(false);
                    _tab->setTabText(_tab->currentIndex(), fileName);
                }
                }
            }
        }
}

void MainWindow::close(){
    int cur = _tab->currentIndex();
    if(textEdits[cur]->document()->isModified()){
        fileSave();
    }
    textEdits.remove(cur);
    _tab->removeTab(cur);
    _buttons.remove(cur);
    higlighters.remove(cur);
}

//Не работает
void MainWindow::closeTab(){
    int ind = 0;
    for(int i =0; i<_buttons.length(); ++i){
        if(sender() == _buttons[i]){
            ind = i;
        }
    }

    if(textEdits[ind]->document()->isModified()){
        fileSave();
    }

    textEdits.remove(ind);
    _tab->removeTab(ind);
    _buttons.remove(ind);
    higlighters.remove(ind);
}

void MainWindow::closeAll(){
    fileSaveAll();
    _tab->clear();
    textEdits.clear();
    _buttons.clear();
    higlighters.clear();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();
}



void MainWindow::NewTab(TextEdit* text){


    if(text==nullptr){
        QString filename_loc = "Новый файл " + QString::number(county);

        TextEdit *new_one = new TextEdit;
        QPushButton *cl = new QPushButton();
        _tab->addTab(new_one, filename_loc);
        int c = _tab->count();
        //_tab->setTabIcon(_tab->currentIndex(), QIcon("qrc:/img/prefix1/close"));
        //_tab->addAction(ac;
        textEdits.push_back(new_one);
        _buttons.push_back(cl);
        _tab->setCurrentWidget(textEdits.last());

        QTabBar *tab = _tab->tabBar();

        _buttons.last()->setIcon(QIcon(":/img/prefix1/close"));
        _buttons.last()->setFlat(true);
        connect(_buttons.last(), SIGNAL(clicked()), this, SLOT(closeTab()));
        tab->setTabButton(_tab->currentIndex(),  QTabBar::RightSide, cl);
        QFileInfo aa(filename_loc);
        LoadXML(aa.completeSuffix());
        county++;
    } else {
        QString *filename_loc = new QString(fileName);
        TextEdit *new_one = new TextEdit;
        QPushButton *cl = new QPushButton();
        new_one->setPlainText(text->toPlainText());
        _tab->addTab(new_one, *filename_loc);
        //_tab->setTabIcon(_tab->currentIndex(), QIcon("qrc:/img/prefix1/close"));
        //_tab->addAction(actionClose);
        textEdits.push_back(new_one);
        _buttons.push_back(cl);
        _tab->setCurrentWidget(textEdits.last());
        QTabBar *tab = _tab->tabBar();
        _buttons.last()->setIcon(QIcon(":/img/prefix1/close"));
        _buttons.last()->setFlat(true);
        connect(_buttons.last(), SIGNAL(clicked()), this, SLOT(closeTab()));
        tab->setTabButton(_tab->currentIndex(),  QTabBar::RightSide, cl);

        int c = _tab->count();


    }



        connect(textEdits.last()->document(), SIGNAL(modificationChanged(bool)),
                actionSave, SLOT(setEnabled(bool)));
        connect(textEdits.last()->document(), SIGNAL(modificationChanged(bool)),
                this, SLOT(setWindowModified(bool)));
        connect(textEdits.last()->document(), SIGNAL(undoAvailable(bool)),
                actionUndo, SLOT(setEnabled(bool)));
        connect(textEdits.last()->document(), SIGNAL(redoAvailable(bool)),
                actionRedo, SLOT(setEnabled(bool)));


        setWindowModified(textEdits.last()->document()->isModified());
        actionSave->setEnabled(textEdits.last()->document()->isModified());
        actionUndo->setEnabled(textEdits.last()->document()->isUndoAvailable());
        actionRedo->setEnabled(textEdits.last()->document()->isRedoAvailable());

        connect(actionUndo, SIGNAL(triggered()), textEdits.last(), SLOT(undo()));
        connect(actionRedo, SIGNAL(triggered()), textEdits.last(), SLOT(redo()));

        actionCut->setEnabled(false);
        actionCopy->setEnabled(false);
        actionSave->setEnabled(false);

        connect(actionCut, SIGNAL(triggered()), textEdits.last(), SLOT(cut()));
        connect(actionCopy, SIGNAL(triggered()), textEdits.last(), SLOT(copy()));
        connect(actionPaste, SIGNAL(triggered()), textEdits.last(), SLOT(paste()));
        connect(actionDel, SIGNAL(triggered()), textEdits.last(), SLOT(clear()));
        connect(actionSelectAll, SIGNAL(triggered()), textEdits.last(), SLOT(selectAll()));

        connect(textEdits.last(), SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
        connect(textEdits.last(), SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));
}

void MainWindow::conductor(){
    QDockWidget *dock = new QDockWidget(tr("Проводник"), this);
    //dock->setFixedSize(800, 600);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    model = new QFileSystemModel;
    QTreeView *treeView = new QTreeView;
    treeView->setModel(model);

    connect(treeView, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(openCond(QModelIndex)));
    model->setRootPath("C:/Users/rinte/Desktop/oop_3sem/textEdit");
    dock->setFloating(true);
    treeView->setEditTriggers( QAbstractItemView::NoEditTriggers);
    dock->setWindowIcon(QIcon(":/img/prefix1/img/folder.png"));
    dock->setWidget(treeView);
    dock->show();

}

void MainWindow::openCond(QModelIndex ind){
   QString aa =  model->filePath(ind);
   QFileInfo inf(aa);

   if(inf.isFile()){
       load(aa);
   }
}

void MainWindow::observer(){
    QDockWidget *dock = new QDockWidget(tr("Обозреватель"), this);
    //dock->setFixedSize(400, 200);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QListView *listView = new QListView;
    QList<QStandardItem*> list;

    QStandardItemModel *item_mod = new QStandardItemModel(_tab->count(), 0);

    for(int i =0; i<_tab->count(); ++i){
        QStandardItem *it = new QStandardItem(_tab->tabText(i));
        list.push_back(it);
    }

    item_mod->insertColumn(0, list);

    listView->setModel(item_mod);

    connect(listView, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(choseObs(QModelIndex)));

    dock->setFloating(true);
    dock->setWindowIcon(QIcon(":/img/prefix1/img/document.png"));
    listView->setEditTriggers( QAbstractItemView::NoEditTriggers);
    dock->setWidget(listView);
    dock->show();

}

void MainWindow::choseObs(QModelIndex ind){
    int a = ind.row();
    _tab->setCurrentWidget(textEdits[ind.row()]);
}

void MainWindow::LoadXML(QString endOfName){
    QDomDocument doc;
    QFile file("syntax.xml");
    qDebug() << doc.toString();
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

    QDomNode language = root.firstChild();
    qDebug() << language.toElement().tagName();
    while(!language.isNull()){
        if (language.toElement().tagName() == "language") {
            FindElements(language.toElement(), endOfName);
        }
        language = language.nextSibling();
    }
}

void MainWindow::FindElements(QDomElement root, QString rash){
    QDomNode node = root.firstChild();
    XMLSyntaxHig* hig = higlighters.value(_tab->currentIndex());
    if(hig == nullptr){
        if(node.toElement().tagName() == "name" ){
            QDomElement name = node.toElement();
            QString deb = name.text();
            if(name.text() == rash){
                node = node.nextSibling();
                if(node.toElement().tagName() == "regexpr" ){
                    QString regexp = node.toElement().text();
                    node = node.nextSibling();
                    if(node.toElement().tagName() == "color"){
                        QColor *col = new QColor(node.toElement().text());
                        hig = new XMLSyntaxHig(textEdits.value(_tab->currentIndex())->document(), regexp, *col);
                    }
                }
            }
        }
        higlighters.push_back(hig);
    } else {
        if(node.toElement().tagName() == "name" ){
            QDomElement name = node.toElement();
            //QString deb = name.text();
            if(name.text() == rash){
                node = node.nextSibling();
                if(node.toElement().tagName() == "regexpr" ){
                    QString regexp = node.toElement().text();
                    node = node.nextSibling();
                    if(node.toElement().tagName() == "color"){
                        QColor *col = new QColor(node.toElement().text());
                        higlighters.value(_tab->currentIndex())->ChangeRegy(regexp, *col);
                    }
                }
            }
        }
    }


}









