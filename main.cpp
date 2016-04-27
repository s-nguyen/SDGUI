
#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "MyLabel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //create the app


        QWidget *myWidget = new QWidget;
        myWidget->setGeometry(400, 300, 680, 580);

        //create an image placeholder for myLabel
        //fill the top left corner with red, just bcuz
        QImage myImage;
        myImage = QImage(640, 480, QImage::Format_RGB888);
        QRgb red = qRgb(255,0,0);
        for(int i=0;i<80;i++) {
            for(int j=0;j<60;j++) {
                myImage.setPixel(i, j, red);
            }
        }

        //create a label, and set it's image to the placeholder

        MyLabel myLabel(myWidget);
        myLabel.setGeometry(10, 10, 640, 480);
        myLabel.setPixmap(QPixmap::fromImage(myImage));

        //create a FFC button
        QPushButton *button1 = new QPushButton("Perform FFC", myWidget);
        button1->setGeometry(640/2-50, 580-50, 100, 30);

        LeptonThread *thread = new LeptonThread();
        QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));

        thread->start();

        myWidget->show();



    return a.exec();
}
