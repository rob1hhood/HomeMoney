#include "DialogEdit.h"

DialogEdit::DialogEdit(QWidget *parent, QSqlDatabase *iDataBase, int *iIdx, bool *iRes)
  : QDialog(parent), DataBase(iDataBase), Idx(iIdx), Res(iRes) {
  QLabel *L1 = new QLabel("Тип записи:");
  QLabel *L2 = new QLabel("Вид:");
  QLabel *L3 = new QLabel("Сумма:");
  QLabel *L4 = new QLabel("Дата:");
  Type = new QComboBox();
  Subject = new QComboBox();
  Amount = new QLineEdit();
  Date = new QDateTimeEdit();
  Ok = new QPushButton("Ок");
  Cancel = new QPushButton("Отмена");
  QHBoxLayout *H1 = new QHBoxLayout();
  QHBoxLayout *H2 = new QHBoxLayout();
  QHBoxLayout *H3 = new QHBoxLayout();
  QVBoxLayout *V = new QVBoxLayout();
  H1->addWidget(L2,0);
  H1->addWidget(Subject,1);
  H2->addStretch();
  H2->addWidget(L3,0);
  H2->addWidget(Amount,0);
  H2->addWidget(L1,0);
  H2->addWidget(Type,0);
  Amount->setFixedWidth(100);
  H2->addWidget(L4,0);
  H2->addWidget(Date,0);
  Date->setCalendarPopup(true);
  Date->setDate(QDate::currentDate());
  H3->addStretch();
  H3->addWidget(Ok);
  H3->addWidget(Cancel);
  V->addLayout(H1);
  V->addLayout(H2);
  V->addLayout(H3);
  setLayout(V);
  resize(400,100);
  Type->addItem("Доход",1);
  Type->addItem("Расход",-1);
  Ok->setEnabled(false);
  connect(Amount,SIGNAL(textChanged(const QString &)),this,SLOT(SlotOkEnabled()));
  connect(Ok,SIGNAL(released()),this,SLOT(SlotOk()));
  connect(Cancel,SIGNAL(released()),this,SLOT(SlotCancel()));
  connect(Type,SIGNAL(currentIndexChanged(int)),this,SLOT(SlotFillCombo()));
  connect(this,SIGNAL(SigFillCombo()),this,SLOT(SlotFillCombo()));
  connect(this,SIGNAL(SigFillData()),this,SLOT(SlotFillData()));
  emit SigFillCombo();
  emit SigFillData();
}

void DialogEdit::SlotOk() {
  QSqlQuery Query(*DataBase);
  if (*Idx>0) {
    Query.prepare("UPDATE \"Payment\" "
                  "SET \"DateTime\"=:DateTime, "
                  "    \"Amount\"=:Amount, "
                  "    \"Subject\"=:Subject "
                  "WHERE \"Id\"=:Id");
    Query.bindValue(":Id",*Idx);
    Query.bindValue(":DateTime",Date->dateTime().toUTC().toTime_t()+QDateTime::currentDateTime().offsetFromUtc());
    Query.bindValue(":Amount",Amount->text());
    Query.bindValue(":Subject",Subject->currentData());
    if (!Query.exec()) QMessageBox::warning(this, "Ошибка", "Ошибка редактирования данных!"); else
      *Res = true;
  } else {
    if (Query.exec("SELECT MAX(\"Payment\".\"Id\") AS \"Max\" FROM \"main\".\"Payment\"")) {
      QSqlRecord R = Query.record();
      Query.next();
      int Max = Query.value(R.indexOf("Max")).toInt()+1;
      Query.prepare("INSERT INTO \"main\".\"Payment\" VALUES "
                    "(:Id,:DateTime,:Amount,:Subject)");
      Query.bindValue(":Id",Max);
      Query.bindValue(":DateTime",Date->dateTime().toUTC().toTime_t()+QDateTime::currentDateTime().offsetFromUtc());
      Query.bindValue(":Amount",Amount->text());
      Query.bindValue(":Subject",Subject->currentData());
      if (!Query.exec()) QMessageBox::warning(this, "Ошибка", "Ошибка вставки новых данных!"); else
        *Res = true;
    } else QMessageBox::warning(this, "Ошибка", "Ошибка получения максимального значения!");
  }
  close();
}

void DialogEdit::SlotCancel() {
  close();
}

void DialogEdit::SlotFillCombo() {
  QSqlQuery Query(*DataBase);
  QString S = "SELECT * FROM \"main\".\"Subjects\" WHERE Subjects.Type=";
  S += (Type->currentIndex() == 0) ? "1" : "-1";
  S += " ORDER BY Subjects.Name";
  if (Query.exec(S)) {
    Subject->clear();
    QSqlRecord R = Query.record();
    while (Query.next()) {
      Subject->addItem(Query.value(R.indexOf("Name")).toString(),Query.value(R.indexOf("Id")).toInt());
    }
  } else QMessageBox::warning(this, "Warning", "Combo query Error!");
}

void DialogEdit::SlotFillData() {
  if (*Idx>0) {
    QSqlQuery Query(*DataBase);
    Query.prepare("SELECT "
                  "\"Payment\".\"Id\", "
                  "\"Payment\".\"DateTime\", "
                  "\"Payment\".\"Amount\", "
                  "\"Subjects\".\"Name\", "
                  "\"Subjects\".\"Type\" "
                  "FROM "
                  " \"main\".\"Payment\" "
                  "LEFT JOIN "
                  " \"main\".\"Subjects\" ON \"Payment\".\"Subject\" = \"Subjects\".\"Id\" "
                  "WHERE "
                  " \"Payment\".\"Id\" = :Id");
    Query.bindValue(":Id",*Idx);
    if (Query.exec()) {
      QSqlRecord R = Query.record();
      if (Query.next()) {
        QDateTime Tmp;
        Tmp.setTime_t(Query.value(R.indexOf("DateTime")).toUInt()-QDateTime::currentDateTime().offsetFromUtc());
        Date->setDateTime(Tmp);
        //FIXME !!!
        Amount->setText(QString("%02").arg(Query.value(R.indexOf("Amount")).toDouble()));
        Type->setCurrentIndex((Query.value(R.indexOf("Type")).toInt()>0) ? 0:1);
        Subject->setCurrentText(Query.value(R.indexOf("Name")).toString());
      }
    } else QMessageBox::warning(this, "Warning", "Fill data query Error!");
  }
}

void DialogEdit::SlotOkEnabled() {
  Ok->setEnabled(Amount->text().toFloat() > 0.0f);
}
