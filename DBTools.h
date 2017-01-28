#ifndef DBTOOLS_H
#define DBTOOLS_H

#include <QObject>
#include <QtSql>

class DBTools : public QObject {
  Q_OBJECT
public:
  explicit DBTools(QObject* parent = 0);
  bool CreateTables(QSqlDatabase& Data);
  bool CopyTables(QSqlDatabase& In, QSqlDatabase& Out);
  bool SaveBase(QString NewName, QSqlDatabase& MemoryDB, QSqlDatabase& FileDB);
  bool TestData(QSqlDatabase& DB);
  bool DelRow(QSqlDatabase& DB, int Idx);
  QString FileName;
  QSqlDatabase MemoryDB;
  QSqlDatabase FileDB;
};

#endif // DBTOOLS_H
