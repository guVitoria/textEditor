#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>
#include <QFontComboBox>
#include <QTableWidget>
#include <QMap>
#include "highlighter.h"
#include "xmlsyntaxhig.h"
#include <QFileSystemModel>
#include <QDropEvent>
#include <QUrl>
#include <QtXml>
#include "reashimport.h"

class QPushButton;

class TextEdit;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

private:
    void setupFileActions();
    void setupEditActions();
    void setupViewActions();
    void setupTextActions();

    bool maybeSave();
    void setCurrentFileName(const QString &fileName);


    void NewTab(TextEdit* text = nullptr);
    void LoadXML(QString endOfName);
    void FindElements(QDomElement root, QString tagname);


private:
    QAction *actionSave,
        *actionUndo,
        *actionRedo,
        *actionCut,
        *actionCopy,
        *actionPaste,
        *actionDel,
        *actionSelectAll,
        *actionClose;

    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    QToolBar *tb;
    QString fileName;
    TextEdit *currentTextEdit;
    QVector<TextEdit*> textEdits;
    QVector<QPushButton*> _buttons;
    QTabWidget *_tab;
    //Highlighter *highlighter;
    QVector<XMLSyntaxHig*> higlighters;
    QFileSystemModel *model;
    QWidget *lineNumberArea;
    int county;

    ReashImport *imp;

protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    bool fileSave();
    bool fileSaveAs();
    void fileSaveAll();
    void close();
    void closeAll();
    void observer();
    void conductor();
    void openCond(QModelIndex);
    void choseObs(QModelIndex);
    bool load(const QString &f);
    void closeTab();



public slots:
    void fileNew();
    void fileOpen();

};

#endif // MAINWINDOW_H
