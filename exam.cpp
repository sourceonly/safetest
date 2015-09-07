#include <QtWidgets>
#include "exam.h"

//! [0]
Exam::Exam(QDomElement *item,QDomDocument* doc)
{
  
  qdoc=doc;
  old_weight=item->attribute("weight").toDouble();
  
  label=new QLabel;
  dom=new QDomElement;
  *dom=*item;
  QDomElement child=item->firstChildElement("title");
  QDomNode parent=item->parentNode();
  QDomElement pchild;
  QString title;
  int level=0;
  QString sp;
  QString qs[10];
  
  // while (!parent.parentNode().parentNode().isNull()) {
  //   level++;
  //   if le
  //   sp=getSP(level);
  //   pchild=parent.firstChildElement("title");
  //   parent=parent.parentNode();
  //   title=pchild.text() + " \n " + sp + title;
  //   qs[level]=pchild.text();
  // }
  // qs[0]=child.text;
  
  pchild=parent.firstChildElement("title");
  title=tr("<font size=\"20\" color=\"green\">")+ pchild.text();
  title+=child.text() + tr ("</font>");

  
  
  // label->setText(title);
  label->setText(title);
  label->setMaximumSize(600,700);
  label->setWordWrap(true);
  QLabel *label_image=new QLabel;
  label_image->setPixmap(QPixmap("../resource/pic10.png"));
  label_image->resize(400,700);

  // label_image->show();
  spinLabel=new QLabel;
  spinLabel->setText(tr("评分"));
  spinbox=new QSpinBox;
  if (item->hasAttribute("score")) {
    spinbox->setValue(item->attribute("score").toDouble());
  } else {
    spinbox->setValue(0);
  }
  
  spinbox->setSuffix(tr("分"));
  spinbox->setMaximum(100);

  spinGroup=new QGroupBox("本项为定量指标，范围为0－100分");
  QHBoxLayout* slayout=new QHBoxLayout;
  slayout->addWidget(spinLabel);
  slayout->addWidget(spinbox);
  spinGroup->setLayout(slayout);

  

  checkbox=new QCheckBox("&0 . 无此项");


  QHBoxLayout* rlayout=new QHBoxLayout;
  for(int i=0;i<10;i++) {
    r[i]=new QRadioButton();
    rlayout->addWidget(r[i]);
    score[i]=new QString;
    value[i]=new QString;
    
    
    connect(r[i],SIGNAL(clicked(bool)),this,SLOT(updateRadio()));
    r[i]->hide();
  }
  if (dom->hasAttribute("select_number") ) {
    for(int i=0;i< dom->attribute("select_number").toInt();i++) {
      r[i]->show();
    }
  }
  
  if (dom->hasAttribute("select_value")) {
    QStringList p=dom->attribute("select_value").split(",");
    for(int i=0;i< dom->attribute("select_number").toInt();i++) {
      r[i]->setText(tr("&")+QString::number(i+1)+tr(". ")+p[i]);
    }
  }

  if (dom->hasAttribute("select_score")) {
    QStringList q=dom->attribute("select_score").split(",");    
    for(int i=0;i< dom->attribute("select_number").toInt();i++) {
      valueMap.insert(r[i],q[i]);
    }
  }

  
  radioGroup=new QGroupBox("本项为定性指标，请从以下选项中选取");
  radioGroup->setLayout(rlayout);

  
  // QGridLayout *GridLayout=new QGridLayout;
  // GridLayout->addWidget(label_image,0,0);

  // GridLayout->addWidget(checkbox,1,0);
  // // GridLayout->addWidget(spinLabel,2,0);
  // // GridLayout->addWidget(spinbox,2,1);
  
  // GridLayout->addWidget(spinGroup,2,0,2,0);
  // GridLayout->addWidget(radioGroup,3,0,2,0);

  
  QGridLayout *GridLayout=new QGridLayout;
  GridLayout->addWidget(label_image,0,0,5,0);
  GridLayout->addWidget(label,1,1);
  GridLayout->addWidget(checkbox,2,1);
  // GridLayout->addWidget(spinLabel,2,0);
  // GridLayout->addWidget(spinbox,2,1);

	
  connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(updateSpinBox()));
  connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(updateCheckBox()));

  setLayout(GridLayout);
  updateDomWeight(qdoc->documentElement());
  if (dom->attribute("select") == "yes") {
    GridLayout->addWidget(radioGroup,3,1);
    GridLayout->addWidget(spinGroup,4,1);

    radioGroup->show();
    spinGroup->hide();
  } else {

    GridLayout->addWidget(radioGroup,4,1);
    GridLayout->addWidget(spinGroup,3,1);



    radioGroup->hide();
    spinGroup->show();
    
  }

  
}

QString Exam::getSP(int level) {
  QString sp;
  for (int i=5;i>level;i--) {
    sp+="  ";
  }
  sp+="\\_";
  return sp;
}



void Exam::updateCheckBox()
{

  
  if (checkbox->isChecked()) {
    dom->setAttribute("weight",0);
    printf("%lf",old_weight);
  } else {
    // (this->dom)->setAttribute("weight")=QString::number(this->old_weight);
    // QDomElement i;
    // for (i=((this->dom)->parentNode()).firstChildElement();!i.isNull();i.nextSiblingElement()) {
    // 	i.setAttribute("weight",this->old_weight);
    // }
      if (old_weight > 0.001) {
    // dom->setAttribute("weight",getWeighSum(*dom)/(1-old_weight)*old_weight);
	dom->setAttribute("weight",1.0*old_weight/(1-old_weight));
  }

  }

  updateDomWeight(qdoc->documentElement());
}





void Exam::updateSpinBox()
{
  // dom->attribute("score")=QString::number(spinbox->value());
  // printf("%d",1111);
  // printf("%d\n",spinbox->value());
  dom->setAttribute("score",spinbox->value());
}

void Exam::updateDomWeight(QDomElement item) {
  double sum=getWeighSum(item);
  QDomElement child = item.firstChildElement();

  while (!child.isNull() ) {
    if (child.tagName()!="folder"  && child.tagName()!="bookmark") {
      child = child.nextSiblingElement();
      continue;
    }
    if (child.tagName()=="folder") {
      updateDomWeight(child);
    }
    if (child.hasAttribute("weight")) {
      // printf("-\n%lf\n-",child.attribute("weight").toDouble()/sum);
      child.setAttribute("weight",QString::number(child.attribute("weight").toDouble()/sum));
    }
    child = child.nextSiblingElement();
  }
  
  //refresh();
}


double Exam::getWeighSum (QDomElement item) {
  QDomElement child = item.firstChildElement();
  double sum=0.0;

  while (!child.isNull()) {
    if (child.hasAttribute("weight")) {
      sum+=child.attribute("weight").toDouble();
    }
    child = child.nextSiblingElement();
  }
  // printf("%lf",sum);
  if (sum > 0.001 ) {
    return sum;
  }
  return 0;
}

void Exam::updateRadio() {
  // if (sender() ==r1) {
  //   dom->setAttribute("score",100);
  //   spinbox->setValue(100);
  // } else if (sender() == r2) {
  //   dom->setAttribute("score",80);
  //   spinbox->setValue(80);
  // } else if (sender() == r3) {
  //   dom->setAttribute("score",60);
  //   spinbox->setValue(60);
  // } else if (sender() == r4) {
  //   dom->setAttribute("score",40);
  //   spinbox->setValue(40);
  // }
  
  QString qs;
  QRadioButton *p= (QRadioButton *) sender();

  qs=valueMap.value(p);
  dom->setAttribute("score",qs);
  spinbox->setValue(qs.toDouble());
    
}


//! [0]

//! [10]
