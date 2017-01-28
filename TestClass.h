#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QTest>
#include "MainWindow.h"

class TestClass : public QObject {
  Q_OBJECT
  public:
    explicit TestClass(QObject *parent = 0);
  signals:
    void SigTest1();
    void SigTest2();
    void SigTest3();
  private slots:
    void SlotTest1();
    void SlotTest2();
    void SlotTest3();
};

//////////////////////////////////////////////////////////////////////////

class Sleeper: public QThread {
  public:
    static void msleep(int ms) {
      QThread::msleep(ms);
    }
};

#endif // TESTCLASS_H
