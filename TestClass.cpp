#include "TestClass.h"

TestClass::TestClass(QObject* parent) : QObject(parent) {
}

void TestClass::SlotTest1() {
  MainWindow *Parent = qobject_cast<MainWindow *>(parent());
  connect(this ,SIGNAL(SigTest1()),Parent,SLOT(SlotNew()));
  Parent->statusBar()->showMessage(tr("Запускается тест №1 ..."));
  Sleeper::msleep(1000);
  emit SigTest1();
  // В данном тесте для успешного прохождения теста нужно тестировать
  // на значение, равное "нулю". Тестируем на "единицу" для демонстрации
  // неудачного прохождения теста
  QSqlQuery Query(Parent->Tools->MemoryDB);
  if (Query.exec("SELECT COUNT(*) AS \"Cnt\" FROM Payment") && Query.next()) {
    QSqlRecord R = Query.record();
    QCOMPARE(Query.value(R.indexOf("Cnt")).toInt(),1);
  } else {
    QMessageBox::warning(0, "Ошибка", "Ошибка исполнение теста №1");
  }
}

void TestClass::SlotTest2() {
  MainWindow *Parent = qobject_cast<MainWindow *>(parent());
  connect(this ,SIGNAL(SigTest2()),Parent,SLOT(SlotTestData()));
  Parent->statusBar()->showMessage(tr("Запускается тест №2 ..."));
  Sleeper::msleep(1000);
  emit SigTest2();
  // В данном тесте для успешного прохождения теста нужно тестировать
  // на значение, равное "трем". Тест должен пройти успешно
  QSqlQuery Query(Parent->Tools->MemoryDB);
  if (Query.exec("SELECT COUNT(*) AS \"Cnt\" FROM Payment") && Query.next()) {
    QSqlRecord R = Query.record();
    QCOMPARE(Query.value(R.indexOf("Cnt")).toInt(),3);
  } else {
    QMessageBox::warning(0, "Ошибка", "Ошибка исполнение теста №2");
  }
}

void TestClass::SlotTest3() {
  MainWindow *Parent = qobject_cast<MainWindow *>(parent());
  Parent->statusBar()->showMessage(tr("Запускается тест №3 ..."));
  Sleeper::msleep(1000);
  // В данном тесте тестируется GUI, а именно функция удаления записи
  // И проверяется результат
  QTest::keyPress(Parent,Qt::Key_Delete);
  QSqlQuery Query(Parent->Tools->MemoryDB);
  if (Query.exec("SELECT COUNT(*) AS \"Cnt\" FROM Payment") && Query.next()) {
    QSqlRecord R = Query.record();
    QCOMPARE(Query.value(R.indexOf("Cnt")).toInt(),2);
  } else {
    QMessageBox::warning(0, "Ошибка", "Ошибка исполнение теста №3");
  }
}
