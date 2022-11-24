#ifndef DIALOGAFECONFIGURATION_H
#define DIALOGAFECONFIGURATION_H

#include <QDialog>

namespace Ui {
class DialogAFEConfiguration;
}

class DialogAFEConfiguration : public QDialog
{
  Q_OBJECT

public:
  explicit DialogAFEConfiguration(QWidget *parent = 0);
  ~DialogAFEConfiguration();

  bool getCheckBoxHPFEnabled();
  bool getCheckBoxLFNSupressorEnabled();
  uint16_t getSpinBoxHPFKValue();
  void setCheckBoxHPFEnabled(const bool &enabled);
  void setCheckBoxLFNSupressorEnabled(const bool &enabled);
  void setSpinBoxHPFKValue(const uint16_t &value);
  void setFPGAFilterEnabled(const bool &enabled);
  bool getFPGAFilterEnabled();
private:
  Ui::DialogAFEConfiguration *ui;
};

#endif // DIALOGAFECONFIGURATION_H
