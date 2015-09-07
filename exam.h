
#ifndef EXAM_H
#define EXAM_H


#include <QPixmap>
#include <QWidget>
#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;
class QRadioButton;
QT_END_NAMESPACE

//! [0]
class Exam : public QWidget
{
    Q_OBJECT

public:
    Exam(QDomElement*, QDomDocument* );
    QDomDocument* qdoc;
    QDomElement* dom;
    
    public slots:
      void updateCheckBox();
      void updateSpinBox();
      void updateRadio();

 private:
      
    QLabel* label;
    QCheckBox *checkbox;
    QSpinBox* spinbox;
    QLabel* spinLabel;
    QString getSP(int level);
    double old_weight;
    void updateDomWeight(QDomElement item) ;
    double getWeighSum (QDomElement item) ;
    QGroupBox *radioGroup;
    QGroupBox *spinGroup;
    /* QRadioButton *r1; */
    /* QRadioButton *r2; */
    /* QRadioButton *r3; */
    /* QRadioButton *r4; */
    QRadioButton *r[10];
    QString *score[10];
    QString *value[10];
    
    QHash<QRadioButton *, QString> valueMap;
    

};
//! [0]

#endif // EXAM_H
