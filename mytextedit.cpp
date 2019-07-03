#include "mytextedit.h"

#include <QMouseEvent>
#include <QtWidgets/QApplication>
#include <QFile>
#include <QtWidgets/QMessageBox>
#include <QTextStream>

myTextEdit::myTextEdit(QWidget *parent) :
    QTextEdit(parent), cursor_overriden(false)
{
    setMouseTracking(true);
}

void myTextEdit::mouseMoveEvent(QMouseEvent* e)
{
    if(anchorAt(e->pos()).isEmpty())
    {
        if(cursor_overriden)
        {
            qApp->restoreOverrideCursor();
            cursor_overriden = false;
        }
    }
    else
    {
        if(!cursor_overriden)
        {
            qApp->setOverrideCursor(QCursor(Qt::PointingHandCursor));
            cursor_overriden = true;
        }
    }
    QTextEdit::mouseMoveEvent(e);
}

void myTextEdit::mousePressEvent(QMouseEvent* e)
{
    QString href = anchorAt(e->pos());
    if(!href.isEmpty())
        emit anchor_clicked(href);
    QTextEdit::mousePressEvent(e);
}


void myTextEdit::save_to_html_file(const QString fname)
{
    QFile file(fname);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
       QMessageBox::warning(this, "Error",
                             "Error save to file: "+
                            QString(fname)+"\n"+
                            QString(file.errorString()));
    }
    else
    {
      QTextStream out(&file);
      QApplication::setOverrideCursor(Qt::WaitCursor);
      out << this->toHtml();            //save data
      QApplication::restoreOverrideCursor();
      file.close();
    }
}


void myTextEdit::open_html_file(const QString fname)
{
     emit(this->clear());

    //------------------
     QFile file(fname);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
        /*QMessageBox::warning(this, "Error",
                             "Error open file: "+
                             QString(fname)+"\n"+
                             QString(file.errorString()) );*/
     }
     else
     {
       QTextStream in(&file);
       QApplication::setOverrideCursor(Qt::WaitCursor);
       emit(this->setHtml(in.readAll()));         //load data
       QApplication::restoreOverrideCursor();
       file.close();
     }
     //------------------
}






