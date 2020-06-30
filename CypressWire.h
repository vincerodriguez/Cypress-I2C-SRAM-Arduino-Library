#ifndef CYPRESSWIRE_H
#define CYPRESSWIRE_H

#include "Arduino.h"

class CypressWire
{
  public:

  // Constructor
  CypressWire();

  // Public Methods
  void Init(double);
  uint8_t SBread(uint32_t);
  void SBwrite(uint32_t, uint8_t);
  void memDump(uint32_t);


private:
  void Wait();
  void Start();
  void Stop();
  void Restart();
  void Ack();
  void Nack();
  void Write(unsigned char, uint8_t);
  void Write(int, int);
  uint8_t Read(uint8_t);

};

extern CypressWire Cwire;



#endif
