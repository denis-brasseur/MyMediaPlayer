#ifndef MULTIICONBUTTON_H
#define MULTIICONBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QList>
#include <QDebug>

class MultiIconButton: public QPushButton
{
public:
    MultiIconButton(QWidget *parent=nullptr);
    MultiIconButton(const QIcon &icon, const QString& text,QWidget *parent=nullptr);
    void addIcon(const QIcon& icon);
    void switchIcon(void);
    void setIcon(const QIcon &icon);
private:
    QList<QIcon> *iconList;
    int current;
};

#endif // MULTIICONBUTTON_H
