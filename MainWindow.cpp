#include "MainWindow.h"
#include "DialogEdit.h"
#include "DialogAbout.h"

/////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent) {
  freopen("HomeMoney-Tests.log", "w", stdout);
  statusBar()->showMessage(tr("Программа готова к работе"));
  SetupActions();
  Tools = new DBTools();
  Table = new QTableView();
  Model = new QSqlQueryModel();
  RightAlign = new RightAlignDelegate();
  CenterAlign = new CenterAlignDelegate();
  setCentralWidget(Table);
  Changed = false;
  Test = new TestClass(this);
  connect(this ,SIGNAL(SigTitleChanged()),this,SLOT(SlotTitleChanged()));
  connect(this, SIGNAL(SigTestData()), this, SLOT(SlotTestData()));
  connect(this, SIGNAL(SigTableRefresh()), this, SLOT(SlotRefresh()));
  connect(this, SIGNAL(SigTest1()),Test, SLOT(SlotTest1()));
  connect(this, SIGNAL(SigTest2()),Test, SLOT(SlotTest2()));
  connect(this, SIGNAL(SigTest3()),Test, SLOT(SlotTest3()));
  resize(600, 400);
  QTimer::singleShot(1, this, SLOT(SlotNew()));
}

/////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow() {
  delete CenterAlign;
  delete RightAlign;
  delete Model;
  delete Table;
  delete Tools;
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotNew() {
  Tools->CreateTables(Tools->MemoryDB);
  emit SigTableRefresh();
  emit SigTitleChanged();
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotOpen() {
  //QMessageBox::information(this, "Info", "SlotOpen");
  QString fileName = QFileDialog::getOpenFileName(this,tr("Открыть"), "", tr("SQLite3 Файлы (*.sqlite)"));
  if (fileName != "") {
    if (Tools->FileDB.isOpen()) Tools->FileDB.close();
    Tools->FileDB.setDatabaseName(fileName);
    if (Tools->FileDB.open()) {
      Changed = false;
      Tools->FileName = fileName;
      Tools->CreateTables(Tools->MemoryDB);
      Tools->CopyTables(Tools->FileDB, Tools->MemoryDB);
      Tools->FileDB.close();
      emit SigTableRefresh();
      emit SigTitleChanged();
    }
  }
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotSave() {
  if (Tools->FileName == "") SlotSaveAs(); else {
    Tools->SaveBase(Tools->FileName,Tools->MemoryDB,Tools->FileDB);
    Changed = false;
    emit SigTitleChanged();
  }
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotSaveAs() {
  QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить в"), "", tr("SQLite3 Файлы (*.sqlite)"));
  if (fileName != "") {
    Tools->FileName = fileName;
    Tools->SaveBase(Tools->FileName,Tools->MemoryDB,Tools->FileDB);
    Changed = false;
    emit SigTitleChanged();
  }
}

void MainWindow::SlotExit() {
  close();
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotAdd() {
  int Idx = -1;
  bool Res = false;
  DialogEdit *Dialog = new DialogEdit(0,&(Tools->MemoryDB),&Idx,&Res);
  Dialog->exec();
  delete Dialog;
  if (Res) {
    Changed = true;
    emit SigTitleChanged();
    emit SlotRefresh();
  }
}

void MainWindow::SlotEdit() {
  int Idx = Table->model()->data(Table->model()->index(Table->selectionModel()->currentIndex().row(),0,QModelIndex())).toInt();
  bool Res = false;
  DialogEdit *Dialog = new DialogEdit(0,&(Tools->MemoryDB),&Idx,&Res);
  Dialog->exec();
  delete Dialog;
  if (Res) {
    Changed = true;
    emit SigTitleChanged();
    emit SlotRefresh();
  }
}

void MainWindow::SlotDel() {
  if (Table->model()->rowCount() > 0) {
      int Num = Table->model()->data(Table->model()->index(Table->selectionModel()->currentIndex().row(),0,QModelIndex())).toInt();
      if (Tools->DelRow(Tools->MemoryDB,Num)) {
        Changed = true;
        emit SigTitleChanged();
        emit SigTableRefresh();
      }
  } else QMessageBox::information(this, "Внимание", "Таблица пуста");
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotAbout() {
  DialogAbout* Dialog = new DialogAbout();
  Dialog->exec();
  delete Dialog;
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotRefresh() {
  QSqlQuery Query(Tools->MemoryDB);
  QString S =
      "SELECT "
      "  Payment.Id, "
      "  date(Payment.DateTime,'unixepoch'), "
      "  time(Payment.DateTime,'unixepoch'), "
      "  CASE WHEN Subjects.Type < 0 THEN printf(\"-%.2f\", Payment.Amount) ELSE printf(\"%.2f\", Payment.Amount) END, "
      "  Subjects.Name "
      "FROM "
      "  Payment "
      "LEFT JOIN "
       " Subjects ON Subjects.Id = Payment.Subject";
  if (!Query.exec(S)) QMessageBox::warning(this, "Warning", "Refresh Error!");
  Model->setQuery(Query);
  Model->setHeaderData(0, Qt::Horizontal, tr("Ид"));
  Model->setHeaderData(1, Qt::Horizontal, tr("Дата"));
  Model->setHeaderData(2, Qt::Horizontal, tr("Время"));
  Model->setHeaderData(3, Qt::Horizontal, tr("Сумма"));
  Model->setHeaderData(4, Qt::Horizontal, tr("Вид"));
  Table->setModel(Model);
  Table->hideColumn(0);
  Table->setSortingEnabled(false);
  Table->resizeColumnsToContents();
  Table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
  Table->setAlternatingRowColors(true);
  Table->verticalHeader()->setHidden(true);
  Table->verticalHeader()->setDefaultSectionSize(20);
  Table->setSelectionBehavior(QAbstractItemView::SelectRows);
  Table->setSelectionMode(QAbstractItemView::SingleSelection);
  Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  Table->horizontalHeader()->setHighlightSections(false);
  Table->verticalHeader()->setHighlightSections(false);
  QFont Head = Table->horizontalHeader()->font();
  Head.setBold(true);
  Table->horizontalHeader()->setFont(Head);
  Table->setItemDelegateForColumn(1,CenterAlign);
  Table->setItemDelegateForColumn(2,CenterAlign);
  Table->setItemDelegateForColumn(3,RightAlign);
  if (Table->model()->rowCount() > 0) {
    Table->selectRow(0);
    Actions["Edit"]->setEnabled(true);
    Actions["Del"]->setEnabled(true);
  } else {
    Actions["Edit"]->setEnabled(false);
    Actions["Del"]->setEnabled(false);
  }
}

void MainWindow::SlotTests() {
  QTest::qExec(Test);
//  emit SigTest1();
//  emit SigTest2();
//  emit SigTest3();
  statusBar()->showMessage(tr("Тесты пройдены"));
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SlotTestData() {
  if (Tools->TestData(Tools->MemoryDB)) {
    Changed = true;
    emit SigTableRefresh();
  } else {
    QMessageBox::warning(this, "Ошибка", "Ошибка заполнения тестовыми данными!");
  }
}

void MainWindow::SlotTitleChanged() {
  QString Title;
  if (Tools->FileName == "") Title = "БезНазвания"; else {
    QFileInfo info(Tools->FileName);
    Title = info.completeBaseName();
  }
  QString Mark = (Changed) ? " (изменен)":"";
  setWindowTitle("HomeMoney :: \""+Title+"\""+Mark);
}

/////////////////////////////////////////////////////////////////////////////

void MainWindow::SetupActions() {
  QToolBar* ToolBar = addToolBar("Панель инструментов");
  QMenu* fileMenu = menuBar()->addMenu("&Файл");
  QMenu* operMenu = menuBar()->addMenu("&Действия");
  QMenu* helpMenu = menuBar()->addMenu("&Справка");
  Names = {
    // key : ico-type,ico-file,rus,rus-descr
    {"New",   {"document-new", "New", "&Новый", "Открыть новый файл базы данных"}},
    {"Open",  {"document-open", "Open", "&Открыть ...", "Открыть существующий файл базы данных"}},
    {"Save",  {"document-save", "Save", "&Сохранить", "Сохранить файл базы данных"}},
    {"SaveAs", {"document-save-as", "SaveAs", "&Сохранить как ...", "Сохранить файл базы данных под другим именем"}},
    {"Exit",  {"application-exit", "Exit", "&Выход", "Завершить работу программы"}},
    {"Add",   {"list-add", "Add", "&Добавить", "Добавить запись в таблицу"}},
    {"Edit",  {"emblem-documents", "Edit", "&Редактировать", "Редактировать запись таблицы"}},
    {"Del",   {"list-remove", "Del", "&Удалить", "Удалить запись из таблицы"}},
    {"Refresh", {"view-refresh", "Refresh", "&Обновить", "Обновить таблицу"}},
    {"Tests", {"applications-science", "Tests", "&Тесты ...", "Провести тесты"}},
    {"About", {"help-about", "Home", "&О программе", "Вывести информацию о программе"}}
  };
  Processor = {
    {"New", {QKeySequence::New, &MainWindow::SlotNew}},
    {"Open",  {QKeySequence::Open, &MainWindow::SlotOpen}},
    {"Save",  {QKeySequence::Save, &MainWindow::SlotSave}},
    {"SaveAs", {Qt::Key_S + Qt::CTRL + Qt::ALT, &MainWindow::SlotSaveAs}},
    {"Exit",  {Qt::Key_F4 + Qt::ALT, &MainWindow::SlotExit}},
    {"Add",   {Qt::Key_Insert, &MainWindow::SlotAdd}},
    {"Edit",  {Qt::Key_Enter, &MainWindow::SlotEdit}},
    {"Del",   {Qt::Key_Delete, &MainWindow::SlotDel}},
    {"Refresh", {Qt::Key_R + Qt::CTRL, &MainWindow::SlotRefresh}},
    {"Tests", {Qt::Key_T + Qt::CTRL, &MainWindow::SlotTests}},
    {"About", {Qt::Key_A + Qt::CTRL, &MainWindow::SlotAbout}}
  };
  for (const auto Key : Names.keys()) Icons[Key]
      = QIcon::fromTheme(Names[Key][0], QIcon(":/img/" + Names[Key][1] + ".png"));
  for (const auto Key : Names.keys()) {
    Actions[Key] = new QAction(Icons[Key], Names[Key][2], this);
    Actions[Key]->setStatusTip(Names[Key][3]);
    Actions[Key]->setShortcut(Processor[Key].first);
    connect(Actions[Key], &QAction::triggered, this, Processor[Key].second);
  }
  ToolBar->addAction(Actions["New"]);
  ToolBar->addAction(Actions["Open"]);
  ToolBar->addAction(Actions["Save"]);
  ToolBar->addSeparator();
  ToolBar->addAction(Actions["Add"]);
  ToolBar->addAction(Actions["Edit"]);
  ToolBar->addAction(Actions["Del"]);
  ToolBar->addSeparator();
  ToolBar->addAction(Actions["Refresh"]);
  ToolBar->addSeparator();
  ToolBar->addAction(Actions["Tests"]);
  ToolBar->addSeparator();
  ToolBar->addAction(Actions["About"]);
  fileMenu->addAction(Actions["New"]);
  fileMenu->addAction(Actions["Open"]);
  fileMenu->addAction(Actions["Save"]);
  fileMenu->addAction(Actions["SaveAs"]);
  fileMenu->addSeparator();
  fileMenu->addSeparator();
  fileMenu->addAction(Actions["Exit"]);
  operMenu->addAction(Actions["Add"]);
  operMenu->addAction(Actions["Edit"]);
  operMenu->addAction(Actions["Del"]);
  operMenu->addSeparator();
  operMenu->addAction(Actions["Refresh"]);
  operMenu->addSeparator();
  operMenu->addAction(Actions["Tests"]);
  helpMenu->addAction(Actions["About"]);
}

/////////////////////////////////////////////////////////////////////////////

void RightAlignDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_ASSERT(index.isValid());
  QVariant value;
  QStyleOptionViewItem opt = setOptions(index, option);
  opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
  QItemDelegate::paint(painter,opt,index);
}

/////////////////////////////////////////////////////////////////////////////

void CenterAlignDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_ASSERT( index.isValid() );
  QVariant value;
  QStyleOptionViewItem opt = setOptions(index, option);
  opt.displayAlignment = Qt::AlignHCenter | Qt::AlignVCenter;
  QItemDelegate::paint(painter,opt,index);
}
