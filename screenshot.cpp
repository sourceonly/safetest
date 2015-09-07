
#include <QtWidgets>
#include <iostream>
#include "screenshot.h"
#include "exam.h"
//! [0]
Screenshot::Screenshot()
{

  greeting=new QWidget;
  scoring=new QWidget;
  Layout=new QHBoxLayout;
  createGreeting();
  createScoring();
  
  
  Layout->addWidget(greeting);
  Layout->addWidget(scoring);

  
  

}

QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->connect(button, SIGNAL(clicked()), receiver, member);
    return button;
}



void Screenshot::showScreen(int index) {
  updateScoring();
  greeting->hide();
  scoring ->hide();
  for(int i=0;i<=max_frame-1;i++) {
    exam[i]->hide();
  }
  QString qs;
  if (current==-1) {
    greeting->show();
    qs=QObject::tr("开始界面");
    label_index->setTitle(qs);
  } else if (current==max_frame) {
    qs=QObject::tr("得分界面");
    label_index->setTitle(qs);
    scoring->show();
  } else {

    exam[index]->show();

    qs=QObject::tr("当前" ) + QString::number(index+1) + QObject::tr( " 项 / 共 ") + QString::number(max_frame) + QObject::tr(" 项");
    label_index->setTitle(qs);
    ScoreLabel->setText(QObject::tr("当前得分是：") + QString::number(this->getScore(qdoc.documentElement())));
  }
  // scoring->show();
}
void Screenshot::prevScreenshot()
{
  if (current >-1) {
    current--;
    showScreen(current);
  } 
}

void Screenshot::nextScreenshot()
{
  if (current < max_frame) {
    current++;
    showScreen(current);
  }
}




void Screenshot::Score()
{
  QMessageBox::about(this, tr("特种设备当前评分"),
		     QObject::tr("当前得分是：") + QString::number(this->getScore(qdoc.documentElement())));
}
  






bool Screenshot::read(QIODevice *device)
{
  QString errorStr;
  int errorLine;
  int errorColumn;
  // Screenshot();
  if (!qdoc.setContent(device, true, &errorStr, &errorLine,
			      &errorColumn)) {
    QMessageBox::information(window(), tr("DOM Bookmarks"),
			     tr("Parse error at line %1, column %2:\n%3")
			     .arg(errorLine)
			     .arg(errorColumn)
			     .arg(errorStr));
    return false;
  }
  QDomElement root = qdoc.documentElement();
  if (root.tagName() != "xbel") {
    QMessageBox::information(window(), tr("DOM Bookmarks"),
			     tr("The file is not an XBEL file."));
    return false;
  } else if (root.hasAttribute("version")
	     && root.attribute("version") != "1.0") {
    QMessageBox::information(window(), tr("DOM Bookmarks"),
			     tr("The file is not an XBEL version 1.0 "
				"file."));
    return false;
  }


  // printf("%d\n",getNumberofDocBookmarks(root));
  current=0;
  max_frame=getNumberofDocBookmarks(root);
  
  QString qs;
  qs=QObject::tr("当前 1 项 / 共 ") + QString::number(max_frame) + QObject::tr(" 项");

  label_index=new QGroupBox(qs);
  
  // label_index->setScaledContents(false);
  initFrame(root);
  current=-1;
  
  int i;

  

  for(i=0;i<max_frame;i++) {
    // exam[i]=new Exam(root);
    Layout->addWidget(exam[i]);
  }

  ScoreLabel=new QLabel();
  ScoreLabel->hide();
  // ScoreLabel->setPixmap(QPixmap("./pic1.jpg"));
  Layout->addWidget(ScoreLabel);
  
  button[0] = createButton(tr("上一项(&P)"), this, SLOT(prevScreenshot()));
  button[1] = createButton(tr("下一项(&N)"), this, SLOT(nextScreenshot()));
  button[2] = createButton(tr("当前得分(&S)"), this, SLOT(Score()));

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  
  buttonsLayout->addStretch();
  buttonsLayout->addWidget(button[0]);
  buttonsLayout->addWidget(button[1]);
  buttonsLayout->addWidget(button[2]);
  
  // button[2]->hide();
  
  
  QVBoxLayout* layout=new QVBoxLayout;
  label_index->setLayout(Layout);
  layout->addWidget(label_index);
  
  // layout->addLayout(Layout);

  layout->addLayout(buttonsLayout);
  
  setLayout(layout);
  showScreen(current);

  // createScoring();
  return true;
}




int Screenshot::getNumberofAllBookmarks() {
  return getNumberofDocBookmarks(qdoc.documentElement());
}
int Screenshot::getNumberofDocBookmarks(QDomElement item) {
  int sum=0;
  // printf("called\n");
  // std::cout<<item.tagName().toUtf8().constData()<<std::endl;
  QDomElement child;
  if (item.tagName()=="bookmark" ) {
    // std::cout<<"\t"<<item.tagName().toUtf8().constData()<<std::endl;
    return 1;
  } else if (item.tagName()=="xbel" || item.tagName() =="folder" ) {
    child=item.firstChildElement();
    while (!child.isNull()) {
      sum+=getNumberofDocBookmarks(child);
      child=child.nextSiblingElement();
    }
  }
  return sum;
}

int Screenshot::initFrame(QDomElement item) {
  int sum=0;
  // printf("called\n");
  // std::cout<<item.tagName().toUtf8().constData()<<std::endl;
  QDomElement child;
  if (item.tagName()=="bookmark" ) {
    // std::cout<<"\t"<<item.tagName().toUtf8().constData()<<std::endl;
    exam[current]=new Exam(&item,&qdoc);
    current++;
    // printf("%d\n", current);
    return 1;
  } else if (item.tagName()=="xbel" || item.tagName() =="folder" ) {
    child=item.firstChildElement();
    while (!child.isNull()) {
      sum+=initFrame(child);
      child=child.nextSiblingElement();
    }
  }
  return sum;
}
bool Screenshot::write(QIODevice *device)
{
  const int IndentSize = 4;

  QTextStream out(device);
  qdoc.save(out, IndentSize);
  return true;
}




double Screenshot::getScore(QDomElement item) {
  double score=0;
  if (item.tagName() ==  "bookmark") {
    if (item.hasAttribute("score")) {
      // printf("book,%lf\n",item.attribute("score").toDouble());
      return item.attribute("score").toDouble();
    }
    return 0;
  }
  QDomElement child = item.firstChildElement();
  // QTreeWidgetItem* titem=new QTreeWidgetItem;
  while (!child.isNull()) {
    score+=getScore(child)*getWeight(child);
    child = child.nextSiblingElement();
  }
  
  item.setAttribute("score",QString::number(score));

  return score;
}



double Screenshot::getWeight(QDomElement item) {
  if (item.tagName() !="bookmark" && item.tagName()!="folder") {
    return 0;
  }
  if (!item.hasAttribute("weight")) {
    return 0;
  }
  return item.attribute("weight").toDouble();
}


void Screenshot::createGreeting() {
  QGridLayout *grid=new QGridLayout;
  
  for (int i=0;i<100;i++) {
    label_greeting[i]=new QLabel;
    grid->addWidget(label_greeting[i],i,0);
    label_greeting[i]->hide();
    edit_greeting[i]=new QLineEdit;
  }
  QDateTimeEdit *editor = new QDateTimeEdit();
  editor->setDisplayFormat("yy年MM月dd日");
  editor->setCalendarPopup(true);
  

  grid->addWidget(editor,1,1);
  label_greeting[0]->setText("特种设备");

  label_greeting[1]->setText("评估时间");
  label_greeting[2]->setText("评估类型");
  
  grid->addWidget(edit_greeting[2],2,1);

  label_greeting[0]->show();
  label_greeting[1]->show();
  label_greeting[2]->show();

  greeting->setLayout(grid);  
  
}

void Screenshot::createScoring() {
  QGridLayout *grid=new QGridLayout;
  for (int i=0;i<300;i++) {
    label_result[i]=new QLabel;
    grid->addWidget(label_result[i],i/3,i%3);
    label_result[i]->hide();
  }
  qtext=new QTextEdit;
  QLabel *l0=new QLabel("结论");
  grid->addWidget(l0,100,0);
  grid->addWidget(qtext,101,0,102,0);
  qtext->setMinimumSize(200, 300);

  scoring->setLayout(grid);
}
void Screenshot::updateScoring() {

  QDomElement root=qdoc.documentElement();
  QDomElement root_child=root.firstChildElement("folder");
  QDomElement j;
  double score;
  QString good=tr("<font size=\"80\" color=\"green\"> 优秀 </font>");
  QString gg=tr("<font size=\"80\" color=\"yellow\"> 良好 </font>");
  QString normal=tr("<font size=\"80\" color=\"blue\"> 一般 </font>");
  QString bad=tr("<font size=\"80\" color=\"red\"> 有待改进 </font>");
      
  int level=0;

  label_result[level*3]->show();
  label_result[level*3+1]->show();
  label_result[level*3+2]->show();

  j=root_child.firstChildElement("title");
  label_result[level*3]->setText(j.text());
  score=this->getScore(root_child);
  label_result[level*3+1]->setText(QString::number(score));

  if (score > 85) {
    label_result[level*3+2]->setText(good);
  } else if (score > 75) {
    label_result[level*3+2]->setText(gg);
  }  else if (score > 60) {
    label_result[level*3+2]->setText(normal);
  }  else  {
    label_result[level*3+2]->setText(bad);
  }
  
  

  
  
  level++;
  for (QDomElement i=root_child.firstChildElement("folder");!i.isNull();i=i.nextSiblingElement("folder")) {
    label_result[level*3]->show();
    label_result[level*3+1]->show();
    label_result[level*3+2]->show();
    j=i.firstChildElement("title");
    label_result[level*3]->setText(j.text());
    score=this->getScore(i);
    label_result[level*3+1]->setText(QString::number(score));


    if (score > 85) {
      label_result[level*3+2]->setText(good);
    } else if (score > 75) {
      label_result[level*3+2]->setText(gg);
    }  else if (score > 60) {
      label_result[level*3+2]->setText(normal);
    }  else  {
      label_result[level*3+2]->setText(bad);
    }

    level++;
  }
}





//! [0]

//! [10]
