#include "playlist.h"

PlaylistItem::PlaylistItem(QString path) : QListWidgetItem(path), QFileInfo(path)
{
    this->setText(this->fileName());
}


Playlist::Playlist(QWidget *parent) : QListWidget(parent)
{
    connect(this,&QListWidget::itemDoubleClicked,this,&Playlist::doubleClickedItem);
    connect(this,&QListWidget::itemClicked,this,&Playlist::clickedItem);
}

void Playlist::doubleClickedItem(QListWidgetItem *item){ emit itemDoubleClicked(dynamic_cast<PlaylistItem*>(item)); }

void Playlist::clickedItem(QListWidgetItem *item){ emit itemClicked(dynamic_cast<PlaylistItem*>(item)); }

void Playlist::addItem(PlaylistItem *item){ this->QListWidget::addItem(item);}

void Playlist::addItem(QString path) {
    PlaylistItem* item = new PlaylistItem(path);
    this->Playlist::addItem(item);
}

void Playlist::addItems(QStringList paths) {
    for(int i=0; i<paths.size(); i++) this->addItem(paths[i]);
}

PlaylistItem* Playlist::item(int index){
    return dynamic_cast<PlaylistItem*>(this->QListWidget::item(index));
}

PlaylistItem* Playlist::currentItem(){
    return dynamic_cast<PlaylistItem*>(this->QListWidget::currentItem());
}

PlaylistItem* Playlist::takeItem(int index){ return dynamic_cast<PlaylistItem*>(this->QListWidget::takeItem(index));}

void Playlist::insertItem(int index, PlaylistItem *item){ this->QListWidget::insertItem(index,item);}

