#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include<QMediaMetaData>

#include<QDebug>

#include "playlist.h"
#include "statusbar.h"
#include "multiiconbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum loop{noLoop,loopOne,loopAll};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void play(void);
    void pause(void);
    void stop(void);

private slots:
    void on_actionOpen_File_triggered();

    void on_actionOpen_Video_File_triggered();

    void on_play_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_loopOne_clicked();

    void on_loopAll_clicked();

    void on_noLoop_clicked();

    void on_previous_clicked();

    void on_next_clicked();

    void on_hidePlaylist_clicked();

    void on_up_clicked();

    void on_down_clicked();

    void on_delete_2_clicked();

    void metaDataChanged(const QString &key, const QVariant &value);
    void metaDataChanged(void);
    void end_of_media(QMediaPlayer::MediaStatus status);
    void mediaChanged(const QMediaContent &media);

    void on_shuffle_clicked();

private:
    Ui::MainWindow *ui;
    Playlist* playlist;
    QMediaPlayer* player;
    QVideoWidget* video;
    StatusBar* statusbar;
    enum loop loop;
    bool hide;
};
#endif // MAINWINDOW_H
