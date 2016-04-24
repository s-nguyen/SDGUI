#include "LeptonThread.h"
#include "uart_socket.h"
#include "Palettes.h"
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

     CUARTSocket a("/dev/ttyACM0", 115200);
     a.Open();


    while(true) {

        //read data packets from lepton over SPI
        //Store transfer data in result

       //std::cout <<  a.Read(result, 9840) << std::endl;
        int t1 = 0;
        int t2 = 0;
        int t3 = 0;
        uint8_t temp1[4000] = {0} ;

        uint8_t temp2[4000] = {0} ;

        uint8_t temp3[1840] = {0} ;
        while(1){
            a.Wait();
            t1 = a.Read(temp1, 4000);
            usleep(20);

            t2 = a.Read(temp2, 4000);
            usleep(20);
            t3 = a.Read(temp3, 1840);
            usleep(20);
            if(t1 != 0)
                std::cout << t1 << std::endl;
            if(t2 != 0)
                std::cout << t2 << std::endl;
            if(t3 != 0)
                std::cout << t3 << std::endl;
            if(t1+t2+t3 == 9840) {
                break;
            }
            else {
                usleep(100);
            }
        }




        if(t1+t2+t3 == 9840) {
            for(int i = 0; i < 9840; i++){
                if(i < 4000){
                    result[i] = temp1[i];
                }
                else if(i < 8000) {
                    result[i] = temp2[i-4000];
                }
                else {
                    result[i] = temp3[i-8000];
                }
            }
        }




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
        a.Flush();
        //a.Close();

    }


}
