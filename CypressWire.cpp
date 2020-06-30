#include "Arduino.h"
#include <CypressWire.h>

CypressWire Cwire;

/*
 * ChipKit uses I2C4 on PIC32
 *
 */

CypressWire::CypressWire()
{};

void CypressWire::Wait()
{
    // Wait for start condition to end      (bit 1)
    // Wait for restart condition to end    (bit 2)
    // Wait for stop condition to end       (bit 3)
    // Wait for receive sequence to end     (bit 4)
    // Wait for acknowledge sequence to end (bit 5)
    while(I2C4CON & 0x1F);

    while(I2C4STATbits.TRSTAT); // Bit = 0 ? Master transmit is not in progress
}

/******************************
 * void init(double)
 *
 * Desc: initialize I2Cbaud rate generator
 *
 */


void CypressWire::Init(double freq)
{
  // Disable I2C4
  I2C4CON = 0;

  // Disable Slew Rate Control (9th bit I2C4CON)
  I2C4CONbits.DISSLW = 1;

  // Magic math to set baud rate
  double baud;
  baud = (1 / (2 * freq)) - 0.000000104;
  baud *= (16000   / 2) - 2;

  // Set Baud Rate to 400kHz
  I2C4BRG = (int)baud;

  // Turn on I2C (15th bit I2C4CON)
  I2C4CONbits.ON = 1;
}



void CypressWire::Start()
{
  // Wait for everything to stop if something was happening before
  Wait();

  // Set Start Condition Enable Bit
  I2C4CONbits.SEN = 1;
  while (I2C4CON & 1);
}

void CypressWire::Stop()
{
  Wait();
  I2C4CONbits.PEN = 1;
}

void CypressWire::Write(unsigned char  addr, uint8_t wt)
{
  I2C4TRN = addr | 0;              // Send slave address with Read/Write bit cleared
  while (I2C4STATbits.TBF == 1);      // Wait until transmit buffer is empty
  Wait();                // Wait until I2C bus is idle
  if (wt) while (I2C4STATbits.ACKSTAT == 1); // Wait until ACK is received
}

void CypressWire::Write(int  addr, int wt)
{
  Write( (unsigned char) addr, (uint8_t) wt );
}

void CypressWire::Ack()
{
  Wait();
  I2C4CONbits.ACKDT = 0; // Set hardware to send ACK bit
  I2C4CONbits.ACKEN = 1; // Send ACK bit, will be automatically cleared by hardware when sent
  while(I2C4CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning ACK bit has been sent
}

void CypressWire::Nack()
{
  Wait();
  I2C4CONbits.ACKDT = 1; // Set hardware to send NACK bit
  I2C4CONbits.ACKEN = 1; // Send NACK bit, will be automatically cleared by hardware when sent
  while(I2C4CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning NACK bit has been sent
}



uint8_t CypressWire::Read(uint8_t ack_nack)
{
  uint8_t value;
    I2C4CONbits.RCEN = 1;               // Receive enable
    while (I2C4CONbits.RCEN);           // Wait until RCEN is cleared (automatic)
    while (!I2C4STATbits.RBF);          // Wait until Receive Buffer is Full (RBF flag)
    value = I2C4RCV | 0;                   // Retrieve value from I2C1RCV



    if (!ack_nack)                      // Do we need to send an ACK or a NACK?
        Ack();                      // Send ACK
    else
        Nack();                     // Send NACK
    return value;
}

void CypressWire::Restart()
{
  Wait();
  I2C4CONbits.RSEN = 1;
  while(I2C4CONbits.RSEN);
}

/*
Read from location "add".
Location is 17-bit value
*/
uint8_t CypressWire::SBread(uint32_t add)
{

  uint8_t value;

  // Get 16th bit
  uint8_t A16 = add >> 16;
  A16 &= 1;

  // Start byte is memory slave address with A16
  uint8_t begin_byte;
  begin_byte = 0b10100000;
  begin_byte |= A16 << 1;

  // Most Significant byte and Least Significant byte
  uint8_t ms, ls;

  ms = add >> 8;
  ls = add & 0xFF;

  // Random Address read (Page 13)
  Start();              // Start
  Write(begin_byte,1);  // Memory Slave Address ( Write )
  Write(ms, 1) ;        // Most Significant Address Byte
  Write(ls, 1);         // Least Significant Address Byte
  Restart();            // Repeated Start Condition
  Write(0b10100001,1);  // Memory Slave Address ( Read )
  value = Read(1);       // Read Data Byte
  Stop();               // Stop Condition

  return value;
}

void CypressWire::SBwrite(uint32_t add, uint8_t value)
{
  uint8_t A16 = add >> 16;
  A16 &= 1;

  uint8_t begin_byte;
  begin_byte = 0b10100000;
  begin_byte |= A16 << 1;

  uint8_t ms, ls;
  ms = add >> 8;
  ls = add & 0xFF;

  Start();               // Start
  Write(begin_byte,1);  // Memory Slave Address ( Write )
  Write(ms, 1) ;        // Most Significant Address Byte
  Write(ls, 1);         // Least Significant Address Byte
  Write(value, 1);      // Write value
  Stop();               // Stop Condition
}

viod CypressWire::memDump(uint32_t start, uint32_t stop)
{

}
