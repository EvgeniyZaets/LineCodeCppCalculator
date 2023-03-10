#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QApplication>
#include "fileopen.xpm"
#include "dir.h"
#include <QDebug>

uint i;
uint cod;
uint comment;
FileFinder::FileFinder(QWidget* pwgt/*= 0*/) : QWidget(pwgt)
{
    Directory    = new QLineEdit(QDir::current().absolutePath());
    Mask   = new QLineEdit("*.cpp *.h");
    Result = new QTextEdit;
    //Result2 = new QTextEdit;

    QLabel*      labelDirectory  = new QLabel("&Каталог");
    QLabel*      labelMask = new QLabel("&Маска");
    labelCount = new QLabel("Количество файлов: ");
    labelCount2 = new QLabel("Количество строк кода в файлах: ");
    QPushButton* ButDirectory  = new QPushButton(QPixmap(fileopen), "");
    QPushButton* ButFind = new QPushButton("&Найти");
    QPushButton* ButLineCode = new QPushButton("&Проанализировать");

    connect(ButDirectory, SIGNAL(clicked()), SLOT(slotBrowse()));
    connect(ButFind, SIGNAL(clicked()), SLOT(slotFind()));
    connect(ButLineCode, SIGNAL(clicked()), SLOT(codeLineCount()));
    connect(ButFind, SIGNAL(clicked()), SLOT(clearCodSize()));

    labelDirectory->setBuddy(Directory);
    labelMask->setBuddy(Mask);

    QGridLayout* pgrdLayout = new QGridLayout;
    pgrdLayout->setMargin(5);
    pgrdLayout->setSpacing(15);
    pgrdLayout->addWidget(labelDirectory, 0, 0);
    pgrdLayout->addWidget(labelMask, 1, 0);
    pgrdLayout->addWidget(Directory, 0, 1);
    pgrdLayout->addWidget(Mask, 1, 1);
    pgrdLayout->addWidget(ButDirectory, 0, 2);
    pgrdLayout->addWidget(ButFind, 1, 2);
    pgrdLayout->addWidget(labelCount, 2, 0,1,3);
    pgrdLayout->addWidget(labelCount2, 3, 0,1,3);
    pgrdLayout->addWidget(Result, 4, 0, 1, 3);
    pgrdLayout->addWidget(ButLineCode, 5, 2);
    //pgrdLayout->addWidget(Result2, 6, 0, 1, 3); //для отладки
    setLayout(pgrdLayout);
}


void FileFinder::slotBrowse()
{
    Result->clear();
    QString str = QFileDialog::getExistingDirectory(0,"Выбор каталога",Directory->text());

    if (!str.isEmpty()) {
       Directory->setText(str);
    }
}


void FileFinder::slotFind()
{
    i=0;
    Result->clear();
    start(QDir(Directory->text()));
    QString stri = QString::number(i);
    labelCount->setText("Количество файлов: "+stri);

}


void FileFinder::start(const QDir& dir)
{
    QApplication::processEvents();

    QStringList listFiles =
        dir.entryList(Mask->text().split(" "), QDir::Files);
     i=i+dir.entryList(Mask->text().split(" "), QDir::Files).count();

    foreach (QString file, listFiles) {
        Result->append(dir.absoluteFilePath(file));
        NameFile.push_back(dir.absoluteFilePath(file));
    }

    QStringList listDir = dir.entryList(QDir::Dirs);

    foreach (QString subdir, listDir) {
        if (subdir == "." || subdir == "..") {
            continue;
        }
        start(QDir(dir.absoluteFilePath(subdir)));
    }
}

void FileFinder::codeLineCount()
{
    cod = 0;
    comment = 0;
    for(int i=0; i < NameFile.size(); i++) {

        QFile file(NameFile[i]);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            QString str;
            bool previousLineComment = false;
            while (!stream.atEnd())
            {
                while(stream.readLineInto(&str))
                {
                    cod++;
                    if (str.contains("//")) {
                        comment++;
                        continue;
                    }
                    else if (str.contains("/*")) {
                        previousLineComment = true;
                        comment++;
                        continue;
                    }
                    else if (str.contains("*/")) {
                        comment++;
                        previousLineComment = false;
                        continue;
                    }
                    else if (previousLineComment == true){
                        comment++;
                        continue;
                    }

                    QString strcod = QString::number(cod);
                    QString strcomment = QString::number(comment);
                    labelCount2->setText("Количество строк кода в файлах: " +strcod + ", из них комментариев " +strcomment);
                }

            if (stream.status() != QTextStream::Ok)
            {
                qDebug() << "Ошибка чтения файла";
            }
            file.close();
            }
        }
    }
}

void FileFinder::clearCodSize()
{
    cod = 0;
    comment = 0;
    QString strcod = QString::number(cod);
    QString strcomment = QString::number(comment);
    labelCount2->setText("Количество строк кода в файлах: " +strcod + ", из них комментариев " +strcomment);
}

