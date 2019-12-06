#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QString>


/**
 * @brief  A empty class,just here to merge the QListWidgetItem and the QFileInfo
 */

class PlaylistItem : public QListWidgetItem, public QFileInfo
{
public:
    /**
     * @brief The empty constructor, just here to call the QListWidgetItem and QFileInfo
     * @param path the absolute path of the file
     */
    PlaylistItem(QString path);
};


/**
 * @brief A reimplementation of the QListWidget, where items are also QFileInfo,
 * used as a graphical playlist
 */

class Playlist : public QListWidget
{

    Q_OBJECT

public:
    Playlist(QWidget *parent=nullptr);
    void addItem(PlaylistItem* item);
    void addItem(QString path);
    void addItems(QStringList paths);
    PlaylistItem* item(int index);
    PlaylistItem* currentItem(void);
    PlaylistItem* takeItem(int index);
    void insertItem(int index, PlaylistItem *item);

signals:
    void itemDoubleClicked(PlaylistItem *item);
    void itemClicked(PlaylistItem *item);

private slots:
    void doubleClickedItem(QListWidgetItem *item);
    void clickedItem(QListWidgetItem *item);
};

#endif // PLAYLIST_H
