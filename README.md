# Cypress-I2C-SRAM-Arduino-Library
Library to read and write to Cypress CYB101J


HOW TO USE CypressWire

Example: 

#include <CypressWire.h> 



uint8_t value; 

setup()
{
	Cwire.Init(1); 
}

loop()
{
	value = Cwire.SBread(0x00000);
}	


Functions: 
Init() - Must be called before any communication with SRAM 

SBread() 	Single-Byte read. 
		Parameters: 32-bit address, only use least significant 17 bits
		Returns: 8--bit contents of SRAM at location given

SBwrite()	Single-Byte write. 
		Parameters: 32-bit address, only use least significant 17 bits
		            8-bit data to write to address
		Returns: Nothing

memDump() 	Dump Conents of memory to serial monitor, stops at address specified
		Parameters: 32-bit stopping address, only use least significant 17 bits

STORE() 	Refer to datasheet
RECALL()	Refer to datasheet
ASENB() 	Refer to datasheet
ASDISB() 	Refer to datasheet
SLEEP() 	Refer to datasheet
