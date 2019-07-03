#ifndef _mytextedit_h_
#define _mytextedit_h_

#include <QtWidgets/QTextEdit>

class myTextEdit : public QTextEdit
{
Q_OBJECT
public:
    explicit myTextEdit(QWidget *parent = 0);

    void open_html_file(const QString fname);
    void save_to_html_file(const QString fname);

signals:
    void anchor_clicked(const QString& href);
protected:
    bool cursor_overriden;
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
};




#endif
