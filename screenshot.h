#ifndef SCREENSHOT_H
#define SCREENSHOT_H

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
class QTextEdit;
QT_END_NAMESPACE

class Exam;
//! [0]
class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot();
    bool read(QIODevice *device);
    bool write(QIODevice *device);
    int getNumberofAllBookmarks();
    QDomDocument qdoc;
    void createScoring() ;
    void createGreeting();
    void updateScoring() ;
    
      
    public slots:
      void prevScreenshot();
      void nextScreenshot();
      void Score();
      
      
 private:
    Exam* exam[1024];
    /* QLabel* label_index; */
    QGroupBox* label_index;
    QLabel *ScoreLabel;
    void showScreen(int);
    int current;
    int max_frame;
    QPushButton* button[3];
    QPushButton *createButton(const QString &text, QWidget *receiver, const char *member);
    int getNumberofDocBookmarks(QDomElement);
    int initFrame(QDomElement);
    double getScore(QDomElement item);
    double getWeight(QDomElement item);
    QWidget* greeting;
    QWidget* scoring;
    QHBoxLayout *Layout;
    QLabel* label_result[300];
    QLineEdit* edit_result[100];
    QLabel* label_greeting[100];
    QLineEdit* edit_greeting[100];
    QTextEdit* qtext;
};  
//! [0]

#endif // SCREENSHOT_H
