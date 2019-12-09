#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->playlist = this->ui->playlist;
    this->loop = noLoop;
    this->hide = false;

    //add a second icon to hidePlaylist Button
    this->ui->hidePlaylist->addIcon(QIcon::fromTheme(":/plus"));
    //add a second icon to play Button
    this->ui->play->addIcon(QIcon::fromTheme(":/pause"));

    //play item when double clicked
    connect(this->playlist,&Playlist::itemDoubleClicked,this,&MainWindow::on_play_clicked);

    //set VolumeSlider at 100
    this->ui->volumeSlider->setValue(100);


    /*** MediaPlayer initialisation ***/
    this->player = new QMediaPlayer();
    this->video = new QVideoWidget(this->ui->videoDisplay);
    this->player->setVideoOutput(this->video);
    connect(this,&MainWindow::play,this->player,&QMediaPlayer::play);
    connect(this,&MainWindow::pause,this->player,&QMediaPlayer::pause);
    connect(this,&MainWindow::stop,this->player,&QMediaPlayer::stop);
    connect(this->player,&QMediaPlayer::mediaStatusChanged,this,&MainWindow::end_of_media);
    connect(this->player,SIGNAL(metaDataChanged(const QString&, const QVariant&)),this,SLOT(metaDataChanged(const QString&, const QVariant&)));
    connect(this->player,&QMediaPlayer::mediaChanged,this->ui->coverArt,&QLabel::clear);
    connect(this->ui->volumeSlider,&ClickableSlider::valueChanged,this->player,&QMediaPlayer::setVolume);
//    connect(this->player,SIGNAL(metaDataChanged()),this,SLOT(metaDataChanged()));

    /*** Status Bar initialisation ***/
    this->statusbar = new StatusBar();
    this->statusbar->setSlider(this->ui->slider);
    this->statusbar->setElapsedTimeLabel(this->ui->elapsedTimeLabel);
    this->statusbar->setRemainingTimeLabel(this->ui->remainingTimeLabel);
    connect(this->player,&QMediaPlayer::durationChanged,this->statusbar,&StatusBar::durationChanged);
    connect(this,&MainWindow::play,this->statusbar,&StatusBar::start);
    connect(this,&MainWindow::pause,this->statusbar,&StatusBar::pause);
    connect(this,&MainWindow::stop,this->statusbar,&StatusBar::reset);
    connect(this->statusbar,&StatusBar::setPosition,this->player,&QMediaPlayer::setPosition);


    //setting window size
    this->setFixedWidth(this->height()*16/9);
//    this->ui->slider->setFixedWidth();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_File_triggered()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Music File"), "/home/arinfo", tr("Music (*.mp3 *.ogg *.flac *.wav)"));
    this->playlist->addItems(paths);
}

void MainWindow::on_actionOpen_Video_File_triggered()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Video File"), "/home/arinfo", tr("Video (*.mp4 *.avi)"));
    this->playlist->addItems(paths);
}

void MainWindow::on_play_clicked()
{
    qDebug() << "play";
    if(this->playlist->currentItem()==nullptr && this->player->media().isNull()) return;
    if(this->player->state() != QMediaPlayer::PlayingState){
        if(this->player->media().canonicalUrl() != QUrl::fromLocalFile(this->playlist->currentItem()->filePath())) this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
        this->ui->play->switchIcon();
        emit play();
    }
    else{
        this->ui->play->switchIcon();
        emit pause();
    }
}

void MainWindow::on_pause_clicked()
{
    qDebug() << "pause";
    if(this->player->media().isNull()) return;
    emit pause();
}

void MainWindow::on_stop_clicked()
{
    qDebug() << "stop";
    if(this->player->media().isNull()) return;
    if(this->player->state() == QMediaPlayer::PlayingState) this->ui->play->switchIcon();
    emit stop();
}

void MainWindow::on_loopOne_clicked()
{
    qDebug() << "loopOne";
    if(this->ui->loopOne->isChecked()){
        this->loop = loopOne;
        this->ui->loopOne->setChecked(true);
        if(this->ui->loopAll->isChecked()) this->ui->loopAll->setChecked(false);
    }
    else{
        this->loop = noLoop;
        this->ui->loopOne->setChecked(false);
    }
}

void MainWindow::on_loopAll_clicked()
{
    qDebug() << "loopAll";
    if(this->ui->loopAll->isChecked()){
        this->loop = loopAll;
        this->ui->loopAll->setChecked(true);
        if(this->ui->loopOne->isChecked()) this->ui->loopOne->setChecked(false);
    }
    else{
        this->loop = noLoop;
        this->ui->loopAll->setChecked(false);
    }
}

void MainWindow::on_noLoop_clicked()
{
    qDebug() << "noLoop";
    this->loop = noLoop;
    if(this->ui->loopOne->isChecked()) this->ui->loopOne->setChecked(false);
    if(this->ui->loopAll->isChecked()) this->ui->loopAll->setChecked(false);
}

void MainWindow::on_previous_clicked()
{
    qDebug() << "previous";
    if(this->playlist->currentRow()>0) this->playlist->setCurrentRow(this->playlist->currentRow()-1);
    else if(this->loop==loopAll) this->playlist->setCurrentRow(this->playlist->count()-1);
    else return;

    if(this->player->state()==QMediaPlayer::PlayingState){
        this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
        emit play();
    }
    else{
        this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
        emit stop();
    }
}

void MainWindow::on_next_clicked()
{
    qDebug() << "next";
    if(this->playlist->currentRow()<(this->playlist->count()-1)) this->playlist->setCurrentRow(this->playlist->currentRow()+1);
    else if(this->loop==loopAll) this->playlist->setCurrentRow(0);
    else return;
//    this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
    if(this->player->state()==QMediaPlayer::PlayingState){
        this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
        emit play();
    }
    else{
        this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
        emit stop();
    }
}

void MainWindow::on_hidePlaylist_clicked()
{
    qDebug() << "hide";
    this->hide = !this->hide;
    this->ui->playlistPanel->setHidden(this->hide);
//    this->ui->hidePlaylist->setChecked(this->hide);
    this->ui->hidePlaylist->switchIcon();
    static int l = this->width();
    if(this->hide){
    this->setGeometry(this->x(),this->y(),l-this->ui->playlistPanel->width() - this->ui->horizontalLayout->contentsMargins().right() - this->ui->centralwidget->contentsMargins().right(),this->height());
    }
    else{
        this->setGeometry(this->x(),this->y(),l,this->height());
    }
}

void MainWindow::on_up_clicked()
{
    qDebug() << "up";
    int i = this->playlist->currentRow();
    if(i>0){
        PlaylistItem *item = this->playlist->takeItem(i);
        this->playlist->insertItem(i-1,item);
        this->playlist->setCurrentItem(item);
    }
    else if(i==0 && this->loop==loopAll){
        PlaylistItem *item = this->playlist->takeItem(i);
        this->playlist->insertItem(this->playlist->count(),item);
        this->playlist->setCurrentItem(item);
    }
    else return;
}

void MainWindow::on_down_clicked()
{
    qDebug() << "down";
    int i = this->playlist->currentRow();
    if(i>=0 && i<(this->playlist->count()-1)){
        PlaylistItem *item = this->playlist->takeItem(i);
        this->playlist->insertItem(i+1,item);
        this->playlist->setCurrentItem(item);
    }
    else if(i==this->playlist->count()-1 && this->loop==loopAll){
        PlaylistItem *item = this->playlist->takeItem(i);
        this->playlist->insertItem(0,item);
        this->playlist->setCurrentItem(item);
    }
    else return;

}

void MainWindow::on_delete_2_clicked()
{
    qDebug() << "delete";
    int i = this->playlist->currentRow();
    if(i>=0 && i<this->playlist->count()){
        PlaylistItem* item = this->playlist->takeItem(i);
        if(this->player->media().canonicalUrl() == QUrl::fromLocalFile(item->filePath())){
            emit stop();
            if(this->playlist->count()>0) this->playlist->setCurrentRow((i<this->playlist->count())?i:i-1);
            if(this->playlist->currentItem()!=nullptr) this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
            else{
                this->player->setMedia(nullptr);
                this->statusbar->clear();
            }
        }
    }

}

void MainWindow::metaDataChanged(const QString &key, const QVariant &value){
    qDebug() << this->player->metaData(QMediaMetaData::AudioCodec);
    if(key==QMediaMetaData::VideoCodec){
        qDebug() << "video";
        this->ui->stackedWidget->setCurrentWidget(this->ui->videoDisplay);
        if(this->ui->coverArt->pixmap() != nullptr) this->ui->coverArt->clear();
//        qDebug() << this->ui->coverArt->children().size();
//        dynamic_cast<QVideoWidget*>(this->ui->coverArt->children()[0])->setAspectRatioMode(Qt::KeepAspectRatio);
        this->video->setAspectRatioMode(Qt::KeepAspectRatio);
//        dynamic_cast<QVideoWidget*>(this->ui->coverArt->children()[0])->show();
        this->video->show();

    }
    else if(key==QMediaMetaData::AudioCodec){
        qDebug() << "audio";
//        qDebug() << this->ui->coverArt->pixmap();
//        if(this->ui->coverArt->pixmap()==nullptr) this->ui->coverArt->setPixmap(QIcon::fromTheme(":/default_icon").pixmap(this->ui->coverArt->size()));
//        dynamic_cast<QVideoWidget*>(this->ui->coverArt->children()[0])->close();
    }
    else if(key==QMediaMetaData::CoverArtImage){
        qDebug() << "cover art";
        this->ui->coverArt->setPixmap(QPixmap::fromImage(value.value<QImage>().scaled(this->ui->coverArt->size(),Qt::KeepAspectRatio)));
        this->ui->stackedWidget->setCurrentWidget(this->ui->coverArtPage);
    }
}
void MainWindow::metaDataChanged(){
    qDebug() << this->player->metaData(QMediaMetaData::VideoCodec);
}

void MainWindow::end_of_media(QMediaPlayer::MediaStatus status){
    if(status==QMediaPlayer::EndOfMedia){
        if(this->loop==loopOne){
            this->statusbar->reset();
            emit play();
        }
        else if(this->playlist->currentRow()<(this->playlist->count()-1)){
            this->playlist->setCurrentItem(this->playlist->item(this->playlist->currentRow()+1));
            this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
            emit play();
        }
        else if(this->loop==loopAll){
            this->playlist->setCurrentItem(this->playlist->item(0));
            this->player->setMedia(QUrl::fromLocalFile(this->playlist->currentItem()->filePath()));
            emit play();
        }
        else emit stop();
    }
}


void MainWindow::on_shuffle_clicked()
{
    int size = this->playlist->count();
    int current = this->playlist->currentRow();
    std::vector<int> index(size);
    QList<PlaylistItem*> buf = QList<PlaylistItem*>();
    for(int i=0; i<size; i++){
        index[i]=i;
        buf.append((dynamic_cast<PlaylistItem*>(this->playlist->takeItem(0))));
    }
    std::random_shuffle(index.begin(),index.end());
    this->playlist->clear();
    for(int i=0; i<size; i++){
        this->playlist->addItem(buf[index[i]]);
    }
    if(current!=-1) this->playlist->setCurrentItem(buf[current]);
}

void MainWindow::mediaChanged(const QMediaContent &media){
}
