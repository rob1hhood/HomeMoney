#include "DialogAbout.h"

DialogAbout::DialogAbout()
{
  setWindowTitle("О программе");
  QVBoxLayout *V = new QVBoxLayout();
  QHBoxLayout *T = new QHBoxLayout();
  QHBoxLayout *B = new QHBoxLayout();
  QVBoxLayout *TV = new QVBoxLayout();
  QPixmap Pix(":/img/HomeMoney.png");
  QLabel *L1 = new QLabel(); L1->setPixmap(Pix);
  QLabel *L2 = new QLabel("<h3>Курсовая работа</h3>");
  QLabel *L3 = new QLabel("«Домашняя Бухгалтерия»");
  QLabel *L4 = new QLabel("выполнил <font color=\"navy\">Карпович В.И.</font>");
  QLabel *L5 = new QLabel("Минск, 2016");
  QWidget *Line = new QWidget;
  QPushButton *Close = new QPushButton("&Закрыть");
  Line->setFixedHeight(1);
  Line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  Line->setStyleSheet(QString("background-color: #c0c0c0;"));
  TV->addStretch(1);
  TV->addWidget(L2,0,Qt::AlignCenter);
  TV->addWidget(L3,0,Qt::AlignCenter);
  TV->addWidget(L4,0,Qt::AlignCenter);
  TV->addWidget(L5,0,Qt::AlignCenter);
  TV->addStretch(1);
  T->addWidget(L1,0);
  T->addLayout(TV,1);
  V->addLayout(T,1);
  V->addWidget(Line,1);
  B->addStretch(1);
  B->addWidget(Close);
  V->addLayout(B,0);
  setLayout(V);
  resize(320,160);
  connect(Close,SIGNAL(clicked(bool)),this,SLOT(close()));
}
