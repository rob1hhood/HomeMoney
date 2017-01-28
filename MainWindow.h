#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtSql>
#include "DBTools.h"
#include "TestClass.h"

class RightAlignDelegate;
class CenterAlignDelegate;
class TestClass;

////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  DBTools *Tools;
signals:
  void SigTestData();
  void SigTableRefresh();
  void SigTitleChanged();
  void SigTest1();
  void SigTest2();
  void SigTest3();
private slots:
  void SlotNew();
  void SlotOpen();
  void SlotSave();
  void SlotSaveAs();
  void SlotExit();
  void SlotAdd();
  void SlotEdit();
  void SlotDel();
  void SlotAbout();
  void SlotRefresh();
  void SlotTests();
  void SlotTestData();
  void SlotTitleChanged();
private:
  QMap<QString, QVector<QString>> Names;
  QMap<QString, QPair<QKeySequence, void(MainWindow::*)()>> Processor;
  QMap<QString, QIcon> Icons;
  QMap<QString, QAction*> Actions;
  QTableView* Table;
  QSqlQueryModel* Model;
  RightAlignDelegate *RightAlign;
  CenterAlignDelegate *CenterAlign;
  TestClass *Test;
  bool Changed;
  void SetupActions();
};

////////////////////////////////////////////////////////////////////////////

class RightAlignDelegate : public QItemDelegate {
  Q_OBJECT
public:
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

////////////////////////////////////////////////////////////////////////////

class CenterAlignDelegate : public QItemDelegate {
  Q_OBJECT
public:
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // MAINWINDOW_H
