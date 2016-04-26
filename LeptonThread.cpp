#include "LeptonThread.h"

#include "Palettes.h"
#include "TCP_Client.h"
#include <iostream>
//#include "Lepton_I2C.h"

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
#define FPS 27;

LeptonThread::LeptonThread() : QThread()
{
}

LeptonThread::~LeptonThread() {
}

void LeptonThread::run()
{
    //create the initial image
    myImage = QImage(80, 60, QImage::Format_RGB888);
    TCP_Client datapipe;
    datapipe.tcp_init();



    while(true) {
        datapipe.tcp_read(result);

        frameBuffer = (uint16_t *)result;
        int row, column;
        uint16_t value;
        uint16_t minValue = 65535;
        uint16_t maxValue = 0;


        for(int i=0;i<FRAME_SIZE_UINT16;i++) {
            //skip the first 2 uint16_t's of every packet, they're 4 header bytes
            if(i % PACKET_SIZE_UINT16 < 2) {
                continue;
            }

            //flip the MSB and LSB at the last second
            int temp = result[i*2];
            result[i*2] = result[i*2+1];
            result[i*2+1] = temp;

            value = frameBuffer[i];
            if(value > maxValue) {
                maxValue = value;
            }
            if(value < minValue) {
                minValue = value;
            }
            column = i % PACKET_SIZE_UINT16 - 2;
            row = i / PACKET_SIZE_UINT16 ;
        }

        float diff = maxValue - minValue;
        float scale = 255/diff;
        QRgb color;
        for(int i=0;i<FRAME_SIZE_UINT16;i++) {
            if(i % PACKET_SIZE_UINT16 < 2) {
                continue;
            }
            value = (frameBuffer[i] - minValue) * scale;
            const int *colormap = colormap_ironblack;
            color = qRgb(colormap[3*value], colormap[3*value+1], colormap[3*value+2]);
            column = (i % PACKET_SIZE_UINT16 ) - 2;
            row = i / PACKET_SIZE_UINT16;
            myImage.setPixel(column, row, color);
        }

        //lets emit the signal for update
        emit updateImage(myImage);


    }


}
