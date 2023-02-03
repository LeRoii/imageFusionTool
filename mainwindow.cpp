#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_x = m_y = 0;

    m_mapImg = cv::imread("/home/zpwang/data/hyxt/1.png");
    m_camImg = cv::imread("/home/zpwang/data/hyxt/cam.png");

    cv::cvtColor(m_mapImg, m_mapImg, cv::COLOR_RGB2BGR);
    cv::cvtColor(m_camImg, m_camImg, cv::COLOR_RGB2BGR);

    auto mapSz = m_mapImg.size();
    m_camSz = m_camImg.size();
    cv::resize(m_camImg, m_camImg, mapSz);

//    cv::addWeighted(camImg, 0.5, mapImg, 0.5, 0, ret);


    m_mapRoi = cv::Rect(0,0,mapSz.width,mapSz.height);
    m_camRoi = cv::Rect(0,0,m_camImg.size().width,m_camImg.size().height);
    m_setSz = m_fusionSz = m_camImg.size();


    ui->roiHhorizontalSlider->setMinimum(1);
    ui->roiHhorizontalSlider->setMaximum(mapSz.height);
    ui->roiWHorizontalSlider->setMinimum(1);
    ui->roiWHorizontalSlider->setMaximum(mapSz.width);

    ui->roiHhorizontalSlider->setValue(mapSz.height);
    ui->roiWHorizontalSlider->setValue(mapSz.width);

    ui->roiHLabel->setText("height:"+QString::number(ui->roiHhorizontalSlider->value()));
    ui->roiWLabel->setText("width:"+QString::number(ui->roiWHorizontalSlider->value()));


}

void MainWindow::fusion()
{
    auto camResize = m_camImg.clone();
    cv::resize(m_camImg, camResize, m_setSz);

    m_fusionSz = m_setSz;

    if(m_y <= 0)
    {
        m_y = m_y < -m_setSz.height ? 1-m_setSz.height : m_y;
        m_camRoi.y = -m_y;
        m_fusionSz.height = m_setSz.height - m_camRoi.y;
        m_mapRoi.y = 0;
    }
    else if(m_y > 0)
    {
        m_y = m_y > m_setSz.height ? m_setSz.height -1 : m_y;
        m_mapRoi.y = m_y;
        m_fusionSz.height = m_setSz.height - m_mapRoi.y;
    }

    if(m_x <= 0)
    {
        m_x = m_x < -m_setSz.width ? 1 - m_setSz.width : m_x;
        m_camRoi.x = -m_x;
        m_fusionSz.width = m_setSz.width - m_camRoi.x;
        m_mapRoi.x = 0;
    }
    else if(m_x > 0)
    {
        m_x = m_x > m_setSz.width ? m_setSz.width -1 : m_x;
        m_mapRoi.x = m_x;
        m_fusionSz.width = m_setSz.width - m_mapRoi.x;
    }

    qDebug("m_y:%d\n",m_y);

    m_camRoi.width = m_mapRoi.width = m_fusionSz.width;
    m_camRoi.height = m_mapRoi.height = m_fusionSz.height;

    cv::addWeighted(camResize(m_camRoi), 0.5, m_mapImg(m_mapRoi), 0.5, 0, m_fusionRet);
    m_final = m_mapImg.clone();
    m_fusionRet.copyTo(m_final(m_mapRoi));

    QImage Qtemp = QImage((const unsigned char*)(m_final.data), m_final.cols, m_final.rows, m_final.cols*m_final.channels(), QImage::Format_RGB888);
    QImage newQtemp = Qtemp.scaled(ui->imgLabel->size());

    ui->imgLabel->setPixmap(QPixmap::fromImage(newQtemp));

    QImage Qfusionrettemp = QImage((const unsigned char*)(m_fusionRet.data), m_fusionRet.cols, m_fusionRet.rows, m_fusionRet.cols*m_fusionRet.channels(), QImage::Format_RGB888);
    QImage newfusionretQtemp = Qfusionrettemp.scaled(ui->fusionretlabel->size());
    ui->fusionretlabel->setPixmap(QPixmap::fromImage(newfusionretQtemp));

//    cv::imshow("ret",m_fusionRet);
//    cv::imshow("map",m_final);
//    cv::waitKey(0);

//    qDebug("size:%d,%d\n",  m_final.cols, m_final.rows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_roiWHorizontalSlider_valueChanged(int value)
{
    ui->roiWLabel->setText("width:"+QString::number(value));
    m_setSz.width = value;


    fusion();
}

void MainWindow::on_roiHhorizontalSlider_valueChanged(int value)
{
    ui->roiHLabel->setText("height:"+QString::number(value));
    m_setSz.height = value;

    fusion();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_W:
            qDebug("keyPressEvent:w\n");
            m_y--;
            break;
        case Qt::Key_A:
            qDebug("keyPressEvent:a\n");
            m_x--;
            break;
        case Qt::Key_S:
            qDebug("keyPressEvent:s\n");
            m_y++;
            break;
        case Qt::Key_D:
            qDebug("keyPressEvent:d\n");
            m_x++;
            break;
        default:
        break;
    }
    fusion();
}
