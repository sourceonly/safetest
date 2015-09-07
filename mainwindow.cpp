
#include <QtWidgets>
#include "mainwindow.h"
#include "xbeltree.h"
#include "screenshot.h"

MainWindow::MainWindow()
{
  init_file="./test1.xbel";
  firstOpen=1;


  // setCentralWidget(xbelTree);

  wxbelTree=new QWidget;

  
  QWidget* w=new QWidget;
  QHBoxLayout *wlay=new QHBoxLayout;
  toolbox=new QToolBox;
  toolbox->setMaximumSize(200, 1000);
  xbelTree = new XbelTree();
  
  QWidget *p1=new QWidget;
  toolbox->addItem(p1,tr("Exam"));
  QVBoxLayout *p1lay=new QVBoxLayout;
  b1=new QPushButton("exam");

  p1lay->addWidget(b1);
  p1lay->addStretch();
  p1->setLayout(p1lay);

  
  QWidget *p2=new QWidget;
  toolbox->addItem(p2,tr("Manage"));
  QVBoxLayout *p2lay=new QVBoxLayout;
  b2=new QPushButton("manage");
  p2lay->addWidget(b2);
  p2lay->addStretch();
  p2->setLayout(p2lay);
  toolbox->resize(100,768);


  b1->setIcon(QPixmap(tr("../resource/bt1.png")));
  b2->setIcon(QPixmap(tr("../resource/bt2.png")));
  
  connect (b1,SIGNAL(clicked()),this,SLOT(showExam()));
  connect (b2,SIGNAL(clicked()),this,SLOT(showTree()));
  
  
  screenshot=new Screenshot();
  toolbox->show();
  wlay->addWidget(toolbox);
  wlay->addWidget(wxbelTree);
  wlay->addWidget(screenshot);
  
  w->setLayout(wlay);

  setCentralWidget(w);

  createActions();

  createLabel();
  createButtonLayout();


  QVBoxLayout *wxtree=new QVBoxLayout;
  wxtree->addWidget(xbelTree);
  wxtree->addLayout(buttonLayout);
  wxbelTree->setLayout(wxtree);
  

  createMenus();
  wxbelTree->hide();  
  statusBar()->showMessage(tr("Ready"));

  // QGridLayout* GridLayout=new QGridLayout;
  // // QVBoxLayout* vbox=new QVBoxLayout;
  // // vbox->addLayout(GridLayout);
  // GridLayout->addWidget(xbelTree,0,0);
  // label->setText("    ");
  // GridLayout->addWidget(label,0,1);
  // GridLayout->setColumnMinimumWidth(0,900);
  // GridLayout->addLayout(buttonLayout,1,1,1,1);
  //   // GridLayout->addLayout(buttonLayout,1,2,1,2);
  // // GridLayout->setColumnMaximunWidth(1,400);
  // w->setLayout(GridLayout);

  // //  w->setAutoFillBackground(true);
  // QPalette palette;
  // //  palette.setColor(QPalette::Background, QColor(192,253,123));
  // palette.setBrush(QPalette::Background, QBrush(QPixmap("../resource/pic1.png")));

  // w->setPalette(palette);

  // xbelTree->show();
  // setLayout(vbox);
    
  setWindowIcon(QIcon("../resource/pic1.png"));
  setWindowTitle(tr("国家特种设备安全水平评价系统"));
  // xbelTree->hide();
  // xbelTree->resize(800,600);
  resize(1366, 800);
  setMaximumSize(1366,800);
  setMinimumSize(1366,800);
}

void MainWindow::open()
{
  QString fileName;
  if (firstOpen) {
      fileName=init_file;
      firstOpen=0;
    } else {
      fileName =
      QFileDialog::getOpenFileName(this, tr("Open Bookmark File"),
				   QDir::currentPath(),
				   tr("XBEL Files (*.xbel *.xml)"));
  }
  if (fileName.isEmpty())
    return;

  QFile file(fileName);
  // screenshot->read(&file);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("SAX Bookmarks"),
			 tr("Cannot read file %1:\n%2.")
			 .arg(fileName)
			 .arg(file.errorString()));
    return;
  }

  if (xbelTree->read(&file))
    statusBar()->showMessage(tr("File loaded"), 2000);

  // if (screenshot->read(&file)) {
  //   // statusBar()->showMessage(tr("File loaded"), 2000);
  //   printf("load");
  // }
  file.close();
  QFile file1(fileName);
  if (!file1.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("SAX Bookmarks"),
			 tr("Cannot read file %1:\n%2.")
			 .arg(fileName)
			 .arg(file.errorString()));
    return;
  }

  // delete screenshot;
  // screenshot=new Sceenshot;
  screenshot->read(&file1);
  file1.close();
  // printf("%daaa\n",screenshot->getNumberofAllBookmarks());
}

void MainWindow::saveAs()
{
  QString fileName =
    QFileDialog::getSaveFileName(this, tr("Save Bookmark File"),
				 QDir::currentPath(),
				 tr("XBEL Files (*.xbel *.xml)"));
  if (fileName.isEmpty())
    return;

  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("SAX Bookmarks"),
  			 tr("Cannot write file %1:\n%2.")
  			 .arg(fileName)
  			 .arg(file.errorString()));
    return;
  }

  if (xbelTree->write(&file))
    statusBar()->showMessage(tr("File saved"), 2000);

  QFile file1(fileName);
  if (!file1.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("SAX Bookmarks"),
			 tr("Cannot write file %1:\n%2.")
			 .arg(fileName)
			 .arg(file.errorString()));
    return;
  }

  
  
  screenshot->write(&file);
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("特种设备"),
             tr("<b></b>特种设备评价系统，多级评分系统"));
}





void MainWindow::createActions()
{
  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAsAct = new QAction(tr("&Save As..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcuts(QKeySequence::Quit);
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  aboutAct = new QAction(tr("&About"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addAction(exitAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  //helpMenu->addAction(aboutQtAct);
}

void MainWindow::createLabel() {
  label=new QLabel;
  label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  label->setAlignment(Qt::AlignCenter);
  label->setMinimumSize(240, 160);
  
}
void MainWindow::createButtonLayout() {
  buttonLayout=new QHBoxLayout;
  buttonupdate = createButton(tr("Update Weight"), this, SLOT(updateWeight()));
  buttonexit = createButton(tr("Quit"), this, SLOT(close()));
  buttonLayout->addStretch();
  buttonLayout->addWidget(buttonupdate);
  buttonLayout->addWidget(buttonexit);
  
}
QPushButton *MainWindow::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
  QPushButton *button = new QPushButton(text);
  button->connect(button, SIGNAL(clicked()), receiver, member);
  return button;
}


void MainWindow::updateWeight() {
  xbelTree->updateDomWeightAll();
}

void MainWindow::showTree() {
  wxbelTree->hide();
  screenshot->hide();
  wxbelTree->show();
}

void MainWindow::showExam() {
  wxbelTree->hide();
  screenshot->hide();
  screenshot->show();
}
