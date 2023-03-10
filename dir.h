#ifndef DIR_H
#define DIR_H

#include <QWidget>
#include <QDir>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QVector>
#include <QString>

using namespace std;

class FileFinder : public QWidget {
    Q_OBJECT
private:
    QLineEdit* Directory;
    QLineEdit* Mask;
    QTextEdit* Result;
    QTextEdit* Result2;
    QLabel *labelCount;
    QLabel *labelCount2;
    QVector<QString> NameFile;

public:
    FileFinder(QWidget* pwgt = 0);
    void start(const QDir& dir);


public slots:
    void slotFind  ();
    void slotBrowse();
    void codeLineCount();
    void clearCodSize();
};
#endif // DIR_H
