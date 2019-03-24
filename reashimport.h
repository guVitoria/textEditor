#ifndef REASHIMPORT_H
#define REASHIMPORT_H

#include <QWidget>

namespace Ui {
class ReashImport;
}

class ReashImport : public QWidget
{
    Q_OBJECT

public:
    explicit ReashImport(QWidget *parent = nullptr);
    ~ReashImport();

private:
    Ui::ReashImport *ui;
private slots:
    void ImportNewToXML();
    void _newRash();
};

#endif // REASHIMPORT_H
