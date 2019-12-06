#include "statusbar.h"

/**
 * @brief a private conversion function in order to convert a time, in seconds, into a string
 * @param time, a time in second, into a Qt alias of a 64 bits integer
 * @return the time pass in parameter, as a string in format hh:mm:ss, or mm:ss when there is less than one hour
 */

QString conversion(qint64 time){
    qint64 hours, minutes, seconds;
    hours = time/3600;
    time -= hours*3600;
    minutes = time/60;
    time -= minutes*60;
    seconds = time;

    QString ret;
    if(hours>0) ret += QString::number(hours) + ":";
    if(minutes<10) ret += "0";
    ret += QString::number(minutes) + ":";
    if(seconds<10) ret += "0";
    ret += QString::number(seconds);

    return ret;
}

/**
 * @brief just a empty constructor, just here to call the QSlider constructor
 * @param parent a pointer to the QWidget who include this slider, null by default
 */
ClickableSlider::ClickableSlider(QWidget *parent) : QSlider(parent){}

/**
 * @brief The method this class was created for,
 * changing the value of the slider in function of the click of the mouse.
 * Only the left click has an effect.
 * @param event the mouse event
 */
void ClickableSlider::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(this->orientation() == Qt::Horizontal){
			this->setValue(this->minimum() + (event->x()*(this->maximum() - this->minimum())/this->width()));
		}
		else{
			setValue(minimum() + ((maximum()-minimum())*(height()-event->y()))/height());
		}
        event->accept();
    }
    QSlider::mousePressEvent(event);
}


/**
 * @brief The constructor, creating the internal timer and setting every internal variables to 0
 * @param parent the QWidget where this object is displayed, null by default
 */

StatusBar::StatusBar(QWidget *parent) : QWidget(parent)
{
    this->timer = new QTimer();
    this->timer->setInterval(1000);
    connect(this->timer,&QTimer::timeout,this,&StatusBar::elapsedSecond);
    this->duration = this->elapsedTime = this->remainingTime = 0;
    this->elapsedTimeLabel = nullptr;
    this->remainingTimeLabel = nullptr;
    this->slider = nullptr;
//    this->isMediaChanged = this->isStopButtonPush = false;
}

/**
 * @brief The setter for the ClickableSlider,
 * it also connect the slider's signal for a changed value to the class slot to handle it.
 * @param slider the ClickableSlider, created anywhere else.
 */

void StatusBar::setSlider(ClickableSlider *slider){
    this->slider = slider; this->slider->setValue(0);
    connect(this->slider,&ClickableSlider::valueChanged,this,&StatusBar::valueChanged);
}

/**
 * @brief The setter for the QLabel displaying the elapsed time of the currently read media, this also set the displayed time to 00:00
 * @param label the QLabel, created anywhere else
 */

void StatusBar::setElapsedTimeLabel(QLabel *label){ this->elapsedTimeLabel = label; this->elapsedTimeLabel->setText("00:00");}

/**
 * @brief The setter for the QLAbel displaying the remaining time of the currently read media, this also set the displayed time to 00:00
 * @param label the QLabel, created anywhere else
 */

void StatusBar::setRemainingTimeLabel(QLabel *label){ this->remainingTimeLabel = label; this->remainingTimeLabel->setText("00:00");}


/**
 * @brief A slot to handle the internal timer signal every second,
 * increasing elapsed time, decreasing remaining time, and changing the Qlabels and the ClickableSlider displayed times
 */

void StatusBar::elapsedSecond(void){
    qDebug() << "one second elapsed";
    this->elapsedTime++;
    this->remainingTime--;
    this->elapsedTimeLabel->setText(conversion(this->elapsedTime));
    this->remainingTimeLabel->setText(conversion(this->remainingTime));
    this->slider->blockSignals(true);
    this->slider->setValue(this->elapsedTime*100/this->duration);
    this->slider->blockSignals(false);
}

/**
 * @brief A slot to handle the signal launch by the player, when the currently read media duration has changed,
 * a duration null, the first one send by player for every media, is ignored.
 * @param duration the media duration, in milliseconds
 */

void StatusBar::durationChanged(qint64 duration){
    qDebug() << "duration = " << duration;
    if(duration==0) return;
    this->duration = duration/1000;
    this->elapsedTime = 0;
    this->remainingTime = this->duration;
//    this->timer->start();
    this->elapsedTimeLabel->setText(conversion(this->elapsedTime));
    this->remainingTimeLabel->setText(conversion(this->remainingTime));
    this->slider->setValue(0);
//    this->timer->start();
}

/**
 * @brief A slot to handle the pause signal of the MainWindow
 */

void StatusBar::pause(){ this->timer->stop(); }

/**
 * @brief A slot to handle the play signal of the MainWindow
 */

void StatusBar::start(){ this->timer->start(); qDebug() <<"start";}

/**
 * @brief A slot to handle the stop signal of the MainWindow, reseting the displayed times
 */

void StatusBar::reset(){
    qDebug() << "reset";
    this->timer->stop();
    this->elapsedTime = 0;
    this->remainingTime = this->duration;
    this->elapsedTimeLabel->setText(conversion(this->elapsedTime));
    this->remainingTimeLabel->setText(conversion(this->remainingTime));
    this->slider->setValue(0);
}

/**
 * @brief A slot to handle the valueChanged signal of the ClickableSlider.
 */

void StatusBar::valueChanged(){
    this->timer->stop();
    int value = this->slider->value();
    this->elapsedTime = (value*this->duration)/100;
    this->remainingTime = this->duration - this->elapsedTime;

    this->elapsedTimeLabel->setText(conversion(this->elapsedTime));
    this->remainingTimeLabel->setText(conversion(this->remainingTime));

    qint64 position = this->elapsedTime*1000;
    this->timer->start();
    qDebug() << "start";
    emit setPosition(position);
}

/**
 * @brief A slot to handle the signal send by the player, when the read media is changed
 * @param state the QMediaPlayer::State send by the player
 */

void StatusBar::stateChanged(QMediaPlayer::State state){
    if(state==QMediaPlayer::PlayingState){
        this->timer->start();
        qDebug() << "start";
    }
    else this->timer->stop();
}

/**
 * @brief A slot to handle the absence of any media read by the player
 */


void StatusBar::clear(void){
    this->timer->stop();
    this->remainingTime = this->duration = this->elapsedTime = 0;
    this->elapsedTimeLabel->setText(conversion(this->elapsedTime));
    this->remainingTimeLabel->setText(conversion(this->remainingTime));
    this->slider->setValue(0);
}
