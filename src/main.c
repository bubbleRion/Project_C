// SHT 5초마다 온습도 측정 timer0 5초 주기로 계산
// EEPROM 주소 0100 온도 0200 습도
// 측정 실패는 error로 성공은 EEPROM에 저장
// INT4로 EEPROM에 데이터 읽어 UART 출력

#include "SHT2x.h"
#include "TWI_driver.h"
#include "lcd.h"
#include "uart0.h"
#include "at25160.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void printf_2dot1(uint8_t sense, uint16_t sense_temp);

uint16_t temperaturC, humidityRH;

volatile uint8_t count;
volatile uint8_t readflag = 1, txflag = 0;

ISR(TIMER0_OVF_vect);
ISR(INT4_vect);
void lcdTempHumid();

int main(void)
{
    SPI_Init();

    Init_TWI();
    SHT2x_Init();

    uart0Init();

    lcdInit();

    DDRE = _BV(PE1); // 0x02

    EICRB = 0x03;
    EIMSK = 0x10;

    TCCR0 = _BV(CS02) | _BV(CS01) | _BV(CS00);
    TIMSK |= _BV(TOIE0);

    sei();
    stdout = &OUTPUT;

    while (1)
    {
        if (count == 0)
        {
            SHT2x_MeasureHM(TEMP, (nt16 *)&temperaturC);
            SHT2x_MeasureHM(HUMIDITY, (nt16 *)&humidityRH);
            temperaturC = SHT2x_CalcTemperatureC(temperaturC) * 10;
            humidityRH = SHT2x_CalcRH(humidityRH) * 10;

            at25160_Write_Arry(0x0100, (uint8_t *)&temperaturC, 2);
            at25160_Write_Arry(0x0200, (uint8_t *)&humidityRH, 2);
            lcdTempHumid();
        }
        if (txflag)
        {
            // eeprom read -> uart printf();
            at25160_Read_Arry(0x0100, (uint8_t *)&temperaturC, 2);
            at25160_Read_Arry(0x0200, (uint8_t *)&humidityRH, 2);
            uart0PrintString("\n\rTemp: ");
            printf("%u.%u", temperaturC / 10, temperaturC % 10);
            uart0PrintString("\n\rHumi: ");
            printf("%u.%u", humidityRH / 10, humidityRH % 10);
            txflag = 0;
        }
    }
    return 0;
}

void printf_2dot1(uint8_t sense, uint16_t sense_temp)
{
    uint8_t s100, s10;
    if (sense == TEMP)
    {
        lcdPrintData("Temp : ", 7);

        s100 = sense_temp / 100;
        if (s100 > 0)
            lcdDataWrite(s100 + '0');

        s10 = sense_temp / 10 - s100 * 10;
        if (s10 > 0)
            lcdDataWrite(s10 + '0');
        lcdDataWrite('.');
        lcdDataWrite(sense_temp % 10 + '0');
        lcdDataWrite('C');
    }
    else if (sense == HUMIDITY)
    {
        lcdPrintData("Humi : ", 7);
        s100 = sense_temp / 100;
        if (s100 > 0)
            lcdDataWrite(s100 + '0');
        s10 = sense_temp / 10 - s100 * 10;
        if (s10 > 0)
            lcdDataWrite(s10 + '0');
        lcdDataWrite('.');
        lcdDataWrite(sense_temp % 10 + '0');
        lcdDataWrite('%');
    }
}

void lcdTempHumid()
{
    nt16 sRH;
    nt16 sT;
    uint8_t error;

    error |= SHT2x_MeasureHM(HUMIDITY, &sRH);
    error |= SHT2x_MeasureHM(TEMP, &sT);
    temperaturC = SHT2x_CalcTemperatureC(sT.u16) * 10;
    humidityRH = SHT2x_CalcRH(sRH.u16) * 10;
    if (error == SUCCESS)
    {
        lcdGotoXY(0, 0);
        printf_2dot1(TEMP, temperaturC);
        lcdGotoXY(0, 1);
        printf_2dot1(HUMIDITY, humidityRH);
    }
    else
    {
        lcdGotoXY(0, 0);
        lcdPrintData(" Temp : --.-C", 12);
        lcdGotoXY(0, 1);
        lcdPrintData(" Humi : --.-%%", 12);
    }
}

ISR(TIMER0_OVF_vect)
{
    cli();
    count++;
    if (count == 320)
    {
        count = 0;
    }
    sei();
}

ISR(INT4_vect)
{
    txflag = 1;
}