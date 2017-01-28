#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QtWidgets>
#include <QtSql>

class DialogEdit : public QDialog {
  Q_OBJECT
public:
  explicit DialogEdit(QWidget* parent = 0, QSqlDatabase *iDataBase = nullptr,
                      int *iIdx = nullptr, bool *iRes = nullptr);

signals:
  void SigFillCombo();
  void SigFillData();
public slots:
  void SlotOk();
  void SlotCancel();
  void SlotFillCombo();
  void SlotFillData();
  void SlotOkEnabled();
private:
  QSqlDatabase *DataBase;
  int *Idx;
  bool *Res;
  QDateTimeEdit *Date;
  QLineEdit *Amount;
  QComboBox *Type;
  QComboBox *Subject;
  QPushButton *Ok;
  QPushButton *Cancel;
};

#endif // DIALOGEDIT_H
