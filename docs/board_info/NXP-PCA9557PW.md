## 1. General description

The PCA9557 is a silicon CMOS circuit which provides parallel input/output expansion for SMBus and I²C-bus applications. The PCA9557 consists of an 8-bit input port register, 8-bit output port register, and an I²C-bus/SMBus interface. It has low current consumption and a high-impedance open-drain output pin, IO0. 

The system master can enable the PCA9557's I/O as either input or output by writing to the configuration register. The system master can also invert the PCA9557 inputs by writing to the active HIGH polarity inversion register. Finally, the system master can reset the PCA9557 in the event of a time-out by asserting a LOW in the reset input. 

The power-on reset puts the registers in their default state and initializes the I²C-bus/SMBus state machine. The RESET pin causes the same reset/initialization to occur without de-powering the part. 

## 2. Features and benefits

■ Lower voltage, higher performance migration path for the PCA9556 

■ 8 general purpose input/output expander/collector 

■ Input/output configuration register 

■ Active HIGH polarity inversion register 

■ I²C-bus and SMBus interface logic 

Internal power-on reset 

■ Noise filter on SCL/SDA inputs 

■ Active LOW reset input 

■ 3 address pins allowing up to 8 devices on the I²C-bus/SMBus 

■ High-impedance open-drain on IO0 

No glitch on power-up 

■ Power-up with all channels configured as inputs 

Low standby current 

- Operating power supply voltage range of 2.3 V to 5.5 V 

■ 5 V tolerant inputs/outputs 

■ 0 kHz to 400 kHz clock frequency 

■ ESD protection exceeds 2000 V HBM per JESD22-A114 and 1000 V CDM per JESD22-C101 

Latch-up testing is done to JEDEC Standard JESD78 which exceeds 100 mA 

Three packages offered: SO16, TSSOP16, HVQFN16 

## 3. Ordering information


Table 1. Ordering information


<table><tr><td rowspan="2">Type number</td><td rowspan="2">Topside marking</td><td colspan="3">Package</td></tr><tr><td>Name</td><td>Description</td><td>Version</td></tr><tr><td>PCA9557BS</td><td>9557</td><td>HVQFN16</td><td>plastic thermal enhanced very thin quad flat package; no leads; 16 terminals; body <eq>4 \times 4 \times 0.85</eq> mm</td><td>SOT629-1</td></tr><tr><td>PCA9557D</td><td>PCA9557D</td><td>SO16</td><td>plastic small outline package; 16 leads; body width 3.9 mm</td><td>SOT109-1</td></tr><tr><td>PCA9557PW</td><td>PCA9557</td><td>TSSOP16</td><td>plastic thin shrink small outline package; 16 leads; body width 4.4 mm</td><td>SOT403-1</td></tr></table>

## 3.1 Ordering options


Table 2. Ordering options


<table><tr><td>Type number</td><td>Orderable part number</td><td>Package</td><td>Packing method</td><td>Minimum order quantity</td><td>Temperature range</td></tr><tr><td>PCA9557BS</td><td>PCA9557BS,118</td><td>HVQFN16</td><td>Reel 13” Q1/T1*Standard mark SMD</td><td>6000</td><td><eq>T_{amb} = -40 °C to +85 °C</eq></td></tr><tr><td rowspan="2">PCA9557D</td><td>PCA9557D,112</td><td>SO16</td><td>Standard marking* IC&#x27;s tube - DSC bulk pack</td><td>1000</td><td><eq>T_{amb} = -40 °C to +85 °C</eq></td></tr><tr><td>PCA9557D,118</td><td>SO16</td><td>Reel 13” Q1/T1*Standard mark SMD</td><td>2500</td><td><eq>T_{amb} = -40 °C to +85 °C</eq></td></tr><tr><td rowspan="2">PCA9557PW</td><td>PCA9557PW,112</td><td>TSSOP16</td><td>Standard marking* IC&#x27;s tube - DSC bulk pack</td><td>2400</td><td><eq>T_{amb} = -40 °C to +85 °C</eq></td></tr><tr><td>PCA9557PW,118</td><td>TSSOP16</td><td>Reel 13” Q1/T1*Standard mark SMD</td><td>2500</td><td><eq>T_{amb} = -40 °C to +85 °C</eq></td></tr></table>

## 4. Block diagram

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/9ef632d9442669d7cdbb480f2d3df2f9dcc41d784979e51416a1c6ba23019149.jpg)



Fig 1. Block diagram of PCA9557


8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/082522e0e833cb49adaf8b532290a4d67690edcddffd26934d2b86d7c52c40fc.jpg)



Fig 2. Simplified schematic of IOO


8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/859caf58b755debfe9f2b9521f3b70a81b2e5ba2669e471bcc80ac3254eff56f.jpg)



Fig 3. Simplified schematic of IO1 to IO7


## 5. Pinning information

## 5.1 Pinning

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/b9b2304d5637d4dc7c3cb9e3f9dc4393cf9970e88372b81da2c313bb7d15f972.jpg)



Fig 4. Pin configuration for SO16


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/8c75aecb17deac3ceb9f11d703d1add945839e9f98406225d4473b3362a18209.jpg)



Fig 5. Pin configuration for TSSOP16


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/b773dcb70c7a78661403272a0f0ffdb1c4ac4fc33933de8e874c507d976703e0.jpg)



Fig 6. Pin configuration for HVQFN16


## 5.2 Pin description


Table 3. Pin description


<table><tr><td rowspan="2">Symbol</td><td colspan="2">Pin</td><td rowspan="2">Description</td></tr><tr><td>SO16, TSSOP16</td><td>HVQFN16</td></tr><tr><td>SCL</td><td>1</td><td>15</td><td>serial clock line</td></tr><tr><td>SDA</td><td>2</td><td>16</td><td>serial data line</td></tr><tr><td>A0</td><td>3</td><td>1</td><td>address input 0</td></tr><tr><td>A1</td><td>4</td><td>2</td><td>address input 1</td></tr><tr><td>A2</td><td>5</td><td>3</td><td>address input 2</td></tr><tr><td>IO0</td><td>6</td><td>4</td><td>input/output 0 (open-drain)</td></tr><tr><td>IO1</td><td>7</td><td>5</td><td>input/output 1</td></tr><tr><td><eq>V_{SS}</eq></td><td>8</td><td>6[1]</td><td>supply ground</td></tr><tr><td>IO2</td><td>9</td><td>7</td><td>input/output 2</td></tr><tr><td>IO3</td><td>10</td><td>8</td><td>input/output 3</td></tr><tr><td>IO4</td><td>11</td><td>9</td><td>input/output 4</td></tr><tr><td>IO5</td><td>12</td><td>10</td><td>input/output 5</td></tr><tr><td>IO6</td><td>13</td><td>11</td><td>input/output 6</td></tr><tr><td>IO7</td><td>14</td><td>12</td><td>input/output 7</td></tr><tr><td><eq>\overline{RESET}</eq></td><td>15</td><td>13</td><td>active LOW reset input</td></tr><tr><td><eq>V_{DD}</eq></td><td>16</td><td>14</td><td>supply voltage</td></tr></table>


[1] HVQFN16 package die supply ground is connected to both the $V_{SS}$ pin and the exposed center pad. The $V_{SS}$ pin must be connected to the supply ground for proper device operation. For enhanced thermal, electrical, and board-level performance, the exposed pad needs to be soldered to the board using a corresponding thermal pad on the board, and for proper heat conduction through the board thermal vias need to be incorporated in the PCB in the thermal pad region. 


## 6. System diagram

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/cfd21a9a42a30a5c2a8509dddb9ce36c5649f11f540f5215de758e715c87a4dc.jpg)



Fig 7. System diagram


## 7. Functional description

Refer to Figure 1 "Block diagram of PCA9557". 

## 7.1 Device address

Following a START condition the bus master must output the address of the slave it is accessing. The address of the PCA9557 is shown in Figure 8. To conserve power, no internal pull-up resistors are incorporated on the hardware selectable address pins and they must be pulled HIGH or LOW. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/36c90c8430bbb7c0ac14c60c2f10497ed5f9cac08061338bdaa566b64a1669b9.jpg)



Fig 8. PCA9557 device address


The last bit of the slave address defines the operation to be performed. When set to logic 1 a read is selected, while a logic 0 selects a write operation. 

## 7.2 Control register

Following the successful acknowledgement of the slave address, the bus master will send a byte to the PCA9557, which will be stored in the control register. This register can be written and read via the I $^{2}$ C-bus. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/8299ffe93da0587fe52b0481a2c8bd23dc7181a2e737a5fd53273849dc8d19b2.jpg)



Fig 9. Control register



Table 4. Register definition


<table><tr><td>D1</td><td>D0</td><td>Name</td><td>Access</td><td>Description</td></tr><tr><td>0</td><td>0</td><td>Register 0</td><td>read-only</td><td>Input port register</td></tr><tr><td>0</td><td>1</td><td>Register 1</td><td>read/write</td><td>Output port register</td></tr><tr><td>1</td><td>0</td><td>Register 2</td><td>read/write</td><td>Polarity inversion register</td></tr><tr><td>1</td><td>1</td><td>Register 3</td><td>read/write</td><td>Configuration register</td></tr></table>

## 7.3 Register descriptions

## 7.3.1 Register 0 - Input port register

This register is a read-only port. It reflects the incoming logic levels of the pins, regardless of whether the pin is defined as an input or an output by the Configuration register. Writes to this register have no effect. 


Table 5. Register 0 - Input port register bit allocation


<table><tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr><tr><td>Symbol</td><td>I7</td><td>I6</td><td>I5</td><td>I4</td><td>I3</td><td>I2</td><td>I1</td><td>I0</td></tr></table>

## 7.3.2 Register 1 - Output port register

This register reflects the outgoing logic levels of the pins defined as outputs by the Configuration register. Bit values in this register have no effect on pins defined as inputs. In turn, reads from this register reflect the value that is in the flip-flop controlling the output selection, not the actual pin value. 


Table 6. Register 1 - Output port register bit allocation


<table><tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr><tr><td>Symbol</td><td>O7</td><td>O6</td><td>O5</td><td>O4</td><td>O3</td><td>O2</td><td>O1</td><td>O0</td></tr><tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr></table>

## 7.3.3 Register 2 - Polarity inversion register

This register enables polarity inversion of pins defined as inputs by the Configuration register. If a bit in this register is set (written with logic 1), the corresponding port pin's polarity is inverted. If a bit in this register is cleared (written with logic 0), the corresponding port pin's original polarity is retained. 


Table 7. Register 2 - Polarity inversion register bit allocation


<table><tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr><tr><td>Symbol</td><td>N7</td><td>N6</td><td>N5</td><td>N4</td><td>N3</td><td>N2</td><td>N1</td><td>N0</td></tr><tr><td>Default</td><td>1</td><td>1</td><td>1</td><td>1</td><td>0</td><td>0</td><td>0</td><td>0</td></tr></table>

## 7.3.4 Register 3 - Configuration register

This register configures the directions of the I/O pins. If a bit in this register is set, the corresponding port pin is enabled as an input with high-impedance output driver. If a bit in this register is cleared, the corresponding port pin is enabled as an output. 


Table 8. Register 3 - Configuration register bit allocation


<table><tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr><tr><td>Symbol</td><td>C7</td><td>C6</td><td>C5</td><td>C4</td><td>C3</td><td>C2</td><td>C1</td><td>C0</td></tr><tr><td>Default</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td><td>1</td></tr></table>

8-bit I²C-bus and SMBus I/O port with reset 

## 7.4 Power-on reset

When power is applied to $V_{DD}$ , an internal Power-On Reset (POR) holds the PCA9557 in a reset condition until $V_{DD}$ has reached $V_{POR}$ . At that point, the reset condition is released and the PCA9557 registers and I²C-bus/SMBus state machine will initialize to their default states. Thereafter, $V_{DD}$ must be lowered below 0.2 V to reset the device. 

## 7.5 RESET input

A reset can be accomplished by holding the $\overline{\mathrm{RESET}}$ pin LOW for a minimum of $t_{w(rst)}$ . The PCA9557 registers and SMBus/I²C-bus state machine will be held in their default state until the $\overline{\mathrm{RESET}}$ input is once again HIGH. This input requires a pull-up resistor to $V_{DD}$ if no active connection is used. 

## 8. Characteristics of the I²C-bus

The I $^{2}$ C-bus is for 2-way, 2-line communication between different ICs or modules. The two lines are a serial data line (SDA) and a serial clock line (SCL). Both lines must be connected to a positive supply via a pull-up resistor when connected to the output stages of a device. Data transfer may be initiated only when the bus is not busy. 

## 8.1 Bit transfer

One data bit is transferred during each clock pulse. The data on the SDA line must remain stable during the HIGH period of the clock pulse as changes in the data line at this time will be interpreted as control signals (see Figure 10). 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/9657dc8e57b829da79f57ce6c6859a08aad24b01be050ce8be04ff28916a4c5a.jpg)



Fig 10. Bit transfer


## 8.1.1 START and STOP conditions

Both data and clock lines remain HIGH when the bus is not busy. A HIGH-to-LOW transition of the data line while the clock is HIGH is defined as the START condition (S). A LOW-to-HIGH transition of the data line while the clock is HIGH is defined as the STOP condition (P) (see Figure 11). 

8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/0a026590fc0a22b3b6b55c4db582d1724cfc222cd4238f0ef2214c68c6cd749b.jpg)



Fig 11. Definition of START and STOP conditions


## 8.2 System configuration

A device generating a message is a ‘transmitter’; a device receiving is the ‘receiver’. The device that controls the message is the ‘master’ and the devices which are controlled by the master are the ‘slaves’ (see Figure 12). 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/701fdbdeaaf3431356cf84140566f4106a974d26701e4f0a2247850fc3c46a07.jpg)



Fig 12. System configuration


## 8.3 Acknowledge

The number of data bytes transferred between the START and the STOP conditions from transmitter to receiver is not limited. Each byte of eight bits is followed by one acknowledge bit. The acknowledge bit is a HIGH level put on the bus by the transmitter, whereas the master generates an extra acknowledge related clock pulse. 

A slave receiver which is addressed must generate an acknowledge after the reception of each byte. Also a master must generate an acknowledge after the reception of each byte that has been clocked out of the slave transmitter. The device that acknowledges has to pull down the SDA line during the acknowledge clock pulse, so that the SDA line is stable LOW during the HIGH period of the acknowledge related clock pulse; set-up and hold times must be taken into account. 

A master receiver must signal an end of data to the transmitter by not generating an acknowledge on the last byte that has been clocked out of the slave. In this event, the transmitter must leave the data line HIGH to enable the master to generate a STOP condition. 

8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/dcf1e2958e3d55aacb3f58984a106bb401bddc3f7ab7da63a7968fb5e851ea5e.jpg)



Fig 13. Acknowledgement on the I²C-bus


## 8.4 Bus transactions

Data is transmitted to the PCA9557 registers using Write Byte transfers (see Figure 14 and Figure 15). Data is read from the PCA9557 registers using Read and Receive Byte transfers (see Figure 16 and Figure 17). 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/1ca7ee4156748f1863ff566a67c52e9d7ad2686da0e2ca44becfacaf5c1a431c.jpg)



Fig 14. Write to output port register


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/96bde4ae6ad08b412b835a9b1f438174bdd6aa42476f94ccce568c452912d235.jpg)



Fig 15. Write to I/O configuration or polarity inversion registers


8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/3514cdd6a72b133b9429c06c591a3dc653e41102768f03fbb4add2edcdd71d2c.jpg)



Fig 16. Read from register


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/ce702d67f53ae37e6b9a304b67b4eb1fa64c56c81abace483ee00129ff84be4a.jpg)



Remark: This figure assumes the command byte has previously been programmed with 00h.



Transfer of data can be stopped at any moment by a STOP condition. When this occurs, data present at the last acknowledge phase is valid (output mode). Input data is lost.


Fig 17. Read input port register 

## 9. Application design-in information

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/ddd4bfc1a8b521abb5347b4e5573f4e8d4979ffffe4502255557eb711d077870.jpg)



Device address configured as 0011 100x for this example.



IO0, IO2, IO3 configured as outputs.



IO1, IO4, IO5 configured as inputs.



IO6, IO7 are not used.


Fig 18. Typical application 

## 9.1 Minimizing $I_{DD}$ when the I/Os are used to control LEDs

When the I/Os are used to control LEDs, they are normally connected to $V_{DD}$ through a resistor as shown in Figure 18. Since the LED acts as a diode, when the LED is off the I/O $V_{I}$ is about 1.2 V less than $V_{DD}$ . The supply current, $I_{DD}$ , increases as $V_{I}$ becomes lower than $V_{DD}$ . 

Designs needing to minimize current consumption, such as battery power applications, should consider maintaining the I/O pins greater than or equal to $V_{DD}$ when the LED is off. Figure 19 shows a high value resistor in parallel with the LED. Figure 20 shows $V_{DD}$ less than the LED supply voltage by at least 1.2 V. Both of these methods maintain the I/O $V_{I}$ at or above $V_{DD}$ and prevents additional supply current consumption when the LED is off. 

8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/2499d918452a3d87383dad926501c255c47e5caa04e35632711b90fcd8c80060.jpg)



Fig 19. High value resistor in parallel with the LED


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/423b1903e8e98c57c728d85beb66aef117a3314a842447dd61a8e13780a7297b.jpg)



Fig 20. Device supplied by a lower voltage


## 10. Limiting values


Table 9. Limiting values



In accordance with the Absolute Maximum Rating System (IEC 60134).


<table><tr><td>Symbol</td><td>Parameter</td><td>Conditions</td><td>Min</td><td>Max</td><td>Unit</td></tr><tr><td><eq>V_{DD}</eq></td><td>supply voltage</td><td></td><td>-0.5</td><td>+6</td><td>V</td></tr><tr><td><eq>V_I</eq></td><td>input voltage</td><td></td><td><eq>V_{SS} - 0.5</eq></td><td>5.5</td><td>V</td></tr><tr><td><eq>I_I</eq></td><td>input current</td><td></td><td>-</td><td>±20</td><td>mA</td></tr><tr><td><eq>I_{IHL(max)}</eq></td><td>maximum allowed input current through protection diode (IO1 to IO7)</td><td><eq>V_I \geq V_{DD}</eq> or <eq>V_I \leq V_{SS}</eq></td><td>-</td><td>±400</td><td>μA</td></tr><tr><td rowspan="2"><eq>V_{I/O}</eq></td><td rowspan="2">voltage on an input/output pin</td><td>I/O as an input, except IO0</td><td><eq>V_{SS} - 0.5</eq></td><td>5.5</td><td>V</td></tr><tr><td>IO0 as an input</td><td><eq>V_{SS} - 0.5</eq></td><td>5.5</td><td>V</td></tr><tr><td rowspan="2"><eq>I_{I/O}</eq></td><td rowspan="2">input/output current</td><td rowspan="2">IO0 as an input</td><td>-</td><td>+400</td><td>μA</td></tr><tr><td>-</td><td>-20</td><td>mA</td></tr><tr><td><eq>I_{O(ION)}</eq></td><td>output current on pin IOn</td><td></td><td>-</td><td>±50</td><td>mA</td></tr><tr><td><eq>I_{DD}</eq></td><td>supply current</td><td></td><td>-</td><td>85</td><td>mA</td></tr><tr><td><eq>I_{SS}</eq></td><td>ground supply current</td><td></td><td>-</td><td>100</td><td>mA</td></tr><tr><td><eq>P_{tot}</eq></td><td>total power dissipation</td><td></td><td>-</td><td>200</td><td>mW</td></tr><tr><td><eq>T_{stg}</eq></td><td>storage temperature</td><td></td><td>-65</td><td>+150</td><td>°C</td></tr><tr><td><eq>T_{amb}</eq></td><td>ambient temperature</td><td>operating</td><td>-40</td><td>+85</td><td>°C</td></tr></table>

## 11. Static characteristics


Table 10. Static characteristics



$V_{DD}=2.3\ V\ to\ 5.5\ V;\ V_{SS}=0\ V;\ T_{amb}=-40\ ^{\circ}C\ to\ +85\ ^{\circ}C;\ unless\ otherwise\ specified.$


<table><tr><td>Symbol</td><td>Parameter</td><td>Conditions</td><td>Min</td><td>Typ</td><td>Max</td><td>Unit</td></tr><tr><td colspan="7">Supplies</td></tr><tr><td><eq>V_{DD}</eq></td><td>supply voltage</td><td></td><td>2.3</td><td>-</td><td>5.5</td><td>V</td></tr><tr><td><eq>I_{DD}</eq></td><td>supply current</td><td>operating mode; <eq>V_{DD}=5.5 V</eq>; no load; <eq>f_{SCL}=100 kHz</eq></td><td>-</td><td>19</td><td>25</td><td>μA</td></tr><tr><td><eq>I_{stbL}</eq></td><td>LOW-level standby current</td><td>standby mode; <eq>V_{DD}=5.5 V</eq>; no load; <eq>V_I=V_{SS}</eq>; <eq>f_{SCL}=0 kHz</eq>; I/O = inputs</td><td>-</td><td>0.25</td><td>1</td><td>μA</td></tr><tr><td><eq>I_{stbH}</eq></td><td>HIGH-level standby current</td><td>standby mode; <eq>V_{DD}=5.5 V</eq>; no load; <eq>V_I=V_{DD}</eq>; <eq>f_{SCL}=0 kHz</eq>; I/O = inputs</td><td>-</td><td>0.25</td><td>1</td><td>μA</td></tr><tr><td><eq>\Delta I_{stb}</eq></td><td>additional standby current</td><td>standby mode; <eq>V_{DD}=5.5 V</eq>; every LED I/O at <eq>V_I=4.3 V</eq>; <eq>f_{SCL}=0 kHz</eq></td><td>-</td><td>0.8</td><td>1</td><td>mA</td></tr><tr><td><eq>V_{POR}</eq></td><td>power-on reset voltage</td><td>no load; <eq>V_I=V_{DD}</eq> or <eq>V_{SS}</eq></td><td>[1]</td><td>1.65</td><td>2.1</td><td>V</td></tr><tr><td colspan="7">Input SCL; input/output SDA</td></tr><tr><td><eq>V_{IL}</eq></td><td>LOW-level input voltage</td><td></td><td>-0.5</td><td>-</td><td>+0.3<eq>V_{DD}</eq></td><td>V</td></tr><tr><td><eq>V_{IH}</eq></td><td>HIGH-level input voltage</td><td></td><td>0.7<eq>V_{DD}</eq></td><td>-</td><td>5.5</td><td>V</td></tr><tr><td><eq>I_{OL}</eq></td><td>LOW-level output current</td><td><eq>V_{OL}=0.4 V</eq>; <eq>V_{DD}=2.3 V</eq></td><td>3</td><td>-</td><td>-</td><td>mA</td></tr><tr><td><eq>I_L</eq></td><td>leakage current</td><td><eq>V_I=V_{DD}</eq> or <eq>V_{SS}</eq></td><td>-1</td><td>-</td><td>+1</td><td>μA</td></tr><tr><td><eq>C_i</eq></td><td>input capacitance</td><td><eq>V_I=V_{SS}</eq></td><td>-</td><td>6</td><td>10</td><td>pF</td></tr><tr><td colspan="7">I/Os</td></tr><tr><td><eq>V_{IL}</eq></td><td>LOW-level input voltage</td><td></td><td>-0.5</td><td>-</td><td>+0.8</td><td>V</td></tr><tr><td><eq>V_{IH}</eq></td><td>HIGH-level input voltage</td><td></td><td>2.0</td><td>-</td><td>5.5</td><td>V</td></tr><tr><td><eq>I_{OL}</eq></td><td>LOW-level output current</td><td><eq>V_{OL}=5.5 V</eq>; <eq>V_{DD}=2.3 V</eq></td><td>[2] 8</td><td>10</td><td>-</td><td>mA</td></tr><tr><td rowspan="3"><eq>I_{OH}</eq></td><td rowspan="3">HIGH-level output current</td><td>except pin IO0; <eq>V_{OH}=2.4 V</eq></td><td>[3] 4</td><td>-</td><td>-</td><td>mA</td></tr><tr><td>pin IO0; <eq>V_{OH}=4.6 V</eq></td><td>-</td><td>-</td><td>1</td><td>μA</td></tr><tr><td>pin IO0; <eq>V_{OH}=3.3 V</eq></td><td>-</td><td>-</td><td>1</td><td>μA</td></tr><tr><td><eq>I_{LI}</eq></td><td>input leakage current</td><td><eq>V_{DD}=5.5 V</eq>; <eq>V_I=V_{SS}</eq></td><td>-</td><td>-</td><td>-100</td><td>μA</td></tr><tr><td><eq>C_i</eq></td><td>input capacitance</td><td></td><td>-</td><td>3.7</td><td>5</td><td>pF</td></tr><tr><td><eq>C_o</eq></td><td>output capacitance</td><td></td><td>-</td><td>3.7</td><td>5</td><td>pF</td></tr><tr><td colspan="7">Select inputs A0, A1, A2 and RESET</td></tr><tr><td><eq>V_{IL}</eq></td><td>LOW-level input voltage</td><td></td><td>-0.5</td><td>-</td><td>+0.8</td><td>V</td></tr><tr><td><eq>V_{IH}</eq></td><td>HIGH-level input voltage</td><td></td><td>2.0</td><td>-</td><td>5.5</td><td>V</td></tr><tr><td><eq>I_{LI}</eq></td><td>input leakage current</td><td></td><td>-1</td><td>-</td><td>+1</td><td>μA</td></tr></table>


[1] $V_{DD}$ must be lowered to 0.2 V in order to reset part. 



[2] The total amount sunk by all I/Os must be limited to 100 mA and 25 mA per bit. 



[3] The total current sourced by all I/Os must be limited to 85 mA and 20 mA per bit. 


## 12. Dynamic characteristics


Table 11. Dynamic characteristics


<table><tr><td rowspan="2">Symbol</td><td rowspan="2">Parameter</td><td rowspan="2">Conditions</td><td colspan="2">Standard-mode <eq>I^{2}C</eq>-bus</td><td colspan="2">Fast-mode <eq>I^{2}C</eq>-bus</td><td rowspan="2">Unit</td></tr><tr><td>Min</td><td>Max</td><td>Min</td><td>Max</td></tr><tr><td><eq>f_{SCL}</eq></td><td>SCL clock frequency</td><td></td><td>0</td><td>100</td><td>0</td><td>400</td><td>kHz</td></tr><tr><td><eq>t_{BUF}</eq></td><td>bus free time between a STOP and START condition</td><td></td><td>4.7</td><td>-</td><td>1.3</td><td>-</td><td>μs</td></tr><tr><td><eq>t_{HD;STA}</eq></td><td>hold time (repeated) START condition</td><td></td><td>4.0</td><td>-</td><td>0.6</td><td>-</td><td>μs</td></tr><tr><td><eq>t_{SU;STA}</eq></td><td>set-up time for a repeated START condition</td><td></td><td>4.7</td><td>-</td><td>0.6</td><td>-</td><td>μs</td></tr><tr><td><eq>t_{SU;STO}</eq></td><td>set-up time for STOP condition</td><td></td><td>4.0</td><td>-</td><td>0.6</td><td>-</td><td>μs</td></tr><tr><td><eq>t_{HD;DAT}</eq></td><td>data hold time</td><td></td><td>0</td><td>-</td><td>0</td><td>-</td><td>ns</td></tr><tr><td><eq>t_{VD;ACK}</eq></td><td>data valid acknowledge time</td><td>[1]</td><td>-</td><td>1</td><td>-</td><td>0.9</td><td>μs</td></tr><tr><td><eq>t_{VD;DAT}</eq></td><td>data valid time</td><td>[2]</td><td>-</td><td>1</td><td>-</td><td>0.9</td><td>μs</td></tr><tr><td><eq>t_{SU;DAT}</eq></td><td>data set-up time</td><td></td><td>250</td><td>-</td><td>100</td><td>-</td><td>ns</td></tr><tr><td><eq>t_{LOW}</eq></td><td>LOW period of the SCL clock</td><td></td><td>4.7</td><td>-</td><td>1.3</td><td>-</td><td>μs</td></tr><tr><td><eq>t_{HIGH}</eq></td><td>HIGH period of the SCL clock</td><td></td><td>4.0</td><td>-</td><td>0.6</td><td>-</td><td>μs</td></tr><tr><td><eq>t_f</eq></td><td>fall time of both SDA and SCL signals</td><td></td><td>-</td><td>300</td><td><eq>20 + 0.1C_b^{[3]}</eq></td><td>300</td><td>ns</td></tr><tr><td><eq>t_r</eq></td><td>rise time of both SDA and SCL signals</td><td></td><td>-</td><td>1000</td><td><eq>20 + 0.1C_b^{[3]}</eq></td><td>300</td><td>ns</td></tr><tr><td><eq>t_{SP}</eq></td><td>pulse width of spikes that must be suppressed by the input filter</td><td></td><td>-</td><td>50</td><td>-</td><td>50</td><td>ns</td></tr><tr><td colspan="8">Port timing</td></tr><tr><td rowspan="2"><eq>t_{v(Q)}</eq></td><td rowspan="2">data output valid time</td><td>pin IO0</td><td>-</td><td>250</td><td>-</td><td>250</td><td>ns</td></tr><tr><td>pins IO1 to IO7</td><td>-</td><td>200</td><td>-</td><td>200</td><td>ns</td></tr><tr><td><eq>t_{su(D)}</eq></td><td>data input set-up time</td><td></td><td>0</td><td>-</td><td>0</td><td>-</td><td>ns</td></tr><tr><td><eq>t_{h(D)}</eq></td><td>data input hold time</td><td></td><td>200</td><td>-</td><td>200</td><td>-</td><td>ns</td></tr><tr><td colspan="8">Reset timing</td></tr><tr><td><eq>t_{w(rst)}</eq></td><td>reset pulse width</td><td></td><td>6</td><td>-</td><td>6</td><td>-</td><td>ns</td></tr><tr><td><eq>t_{rec(rst)}</eq></td><td>reset recovery time</td><td></td><td>0</td><td>-</td><td>0</td><td>-</td><td>ns</td></tr><tr><td><eq>t_{rst}</eq></td><td>reset time</td><td></td><td>400</td><td>-</td><td>400</td><td>-</td><td>ns</td></tr></table>


[1] $t_{VD;ACK}$ = time for acknowledgement signal from SCL LOW to SDA (out) LOW. 



[2] $t_{VD;DAT}$ = minimum time for SDA data out to be valid following SCL LOW. 



[3] $C_{b}$ = total capacitance of one bus line in pF. 


8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/276e4f3972175770f8a82d8aa99a1db506ff9b49cc51521b4262e66b0df7d86f.jpg)



Fig 21. Definition of timing on the I²C-bus


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/0596b170e880f28b54f0aa6c9695e152377d6926ebe1308b29ac5f3f04229c10.jpg)



Fig 22. Definition of RESET timing


## 13. Package outline

SO16: plastic small outline package; 16 leads; body width 3.9 mm 

SOT109-1 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/c4cef4b83a577348d46476f98c531e1e35dd81bc7afe20c12f133c57ca22085c.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/08afbfb2144ddad0eba90ee77f717461dccd1f5bc6bd86b2c63a622874a3cc4a.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/100c014da103e13a6d5d40054720a5839a9e81c2052fa2d9b17a2907bef251e0.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/8433b36a7d338200c6542d908df46237b15946c2f27a57526fbf5b81e33f9799.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/b0397b0b24be73a7b59a2be2735c9fcfeadee5b7f36fbde9019b06116840b3ce.jpg)



DIMENSIONS (inch dimensions are derived from the original mm dimensions)


<table><tr><td>UNIT</td><td>A max.</td><td><eq>A_1</eq></td><td><eq>A_2</eq></td><td><eq>A_3</eq></td><td><eq>b_p</eq></td><td>c</td><td><eq>D^{(1)}</eq></td><td><eq>E^{(1)}</eq></td><td>e</td><td><eq>H_E</eq></td><td>L</td><td><eq>L_p</eq></td><td>Q</td><td>v</td><td>w</td><td>y</td><td><eq>Z^{(1)}</eq></td><td>θ</td></tr><tr><td>mm</td><td>1.75</td><td>0.250.10</td><td>1.451.25</td><td>0.25</td><td>0.490.36</td><td>0.250.19</td><td>10.09.8</td><td>4.03.8</td><td>1.27</td><td>6.25.8</td><td>1.05</td><td>1.00.4</td><td>0.70.6</td><td>0.25</td><td>0.25</td><td>0.1</td><td>0.70.3</td><td rowspan="2"><eq>8^\circ</eq><eq>0^\circ</eq></td></tr><tr><td>inches</td><td>0.069</td><td>0.0100.004</td><td>0.0570.049</td><td>0.01</td><td>0.0190.014</td><td>0.01000.0075</td><td>0.390.38</td><td>0.160.15</td><td>0.05</td><td>0.2440.228</td><td>0.041</td><td>0.0390.016</td><td>0.0280.020</td><td>0.01</td><td>0.01</td><td>0.004</td><td>0.0280.012</td></tr></table>

## Note

1. Plastic or metal protrusions of 0.15 mm (0.006 inch) maximum per side are not included. 

<table><tr><td rowspan="2">OUTLINE VERSION</td><td colspan="4">REFERENCES</td><td rowspan="2">EUROPEAN PROJECTION</td><td rowspan="2">ISSUE DATE</td></tr><tr><td>IEC</td><td>JEDEC</td><td>JEITA</td><td></td></tr><tr><td>SOT109-1</td><td>076E07</td><td>MS-012</td><td></td><td></td><td></td><td>99-12-2703-02-19</td></tr></table>

## Fig 23. Package outline SOT109-1 (SO16)

TSSOP16: plastic thin shrink small outline package; 16 leads; body width 4.4 mm 

SOT403-1 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/839bcce517af091226495a6904e8d9cd02790875c9bea10b7f4263b44f60b1bf.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/f35d7d3df3d7ec50dae9aa97c332c3e9ed34da3ded1cbeff60087c86525940da.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/589234cdfd8dcfa90d42073e9ab5fa5d4a10c1ba0370fe117924e852e21b3076.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/90910c4dc49ee8fc2c5bbf01ea241e8e48afc1d9551f3a62b30b9502f85e8c2b.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/2a00a589b3156802eee4c2018e190e5cc02be2fc0df6bb820f1925973e587f1d.jpg)



DIMENSIONS (mm are the original dimensions)


<table><tr><td>UNIT</td><td>A max.</td><td><eq>A_1</eq></td><td><eq>A_2</eq></td><td><eq>A_3</eq></td><td><eq>b_p</eq></td><td>c</td><td><eq>D^{(1)}</eq></td><td><eq>E^{(2)}</eq></td><td>e</td><td><eq>H_E</eq></td><td>L</td><td><eq>L_p</eq></td><td>Q</td><td>v</td><td>w</td><td>y</td><td><eq>Z^{(1)}</eq></td><td>θ</td></tr><tr><td>mm</td><td>1.1</td><td>0.150.05</td><td>0.950.80</td><td>0.25</td><td>0.300.19</td><td>0.20.1</td><td>5.14.9</td><td>4.54.3</td><td>0.65</td><td>6.66.2</td><td>1</td><td>0.750.50</td><td>0.40.3</td><td>0.2</td><td>0.13</td><td>0.1</td><td>0.400.06</td><td><eq>8^{\circ}0^{\circ}</eq></td></tr></table>


Notes 



1. Plastic or metal protrusions of 0.15 mm maximum per side are not included. 



2. Plastic interlead protrusions of 0.25 mm maximum per side are not included. 


<table><tr><td rowspan="2">OUTLINE VERSION</td><td colspan="4">REFERENCES</td><td rowspan="2">EUROPEAN PROJECTION</td><td rowspan="2">ISSUE DATE</td></tr><tr><td>IEC</td><td>JEDEC</td><td>JEITA</td><td></td></tr><tr><td>SOT403-1</td><td></td><td>MO-153</td><td></td><td></td><td></td><td>99-12-2703-02-18</td></tr></table>


Fig 24. Package outline SOT403-1 (TSSOP16)


HVQFN16: plastic thermal enhanced very thin quad flat package; no leads; 16 terminals; body 4 x 4 x 0.85 mm 

SOT629-1 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/79eedc0804646be1bdfa8b80f1c37b43582e5f127272a21173fd0b02dab84f47.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/51bf4bdfbee6e71de0a54dea3941e6b39ec33701b1fdcfc5f2cc4f59cbc3affe.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/5c8d59afc06e42b3b0ce42dc0620ca9f379b652183d7cd0f64b9782bda4ff9bb.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/8286e94330a6cf8f10f6a1293c6b6e88ef625ddd30266c1752d79a690d80c01c.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/38cdef61a3805b56e01417eabc7ab3c9d4a52f48ea15383c278eb87463d0cd16.jpg)



DIMENSIONS (mm are the original dimensions)


<table><tr><td>UNIT</td><td><eq>A^{(1)}</eq>max.</td><td><eq>A_1</eq></td><td>b</td><td>c</td><td><eq>D^{(1)}</eq></td><td><eq>D_h</eq></td><td><eq>E^{(1)}</eq></td><td><eq>E_h</eq></td><td>e</td><td><eq>e_1</eq></td><td><eq>e_2</eq></td><td>L</td><td>v</td><td>w</td><td>y</td><td><eq>y_1</eq></td></tr><tr><td>mm</td><td>1</td><td>0.050.00</td><td>0.380.23</td><td>0.2</td><td>4.13.9</td><td>2.251.95</td><td>4.13.9</td><td>2.251.95</td><td>0.65</td><td>1.95</td><td>1.95</td><td>0.750.50</td><td>0.1</td><td>0.05</td><td>0.05</td><td>0.1</td></tr></table>


Note



1. Plastic or metal protrusions of 0.075 mm maximum per side are not included.


<table><tr><td rowspan="2">OUTLINE VERSION</td><td colspan="4">REFERENCES</td><td rowspan="2">EUROPEAN PROJECTION</td><td rowspan="2">ISSUE DATE</td></tr><tr><td>IEC</td><td>JEDEC</td><td>JEITA</td><td></td></tr><tr><td>SOT629-1</td><td>---</td><td>MO-220</td><td>---</td><td></td><td></td><td>-01-08-08-02-10-22</td></tr></table>


Fig 25. Package outline SOT629-1 (HVQFN16)


## 14. Handling information

All input and output pins are protected against ElectroStatic Discharge (ESD) under normal handling. When handling ensure that the appropriate precautions are taken as described in JESD625-A or equivalent standards. 

## 15. Soldering of SMD packages

This text provides a very brief insight into a complex technology. A more in-depth account of soldering ICs can be found in Application Note AN10365 "Surface mount reflow soldering description". 

## 15.1 Introduction to soldering

Soldering is one of the most common methods through which packages are attached to Printed Circuit Boards (PCBs), to form electrical circuits. The soldered joint provides both the mechanical and the electrical connection. There is no single soldering method that is ideal for all IC packages. Wave soldering is often preferred when through-hole and Surface Mount Devices (SMDs) are mixed on one printed wiring board; however, it is not suitable for fine pitch SMDs. Reflow soldering is ideal for the small pitches and high densities that come with increased miniaturization. 

## 15.2 Wave and reflow soldering

Wave soldering is a joining technology in which the joints are made by solder coming from a standing wave of liquid solder. The wave soldering process is suitable for the following: 

• Through-hole components 

- Leaded or leadless SMDs, which are glued to the surface of the printed circuit board 

Not all SMDs can be wave soldered. Packages with solder balls, and some leadless packages which have solder lands underneath the body, cannot be wave soldered. Also, leaded SMDs with leads having a pitch smaller than $\sim$ 0.6 mm cannot be wave soldered, due to an increased probability of bridging. 

The reflow soldering process involves applying solder paste to a board, followed by component placement and exposure to a temperature profile. Leaded packages, packages with solder balls, and leadless packages are all reflow solderable. 

Key characteristics in both wave and reflow soldering are: 

- Board specifications, including the board finish, solder masks and vias 

- Package footprints, including solder thieves and orientation 

• The moisture sensitivity level of the packages 

- Package placement 

- Inspection and repair 

- Lead-free soldering versus SnPb soldering 

## 15.3 Wave soldering

Key characteristics in wave soldering are: 

## 8-bit I²C-bus and SMBus I/O port with reset

- Process issues, such as application of adhesive and flux, clinching of leads, board transport, the solder wave parameters, and the time during which components are exposed to the wave 

- Solder bath specifications, including temperature and impurities 

## 15.4 Reflow soldering

Key characteristics in reflow soldering are: 

- Lead-free versus SnPb soldering; note that a lead-free reflow process usually leads to higher minimum peak temperatures (see Figure 26) than a SnPb process, thus reducing the process window 

- Solder paste printing issues including smearing, release, and adjusting the process window for a mix of large and small components on one board 

- Reflow temperature profile; this profile includes preheat, reflow (in which the board is heated to the peak temperature) and cooling down. It is imperative that the peak temperature is high enough for the solder to make reliable solder joints (a solder paste characteristic). In addition, the peak temperature must be low enough that the packages and/or boards are not damaged. The peak temperature of the package depends on package thickness and volume and is classified in accordance with Table 12 and 13 


Table 12. SnPb eutectic process (from J-STD-020D)


<table><tr><td rowspan="3">Package thickness (mm)</td><td colspan="2">Package reflow temperature (°C)</td></tr><tr><td colspan="2">Volume (mm3)</td></tr><tr><td>&lt; 350</td><td>≥ 350</td></tr><tr><td>&lt; 2.5</td><td>235</td><td>220</td></tr><tr><td>≥ 2.5</td><td>220</td><td>220</td></tr></table>


Table 13. Lead-free process (from J-STD-020D)


<table><tr><td rowspan="3">Package thickness (mm)</td><td colspan="3">Package reflow temperature (°C)</td></tr><tr><td colspan="3">Volume (mm3)</td></tr><tr><td>&lt; 350</td><td>350 to 2000</td><td>&gt; 2000</td></tr><tr><td>&lt; 1.6</td><td>260</td><td>260</td><td>260</td></tr><tr><td>1.6 to 2.5</td><td>260</td><td>250</td><td>245</td></tr><tr><td>&gt; 2.5</td><td>250</td><td>245</td><td>245</td></tr></table>

Moisture sensitivity precautions, as indicated on the packing, must be respected at all times. 

Studies have shown that small packages reach higher temperatures during reflow soldering, see Figure 26. 

8-bit I²C-bus and SMBus I/O port with reset 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/ebb42623e8af85ed42379717580eb1762ad9a26900f37f934d17437e2ff33b22.jpg)



MSL: Moisture Sensitivity Level


Fig 26. Temperature profiles for large and small components 

For further information on temperature profiles, refer to Application Note AN10365 "Surface mount reflow soldering description". 

## 16. Soldering: PCB footprints

## Footprint information for reflow soldering of SO16 package

SOT109-1 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/cc22f73cd076d3ac62a5dd55c6fb7aac3267a50196367058551d2d8239a3240b.jpg)



Generic footprint pattern



Refer to the package outline drawing for actual layout


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/0268d8225870ed030a235d25f62f41315700874b7d23dd5a82756cbdd53a3e65.jpg)



solder land



---- occupied area



DIMENSIONS in mm


<table><tr><td>P1</td><td>P2</td><td>Ay</td><td>By</td><td>C</td><td>D1</td><td>D2</td><td>Gx</td><td>Gy</td><td>Hx</td><td>Hy</td></tr><tr><td>1.270</td><td>1.320</td><td>7.400</td><td>3.900</td><td>1.750</td><td>0.700</td><td>0.800</td><td>10.040</td><td>5.200</td><td>11.900</td><td>7.650</td></tr></table>


Fig 27. PCB footprint for SOT109-1 (SO16); reflow soldering


Footprint information for reflow soldering of TSSOP16 package 

SOT403-1 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/de1f143cf626a606d8b147436e577a6e392e4d01d6b70017fb15c410a6ed877a.jpg)



Generic footprint pattern



Refer to the package outline drawing for actual layout


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/8780c3253189a1968f3e3d1bc0ce5f1c8e13aab63b5129075140fb00f05d118f.jpg)



solder land



---- occupied area



DIMENSIONS in mm


<table><tr><td>P1</td><td>P2</td><td>Ay</td><td>By</td><td>C</td><td>D1</td><td>D2</td><td>Gx</td><td>Gy</td><td>Hx</td><td>Hy</td></tr><tr><td>0.650</td><td>0.750</td><td>7.200</td><td>4.500</td><td>1.350</td><td>0.400</td><td>0.600</td><td>5.600</td><td>5.300</td><td>5.800</td><td>7.450</td></tr></table>

sot403-1_fr 


Fig 28. PCB footprint for SOT403-1 (TSSOP16); reflow soldering



Footprint information for reflow soldering of HVQFN16 package



SOT629-1


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/ad3d3e641b4fbe8bce2e702496d0dd8d214d9be4790501279356dceb8f47c918.jpg)



Generic footprint pattern


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/6d7574999a8e612497e5c8c6856ce6f097b4aa8eda552ee1909410bb1bb8b595.jpg)



solder land



Refer to the package outline drawing for actual layout


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/ebf26fe5d0714da1a9c022f90ef30c23110a62c53683479698478542efd37f76.jpg)



solder paste deposit


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/7ab171d6-70f3-4a87-92b1-4d6a0072277d/68a4a9a084d9e47211fa7fb5b50bbc593abaec81e3a034bd01d3de27418f11db.jpg)



solder land plus solder paste



---- occupied area


nSPx nSPy 

2 2 


Dimensions in mm


<table><tr><td>P</td><td>Ax</td><td>Ay</td><td>Bx</td><td>By</td><td>C</td><td>D</td><td>SLx</td><td>SLy</td><td>SPx tot</td><td>SPy tot</td><td>SPx</td><td>SPy</td><td>Gx</td><td>Gy</td><td>Hx</td><td>Hy</td></tr><tr><td>0.650</td><td>5.000</td><td>5.000</td><td>2.800</td><td>2.800</td><td>1.100</td><td>0.300</td><td>2.000</td><td>2.000</td><td>1.200</td><td>1.200</td><td>0.450</td><td>0.450</td><td>4.300</td><td>4.300</td><td>5.250</td><td>5.250</td></tr><tr><td>Issue date</td><td colspan="2">-07-05-07-09-06-15</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td>sot629-1_fr</td></tr></table>


Fig 29. PCB footprint for SOT629-1 (HVQFN16); reflow soldering


## 17. Abbreviations


Table 14. Abbreviations


<table><tr><td>Acronym</td><td>Description</td></tr><tr><td>CBT</td><td>Cross Bar Technology</td></tr><tr><td>CDM</td><td>Charged-Device Model</td></tr><tr><td>CMOS</td><td>Complementary Metal-Oxide Semiconductor</td></tr><tr><td>ESD</td><td>ElectroStatic Discharge</td></tr><tr><td>HBM</td><td>Human Body Model</td></tr><tr><td>I2C-bus</td><td>Inter-Integrated Circuit bus</td></tr><tr><td>I/O</td><td>Input/Output</td></tr><tr><td>LED</td><td>Light-Emitting Diode</td></tr><tr><td>MM</td><td>Machine Model</td></tr><tr><td>PCB</td><td>Printed-Circuit Board</td></tr><tr><td>POR</td><td>Power-On Reset</td></tr><tr><td>SMBus</td><td>System Management Bus</td></tr></table>

## 18. Revision history


Table 15. Revision history


<table><tr><td>Document ID</td><td>Release date</td><td>Data sheet status</td><td>Change notice</td><td>Supersedes</td></tr><tr><td>PCA9557 v.7</td><td>20131210</td><td>Product data sheet</td><td>-</td><td>PCA9557 v.6</td></tr><tr><td>Modifications:</td><td colspan="4">Section 2 “Features and benefits”, 17th bullet item: deleted phrase “150 V MM per JESD22-A115”Table 1 “Ordering information”: added column ‘Topside marking’ (moved from Table 2)Table 2 “Ordering options”:– deleted column ‘Topside mark’ (moved to Table 1)– added columns ‘Orderable part number’, ‘Package’, ‘Packing method’ and ‘Minimum order quantity’Table 10 “Static characteristics”, sub-section “Input SCL; input/output SDA”:– <eq>I_{OL}</eq>: added “<eq>V_{DD} = 2.3</eq> V” to ConditionsTable 10 “Static characteristics”, sub-section “I/Os”:– <eq>I_{OL}</eq>: added “<eq>V_{OL} = 0.55</eq> V; <eq>V_{DD} = 2.3</eq> V” to ConditionsTable 11 “Dynamic characteristics”, <eq>t_{VD;DAT}</eq>: Unit corrected from “ms” to “μs” (this is a correction to documentation only, no change to device)Added Section 16 “Soldering: PCB footprints”</td></tr><tr><td>PCA9557 v.6</td><td>20080611</td><td>Product data sheet</td><td>-</td><td>PCA9557 v.5</td></tr><tr><td>PCA9557 v.5</td><td>20070912</td><td>Product data sheet</td><td>-</td><td>PCA9557 v.4</td></tr><tr><td>PCA9557 v.4(9397 750 13336)</td><td>20041124</td><td>Product data sheet</td><td>-</td><td>PCA9557 v.3</td></tr><tr><td>PCA9557 v.3(9397 750 10872)</td><td>20021213</td><td>Product data</td><td>ECN 853-2308 29160 of 06 Nov 2002</td><td>PCA9557 v.2</td></tr><tr><td>PCA9557 v.2(9397 750 09819)</td><td>20020513</td><td>Product data</td><td>ECN 853-2308 28188 of 13 May 2002</td><td>PCA9557 v.1</td></tr><tr><td>PCA9557 v.1</td><td>20011212</td><td>Product data</td><td>ECN 853-2308 27449 of 12 Dec 2001</td><td>-</td></tr></table>

## 19. Legal information

## 19.1 Data sheet status

<table><tr><td>Document status[1][2]</td><td>Product status[3]</td><td>Definition</td></tr><tr><td>Objective [short] data sheet</td><td>Development</td><td>This document contains data from the objective specification for product development.</td></tr><tr><td>Preliminary [short] data sheet</td><td>Qualification</td><td>This document contains data from the preliminary specification.</td></tr><tr><td>Product [short] data sheet</td><td>Production</td><td>This document contains the product specification.</td></tr></table>


[1] Please consult the most recently issued document before initiating or completing a design. 



[2] The term ‘short data sheet’ is explained in section “Definitions”. 



[3] The product status of device(s) described in this document may have changed since this document was published and may differ in case of multiple devices. The latest product status information is available on the Internet at URL http://www.nxp.com. 


## 19.2 Definitions

Draft — The document is a draft version only. The content is still under internal review and subject to formal approval, which may result in modifications or additions. NXP Semiconductors does not give any representations or warranties as to the accuracy or completeness of information included herein and shall have no liability for the consequences of use of such information. 

Short data sheet — A short data sheet is an extract from a full data sheet with the same product type number(s) and title. A short data sheet is intended for quick reference only and should not be relied upon to contain detailed and full information. For detailed and full information see the relevant full data sheet, which is available on request via the local NXP Semiconductors sales office. In case of any inconsistency or conflict with the short data sheet, the full data sheet shall prevail. 

Product specification — The information and data provided in a Product data sheet shall define the specification of the product as agreed between NXP Semiconductors and its customer, unless NXP Semiconductors and customer have explicitly agreed otherwise in writing. In no event however, shall an agreement be valid in which the NXP Semiconductors product is deemed to offer functions and qualities beyond those described in the Product data sheet. 

## 19.3 Disclaimers

Limited warranty and liability — Information in this document is believed to be accurate and reliable. However, NXP Semiconductors does not give any representations or warranties, expressed or implied, as to the accuracy or completeness of such information and shall have no liability for the consequences of use of such information. NXP Semiconductors takes no responsibility for the content in this document if provided by an information source outside of NXP Semiconductors. 

In no event shall NXP Semiconductors be liable for any indirect, incidental, punitive, special or consequential damages (including - without limitation - lost profits, lost savings, business interruption, costs related to the removal or replacement of any products or rework charges) whether or not such damages are based on tort (including negligence), warranty, breach of contract or any other legal theory. 

Notwithstanding any damages that customer might incur for any reason whatsoever, NXP Semiconductors' aggregate and cumulative liability towards customer for the products described herein shall be limited in accordance with the Terms and conditions of commercial sale of NXP Semiconductors. 

Right to make changes — NXP Semiconductors reserves the right to make changes to information published in this document, including without limitation specifications and product descriptions, at any time and without notice. This document supersedes and replaces all information supplied prior to the publication hereof. 

Suitability for use — NXP Semiconductors products are not designed, authorized or warranted to be suitable for use in life support, life-critical or safety-critical systems or equipment, nor in applications where failure or malfunction of an NXP Semiconductors product can reasonably be expected to result in personal injury, death or severe property or environmental damage. NXP Semiconductors and its suppliers accept no liability for inclusion and/or use of NXP Semiconductors products in such equipment or applications and therefore such inclusion and/or use is at the customer's own risk. 

Applications — Applications that are described herein for any of these products are for illustrative purposes only. NXP Semiconductors makes no representation or warranty that such applications will be suitable for the specified use without further testing or modification. 

Customers are responsible for the design and operation of their applications and products using NXP Semiconductors products, and NXP Semiconductors accepts no liability for any assistance with applications or customer product design. It is customer's sole responsibility to determine whether the NXP Semiconductors product is suitable and fit for the customer's applications and products planned, as well as for the planned application and use of customer's third party customer(s). Customers should provide appropriate design and operating safeguards to minimize the risks associated with their applications and products. 

NXP Semiconductors does not accept any liability related to any default, damage, costs or problem which is based on any weakness or default in the customer's applications or products, or the application or use by customer's third party customer(s). Customer is responsible for doing all necessary testing for the customer's applications and products using NXP Semiconductors products in order to avoid a default of the applications and the products or of the application or use by customer's third party customer(s). NXP does not accept any liability in this respect. 

Limiting values — Stress above one or more limiting values (as defined in the Absolute Maximum Ratings System of IEC 60134) will cause permanent damage to the device. Limiting values are stress ratings only and (proper) operation of the device at these or any other conditions above those given in the Recommended operating conditions section (if present) or the Characteristics sections of this document is not warranted. Constant or repeated exposure to limiting values will permanently and irreversibly affect the quality and reliability of the device. 

Terms and conditions of commercial sale — NXP Semiconductors products are sold subject to the general terms and conditions of commercial sale, as published at http://www.nxp.com/profile/terms, unless otherwise agreed in a valid written individual agreement. In case an individual agreement is concluded only the terms and conditions of the respective agreement shall apply. NXP Semiconductors hereby expressly objects to applying the customer's general terms and conditions with regard to the purchase of NXP Semiconductors products by customer. 

No offer to sell or license — Nothing in this document may be interpreted or construed as an offer to sell products that is open for acceptance or the grant, conveyance or implication of any license under any copyrights, patents or other industrial or intellectual property rights. 

## 8-bit I²C-bus and SMBus I/O port with reset

Export control — This document as well as the item(s) described herein may be subject to export control regulations. Export might require a prior authorization from competent authorities. 

Non-automotive qualified products — Unless this data sheet expressly states that this specific NXP Semiconductors product is automotive qualified, the product is not suitable for automotive use. It is neither qualified nor tested in accordance with automotive testing or application requirements. NXP Semiconductors accepts no liability for inclusion and/or use of non-automotive qualified products in automotive equipment or applications. 

In the event that customer uses the product for design-in and use in automotive applications to automotive specifications and standards, customer (a) shall use the product without NXP Semiconductors' warranty of the product for such automotive applications, use and specifications, and (b) whenever customer uses the product for automotive applications beyond NXP Semiconductors' specifications such use shall be solely at customer's own risk, and (c) customer fully indemnifies NXP Semiconductors for any liability, damages or failed product claims resulting from customer design and use of the product for automotive applications beyond NXP Semiconductors' standard warranty and NXP Semiconductors' product specifications. 

Translations — A non-English (translated) version of a document is for reference only. The English version shall prevail in case of any discrepancy between the translated and English versions. 

## 19.4 Trademarks

Notice: All referenced brands, product names, service names and trademarks are the property of their respective owners. 

I²C-bus — logo is a trademark of NXP B.V. 

## 20. Contact information

For more information, please visit: http://www.nxp.com 

For sales office addresses, please send an email to: salesaddresses@nxp.com 

## 21. Contents

1 General description.... 1 

2 Features and benefits .... 1 

3 Ordering information.... 2 

3.1 Ordering options.... 2 

4 Block diagram 2 

5 Pinning information.... 5 

5.1 Pinning 5 

5.2 Pin description 5 

6 System diagram.... 6 

7 Functional description 7 

7.1 Device address.... 7 

7.2 Control register.... 7 

7.3 Register descriptions 8 

7.3.1 Register 0 - Input port register ..... 8 

7.3.2 Register 1 - Output port register....8 

7.3.3 Register 2 - Polarity inversion register ..... 8 

7.3.4 Register 3 - Configuration register ..... 8 

7.4 Power-on reset 9 

7.5 RESET input....9 

8 Characteristics of the I²C-bus 9 

8.1 Bit transfer 9 

8.1.1 START and STOP conditions 9 

8.2 System configuration 10 

8.3 Acknowledge 10 

8.4 Bus transactions.... 11 

9 Application design-in information ..... 13 

9.1 Minimizing $I_{DD}$ when the I/Os are used to control LEDs.... 13 

10 Limiting values.... 14 

11 Static characteristics.... 15 

12 Dynamic characteristics ..... 16 

13 Package outline 18 

14 Handling information.... 21 

15 Soldering of SMD packages 21 

15.1 Introduction to soldering 21 

15.2 Wave and reflow soldering 21 

15.3 Wave soldering.... 21 

15.4 Reflow soldering.... 22 

16 Soldering: PCB footprints.... 24 

17 Abbreviations.... 27 

18 Revision history.... 27 

19 Legal information.... 28 

19.1 Data sheet status 28 

19.2 Definitions.... 28 

19.3 Disclaimers 28 

19.4 Trademarks 29 

20 Contact information 29 

21 Contents.... 30 

Please be aware that important notices concerning this document and the product(s) described herein, have been included in section ‘Legal information’. 

© NXP B.V. 2013. 

For more information, please visit: http://www.nxp.com 

For sales office addresses, please send an email to: salesaddresses@nxp.com 

All rights reserved. 

Date of release: 10 December 2013 

Document identifier: PCA9557 