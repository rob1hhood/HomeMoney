#include "DBTools.h"
#include <QtWidgets>

DBTools::DBTools(QObject* parent) : QObject(parent) {
  FileName = "";
  MemoryDB = QSqlDatabase::addDatabase("QSQLITE", ":memory:");
  FileDB = QSqlDatabase::addDatabase("QSQLITE");
  if (!MemoryDB.open()) QMessageBox::information(0, "Ошибка", "Ошибка открытия БД в памяти");
}

/////////////////////////////////////////////////////////////////////////////

bool DBTools::CreateTables(QSqlDatabase& Data) {
  try {
    QSqlQuery Query(Data);
    QString S;
    S = "PRAGMA writable_schema = 1";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "DELETE FROM sqlite_master WHERE type in ('table', 'index', 'trigger')";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "PRAGMA writable_schema = 0";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "VACUUM";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "PRAGMA INTEGRITY_CHECK";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE TABLE \"Payment\" ( "
        " \"Id\" INTEGER NOT NULL, "
        " \"DateTime\" INTEGER NOT NULL, "
        " \"Amount\" REAL NOT NULL, "
        " \"Subject\" INTEGER NOT NULL)";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE UNIQUE INDEX \"main\".\"Idx\" "
        "ON \"Payment\" (\"Id\" ASC)";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE INDEX \"main\".\"Subject\" "
        "ON \"Payment\" (\"Subject\" ASC)";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE TABLE \"Subjects\" ( "
        "\"Id\"  INTEGER NOT NULL, "
        "\"Name\"  TEXT NOT NULL, "
        "\"Type\"  INTEGER NOT NULL)";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE UNIQUE INDEX \"main\".\"Id\" "
        "ON \"Subjects\" (\"Id\" ASC) ";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE UNIQUE INDEX \"main\".\"Name\" "
        "ON \"Subjects\" (\"Name\" ASC) ";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "CREATE INDEX \"main\".\"Type\" "
        "ON \"Subjects\" (\"Type\" ASC) ";
    if (!Query.exec(S)) throw std::runtime_error("");
    S = "INSERT INTO \"main\".\"Subjects\" VALUES "
        "(1, 'Заработная плата', 1), "
        "(2, 'Дивиденды', 1), "
        "(3, 'Топливо', -1), "
        "(4, 'Продукты питания', -1), "
        "(5, 'Коммунальные платежи', -1), "
        "(6, 'Погашение кредита', -1), "
        "(7, 'Кредит', 1)";
    if (!Query.exec(S)) throw std::runtime_error("");
    return true;
  } catch (...) {
    QMessageBox::information(0, "Ошибка", "Ошибка создания таблиц");
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////

bool DBTools::CopyTables(QSqlDatabase& In, QSqlDatabase& Out) {
  try {
    QSqlQuery QueryIn(In);
    QSqlQuery QueryOut(Out);
    if (!QueryIn.exec("SELECT * FROM Payment")) {
      qDebug() << ":::" << QueryIn.lastError();
      throw std::runtime_error("");
    }
    QSqlRecord R = QueryIn.record();
    while (QueryIn.next()) {
      QueryOut.prepare("INSERT INTO \"Payment\" VALUES (:1,:2,:3,:4)");
      QueryOut.bindValue(":1", QueryIn.value(R.indexOf("Id")).toInt());
      QueryOut.bindValue(":2", QueryIn.value(R.indexOf("DateTime")).toInt());
      QueryOut.bindValue(":3", QueryIn.value(R.indexOf("Amount")).toFloat());
      QueryOut.bindValue(":4", QueryIn.value(R.indexOf("Subject")).toInt());
      if (!QueryOut.exec()) throw std::runtime_error("");
    }
    return true;
  } catch (...) {
    QMessageBox::information(0, "Ошибка", "Ошибка копирования таблиц");
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////

bool DBTools::SaveBase(QString NewName, QSqlDatabase& MemoryDB, QSqlDatabase& FileDB) {
  try {
    if (QFile::exists(NewName) && (!QFile::remove(NewName))) throw std::runtime_error("");
    FileDB.setDatabaseName(NewName);
    if (!FileDB.open()) throw std::runtime_error("");
    if (!CreateTables(FileDB)) throw std::runtime_error("");
    if (!CopyTables(MemoryDB, FileDB)) throw std::runtime_error("");
    FileDB.close();
    return true;
  } catch (...) {
    QMessageBox::information(0, "Ошибка", "Ошибка сохранения базы");
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////

bool DBTools::TestData(QSqlDatabase& DB) {
  try {
    if (!CreateTables(DB)) throw std::runtime_error("");
    QSqlQuery Query(DB);
    QString S = "INSERT INTO \"main\".\"Payment\" VALUES "
                "(1, 1484379665, 111.22, 1), "
                "(2, 1484374665, 33.04, 4), "
                "(3, 1484329665, 22.1, 5)";
    if (!Query.exec(S)) throw std::runtime_error("");
    return true;
  } catch (...) {
    QMessageBox::information(0, "Ошибка", "Ошибка заполнения тестовыми данными таблиц");
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////

bool DBTools::DelRow(QSqlDatabase& DB, int Idx) {
  QSqlQuery Query(DB);
  Query.prepare("DELETE FROM main.\"Payment\" WHERE \"Id\"=:Id");
  Query.bindValue(":Id", Idx);
  return Query.exec();
}
