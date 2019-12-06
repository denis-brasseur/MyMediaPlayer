#ifndef STATUSBAR_H
#define STATUSBAR_H

#include<QLayout>
#include<QLabel>
#include<QSlider>
#include<QMouseEvent>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer>
#include<QString>

#include <QDebug>


/**
 * @brief A simple reimplementation of the QSlider class,
 *  where clicking on the slider will change the returned value
 */

class ClickableSlider : public QSlider{
public:
    ClickableSlider(QWidget *parent=nullptr);
    void mousePressEvent(QMouseEvent *event);
};

/**
 * @brief a concatenation of 2 QLabels and a ClickableSlider,
 * in order to display the progression of the played media,
 * and the signals and slots to handle it
 */


class StatusBar : public QWidget
{

    Q_OBJECT

public:
    StatusBar(QWidget *parent = nullptr);
    void setSlider(ClickableSlider *slider);
    void setElapsedTimeLabel(QLabel *label);
    void setRemainingTimeLabel(QLabel *label);

signals:
    /**
     * @brief A signal send to the player to change the second played in the currently read media
     * @param the new second to play
     */
    void setPosition(qint64 newPosition);

public slots:
//    void mediaChanged(const QMediaContent& media);
    void durationChanged(qint64 duration);
    void elapsedSecond(void);
    void start(void);
    void pause(void);
    void reset(void);
    void valueChanged(void);
    void stateChanged(QMediaPlayer::State state);
    void clear(void);

private:
    QLabel *elapsedTimeLabel, *remainingTimeLabel;
    qint64 duration, elapsedTime, remainingTime;
    ClickableSlider* slider;
    QTimer* timer;
//    bool isMediaChanged, isStopButtonPush;
};

#endif // STATUSBAR_H
