
#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

const unsigned int MAX_X = 512;
const unsigned int MAX_Y = 512;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.graphicsView->setScene(&scene);
}

void MainWindow::closeEvent(QCloseEvent* e)
{
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    scene.clear();

    uchar rData[MAX_X * MAX_Y] = {};
    uchar gData[MAX_X * MAX_Y] = {};
    uchar bData[MAX_X * MAX_Y] = {};
    makeColorBar(rData, gData, bData, MAX_X, MAX_Y);
    unsigned char *pR = &rData[0];
    unsigned char *pG = &gData[0];
    unsigned char *pB = &bData[0];
    QImage image(MAX_X, MAX_Y, QImage::Format_RGB32);
    for (int i = 0; i < MAX_Y; i++)
    {
        for (int j = 0; j < MAX_X; j++)
        {
            int r = (int)*(pR++);
            int g = (int)*(pG++);
            int b = (int)*(pB++);
            image.setPixel(j, i, qRgb(r, g, b));
        }
    }

    QGraphicsPixmapItem *image_item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene.addItem(image_item);
    ui.graphicsView->setScene(&scene);
}

void MainWindow::makeColorBar(unsigned char* rimage
                      , unsigned char* gimage
                      , unsigned char* bimage
                      , int xsize
                      , int ysize)
{
    // rimage, gimage, bimage is a one-dimensional array.
    // If the data in the first column is written, the data in the second column must be written xsize(=512) bytes ahead.

    // Since there are 8 different colors, divide into 8 parts.
    int width = xsize / 8;
    // Fix the y direction and write data in the x direction.
    // 'i' is y direction
    for (int i = 0; i < ysize; i++)
    {
        // 'j' is x direction
        for (int j = 0; j < xsize; j++)
        {
            // r
            if (((j >= 0) && (j < 2 * width)) ||
                ((j >= 4 * width) && (j < 6 * width)))
            {
                rimage[i * xsize + j] = 255;
            }
            else
            {
                rimage[i * xsize + j] = 0;
            }

            // g
            if ((j >= 0) && (j < 4 * width))
            {
                gimage[i * xsize + j] = 255;
            }
            else
            {
                gimage[i * xsize + j] = 0;
            }

            // b
            if (((j >= 0) && (j < width)) ||
                ((j >= 2 * width) && (j < 3 * width)) ||
                ((j >= 4 * width) && (j < 5 * width)) ||
                ((j >= 6 * width) && (j < 7 * width)))
            {
                bimage[i * xsize + j] = 255;
            }
            else
            {
                bimage[i * xsize + j] = 0;
            }
        }
    }
}
