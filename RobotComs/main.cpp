#include "mbed.h"
#include "nRF24L01P.h"
#include <cstdio>

nRF24L01P my_nrf24l01p(D11, D12, D13,D2,D4,D7);    // mosi, miso, sck, csn, ce, irq

AnalogIn xin(A3);
AnalogIn yin(A2);
AnalogIn accept(A4);

int gx,gy,st;

int TrCounter=0;

int main(){
    #define TRANSFER_SIZE   3
    char txData[TRANSFER_SIZE];
    int txDataCnt = 0;
    my_nrf24l01p.powerUp();
    my_nrf24l01p.setRfOutputPower(-6);
    my_nrf24l01p.setTxAddress((0x1F22676D90),DEFAULT_NRF24L01P_ADDRESS_WIDTH);
    my_nrf24l01p.setAirDataRate(2000);
    // Display the (default) setup of the nRF24L01+ chip
    printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    my_nrf24l01p.setTransferSize( TRANSFER_SIZE );
    my_nrf24l01p.enable();
    while (1) {
        gx = 0;
        gy = 0;
        st = 2;
        if (xin<0.1f) gx = 0;
        else if (xin < 0.2f) gx = 1;
        else if (xin < 0.3f) gx = 2;
        else if (xin < 0.4) gx = 3;
        else if (xin < 0.5f) gx = 4;
        else if (xin < 0.6f) gx = 5;
        else if (xin < 0.7f) gx = 6;
        else if (xin >= 0.7f) gx = 7;
        if (yin < 0.1f) gy = 0;
        else if (yin < 0.2f) gy = 1;
        else if (yin < 0.3f) gy = 2;
        else if (yin < 0.4f) gy = 3;
        else if (yin < 0.5f) gy = 4;
        else if (yin < 0.6f) gy = 5;
        else if (yin < 0.7f) gy = 6;
        else if (yin >=0.7f) gy = 7;
        if (accept > 0.7f) st = 1;

        txData[0] = st;
        txData[1] = gy;
        txData[2] = gx;
        wait_us(1000000);
        txDataCnt = 3;
        printf("Start? = %d\n",txData[0]);
        printf( "delta x = %d\n",txData[2] );
        printf( "delta y = %d\n",txData[1] );
        st = 2;
           if ( txDataCnt >= sizeof( txData ) && txData[0] == 1)
            {
               // Send the transmitbuffer via the nRF24L01+
              
                TrCounter=my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt );
               // wait_us(500000);
                printf("sending data of size %d\n", TrCounter);
                printf("Sent\n");
                //Toggle LED1 (to help debug Host -> nRF24L01+ communication)
               // myled1 = !myled1;
               wait_us(10000000);
            }    
    }
}