#ifndef DIALOGALIGMENT_H
#define DIALOGALIGMENT_H

#include <QDialog>

#include "mainwindow.h"
#include "daphnesocket.h"

namespace Ui {
class DialogAligment;
}

class DialogAligment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAligment(QWidget *parent = nullptr);
    ~DialogAligment();

private slots:
    void pushButtonAlignPressed();
private:
    Ui::DialogAligment *ui;
    bool areAFEsAligned(const QVector<bool> &isAfeAligned);
    void writeAlignedLabels(const QVector<QString> &isAfeAlignedStr);
};

#endif // DIALOGALIGMENT_H
