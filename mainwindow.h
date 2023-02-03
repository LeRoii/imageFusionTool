#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<opencv2/opencv.hpp>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fusion();

protected:
    void keyPressEvent(QKeyEvent *event);


private slots:
    void on_roiWHorizontalSlider_valueChanged(int value);

    void on_roiHhorizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    cv::Rect m_mapRoi, m_camRoi;
    cv::Mat m_mapImg, m_camImg, m_fusionRet, m_final;
    int m_x, m_y;
    cv::Size m_fusionSz, m_setSz, m_camSz;
};

#endif // MAINWINDOW_H
