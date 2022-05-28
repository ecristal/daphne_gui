#ifndef DIALOGACQUISITIONCONFIGURATION_H
#define DIALOGACQUISITIONCONFIGURATION_H

#include <QDialog>

namespace Ui {
class DialogAcquisitionConfiguration;
}

class DialogAcquisitionConfiguration : public QDialog
{
  Q_OBJECT

public:
  explicit DialogAcquisitionConfiguration(QWidget *parent = 0);
  ~DialogAcquisitionConfiguration();

private:
  Ui::DialogAcquisitionConfiguration *ui;
};

#endif // DIALOGACQUISITIONCONFIGURATION_H
