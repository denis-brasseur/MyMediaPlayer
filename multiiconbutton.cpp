#include "multiiconbutton.h"

MultiIconButton::MultiIconButton(QWidget *parent) : QPushButton(parent){
    this->iconList = new QList<QIcon>();
    this->current = 0;
}

MultiIconButton::MultiIconButton(const QIcon &icon, const QString &text, QWidget *parent): QPushButton(icon,text,parent)
{
    this->iconList = new QList<QIcon>();
    this->iconList->append(icon);
    this->current = 0;
}

void MultiIconButton::addIcon(const QIcon &icon){this->iconList->append(icon);}

void MultiIconButton::switchIcon(){
    qDebug() << this->iconList->size();
    this->current++;
    if(this->current >= this->iconList->size()) this->current=0;
    qDebug() << this->current;
    this->QPushButton::setIcon(this->iconList->at(this->current));
}
void MultiIconButton::setIcon(const QIcon &icon){
    this->addIcon(icon);
    this->QPushButton::setIcon(icon);
}
