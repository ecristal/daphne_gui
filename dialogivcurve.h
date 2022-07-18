#ifndef DIALOGIVCURVE_H
#define DIALOGIVCURVE_H

#include <QDialog>

namespace Ui {
class DialogIVcurve;
}

class DialogIVcurve : public QDialog
{
  Q_OBJECT

public:
  explicit DialogIVcurve(QWidget *parent = 0);
  ~DialogIVcurve();

private:
  Ui::DialogIVcurve *ui;
};

#endif // DIALOGIVCURVE_H
