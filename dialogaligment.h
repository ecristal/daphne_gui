#ifndef DIALOGALIGMENT_H
#define DIALOGALIGMENT_H

#include <QDialog>

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
};

#endif // DIALOGALIGMENT_H
