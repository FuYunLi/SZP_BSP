## QMI8658A DATASHEET

JUNE 20, 2022 


Security Level: 3


<table><tr><td>Document No.: 13-52-25</td><td>Title: QMI8658A Datasheet</td><td>Rev: A</td></tr><tr><td colspan="3">Originator: Chunlei Dai</td></tr></table>

## QMI8658A

# Low Noise, Wide Bandwidth 6D Inertial Measurement Unit with Motion Co-Processor

## Features

Low 13 mdps/√Hz gyroscope noise, low-latency, and wide bandwidth 

▪ Low Noise 150µg/√Hz accelerometer noise 

Host (slave) interface supports MIPI™ I3C, I2C, and 3-wire or 4-wire SPI 

Accelerometer and gyroscope sensors feature signal processing paths with digitally programmable data rates and filtering 

3-axis gyroscope and 3-axis accelerometer in a small 2.5 x 3.0 x 0.86 mm 14-pin LGA package 

Large 1536-byte FIFO can be used to buffer sensor data to lower system power dissipation 

Integrated Pedometer, Tap, Any-Motion, No-Motion, Significant-Motion detection 

Large sensor dynamic ranges from ±16°/s to ±2048°/s for gyroscope and ±2 g to ±16 g for accelerometer 

Low power modes for effective power management 

Digitally programmable sampling rate and filters 

Embedded temperature sensor 

Wide extended operating temperature range (-40°C to 85°C) 

## Description

The QMI8658A is a complete 6D MEMS inertial measurement unit (IMU). With tight board-level gyroscope sensitivity of ±3%, gyroscope noise density of 13 mdps/√Hz, and low latency, the QMI8658A is ideal for consumer and industrial applications. 

The QMI8658A incorporates a 3-axis gyroscope and a 3- axis accelerometer. It provides a host-processor interface supporting I3C, I2C and 3-wire or 4-wire SPI. 

With its built-in digital functionality, low power, and small size, the QMI8658A is the ideal part for applications requiring motion-based functionality. 

## Applications

Smartphones 

Game controllers, remote controls and pointing devices 

▪ Robotic vacuums 

E-bikes and scooters 

Bluetooth headsets 

Automotive security systems 

Toys 

Portrait-landscape display control 

## Table of Contents

## 1 General Information ...

1.1 Ordering Information ... 

1.2 Marking Information........... 

1.3 Internal Block Diagram..... .6 

1.4 Interface Operating... 

1.5 Application Diagrams ... .8 

1.6 Package & Pin Information... .10 

1.7 Recommended External Components ... 

## 2 QMI8658A Chip Orientation Coordinate System ..... 13

## 3 System, Electrical and Electro-Mechanical Characteristics..... 14

3.1 Absolute Maximum Ratings ..... 

3.2 Recommended Operating Conditions .... .14 

3.3 Power On Sequence of VDDIO and VDD... .15 

3.3.1 Power-On Reset(POR).... . 15 

3.3.2 VDDIO and VDD Are Driven by Single Power.............. . 15 

3.3.3 VDDIO and VDD Are Driven by Separate Power Lines... .16 

3.4 Electro-Mechanical Specifications................... 

3.5 Accelerometer Programmable Characteristics................. 

3.6 Gyroscope Programmable Characteristics...... .21 

3.7 Electrical Characteristics.............. 

3.8 Current Consumption...................... 

3.9 Temperature Sensor ....................... 

## 4 Register Map Overview..... . 25

4.1 UI Register Map Overview...... .25 

## 5 UI Sensor Configuration Settings and Output Data..... .. 28

5.1 Typical Sensor Mode Configuration and Output Data............ 

5.2 Chip Information Register ......... 

5.3 Configuration Registers .............. 

5.4 FIFO Registers .................... 

5.5 Status and Time Stamp Registers............. ......35 

5.6 Sensor Data Output Registers............. 

5.7 Calibration-On-Demand (COD) Status Register............. .....38 

5.8 Activity Detection Output Registers....... ...39 

5.9 Reset Register.............. 

5.10 CTRL 9 Functionality (Executing Pre-defined Commands) ..... ....41 

5.10.1 CTRL9 Protocol Description . .41 

5.10.2 CTRL9 Command List .. . 42 

5.10.3 CAL Registers... . 42 

5.10.4 WCtrl9 (Write – CTRL9 Protocol) .. .. 43 

5.10.5 Ctrl9R (CTRL9 Protocol - Read) . .. 43 

5.10.6 CTRL9 Commands in Details . .. 43 

## 6 Interrupts ......... .. 46

6.1 SyncSample mode.... ..46 

6.2 Non-SyncSample mode ..... ..47 

6.3 DRDY(Data Ready) ... ..47 

## 7 Operating Modes .........

7.1 Operating Modes Descriptions ........... 

7.2 General Mode Transitioning... ..49 

7.3 Transition Time ............ 

7.4 Chip Reset Process ......... ..51 

## 8 FIFO Description..... . 52

8.1 FIFO Structure............... 

8.2 FIFO Size ....... .52 

8.3 Configure FIFO Mode . 

8.4 FIFO Sample Count............. 

8.5 FIFO Watermark Interrupt...... 

8.6 FIFO Full . .53 

8.7 FIFO Read Mode .53 

8.8 Read FIFO Data .... .53 

8.9 FIFO Data Pattern .... .54 

8.10 Reset FIFO ............... 

## 9 Motion Detection......... 55

9.1 Motion Detection Principle . .55 

9.1.1 Any-Motion Detection Principle . . 55 

9.1.2 No-Motion Detection Principle.. .55 

9.1.3 Significant-Motion Detection Principle..... . 56 

9.2 Motion Detection Flow ...................... 

9.3 Parameters of Motion Detection ................... 

9.4 Config Motion Detection .................. 

9.5 Enabling Motion Detection................. 

9.6 Motion Interrupt... .59 

## 10 . 60

10.1 Tap Detection Principle ............ 

10.2 Tap Detection Parameters...... ..61 

10.3 Configure Tap ....................... 

10.4 Enable Tap Detection ............ 

10.5 Tap Interrupt .............. 

10.6 Tap Detection Output . 

## 11 Pedometer..... . 64

11.1 Pedometer Parameters .............. 

11.2 Configure Pedometer ................ 

11.3 Enable Pedometer .............. 

11.4 Pedometer Interrupt..... ....66 

11.5 Read Step Count ........... ......66 

11.6 Reset Step Count ...... ....66 

## Wake on Motion (WoM) .... . 67

12.1 Wake on Motion Principle ............. 

12.2 Wake on Motion Parameters ....... ..67 

12.3 Accelerometer Configuration ......... ..67 

12.4 Wake on Motion Event .............. 

12.5 Configuration Procedure ....... ...68 

12.6 Exit Wake on Motion Mode..... ...68 

## 13 Locking Mechanism....... 69

13.1 Locking Mechanism Principle ............. 

13.2 Locking Mechanism Data Reading Process .. ..69 

13.2.1 Disable/Enable AHB Clock Gating .. ... 70 

13.2.2 Enable Locking Mechanism.. ... 70 

13.2.3 Reading Sensor Data..... .. 70 

13.3 Data_Lock_Delay .............. 

13.4 Exit Locking Mechanism .... ..71 

13.5 On-The-Fly ODR Change in Locking Mechanism.... ..71 

## 14 Calibration-On-Demand (COD) .... 72

14.1 COD Principle... 

14.2 Run COD... 

14.3 COD Status ..... 

14.4 Save and Restore the New Gain Parameters ... .72 

## 15 Self-Test (Check-Alive) .... 73

15.1 Accelerometer Self-Test .... ..73 

15.2 Gyroscope Self-Test ... ..73 

## 16 Host Serial Interface.... . 75

16.1 Address Auto Increment .... ..75 

16.2 Serial Peripheral Interface (SPI) . ..75 

16.2.1 SPI Features .... ... 75 

16.2.2 SPI Interface Connection........... ... 76 

16.2.3 SPI Transaction Protocol ...... 77 

16.2.4 SPI Timing Characteristics. ... 81 

16.3 I2C Interface ........... 

16.3.1 I2C Slave Address Selection ...... .. 83 

16.3.2 I2C Interface Characteristics.... .. 83 

16.4 I3C Interface ............ 

## 17 Package and Handling ....... . 85

17.1 Package Drawing ............ 

17.2 Reflow Specification.............. 

17.3 Storage Specifications . ..86 

## 18 Document Information.... 87

18.1 Revision History ........... 

## 1 General Information

## 1.1 Ordering Information


Table 1. Ordering Information


<table><tr><td>Part Number</td><td>Package</td><td>Packing Method</td></tr><tr><td>QMI8658A</td><td>LGA14</td><td>Tape &amp; Reel</td></tr></table>

## 1.2 Marking Information

<table><tr><td>ROW</td><td>EXAMPLE</td><td>CODE/EXPLANATION</td></tr><tr><td>1</td><td>8658</td><td>DDDD – Device code</td></tr><tr><td>2</td><td>2113</td><td>YWLL– Y (Year code), W (1-digit, biweekly code), LL (Lot indication)</td></tr><tr><td>3</td><td>RB</td><td>CR – C (Assembly location), R (Product revision)</td></tr></table>


Figure 1. Top Mark


## 1.3 Internal Block Diagram

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/c88819296ebe4b6725a76dc2eb8a28d2a663785d97b297ba4bcffff70149654e.jpg)



Figure 2. Internal Block Diagram


## 1.4 Interface Operating

The QMI8658A can operate in below mode, as shown in the Figure 3. 

The QMI8658A is a slave device to a host processor that communicates to it using one of the following interfaces: I2C, I3C, and SPI (3-wire or 4-wire modes). This slave relationship to the host is the same for all operating modes. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/ac277d464f22c8aafd46e1ae3fa27f59d1856b1a0dd9614f1d1461319fee1f9b.jpg)



Figure 3. Operating Mode


## 1.5 Application Diagrams

The typical application diagrams are shown in this section. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/fb3308d190caf085c78a4b7da67892a2884438947907c767d0b4ca4b7a8af6bd.jpg)



Figure 4. 4-Wire SPI-UI mode


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/34d199d99562e46c2c3d00922f7bc81cb862f3c508d6bec9d4ce279133d7515d.jpg)



Figure 5. 3-Wire SPI-UI mode


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/33af9ecfaefb91f7d9b81dafb550df2662d6e72261772a4cae6d3cd939ad686f.jpg)



Figure 6. I3C/I2C-UI mode


There is internal 200Kohm pull-up resistor on the SCL, SDA, CS, SDx, SCx, RESV (Pin10). By default, all those pullup resistors are enabled. All the resistors can be disabled by CTRL9 command, refer to 1.6 Package & Pin Information and 5.10.6 CTRL_CMD_SET_RPU for details. 

There is internal 200Kohm pull-up resistor on SDO/SA0 pin, and is enabled during Power-On Reset or Soft-Reset, is automatically disabled after detecting the I2C slave address during the Reset Process. Therefore, in I2C/I3C mode leave the SDO/SA0 float or connect it to High level (recommended, to provide a stable level), will set the I2C slave address/I3C static address to 0x6A. And connect it to Low level, will set the I2C slave address/ I3C static address to 0x6B. 

In 3-wire SPI mode, leave the SDO/SA0 pin float. 

SCx and SDx can be connected to VDDIO or Logic High, GND or Logic Low, or be left float if internal pull-up resistors are enabled. 

RESV (Pin 10) should NOT be connected to GND or Logic Low. It can be connected to VDDIO or Logic High, or leave it externally float and enable the internal pull-up resistor(by default the pull-up resistor is enabled). Connecting it to VDDIO is preferred, which can provide a stable High level. 

RESV-NC (Pin 11) is by default an output pin, should be float (no connection). In case of the necessity to connect it to High or Low level, the RESV(Pin 10) should be firmly connected to VDDIO, providing a stable High level, to disable the output of Pin 11. 

Table 2 describes the names for the pins in different functions. The later descriptions will directly use the function name in different scenarios instead of the pin name. 


Table 2. Pin Name Mapped to Function Name


<table><tr><td>Pin Number</td><td>Type</td><td>Pin Name</td><td>Function Name in 4-wire SPI</td><td>Function Name in 3-wire SPI</td><td>Function Name in Host I2C / I3C</td></tr><tr><td>1</td><td>O</td><td>SDO/SA0</td><td>SDO</td><td></td><td>SA0</td></tr><tr><td>12</td><td>I</td><td>CS</td><td>CS</td><td>CS</td><td></td></tr><tr><td>13</td><td>IO</td><td>SCL</td><td>SPC</td><td>SPC</td><td>SCL</td></tr><tr><td>14</td><td>IO</td><td>SDA</td><td>SDI</td><td>SDIO</td><td>SDA</td></tr></table>

## 1.6 Package & Pin Information

The pinout of the QMI8658A is shown in Figure 7 and Figure 8. The pin names and functionality are detailed in Table 3. The pin functionality is dictated by the QMI8658A’s operating mode, as described in 1.5 Application Diagrams. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/a3c851634dc84ebd21da127c421bc228ea186565f66526c4f6cf209e0e5e0051.jpg)



Figure 7. Pins Face Down (Top View)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/dc3ae9687dd53acfd2a17741957ee81002df7c6fd08d261f352a5df84ad80e69.jpg)



Figure 8. Pins Face Up (Bottom View)



Table 3. Pin Definitions


<table><tr><td>Pin Number</td><td>Type</td><td>Pin Name</td><td>Function</td></tr><tr><td>1</td><td>O</td><td><eq>SDO/SA0^{(1)(3)}</eq></td><td>SPI-UI Data Out (SDO) in SPI-UI 4-Wire ModeSA0, <eq>I^2C</eq> Slave address and <eq>I^3C</eq> Static address select:If SA0 = 0, <eq>I^2C / I^3C</eq> address = 0x6BIf SA0 = 1, <eq>I^2C / I^3C</eq> address = 0x6A</td></tr><tr><td>2</td><td>IO</td><td><eq>SDx^{(1)(2)}</eq></td><td>Reserved. Connect to VDDIO, GND or No Connection</td></tr><tr><td>3</td><td>IO</td><td><eq>SCx^{(1)(2)}</eq></td><td>Reserved. Connect to VDDIO, GND or No Connection</td></tr><tr><td>4</td><td>O</td><td>INT1</td><td>Programmable Interrupt 1</td></tr><tr><td>5</td><td>I</td><td>VDDIO</td><td>Power Supply for IO Pins</td></tr><tr><td>6</td><td>I</td><td>GND</td><td>Ground (0 V supply)</td></tr><tr><td>7</td><td>I</td><td>GND</td><td>Ground (0 V supply)</td></tr><tr><td>8</td><td>I</td><td>VDD</td><td>Power supply</td></tr><tr><td>9</td><td>O</td><td>INT2</td><td>Programmable Interrupt 2 (INT2) / Data Ready (DRDY)</td></tr><tr><td>10</td><td>IO</td><td><eq>RESV^{(1)(2)}</eq></td><td>Reserved. Connect to VDDIO or Logic High, or No Connection and enable (by default) internal pull up resistor.Refer to 1.5 Application Diagrams.</td></tr><tr><td>11</td><td>I</td><td>RESV-NC</td><td>Reserved. Connect to VDDIO or Logic High, GND or Logic Low.Refer to 1.5 Application Diagrams.</td></tr><tr><td>12</td><td>I</td><td><eq>CS^{(1)(2)}</eq></td><td><eq>I^2C/ I^3C /SPI-UI</eq> selection Pin.(If 1: <eq>I^2C-UI</eq> Mode: <eq>I^2C/I^3C</eq> communication enabled, SPI idle mode)(If 0: SPI-UI mode: <eq>I^2C/I^3C</eq> disabled)</td></tr><tr><td>13</td><td>IO</td><td><eq>SCL^{(1)(2)}</eq></td><td><eq>I^2C/I^3C-UI</eq> Data (SDA) in <eq>I^2C/ I^3C</eq> modeSPI-UI Serial Clock (SPC)<eq>^{(3)}</eq> in SPI mode</td></tr><tr><td>14</td><td>IO</td><td><eq>SDA^{(1)(2)}</eq></td><td><eq>I^2C/I^3C-UI</eq> Data (SDA)SPI-UI Data In (SDI)<eq>^{(3)}</eq> in 4 wire ModeSPI-UI Data IO (SDIO)<eq>^{(3)}</eq> in 3 Wire Mode</td></tr></table>

## Notes:

1. This pin has an internal 200K pull up resistor. 

2. The internal pull-up resistor can be disabled by CTRL9 command (CTRL_CMD_SET_RPU). Refer to 5.10.6 CTRL_CMD_SET_RPU for details. 

3. Refer to Section 16 for detailed configuration information. 

## 1.7 Recommended External Components


Table 4. Recommended External Components


<table><tr><td>Component</td><td>Description</td><td>Parameter</td><td>Typical</td></tr><tr><td><eq>C_{p1}</eq></td><td>Capacitor</td><td>Capacitance</td><td>100 nF</td></tr><tr><td><eq>C_{p2}</eq></td><td>Capacitor</td><td>Capacitance</td><td>100 nF</td></tr><tr><td><eq>R_{pu}^{(4)}</eq></td><td>Resistor</td><td>Resistance</td><td>2KΩ ~ 10 kΩ</td></tr></table>

## Note:

4. $\mathsf { R } _ { \mathsf { p u } }$ resistors are only needed when the Host Serial Interface is configured for I2C (see I2C Interface section). They are not needed when the Host Serial Interface is configured for SPI or I3C. If pull-up resistors are used on SCL and SDA, then SPI, I3C and I2C Modes are all possible. If a pull-down resistor is used on SA0, an alternate slave address is used for I2C. SPI and I3C modes will be unaltered with the use of pull-up resistors for I2C. Additionally, a suitable pull up resistance $( \mathsf { R } _ { \mathsf { p u } } )$ value should be selected, accounting for the tradeoff between current consumption and rise time. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/8a10a293b7219c45cd331b29d75652708acd2b5e73362391deccd02a7fcd7914.jpg)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/38fa511ebbbdc7ef257213d4385e1961bdad9e92d0a61cc4db1c8254426a1094.jpg)



Figure 9. Typical Electrical Connections


## 2 QMI8658A Chip Orientation Coordinate System

The QMI8658A uses a right-handed coordinate system as the basis for the sensor frame of reference. Acceleration $( \mathsf { a } _ { \mathsf { x } } , \mathsf { a } _ { \mathsf { y } } , \mathsf { a } _ { z } )$ are given with respect to the X-Y-Z coordinate system shown above. Increasing accelerations along the positive X-Y-Z axes are considered positive. Angular Rate $( \cos , \cos y , \cos z )$ in the counterclockwise direction around the respective axis are considered positive. 

Figure 10 shows the various frames of reference and conventions for using the QMI8658A. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/42e9c5b9afb9a8789e4702114472a6bbfab29b8d37535445d4020ac6a160716c.jpg)



Figure 10. Chip Orientation Coordinate System


# 3 System, Electrical and Electro-Mechanical Characteristics

## 3.1 Absolute Maximum Ratings

Stresses exceeding the absolute maximum ratings may damage the device. The device may not function or be operable above the recommended operating conditions. Stressing the parts to these levels is not recommended. In addition, extended exposure to stresses above the recommended operating conditions may affect device reliability. The absolute maximum ratings are stress ratings only. 


Table 5. Absolute Maximum Ratings


<table><tr><td>Symbol</td><td colspan="2">Parameter</td><td>Min.</td><td>Max.</td><td>Unit</td></tr><tr><td><eq>T_{STG}</eq></td><td colspan="2">Storage Temperature</td><td>-40</td><td>+125</td><td>°C</td></tr><tr><td><eq>T_{Pmax}</eq></td><td colspan="2">Lead Soldering Temperature, 10 Seconds</td><td></td><td>+260</td><td>°C</td></tr><tr><td>VDD</td><td colspan="2">Supply Voltage</td><td>-0.3</td><td>3.6</td><td>V</td></tr><tr><td>VDDIO</td><td colspan="2">I/O Pins Supply Voltage</td><td>-0.3</td><td>3.6</td><td>V</td></tr><tr><td><eq>S_{g}^{(5)}</eq></td><td colspan="2">Acceleration g for 0.2 ms (Un-powered)</td><td></td><td>10,000</td><td>g</td></tr><tr><td rowspan="2"><eq>ESD^{(6)}</eq></td><td rowspan="2">Electrostatic Discharge Protection Level</td><td>Human Body Model per JES001-2014</td><td colspan="2">±2000</td><td rowspan="2">V</td></tr><tr><td>Charged Device Model per JESD22-C101</td><td colspan="2">±500</td></tr></table>


Notes: 



5. This is a mechanical shock (g) sensitive device. Proper handling is required to prevent damage to the part. 



6. This is an ESD-sensitive device. Proper handling is required to prevent damage to the part. 


## 3.2 Recommended Operating Conditions

The Recommended Operating Conditions table defines the conditions for device operation. Recommended operating conditions are specified to ensure optimal performance. It is not forbidden to design beyond to Absolute Maximum Ratings. 


Table 6. Recommended Operating Conditions


<table><tr><td>Symbol</td><td>Parameter</td><td>Min</td><td>Typ</td><td>Max</td><td>Unit</td></tr><tr><td>VDD</td><td>Supply Voltage</td><td>1.71</td><td>1.8</td><td>3.6</td><td>V</td></tr><tr><td>VDDIO</td><td>I/O Pins Supply Voltage</td><td>1.71</td><td>1.8</td><td>3.6</td><td>V</td></tr><tr><td><eq>V_{IL}</eq></td><td>Digital Low Level Input Voltage</td><td></td><td></td><td>0.3 *VDDIO</td><td>V</td></tr><tr><td><eq>V_{IH}</eq></td><td>Digital High Level Input Voltage</td><td>0.7 *VDDIO</td><td></td><td>VDDIO + 0.3</td><td>V</td></tr><tr><td><eq>V_{OL}</eq></td><td>Digital Low Level Output Voltage</td><td></td><td></td><td>0.1 *VDDIO</td><td>V</td></tr><tr><td><eq>V_{OH}</eq></td><td>Digital High Level Output Voltage</td><td>0.9 *VDDIO</td><td></td><td></td><td>V</td></tr><tr><td><eq>V_{POR\_RLS}</eq></td><td>POR Threshold Voltage</td><td></td><td>1.1</td><td></td><td>V</td></tr></table>

## 3.3 Power On Sequence of VDDIO and VDD

## 3.3.1 Power-On Reset(POR)

Once the VDD & VDDIO are powered from 0V to a certain level, the internal power voltage detector will trigger the Power-On Reset (POR) automatically, and then exit/release the POR mode as the VDD voltage rises over the POR Threshold (about 1.1V). Refer to Table 6. 

After POR release, there will be about 200us Startup Delay, followed by the QMI8658A Initialization. 

The instability of VDDIO & VDD power lines, especially the power increase/drop with high slew rate, would interfere the QMI8658A Initialization and operation. Therefore, there should be no sudden transients and spikes on power lines after Startup Delay, to make sure the Initialization and later on operations are properly implemented. 

Normally it takes within about 15ms (refer to System Turn On Time in Table 7 and Table 8) for QMI8658A to finish the Initialization and during which, there should be no write/configuration to QMI8658A, to prevent possible interference and failure. 

Note that the Software Reset is triggered by the reset command that host write to QMI8658A, which means POR is not involved, while the following sequence are similar (Startup Delay and Initialization) to POR. Refer to 7.4 for more details. 

## 3.3.2 VDDIO and VDD Are Driven by Single Power

As shown in Figure 11, when QMI8658A is driven by single/same power line, the power line should ramp up from POR Threshold (about 1.1V) to Min Operation Voltage(1.71V) with slew rate higher than 40V/s, otherwise the Initialization might fail, and QMI8658A might not work properly. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/5b9d88a58eb744cccc617cdc8ce766f0bbe38277dd75c880b5aa50157a314b62.jpg)



Figure 11. Power On process When Driven by Single Power Line


## 3.3.3 VDDIO and VDD Are Driven by Separate Power Lines

As shown in Figure 12, when QMI8658A is driven by separate power lines, the VDDIO should be always powered ahead of (no later than) VDD, which means: 

$$
\text { VDDdelay } > = 0, \text { VDDIO   should   be   driven   no   later   than   VDD }
$$

Note that the VDDdelay starts from the point when VDDIO rises over the POR Threshold and ended at the VDD rises over the POR Threshold. It should always be non-negative. 

The power lines should ramp up from POR Threshold (about 1.1V) to Min Operation Voltage(1.71V) with slew rate higher than 40V/s, otherwise the Initialization might fail, and QMI8658A might not work properly. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/b659c9c4f4b2467a2c61dcc6f17aa0f1d736f10a06b971ddfc7f113a806191ba.jpg)



Figure 12. Power On process When Driven by Separate Power Lines


## 3.4 Electro-Mechanical Specifications

$\mathsf { V D D } = \mathsf { V D D } \mathsf { I O } = 1 . 8 ~ \mathsf { V } , \mathsf { T } = 2 5 ^ { \circ } \mathsf { C }$ unless otherwise noted. 


Table 7. Accelerometer Electro-Mechanical Specifications


<table><tr><td>Subsystem</td><td>Parameter</td><td colspan="2">Typical</td><td>Unit</td><td>Comments</td></tr><tr><td rowspan="13">Accelerometer</td><td>Noise Density</td><td colspan="2">150</td><td>μg/√Hz</td><td>High-Resolution Mode</td></tr><tr><td rowspan="5">Sensitivity Scale Factor</td><td>Scale Setting</td><td>Sensitivity</td><td rowspan="5">LSB/g</td><td rowspan="5">16-Bit Output</td></tr><tr><td>±2 g</td><td>16,384</td></tr><tr><td>±4 g</td><td>8,192</td></tr><tr><td>±8 g</td><td>4,096</td></tr><tr><td>±16 g</td><td>2,048</td></tr><tr><td>Cross-Axis Sensitivity</td><td colspan="2">±1</td><td>%</td><td></td></tr><tr><td>Temperature Coefficient of Offset (TCO)</td><td colspan="2">±1</td><td>mg/°C</td><td>Over-Temperature Range of -40°C to 85°C, at Board Level</td></tr><tr><td>Temperature Coefficient of Sensitivity (TCS)</td><td colspan="2">±0.04</td><td>%/°C</td><td>Over-Temperature Range of -40°C to 85°C, at Board Level</td></tr><tr><td>Initial Offset Tolerance</td><td colspan="2">±100</td><td>mg</td><td>Board Level</td></tr><tr><td>Initial Sensitivity Tolerance</td><td colspan="2">±6</td><td>%</td><td>Board Level</td></tr><tr><td>Non-Linearity</td><td colspan="2">±0.75</td><td>%</td><td>Best Fit Line</td></tr><tr><td>System Turn On Time(1)</td><td colspan="2">15</td><td>ms</td><td>From Software Reset, No Power, or Power Down to Power-on Default state = t0 in Figure 16</td></tr><tr><td></td><td>Accel Turn On Time</td><td colspan="2">3 ms + 3/ODR</td><td>ms</td><td>Accel Turn on from Power-On Default state or from Low Power state = t2 + t5 in Figure 16.</td></tr></table>

## Note:

7. System Turn-On Time defines the initialization duration of QMI8658A, it starts from about 200us later than the release of POR(Power On Reset) or the Software Reset. Refer to 3.3 and 7.4 for details. 


Table 8. Gyroscope Electro-Mechanical Specifications


<table><tr><td>Subsystem</td><td>Parameter</td><td colspan="2">Typical</td><td>Unit</td><td>Comments</td></tr><tr><td rowspan="20">Gyroscope</td><td rowspan="9">Sensitivity</td><td>Scale Setting</td><td>Sensitivity</td><td rowspan="9">LSB/dps</td><td rowspan="9">16-Bit Output</td></tr><tr><td>±16 dps</td><td>2048</td></tr><tr><td>±32 dps</td><td>1024</td></tr><tr><td>±64 dps</td><td>512</td></tr><tr><td>±128 dps</td><td>256</td></tr><tr><td>±256 dps</td><td>128</td></tr><tr><td>±512 dps</td><td>64</td></tr><tr><td>±1024 dps</td><td>32</td></tr><tr><td>±2048 dps</td><td>16</td></tr><tr><td>Natural Frequency</td><td colspan="2">22.42</td><td>kHz</td><td>Precision +/- 2%(typical)</td></tr><tr><td>Noise Density</td><td colspan="2">13</td><td>mdps/√Hz</td><td>High-Resolution Mode</td></tr><tr><td>Non-Linearity</td><td colspan="2">±0.2</td><td>%</td><td></td></tr><tr><td>Cross-Axis Sensitivity</td><td colspan="2">±2</td><td>%</td><td></td></tr><tr><td>g-Sensitivity</td><td colspan="2">±0.1</td><td>dps/g</td><td></td></tr><tr><td>System Turn On Time(1)</td><td colspan="2">15</td><td>ms</td><td>From Software Reset, No Power, or Power Down to Power-on Default state = t0 in Figure 16</td></tr><tr><td>Gyro Turn On Time</td><td colspan="2">150 ms + 3/ODR</td><td>ms</td><td>Gyro Turn on from Power-On Default = t1 + t5 in Figure 16</td></tr><tr><td>Temperature Coefficient of Offset (TCO)</td><td colspan="2">X/Y: ±0.1Z: ±0.05</td><td>dps/°C</td><td>Over-Temperature Range of -40°C to 85°C, at Board Level</td></tr><tr><td>Temperature Coefficient of Sensitivity (TCS)</td><td colspan="2">X/Y: ±0.05Z: ±0.01</td><td>%/°C</td><td>Over-Temperature Range of -40°C to 85°C, at Board Level</td></tr><tr><td>Initial Offset Tolerance</td><td colspan="2">±10</td><td>dps</td><td>Board Level</td></tr><tr><td>Initial Sensitivity Tolerance</td><td colspan="2">±3</td><td>%</td><td>Board Level</td></tr></table>


Note: 



8. System Turn-On Time defines the initialization duration of QMI8658A, it starts from about 200us later than the release of POR(Power On Reset) or the Software Reset. Refer to 3.3 and 7.4 for details. 


## 3.5 Accelerometer Programmable Characteristics

VDD = VDDIO = 1.8 V, T = 25°C unless otherwise noted. Typical numbers are provided below unless otherwise noted. 

If only accelerometer is enabled, the ODR frequency is derived from the internal oscillator. If both accelerometer and gyroscope (6DOF mode) are enabled, the ODR frequency is derived from the natural frequency of gyroscope. Table 9 shows the two ODR frequencies, which can be referenced for later descriptions in the datasheet. Refer to section 5.3 for detailed ODR configuration. 

RMS noise can be calculated based on the noise density and the bandwidth. 


Table 9. Accelerometer Noise Density


<table><tr><td>Mode</td><td colspan="9">High-Resolution</td><td colspan="4">Low-Power</td><td>Unit</td></tr><tr><td>ODR (Accel only)</td><td></td><td></td><td></td><td>1000</td><td>500</td><td>250</td><td>125</td><td>62.5</td><td>31.25</td><td>128</td><td>21</td><td>11</td><td>3</td><td>Hz</td></tr><tr><td>ODR (Accel + Gyro)</td><td>7174.4</td><td>3587.2</td><td>1793.6</td><td>896.8</td><td>448.4</td><td>224.2</td><td>112.1</td><td>56.05</td><td>28.025</td><td></td><td></td><td></td><td></td><td>Hz</td></tr><tr><td>Typical Noise Density</td><td>150</td><td>150</td><td>150</td><td>150</td><td>150</td><td>150</td><td>150</td><td>150</td><td>150</td><td>125</td><td>180</td><td>285</td><td>700</td><td>μg/√Hz</td></tr></table>


Table 10. Accelerometer Filter Characteristics (Accelerometer only)(9)


<table><tr><td>Mode</td><td colspan="9">High-Resolution</td><td colspan="4">Low-Power</td><td>Unit</td></tr><tr><td>ODR</td><td>8000</td><td>4000</td><td>2000</td><td>1000</td><td>500</td><td>250</td><td>125</td><td>62.5</td><td>31.25</td><td>128</td><td>21</td><td>11</td><td>3</td><td rowspan="6">Hz</td></tr><tr><td>Bandwidth (Default, 27.5% of ODR)</td><td>NA</td><td>NA</td><td>NA</td><td>275</td><td>137.5</td><td>68.8</td><td>34.4</td><td>17.2</td><td>8.6</td><td>35.2</td><td>5.8</td><td>3.0</td><td>0.8</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 00 (2.66% of ODR)</td><td>NA</td><td>NA</td><td>NA</td><td>26.6</td><td>13.3</td><td>6.7</td><td>3.3</td><td>1.7</td><td>0.8</td><td>3.4</td><td>0.6</td><td>0.3</td><td>0.1</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 01 (3.63% of ODR)</td><td>NA</td><td>NA</td><td>NA</td><td>36.3</td><td>18.2</td><td>9.1</td><td>4.5</td><td>2.3</td><td>1.1</td><td>4.6</td><td>0.8</td><td>0.4</td><td>0.1</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 10 (5.39% of ODR)</td><td>NA</td><td>NA</td><td>NA</td><td>53.9</td><td>27</td><td>13.5</td><td>6.7</td><td>3.4</td><td>1.7</td><td>6.9</td><td>1.1</td><td>0.6</td><td>0.2</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 11 (13.37% of ODR)</td><td>NA</td><td>NA</td><td>NA</td><td>133.7</td><td>66.9</td><td>33.4</td><td>16.7</td><td>8.4</td><td>4.2</td><td>17.1</td><td>2.8</td><td>1.5</td><td>0.4</td></tr></table>


Note: 



When only accelerometer is enabled, the ODR is derived from the internal oscillator, rather than the nature frequency of Gyroscope 



Table 11. Accelerometer Filter Characteristics (6DOF)(10)


<table><tr><td>Mode</td><td colspan="9">High-Resolution</td><td colspan="4">Low-Power</td><td>Unit</td></tr><tr><td>ODR</td><td>7174.4</td><td>3587.2</td><td>1793.6</td><td>896.8</td><td>448.4</td><td>224.2</td><td>112.1</td><td>56.05</td><td>28.025</td><td>128</td><td>21</td><td>11</td><td>3</td><td rowspan="6">Hz</td></tr><tr><td>Bandwidth (Default, 27.5% of ODR)</td><td>1973.0</td><td>986.5</td><td>493.2</td><td>246.6</td><td>123.3</td><td>61.7</td><td>30.8</td><td>15.4</td><td>7.7</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 00 (2.66% of ODR)</td><td>190.8</td><td>95.4</td><td>47.7</td><td>23.9</td><td>11.9</td><td>6.0</td><td>3.0</td><td>1.5</td><td>0.7</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 01 (3.63% of ODR)</td><td>260.4</td><td>130.2</td><td>65.1</td><td>32.6</td><td>16.3</td><td>8.1</td><td>4.1</td><td>2.0</td><td>1.0</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 10 (5.39% of ODR)</td><td>386.7</td><td>193.4</td><td>96.7</td><td>48.3</td><td>24.2</td><td>12.1</td><td>6.0</td><td>3.0</td><td>1.5</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 11 (13.37% of ODR)</td><td>959.2</td><td>479.6</td><td>239.8</td><td>119.9</td><td>60.0</td><td>30.0</td><td>15.0</td><td>7.5</td><td>3.7</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr></table>


Note: 



10. When both accelerometer and gyroscope are both enabled, all frequencies are synchronized to the nature frequency of gyroscope. 


## 3.6 Gyroscope Programmable Characteristics

VDD = VDDIO = 1.8 V, T = 25°C, and represent typical numbers unless otherwise noted. All frequencies are synchronized to the gyroscope nature frequency. 

Table 12 shows the noise density of gyroscope output over different ODR configurations. 

The typical bandwidths of gyroscope over different ODR settings are listed in Table 13. 

RMS noise can be calculated based on the noise density and the bandwidth. 


Table 12. Gyroscope Noise Density


<table><tr><td>Mode</td><td colspan="9">High-Resolution</td><td>Unit</td></tr><tr><td>ODR (Gyro and/or Accel)</td><td>7174.4</td><td>3587.2</td><td>1793.6</td><td>896.8</td><td>448.4</td><td>224.2</td><td>112.1</td><td>56.05</td><td>28.025</td><td>Hz</td></tr><tr><td>Typical Noise Density</td><td><eq>RSV^{(11)}</eq></td><td><eq>RSV^{(11)}</eq></td><td><eq>RSV^{(11)}</eq></td><td>13</td><td>13</td><td>13</td><td>13</td><td>13</td><td>13</td><td>mdps/<eq>\sqrt{}</eq>Hz</td></tr></table>

## Note:

11. The gyroscope noise increases significantly when the ODR is set above 1KHz. It is not recommended to set the ODR beyond 1KHz for the applications that are impacted by gyroscope noise. For more information please contact QST FAE team. 


Table 13. Gyroscope Filter Characteristics


<table><tr><td>Mode</td><td colspan="9">High-Resolution</td><td>Unit</td></tr><tr><td>ODR</td><td>7174.4</td><td>3587.2</td><td>1793.6</td><td>896.8</td><td>448.4</td><td>224.2</td><td>112.1</td><td>56.05</td><td>28.025</td><td rowspan="6">Hz</td></tr><tr><td>Bandwidth (Default, 27.5% of ODR)</td><td>1973.0</td><td>986.5</td><td>493.2</td><td>246.6</td><td>123.3</td><td>61.7</td><td>30.8</td><td>15.4</td><td>7.7</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 00 (2.66% of ODR)</td><td>190.8</td><td>95.4</td><td>47.7</td><td>23.9</td><td>11.9</td><td>6.0</td><td>3.0</td><td>1.5</td><td>0.7</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 01 (3.63% of ODR)</td><td>260.4</td><td>130.2</td><td>65.1</td><td>32.6</td><td>16.3</td><td>8.1</td><td>4.1</td><td>2.0</td><td>1.0</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 10 (5.39% of ODR)</td><td>386.7</td><td>193.4</td><td>96.7</td><td>48.3</td><td>24.2</td><td>12.1</td><td>6.0</td><td>3.0</td><td>1.5</td></tr><tr><td>Bandwidth with Low-Pass Filter Enabled Mode 11 (13.37% of ODR)</td><td>959.2</td><td>479.6</td><td>239.8</td><td>119.9</td><td>60.0</td><td>30.0</td><td>15.0</td><td>7.5</td><td>3.7</td></tr></table>

## 3.7 Electrical Characteristics

VDD = VDDIO = 1.8 V, T = 25°C unless otherwise noted. 


Table 14. Electrical Subsystem Characteristics


<table><tr><td>Symbol</td><td colspan="2">Parameter</td><td>Min.</td><td>Typ.</td><td>Max.</td><td>Unit</td></tr><tr><td><eq>f_{SPC}</eq></td><td colspan="2">Host SPI Interface Speed</td><td></td><td></td><td>15</td><td>MHz</td></tr><tr><td><eq>f_{SCL}</eq></td><td colspan="2">Host <eq>I^{2}C</eq> Interface Speed (standard mode and Fast Mode are supported)</td><td></td><td></td><td>400</td><td>kHz</td></tr><tr><td><eq>f_{SCL3}</eq></td><td>Host <eq>I^{3}C</eq> Interface Speed</td><td>Standard Data Rate (SDR)</td><td></td><td></td><td>12.5</td><td>MHz</td></tr></table>

## 3.8 Current Consumption

VDD = VDDIO = 1.8 V, T = 25°C unless otherwise noted. IDD Current refers to the current flowing into the VDD pin. Typical numbers are provided below. 


Table 15. Current Consumption for Accelerometer Only Sensor Mode (Gyroscope Disabled)


<table><tr><td colspan="2">Mode</td><td colspan="6">High-Resolution</td><td colspan="4">Low-Power</td><td>Unit</td></tr><tr><td colspan="2">ODR</td><td>1000</td><td>500</td><td>250</td><td>125</td><td>62.5</td><td>31.25</td><td>128</td><td>21</td><td>11</td><td>3</td><td>Hz</td></tr><tr><td rowspan="2">Typical Overall IDD Current</td><td>Filters Disabled (aLPF=0)</td><td>182</td><td>155</td><td>142</td><td>134</td><td>133</td><td>132</td><td>55</td><td>42</td><td>35</td><td>30</td><td rowspan="2">μA</td></tr><tr><td>Filters Enabled (aLPF=1)</td><td>182</td><td>155</td><td>142</td><td>134</td><td>133</td><td>132</td><td>55</td><td>42</td><td>35</td><td>30</td></tr></table>


Table 16. Current Consumption for Gyroscope Only Sensor Mode (Accelerometer Disabled)


<table><tr><td colspan="2">Mode</td><td colspan="9">High-Resolution</td><td>Unit</td></tr><tr><td colspan="2">ODR</td><td>7174.4</td><td>3587.2</td><td>1793.6</td><td>896.8</td><td>448.4</td><td>224.2</td><td>112.1</td><td>56.05</td><td>28.025</td><td>Hz</td></tr><tr><td rowspan="2">Typical Overall IDD Current</td><td>Filters Disabled (gLPF=0)</td><td>908</td><td>861</td><td>748</td><td>689</td><td>659</td><td>656</td><td>654</td><td>653</td><td>651</td><td rowspan="2">μA</td></tr><tr><td>Filters Enabled (gLPF=1)</td><td>916</td><td>863</td><td>748</td><td>689</td><td>659</td><td>656</td><td>654</td><td>653</td><td>651</td></tr></table>


Table 17. Current Consumption for 6DOF Sensor Mode (Accelerometer and Gyroscope Enabled)


<table><tr><td colspan="2">Mode</td><td colspan="9">High-Resolution</td><td>Unit</td></tr><tr><td colspan="2">ODR</td><td>7174.4</td><td>3587.2</td><td>1793.6</td><td>896.8</td><td>448.4</td><td>224.2</td><td>112.1</td><td>56.05</td><td>28.025</td><td>Hz</td></tr><tr><td rowspan="2">Typical Overall IDD Current</td><td>Filters Disabled (aLPF=0; gLPF=0)</td><td>1004</td><td>956</td><td>843</td><td>786</td><td>757</td><td>754</td><td>752</td><td>751</td><td>750</td><td rowspan="2">μA</td></tr><tr><td>Filters Enabled (aLPF=1; gLPF=1)</td><td>1031</td><td>970</td><td>850</td><td>789</td><td>758</td><td>756</td><td>753</td><td>751</td><td>750</td></tr></table>

## 3.9 Temperature Sensor

The QMI8658A is equipped with an internal 16-bit embedded temperature sensor that is automatically turned on by default whenever the accelerometer or gyroscope is enabled. The temperature sensor is used internally to correct the temperature dependency of calibration parameters of the accelerometer and gyroscope. The temperature compensation is optimal in the range of -40°C to 85°C with a resolution of 0.0625°C (1/16 °C) or inversely, 16 LSB/ °C. 

The QMI8658A outputs the internal chip temperature that the HOST can read. The output is 16 bits, with a (1/256)°C per LSB resolution. To read the temperature, the HOST needs to access the TEMP register (see TEMP_L and TEMP_H in Data Output Registers in Table 25). 

The calculation formular is: 

$$
T = T E M P \_ H + (T E M P \_ L / 2 5 6)
$$

To read the temperature sensor data properly, the Host is expected to follow the guidelines in 13 Locking Mechanism. 


Table 18. Temperature Sensor Specifications


<table><tr><td>Subsystem</td><td>Parameter</td><td>Typical</td><td>Unit</td></tr><tr><td rowspan="6">Digital Temperature Sensor</td><td>Range</td><td>-40 to +85</td><td>°C</td></tr><tr><td>Internal Resolution</td><td>16</td><td>Bits</td></tr><tr><td>Internal Sensitivity</td><td>256</td><td>LSB/°C</td></tr><tr><td>Output Register Width</td><td>16</td><td>Bits</td></tr><tr><td>Output Sensitivity</td><td>256</td><td>LSB/°C</td></tr><tr><td>Refresh Rate</td><td>8</td><td>Hz</td></tr></table>

## 4 Register Map Overview

The QMI8658A UI registers enable programming and control of the inertial measurement unit and associated on-chip signal processing. These registers are accessed through the UI interface – either SPI (4 wires or 3 wires) I3C, or I2C. 

## 4.1 UI Register Map Overview

UI register map may be classified into the following register categories: 

Chip Information Registers 

Setup and Control Registers: control various aspects of the IMU. 

Host Controlled Calibration Registers: control and configure various aspects of the IMU via the host command interface called CTRL9 

Count Register for time stamping the sensor samples 

Sensor data registers 

▪ FIFO Registers: to set up the FIFO and detect data availability and over-run 

Activity Detection status registers 

General Purpose Registers 


Table 19. UI Register Overview


<table><tr><td rowspan="2">Name</td><td rowspan="2">Type</td><td colspan="3">Register Address</td><td>Default</td><td>Comment</td></tr><tr><td>Dec</td><td>Hex</td><td>Binary</td><td>Binary</td><td></td></tr><tr><td colspan="7">General Purpose Registers</td></tr><tr><td>WHO_AM_I</td><td>r</td><td>0</td><td>00</td><td>00000000</td><td>00000101</td><td>Device Identifier</td></tr><tr><td>REVISION_ID</td><td>r</td><td>1</td><td>01</td><td>00000001</td><td>01101000</td><td>Device Revision ID</td></tr><tr><td colspan="7">Setup and Control Registers</td></tr><tr><td>CTRL1</td><td>rw</td><td>2</td><td>02</td><td>00000010</td><td>00100000</td><td>SPI Interface and Sensor Enable</td></tr><tr><td>CTRL2</td><td>rw</td><td>3</td><td>03</td><td>00000011</td><td>00000000</td><td>Accelerometer: Output Data Rate, Full Scale, Self-Test</td></tr><tr><td>CTRL3</td><td>rw</td><td>4</td><td>04</td><td>00000100</td><td>00000000</td><td>Gyroscope: Output Data Rate, Full Scale, Self-Test</td></tr><tr><td>Reserved</td><td>rw</td><td>5</td><td>05</td><td>00000101</td><td>00000000</td><td>Reserved</td></tr><tr><td>CTRL5</td><td>rw</td><td>6</td><td>06</td><td>00000110</td><td>00000000</td><td>Low pass filter setting</td></tr><tr><td>Reserved</td><td>rw</td><td>7</td><td>07</td><td>00000111</td><td>00000000</td><td>Reserved</td></tr><tr><td>CTRL7</td><td>rw</td><td>8</td><td>08</td><td>00001000</td><td>00000000</td><td>Enable Sensors</td></tr><tr><td>CTRL8</td><td>rw</td><td>9</td><td>09</td><td>00001001</td><td>00000000</td><td>Motion Detection Control</td></tr><tr><td>CTRL9</td><td>rw</td><td>10</td><td>0A</td><td>00001010</td><td>00000000</td><td>Host Commands</td></tr><tr><td colspan="7">Host Controlled Calibration Registers (See CTRL9, Usage is Optional)</td></tr><tr><td>CAL1_L</td><td>rw</td><td>11</td><td>0B</td><td>00001011</td><td>00000000</td><td rowspan="2">Calibration RegisterCAL1_L - lower 8 bits. CAL1_H - upper 8 bits.</td></tr><tr><td>CAL1_H</td><td>rw</td><td>12</td><td>0C</td><td>00001100</td><td>00000000</td></tr><tr><td>CAL2_L</td><td>rw</td><td>13</td><td>0D</td><td>00001101</td><td>00000000</td><td rowspan="2">Calibration RegisterCAL2_L - lower 8 bits. CAL2_H - upper 8 bits.</td></tr><tr><td>CAL2_H</td><td>rw</td><td>14</td><td>0E</td><td>00001110</td><td>00000000</td></tr><tr><td>CAL3_L</td><td>rw</td><td>15</td><td>0F</td><td>00001111</td><td>00000000</td><td rowspan="2">Calibration RegisterCAL3_L - lower 8 bits. CAL3_H - upper 8 bits.</td></tr><tr><td>CAL3_H</td><td>rw</td><td>16</td><td>10</td><td>00010000</td><td>00000000</td></tr><tr><td>CAL4_L</td><td>rw</td><td>17</td><td>11</td><td>00010001</td><td>00000000</td><td rowspan="2">Calibration RegisterCAL4_L - lower 8 bits. CAL4_H - upper 8 bits.</td></tr><tr><td>CAL4_H</td><td>rw</td><td>18</td><td>12</td><td>00010010</td><td>00000000</td></tr><tr><td colspan="7">FIFO Registers</td></tr><tr><td>FIFO_WTM_TH</td><td>rw</td><td>19</td><td>13</td><td>00010011</td><td>00000000</td><td>FIFO watermark level, in ODRs</td></tr><tr><td>FIFO_CTRL</td><td>rw</td><td>20</td><td>14</td><td>00010100</td><td>00000000</td><td>FIFO Setup</td></tr><tr><td>FIFO_SMPL_CNT</td><td>r</td><td>21</td><td>15</td><td>00010101</td><td>00000000</td><td>FIFO sample count LSBs</td></tr><tr><td>FIFO_STATUS</td><td>r</td><td>22</td><td>16</td><td>00010110</td><td>00000000</td><td>FIFO Status</td></tr><tr><td>FIFO_DATA</td><td>r</td><td>23</td><td>17</td><td>00010111</td><td>00000000</td><td>FIFO Data</td></tr><tr><td colspan="7">Status Registers</td></tr><tr><td>STATUSINT</td><td>r</td><td>45</td><td>2D</td><td>00101101</td><td>00000000</td><td>Sensor Data Availability with the Locking mechanism, CmdDone (CTRL9 protocol bit).</td></tr><tr><td>STATUS0</td><td>r</td><td>46</td><td>2E</td><td>00101110</td><td>00000000</td><td>Output Data Over Run and Data Availability.</td></tr><tr><td>STATUS1</td><td>r</td><td>47</td><td>2F</td><td>00101111</td><td>00000000</td><td>Miscellaneous Status: Any Motion, No Motion, Significant Motion, Pedometer, Tap.</td></tr><tr><td colspan="7">Timestamp Register</td></tr><tr><td rowspan="2">TIMESTAMP_LOW</td><td rowspan="2">r</td><td rowspan="2">48</td><td rowspan="2">30</td><td rowspan="2">00110000</td><td rowspan="2">00000000</td><td>Sample Time Stamp</td></tr><tr><td>TIMESTAMP_LOW - lower 8 bits.</td></tr><tr><td>TIMESTAMP_MID</td><td>r</td><td>49</td><td>31</td><td>00110001</td><td>00000000</td><td>TIMESTAMP_MID - middle 8 bits.</td></tr><tr><td>TIMESTAMP_HIGH</td><td>r</td><td>50</td><td>32</td><td>00110010</td><td>00000000</td><td>TIMESTAMP_HIGH - upper 8 bits</td></tr><tr><td colspan="7">Data Output Registers (16 bits 2's Complement Except COD Sensor Data)</td></tr><tr><td>TEMP_L</td><td>r</td><td>51</td><td>33</td><td>00110011</td><td>00000000</td><td>Temperature Output Data</td></tr><tr><td>TEMP_H</td><td>r</td><td>52</td><td>34</td><td>00110100</td><td>00000000</td><td>TEMP_L - lower 8 bits. TEMP_H - upper 8 bits</td></tr><tr><td>AX_L</td><td>r</td><td>53</td><td>35</td><td>00110101</td><td>00000000</td><td>X-axis Acceleration</td></tr><tr><td>AX_H</td><td>r</td><td>54</td><td>36</td><td>00110110</td><td>00000000</td><td>AX_L - lower 8 bits. AX_H - upper 8 bits</td></tr><tr><td>AY_L</td><td>r</td><td>55</td><td>37</td><td>00110111</td><td>00000000</td><td>Y-axis Acceleration</td></tr><tr><td>AY_H</td><td>r</td><td>56</td><td>38</td><td>00111000</td><td>00000000</td><td>AY_L - lower 8 bits. AY_H - upper 8 bits</td></tr><tr><td>AZ_L</td><td>r</td><td>57</td><td>39</td><td>00111001</td><td>00000000</td><td>Z-axis Acceleration</td></tr><tr><td>AZ_H</td><td>r</td><td>58</td><td>3A</td><td>00111010</td><td>00000000</td><td>AZ_L - lower 8 bits. AZ_H - upper 8 bits</td></tr><tr><td>GX_L</td><td>r</td><td>59</td><td>3B</td><td>00111011</td><td>00000000</td><td>X-axis Angular Rate</td></tr><tr><td>GX_H</td><td>r</td><td>60</td><td>3C</td><td>00111100</td><td>00000000</td><td>GX_L - lower 8 bits. GX_H - upper 8 bits</td></tr><tr><td>GY_L</td><td>r</td><td>61</td><td>3D</td><td>00111101</td><td>00000000</td><td>Y-axis Angular Rate</td></tr><tr><td>GY_H</td><td>r</td><td>62</td><td>3E</td><td>00111110</td><td>00000000</td><td>GY_L - lower 8 bits. GY_H - upper 8 bits</td></tr><tr><td>GZ_L</td><td>r</td><td>63</td><td>3F</td><td>00111111</td><td>00000000</td><td>Z-axis Angular Rate</td></tr><tr><td>GZ_H</td><td>r</td><td>64</td><td>40</td><td>01000000</td><td>00000000</td><td>GZ_L - lower 8 bits. GZ_H - upper 8 bits</td></tr><tr><td colspan="7">COD Indication and General Purpose Registers</td></tr><tr><td>COD_STATUS</td><td>r</td><td>70</td><td>46</td><td>01000110</td><td>00000000</td><td>Calibration-On-Demand status register</td></tr><tr><td>dQW_L</td><td>r</td><td>73</td><td>49</td><td>01001001</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dQW_H</td><td>r</td><td>74</td><td>4A</td><td>01001010</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dQX_L</td><td>r</td><td>75</td><td>4B</td><td>01001011</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dQX_H</td><td>r</td><td>76</td><td>4C</td><td>01001100</td><td>00000000</td><td>Reserved</td></tr><tr><td>dQY_L</td><td>r</td><td>77</td><td>4D</td><td>01001101</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dQY_H</td><td>r</td><td>78</td><td>4E</td><td>01001110</td><td>00000000</td><td>Reserved</td></tr><tr><td>dQZ_L</td><td>r</td><td>79</td><td>4F</td><td>01001111</td><td>00000000</td><td>Reserved</td></tr><tr><td>dQZ_H</td><td>r</td><td>80</td><td>50</td><td>01010000</td><td>00000000</td><td>Reserved</td></tr><tr><td>dVX_L</td><td>r</td><td>81</td><td>51</td><td>01010001</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dVX_H</td><td>r</td><td>82</td><td>52</td><td>01010010</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dVY_L</td><td>r</td><td>83</td><td>53</td><td>01010011</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dVY_H</td><td>r</td><td>84</td><td>54</td><td>01010100</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dVZ_L</td><td>r</td><td>85</td><td>55</td><td>01010101</td><td>00000000</td><td>General purpose register</td></tr><tr><td>dVZ_H</td><td>r</td><td>86</td><td>56</td><td>01010110</td><td>00000000</td><td>General purpose register</td></tr><tr><td colspan="7">Activity Detection Output Registers</td></tr><tr><td>TAP_STATUS</td><td>r</td><td>89</td><td>59</td><td>01011001</td><td>00000000</td><td>Axis, direction, number of detected Tap</td></tr><tr><td>STEP_CNT_LOW</td><td>r</td><td>90</td><td>5A</td><td>01011010</td><td>00000000</td><td>Low byte of step count of Pedometer</td></tr><tr><td>STEP_CNT_MIDL</td><td>r</td><td>91</td><td>5B</td><td>01011011</td><td>00000000</td><td>Middle byte of step count of Pedometer</td></tr><tr><td>STEP_CNT_HIGH</td><td>r</td><td>92</td><td>5C</td><td>01011100</td><td>00000000</td><td>High byte of step count of Pedometer</td></tr><tr><td colspan="7">Reset Register</td></tr><tr><td>RESET</td><td>w</td><td>96</td><td>60</td><td>01100000</td><td>00000000</td><td>Soft Reset Register</td></tr></table>

## 5 UI Sensor Configuration Settings and Output Data

## 5.1 Typical Sensor Mode Configuration and Output Data

In Typical sensor mode, QMI8658A outputs raw sensor values. The sensors are configured and read using the registers described below. The accelerometer and gyroscope can be independently configured. Table 20 summarizes these pertinent registers. 


Table 20. Typical Sensor Mode Configuration and Output Data


<table><tr><td colspan="4">Typical Sensor Configuration and Output Data</td></tr><tr><td>Description</td><td>Registers</td><td>Unit</td><td>Comments</td></tr><tr><td>Sensor Enable, SPI 3 or 4 Wire</td><td>CTRL1</td><td></td><td>Control power states, configure SPI communications</td></tr><tr><td>Enable Sensor</td><td>CTRL7</td><td></td><td>Individually Enable/Disable the AttitudeEngine, Accelerometer, and Gyroscope Using sEN, aEN, and gEN bits, respectively.</td></tr><tr><td>Configure Accelerometer, Enable Self-Test</td><td>CTRL2</td><td></td><td>Configure Full Scale and Output Data Rate; Enable Self-Test</td></tr><tr><td>Configure Gyroscope, Enable Self-Test</td><td>CTRL3</td><td></td><td>Configure Full Scale and Output Data Rate; Enable Self-Test</td></tr><tr><td>Sensor Filters</td><td>CTRL5</td><td></td><td>Configure and Enable/Disable Low Pass Filters</td></tr><tr><td>Status</td><td>STATUSINT STATUS0, STATUS1</td><td></td><td>Data Availability, FIFO Ready to be Read, CTRL9 Protocol Bit</td></tr><tr><td>Time Stamp</td><td>TIMESTAMP[H,M,L]</td><td></td><td>Sample Time Stamp (Circular Register 0 – 0xFFFFFFF)</td></tr><tr><td>Acceleration</td><td>A[X,Y,Z]_[H,L]</td><td>g</td><td>In Sensor Frame of Reference, Right-handed Coordinate System</td></tr><tr><td>Angular Rate</td><td>G[X,Y,Z]_[H,L]</td><td>dps</td><td>In Sensor Frame of Reference, Right-handed Coordinate System</td></tr><tr><td>Temperature</td><td>TEMP_[H,L]</td><td>°C</td><td>Temperature of the Sensor</td></tr><tr><td>FIFO Based Output</td><td>FIFO_DATA</td><td></td><td>1 Byte FIFO Data Outputs</td></tr></table>

## 5.2 Chip Information Register


Table 21. Chip Information Register Description


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">WHO_AM_I</td><td colspan="2">Register Address: 0 (0x00)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>WHO_AM_I</td><td>0x05</td><td>Device identifier 0x05 - to identify the device is a QST sensor. Read-only.</td></tr><tr><td colspan="2">REVISION_ID</td><td colspan="2">Register Address: 1 (0x01)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>REVISION_ID</td><td>0x7C</td><td>Device Revision ID. Read-only.</td></tr></table>

## 5.3 Configuration Registers

This section describes the various operating modes and register configurations of the QMI8658A. 


Table 22. Configuration Registers Description


<table><tr><td colspan="2">Register Name</td><td colspan="5"></td></tr><tr><td colspan="2">CTRL1</td><td colspan="5">Serial Interface and Sensor Enable. Register Address: 2 (0x02)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="4">Description</td></tr><tr><td>7</td><td>SIM</td><td>1&#x27;b0</td><td colspan="4">0: Enables 4-wire SPI interface1: Enables 3-wire SPI interface</td></tr><tr><td>6</td><td>ADDR_AI</td><td>1&#x27;b0</td><td colspan="4">0: Serial interface (SPI, I2C, I3C) address non-increment.1: Serial interface (SPI, I2C, I3C) address auto increment</td></tr><tr><td>5</td><td>BE</td><td>1&#x27;b1</td><td colspan="4">0: Serial interface (SPI, I2C, I3C) read data Little-Endian1: Serial interface (SPI, I2C, I3C) read data Big-Endian</td></tr><tr><td>4</td><td>INT2_EN</td><td>1&#x27;b0</td><td colspan="4">0: INT2 pin is high-Z mode1: INT2 pin output is enabled</td></tr><tr><td>3</td><td>INT1_EN</td><td>1&#x27;b0</td><td colspan="4">0: INT1 pin is high-Z mode1: INT1 pin output is enabled</td></tr><tr><td>2</td><td>FIFO_INT_SEL</td><td>1&#x27;b0</td><td colspan="4">0: FIFO interrupt is mapped to INT2 pin1: FIFO interrupt is mapped to INT1 pin</td></tr><tr><td>1</td><td>Reserved</td><td>1&#x27;b0</td><td colspan="4"></td></tr><tr><td>0</td><td>SensorDisable</td><td>1&#x27;b0</td><td colspan="4">0: Enable internal high-speed oscillator1: Disable internal high-speed oscillator. Refer to 7.1.</td></tr><tr><td colspan="2">CTRL2</td><td colspan="5">Accelerometer Settings: Address: 3 (0x03)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="4">Description</td></tr><tr><td>7</td><td>aST</td><td>1&#x27;b0</td><td colspan="4">0: Disable Accelerometer Self-Test;1: Enable Accelerometer Self-Test.</td></tr><tr><td>6:4</td><td>aFS&lt;2:0&gt;</td><td>3&#x27;b0</td><td colspan="4">Set Accelerometer Full-scale (1xx - N/A):000 - Accelerometer Full-scale = ±2 g001 - Accelerometer Full-scale = ±4 g010 - Accelerometer Full-scale = ±8 g011 - Accelerometer Full-scale = ±16 g</td></tr><tr><td rowspan="18">3:0</td><td rowspan="18">aODR&lt;3:0&gt;(12)(13)</td><td rowspan="18">4&#x27;b0</td><td colspan="4">Set Accelerometer Output Data Rate (ODR):</td></tr><tr><td>Setting</td><td>ODR Rate (Hz)(Accel only)</td><td>ODR Rate (Hz)(6DOF)(13)</td><td>Mode</td></tr><tr><td>0000</td><td>N/A</td><td>7174.4</td><td>Normal</td></tr><tr><td>0001</td><td>N/A</td><td>3587.2</td><td>Normal</td></tr><tr><td>0010</td><td>N/A</td><td>1793.6</td><td>Normal</td></tr><tr><td>0011</td><td>1000</td><td>896.8</td><td>Normal</td></tr><tr><td>0100</td><td>500</td><td>448.4</td><td>Normal</td></tr><tr><td>0101</td><td>250</td><td>224.2</td><td>Normal</td></tr><tr><td>0110</td><td>125</td><td>112.1</td><td>Normal</td></tr><tr><td>0111</td><td>62.5</td><td>56.05</td><td>Normal</td></tr><tr><td>1000</td><td>31.25</td><td>28.025</td><td>Normal</td></tr><tr><td>1001</td><td>N/A</td><td>N/A</td><td></td></tr><tr><td>1010</td><td>N/A</td><td>N/A</td><td></td></tr><tr><td>1011</td><td>N/A</td><td>N/A</td><td></td></tr><tr><td>1100</td><td>128</td><td>N/A</td><td>Low Power</td></tr><tr><td>1101</td><td>21</td><td>N/A</td><td>Low Power</td></tr><tr><td>1110</td><td>11</td><td>N/A</td><td>Low Power</td></tr><tr><td>1111</td><td>3</td><td>N/A</td><td>Low Power</td></tr></table>


Table 22 Configuration Register Description (Continued)


<table><tr><td colspan="2">Register Name</td><td colspan="4"></td></tr><tr><td colspan="2">CTRL3</td><td colspan="4">Gyroscope Settings: Address 4 (0x04)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="3">Description</td></tr><tr><td>7</td><td>gST</td><td>1&#x27;b0</td><td colspan="3">0: Disable Gyro self-Test1: Enable Gyro Self-Test.</td></tr><tr><td>6:4</td><td>gFS&lt;2:0&gt;</td><td>3&#x27;b0</td><td colspan="3">Set Gyroscope Full-scale:000 - ±16 dps001 - ±32 dps010 - ±64 dps011 - ±128 dps100 - ±256 dps101 - ±512 dps110 - ±1024dps111 - ±2048 dps</td></tr><tr><td rowspan="18">3:0</td><td rowspan="18">gODR&lt;3:0&gt;(13)</td><td rowspan="18">4&#x27;b0</td><td colspan="3">Set Gyroscope Output Data Rate (ODR):</td></tr><tr><td>Setting</td><td>ODR Rate (Hz)</td><td>Mode</td></tr><tr><td>0000</td><td>7174.4</td><td>Normal</td></tr><tr><td>0001</td><td>3587.2</td><td>Normal</td></tr><tr><td>0010</td><td>1793.6</td><td>Normal</td></tr><tr><td>0011</td><td>896.8</td><td>Normal</td></tr><tr><td>0100</td><td>448.4</td><td>Normal</td></tr><tr><td>0101</td><td>224.2</td><td>Normal</td></tr><tr><td>0110</td><td>112.1</td><td>Normal</td></tr><tr><td>0111</td><td>56.05</td><td>Normal</td></tr><tr><td>1000</td><td>28.025</td><td>Normal</td></tr><tr><td>1001</td><td>N/A</td><td></td></tr><tr><td>1010</td><td>N/A</td><td></td></tr><tr><td>1011</td><td>N/A</td><td></td></tr><tr><td>1100</td><td>N/A</td><td></td></tr><tr><td>1101</td><td>N/A</td><td></td></tr><tr><td>1110</td><td>N/A</td><td></td></tr><tr><td>1111</td><td>N/A</td><td></td></tr></table>

## Note:

12. The accelerometer low power mode is only available when the gyroscope is disabled 

13. In 6DOF mode (accelerometer and gyroscope are both enabled), the ODR is derived from the nature frequency of gyroscope, refer to section 3.5 for more information. 


Table 22 Configuration Register Description (Continued)


<table><tr><td colspan="2">Register Name</td><td colspan="3"></td></tr><tr><td colspan="2">CTRL5</td><td colspan="3">Sensor Data Processing Settings. Register Address: 6 (0x06)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="2">Description</td></tr><tr><td>7</td><td>Reserved</td><td>1&#x27;b0</td><td colspan="2"></td></tr><tr><td rowspan="5">6:5</td><td rowspan="5">gLPF_MODE</td><td rowspan="5">2&#x27;b0</td><td>gLPF_MODE</td><td>BW [Hz]</td></tr><tr><td>00</td><td>2.66% of ODR</td></tr><tr><td>01</td><td>3.63% of ODR</td></tr><tr><td>10</td><td>5.39% of ODR</td></tr><tr><td>11</td><td>13.37% of ODR</td></tr><tr><td>4</td><td>gLPF_EN</td><td>1&#x27;b0</td><td colspan="2">0: Disable Gyroscope Low-Pass Filter.1: Enable Gyroscope Low-Pass Filter with the mode given by gLPF_MODE.</td></tr><tr><td>3</td><td>Reserved</td><td>1&#x27;b0</td><td colspan="2"></td></tr><tr><td rowspan="5">2:1</td><td rowspan="5">aLPF_MODE</td><td rowspan="5">2&#x27;b0</td><td>aLPF_MODE</td><td>BW [Hz]</td></tr><tr><td>00</td><td>2.66% of ODR</td></tr><tr><td>01</td><td>3.63% of ODR</td></tr><tr><td>10</td><td>5.39% of ODR</td></tr><tr><td>11</td><td>13.37% of ODR</td></tr><tr><td>0</td><td>aLPF_EN</td><td>1&#x27;b0</td><td colspan="2">0: Disable Accelerometer Low-Pass Filter.1: Enable Accelerometer Low-Pass Filter with the mode given by aLPF_MODE.</td></tr></table>


Table 22 Configuration Register Description (Continued)


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">CTRL7</td><td colspan="2">Enable Sensors and Configure Data Reads. Register Address: 8 (0x08)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7</td><td>SyncSample</td><td>1&#x27;b0</td><td>0: Disable SyncSample mode1: Enable SyncSample mode</td></tr><tr><td>6</td><td>Reserved</td><td>1&#x27;b0</td><td></td></tr><tr><td>5</td><td>DRDY_DIS</td><td>1&#x27;b0</td><td>0: DRDY(Data Ready) is enabled, is driven to the INT2 pin1: DRDY(Data Ready) is disabled, is blocked from the INT2 pin</td></tr><tr><td>4</td><td>gSN</td><td>1&#x27;b0</td><td>0: Gyroscope in Full Mode (Drive and Sense are enabled).1: Gyroscope in Snooze Mode (only Drive enabled).This bit is effective only when gEN is set to 1. Refer to 7.1.</td></tr><tr><td>3:2</td><td>Reserved</td><td>2&#x27;b0</td><td></td></tr><tr><td>1</td><td>gEN</td><td>1&#x27;b0</td><td>0: Disable Gyroscope.1: Enable Gyroscope.</td></tr><tr><td>0</td><td>aEN</td><td>1&#x27;b0</td><td>0: Disable Accelerometer.1: Enable Accelerometer.</td></tr><tr><td colspan="2">CTRL8</td><td colspan="2">Motion Detection Control. Register Address: 9 (0x09)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7</td><td>CTRL9_HandShake_Type</td><td>1b&#x27;0</td><td>0: use INT1 as CTRL9 handshake1: use STATUSINT.bit7 as CTRL9 handshake</td></tr><tr><td>6</td><td>ACTIVITY_INT_SEL</td><td>1b&#x27;0</td><td>0: INT2 is used for Activity Detection event interrupt1: INT1 is used for Activity Detection event interruptNote: this bit influences the Any/No/Sig-motion, Pedometer, Tap Detection interrupt</td></tr><tr><td>5</td><td>reserved</td><td>1b&#x27;0</td><td></td></tr><tr><td>4</td><td>Pedo_EN</td><td>1b&#x27;0</td><td>0: disable Pedometer engine1: enable Pedometer engine</td></tr><tr><td>3</td><td>Sig-Motion_EN</td><td>1b&#x27;0</td><td>0: disable Significant Motion engine1: enable Significant Motion engine</td></tr><tr><td>2</td><td>No-Motion_EN</td><td>1b&#x27;0</td><td>0: disable No Motion engine1: enable No Motion engine</td></tr><tr><td>1</td><td>Any-Motion_EN</td><td>1b&#x27;0</td><td>0: disable Any Motion engine1: enable Any Motion engine</td></tr><tr><td>0</td><td>Tap_EN</td><td>1b&#x27;0</td><td>0: disable Tap engine1: enable Tap engine</td></tr><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">CTRL9</td><td colspan="2">Host Commands. Register Address: 10 (0x0A), Referred to:CTRL 9 Functionality (Executing Pre-defined Commands)</td></tr></table>

## 5.4 FIFO Registers


Table 23. FIFO Control/Status/Data Registers


<table><tr><td colspan="2">Register Name</td><td colspan="4"></td></tr><tr><td colspan="2">FIFO_WTM_TH</td><td colspan="4">FIFO Watermark Register Address: 19 (0x13)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="3">Description</td></tr><tr><td>7:0</td><td>FIFO_WTM</td><td>8&#x27;h0</td><td colspan="3">Number of ODRs(Samples) needed to trigger FIFO watermark</td></tr><tr><td colspan="2">FIFO_CTRL</td><td colspan="4">FIFO Control Register Address: 20 (0x14)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="3">Description</td></tr><tr><td>7</td><td>FIFO_RD_MODE</td><td>1&#x27;b0</td><td colspan="3">0: FIFO is in Write mode, sensor data (if enabled) can be filled into FIFO1: FIFO is in Read mode, FIFO data can be read via FIFO_DATA registerThis bit is automatically set by using a CTRL9 command. It must be cleared again (by write 1&#x27;b0 to this bit) after the data read is complete so that filling data to the FIFO can resume. Refer to 5.10.6.3.</td></tr><tr><td>6:4</td><td>Reserved</td><td>3&#x27;b0</td><td colspan="3"></td></tr><tr><td rowspan="5">3:2</td><td rowspan="5">FIFO_SIZE</td><td rowspan="5">2&#x27;b0</td><td>FIFO_SIZE[1:0]</td><td>FIFO Sample Size</td><td rowspan="5"></td></tr><tr><td>00</td><td>16 samples</td></tr><tr><td>01</td><td>32 samples</td></tr><tr><td>10</td><td>64 samples</td></tr><tr><td>11</td><td>128 samples</td></tr><tr><td rowspan="5">1:0</td><td rowspan="5">FIFO_MODE</td><td rowspan="5">2&#x27;b0</td><td>FIFO_MODE[1:0]</td><td>FIFO Mode</td><td rowspan="5"></td></tr><tr><td>00</td><td>Bypass (FIFO disable)</td></tr><tr><td>01</td><td>FIFO</td></tr><tr><td>10</td><td>Stream</td></tr><tr><td>11</td><td>Reserved</td></tr><tr><td colspan="2">FIFO_SMPL_CNT</td><td colspan="4">FIFO Sample Count Register Address: 21 (0x15)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="3">Description</td></tr><tr><td>7:0</td><td>FIFO_SMPL_CNT_LSB</td><td>8&#x27;b0</td><td colspan="3">8 LS bits of FIFO Sample Count, in word (2bytes).</td></tr><tr><td colspan="2">FIFO_STATUS</td><td colspan="4">FIFO Status. Register Address 22 (0x16)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="3">Description</td></tr><tr><td>7</td><td>FIFO_FULL</td><td>1&#x27;b0</td><td colspan="3">0: FIFO is not Full1: FIFO is Full</td></tr><tr><td>6</td><td>FIFO_WTM</td><td>1&#x27;b0</td><td colspan="3">0: FIFO Water Mark Level not hit.1: FIFO Water Mark Level Hit</td></tr><tr><td>5</td><td>FIFO_OVFLOW</td><td>1&#x27;b0</td><td colspan="3">0: FIFO Overflow has not happened1: FIFO Overflow condition has happened (data dropping happened)</td></tr><tr><td>4</td><td>FIFO_NOT_EMPTY</td><td>1&#x27;b0</td><td colspan="3">0: FIFO is Empty1: FIFO is not Empty</td></tr><tr><td>3:2</td><td>Reserved</td><td>2&#x27;b0</td><td colspan="3"></td></tr><tr><td>1:0</td><td>FIFO_SMPL_CNT_MSB</td><td>2&#x27;b0</td><td colspan="3">2 MS bits of FIFO Sample Count in word (2bytes).</td></tr><tr><td colspan="2">FIFO_DATA</td><td colspan="4">FIFO DATA Output Register Address: 23 (0x17)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td colspan="3">Description</td></tr><tr><td>7:0</td><td>FIFO_DATA</td><td>8&#x27;b0</td><td colspan="3">8 bit FIFO data output.</td></tr></table>

## 5.5 Status and Time Stamp Registers


Table 24. Status and Time Stamp Registers


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">STATUSINT</td><td colspan="2">Sensor Data Available and Lock Register Address: 45 (0x2D)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7</td><td>Ctrl9 CmdDone</td><td>1'b0</td><td>Indicates CTRL9 Command was done, as part of CTRL9 protocol0: Not Completed1: Done</td></tr><tr><td>6:2</td><td>Reserved</td><td>5'b0</td><td></td></tr><tr><td>1</td><td>Locked</td><td>1'b0</td><td>If syncSmpl (CTRL7.bit7) = 1:0: Sensor Data is not locked.1: Sensor Data is locked.If syncSmpl = 0, this bit shows the same value of INT1 level</td></tr><tr><td>0</td><td>Avail</td><td>1'b0</td><td>If syncSmpl (CTRL7.bit7) = 1:0: Sensor Data is not available1: Sensor Data is available for readingIf syncSmpl = 0, this bit shows the same value of INT2 level</td></tr><tr><td colspan="2">STATUS0</td><td colspan="2">Output Data Status Register Address: 46 (0x2E)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:2</td><td>Reserved</td><td>6'b0</td><td></td></tr><tr><td>1</td><td>gDA</td><td>1'b0</td><td>Gyroscope new data available0: No updates since last read.1: New data available.</td></tr><tr><td>0</td><td>aDA</td><td>1'b0</td><td>Accelerometer new data available0: No updates since last read.1: New data available.</td></tr><tr><td colspan="2">STATUS1</td><td colspan="2">Miscellaneous Status. Register Address 47 (0x2F)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7</td><td>Significant Motion</td><td>1'b0</td><td>0: No Significant-Motion was detected1: Significant-Motion was detected</td></tr><tr><td>6</td><td>No Motion</td><td>1'b0</td><td>0: No No-Motion was detected1: No-Motion was detected</td></tr><tr><td>5</td><td>Any Motion</td><td>1'b0</td><td>0: No Any-Motion was detected1: Any-Motion was detected</td></tr><tr><td>4</td><td>Pedometer</td><td>1'b0</td><td>0: No step was detected1: step was detected</td></tr><tr><td>3</td><td>Reserved</td><td>1'b0</td><td></td></tr><tr><td>2</td><td>WoM</td><td>1'b0</td><td>0: No WoM was detected1: WoM was detected</td></tr><tr><td>1</td><td>TAP</td><td>1'b0</td><td>0: No Tap was detected1: Tap was detected</td></tr><tr><td>0</td><td>Reserved</td><td>1'b0</td><td></td></tr><tr><td colspan="2">TIMESTAMP</td><td colspan="2">3 Bytes Sample Time Stamp - Output Count.Register Address: 48 - 50 (0x30 - 0x32)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>TIMESTAMP_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="3">Sample time stamp. Count incremented by one for each sample (x, y, z data set) from sensor with highest ODR (circular register 0x0-0xFFFFFFF).</td></tr><tr><td>7:0</td><td>TIMESTAMP_M&lt;15:8&gt;</td><td>0x00</td></tr><tr><td>7:0</td><td>TIMESTAMP_H&lt;23:16&gt;</td><td>0x00</td></tr></table>

## 5.6 Sensor Data Output Registers


Table 25. Sensor Data Output Registers Description


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">TEMP_[H,L]</td><td colspan="2">Temp Sensor Output. Register Address: 0x33 - 0x34</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>TEMP_L</td><td>0x00</td><td rowspan="2">Temperature output (°C) in two&#x27;s complement.T = TEMP_H + (TEMP_L / 256)</td></tr><tr><td>7:0</td><td>TEMP_H</td><td>0x00</td></tr><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">A[X,Y,Z]_[H,L]</td><td colspan="2">Acceleration Output. Register Address: 0x35 - 0x3A</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>AX_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="2">X-axis acceleration in two&#x27;s complement.AX_L - lower 8 bits. AX_H - upper 8 bits.</td></tr><tr><td>7:0</td><td>AX_H&lt;15:8&gt;</td><td>0x00</td></tr><tr><td>7:0</td><td>AY_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="2">Y-axis acceleration in two&#x27;s complement.AY_L - lower 8 bits. AY_H - upper 8 bits.</td></tr><tr><td>7:0</td><td>AY_H&lt;15:8&gt;</td><td>0x00</td></tr><tr><td>7:0</td><td>AZ_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="2">Z-axis acceleration in two&#x27;s complement.AZ_L - lower 8 bits. AZ_H - upper 8 bits.</td></tr><tr><td>7:0</td><td>AZ_H&lt;15:8&gt;</td><td>0x00</td></tr><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">G[X,Y.Z]_[H,L]</td><td colspan="2">Angular Rate Output. Register Address: 0x3B - 0x40</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>GX_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="2">X-axis angular rate in two&#x27;s complement.GX_L - lower 8 bits. GX_H - upper 8 bits.</td></tr><tr><td>7:0</td><td>GX_H&lt;15:8&gt;</td><td>0x00</td></tr><tr><td>7:0</td><td>GY_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="2">Y-axis angular rate in two&#x27;s complement.GY_L - lower 8 bits. GY_H - upper 8 bits.</td></tr><tr><td>7:0</td><td>GY_H&lt;15:8&gt;</td><td>0x00</td></tr><tr><td>7:0</td><td>GZ_L&lt;7:0&gt;</td><td>0x00</td><td rowspan="2">Z-axis angular rate in two&#x27;s complement.GZ_L - lower 8 bits. GZ_H - upper 8 bits.</td></tr><tr><td>7:0</td><td>GZ_H&lt;15:8&gt;</td><td>0x00</td></tr></table>


5.7 Calibration-On-Demand (COD) Status Register


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">COD_STATUS</td><td colspan="2">Register Address: 70 (0x46)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7</td><td>X_Limit_L_Fail</td><td>1&#x27;b0</td><td>0: COD passed for checking low sensitivity limit of X axis of gyroscope1: COD failed for checking low sensitivity limit of X axis of gyroscope</td></tr><tr><td>6</td><td>X_Limit_H_Fail</td><td>1&#x27;b0</td><td>0: COD passed for checking high sensitivity limit of X axis of gyroscope1: COD failed for checking high sensitivity limit of X axis of gyroscope</td></tr><tr><td>5</td><td>Y_Limit_L_Fail</td><td>1&#x27;b0</td><td>0: COD passed for checking low sensitivity limit of Y axis of gyroscope1: COD failed for checking low sensitivity limit of Y axis of gyroscope</td></tr><tr><td>4</td><td>Y_Limit_H_Fail</td><td>1&#x27;b0</td><td>0: COD passed for checking high sensitivity limit of Y axis of gyroscope1: COD failed for checking high sensitivity limit of Y axis of gyroscope</td></tr><tr><td>3</td><td>Accel_Check</td><td>1&#x27;b0</td><td>0: Accelerometer checked pass (no significant vibration happened during COD)1: Accelerometer checked failed (significant vibration happened during COD)</td></tr><tr><td>2</td><td>Startup_Failed</td><td>1&#x27;b0</td><td>0: Gyroscope startup succeeded1: Gyroscope startup failure happened when COD was called</td></tr><tr><td>1</td><td>Gyro_Enabled</td><td>1&#x27;b0</td><td>0: COD was called when gyroscope was not enabled1: COD was called while gyroscope was enabled, COD return failure</td></tr><tr><td>0</td><td>COD_Failed</td><td>1&#x27;b0</td><td>0: COD succeeded, new gain parameters will be applied to GX &amp; GY data1: COD failed; no COD correction applied</td></tr></table>


Note the value of this register is only valid after the COD command. Refer to 14 Calibration-On-Demand (COD). 


## 5.8 Activity Detection Output Registers


Table 26. Activity Detection Output Registers


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">TAP_STATUS</td><td colspan="2">Register Address: 89 (0x59)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7</td><td>TAP_POLARITY</td><td>1&#x27;b0</td><td>0: Tap was detected on the positive direction of the Tap axis1: Tap was detected on the negative direction of the Tap axis</td></tr><tr><td>6</td><td>Reserved</td><td>1&#x27;b0</td><td></td></tr><tr><td>5:4</td><td>TAP_AXIS</td><td>2&#x27;b0</td><td>0: No Tap was detected1: Tap was detected on X axis2: Tap was detected on Y axis3: Tap was detected on Z axis</td></tr><tr><td>3:2</td><td>Reserved</td><td>2&#x27;b0</td><td></td></tr><tr><td>1:0</td><td>TAP_NUM</td><td>2&#x27;b0</td><td>0: No Tap was detected1: Single-Tap was detected2: Double-Tap was detected3: NA</td></tr><tr><td colspan="2">STEP_COUNT[23:0]</td><td colspan="2"></td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>23:16</td><td>STEP_CNT_HIGH</td><td>0x00</td><td rowspan="3">24-bit Step Count detected by Pedometer engine</td></tr><tr><td>15:8</td><td>STEP_CNT_MID</td><td>0x00</td></tr><tr><td>7:0</td><td>STEP_CNT_LOW</td><td>0x00</td></tr></table>

## 5.9 Reset Register


Table 27. Reset Register Description


<table><tr><td colspan="2">Register Name</td><td colspan="2"></td></tr><tr><td colspan="2">RESET</td><td colspan="2">Register Address: 96 (0x60)</td></tr><tr><td>Bits</td><td>Name</td><td>Default</td><td>Description</td></tr><tr><td>7:0</td><td>RESET</td><td>0x00</td><td>Soft Reset Register - Write 0xB0 to this register from any modes, will trigger the sensor reset process immediately.The register 0x4D will equals to 0x80 if there is a successful reset (Power-on Reset or Soft Reset) process.Refer to 7.4 Chip Reset Process.</td></tr></table>

## 5.10 CTRL 9 Functionality (Executing Pre-defined Commands)

## 5.10.1 CTRL9 Protocol Description

The protocol for executing predefined commands from an external host processor on the QMI8658A is facilitated by using the CTRL9 register. The register is available to the host via the UI SPI/I2C/I3C bus. 

It operates by the host writing a pre-defined value (Command, refer to 5.10.2 CTRL9 Command List) to the CTRL9 register. The firmware of the QMI8658A evaluates this command and if a match is found it executes the corresponding pre-defined function. 

Once the function has been executed, the QMI8658A signals the completion by setting STATUSINT.bit7 to 1, and raising INT1 interrupt if CTRL1.bit3 = 1 & CTRL8.bit7 == 0. The host must acknowledge this by writing CTRL_CMD_ACK (0x00) to CTRL9 register. After receiving the CTRL_CMD_ACK command, the QMI8658A clears the STATUSINT.bit7 to 0 and pulls down the INT1 interrupt if CTRL1.bit3 = 1 & CTRL8.bit7 == 0. 

This command presentation from the host to the QMI8658A and the subsequent execution and handshake between the host and the QMI8658A will be referred to as the “CTRL9 Protocol”. 

There are three types of interactions between the host and QMI8658A that follow the CTRL9 Protocol. 

WCtrl9: The host needs to supply data to QMI8658A prior to the Ctrl9 protocol. (Write – Ctrl9 Protocol) 

Ctrl9R: The host gets data from QMI8658A following the Ctrl9 protocol. (Ctrl9 protocol – Read) 

Ctrl9: No data transaction is required prior to or following the Ctrl9 protocol. (Ctrl9). 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/a13be8c32563b994a33fedcde036988273ccd2930dc0bf409b33d86048c2bd68.jpg)



Figure 13. CTRL9 Protocol Flow Chart


## 5.10.2 CTRL9 Command List

The predefined CTRL9 commands are listed in Table 28 List of CTRL9 Command. 


Table 28. List of CTRL9 Commands


<table><tr><td>Command Name</td><td>CTRL9 Command Value</td><td>Protocol Type</td><td>Description</td></tr><tr><td>CTRL_CMD_ACK</td><td>0x00</td><td>Ctrl9</td><td>Acknowledgement. Host acknowledges to QMI8658, to end the protocol.</td></tr><tr><td>CTRL_CMD_RST_FIFO</td><td>0x04</td><td>Ctrl9</td><td>Reset FIFO from Host</td></tr><tr><td>CTRL_CMD_REQ_FIFO</td><td>0x05</td><td>Ctrl9R</td><td>Get FIFO data from Device</td></tr><tr><td>CTRL_CMD_WRITE_WOM_SETTING</td><td>0x08</td><td>WCtrl9</td><td>Set up and enable Wake on Motion (WoM)</td></tr><tr><td>CTRL_CMD_ACCEL_HOST_DELTA_OFFSET</td><td>0x09</td><td>WCtrl9</td><td>Change accelerometer offset</td></tr><tr><td>CTRL_CMD_GYRO_HOST_DELTA_OFFSET</td><td>0x0A</td><td>WCtrl9</td><td>Change gyroscope offset</td></tr><tr><td>CTRL_CMD_CONFIGURE_TAP</td><td>0x0C</td><td>WCtrl9</td><td>Configure Tap detection</td></tr><tr><td>CTRL_CMD_CONFIGURE_PEDOMETER</td><td>0x0D</td><td>WCtrl9</td><td>Configure Pedometer</td></tr><tr><td>CTRL_CMD_CONFIGURE_MOTION</td><td>0x0E</td><td>WCtrl9</td><td>Configure Any Motion / No Motion / Significant Motion detection</td></tr><tr><td>CTRL_CMD_RESET_PEDOMETER</td><td>0x0F</td><td>WCtrl9</td><td>Reset pedometer count (step count)</td></tr><tr><td>CTRL_CMD_COPY_USID</td><td>0x10</td><td>Ctrl9R</td><td>Copy USID and FW Version to UI registers</td></tr><tr><td>CTRL_CMD_SET_RPU</td><td>0x11</td><td>WCtrl9</td><td>Configures IO pull-ups</td></tr><tr><td>CTRL_CMD_AHB_CLOCK_GATING</td><td>0x12</td><td>WCtrl9</td><td>Internal AHB clock gating switch</td></tr><tr><td>CTRL_CMD_ON_DEMAND_CALIBRATION</td><td>0xA2</td><td>WCtrl9</td><td>On-Demand Calibration on gyroscope</td></tr><tr><td>CTRL_CMD_APPLY_GYRO_GAINS</td><td>0xAA</td><td>WCtrl9</td><td>Restore the saved Gyroscope gains</td></tr></table>

## 5.10.3 CAL Registers

The set of CAL registers can be used for the parameter transferring, if WCTL9 or CTRL9R commands are implemented. Refer to Table 28 and 5.10.6 for details. 


Table 29. CAL Register Addresses


<table><tr><td rowspan="2">Register Name</td><td colspan="2">Register Address</td></tr><tr><td>Dec</td><td>Hex</td></tr><tr><td>CAL1_L</td><td>11</td><td>0x0B</td></tr><tr><td>CAL1_H</td><td>12</td><td>0x0C</td></tr><tr><td>CAL2_L</td><td>13</td><td>0x0D</td></tr><tr><td>CAL2_H</td><td>14</td><td>0x0E</td></tr><tr><td>CAL3_L</td><td>15</td><td>0x0F</td></tr><tr><td>CAL3_H</td><td>16</td><td>0x10</td></tr><tr><td>CAL4_L</td><td>17</td><td>0x11</td></tr><tr><td>CAL4_H</td><td>18</td><td>0x12</td></tr></table>

## 5.10.4 WCtrl9 (Write – CTRL9 Protocol)

1. The host needs to provide the required data for this command to the QMI8658A. The host typically does this by placing the data in a set of registers called the CAL registers. Maximum eight CAL registers are used. Refer to Table 29. 

2. Write Ctrl9 register 0x0A with the appropriate Command value, refer to Table 28. 

3. The Device will set STATUSINT.bit7 to 1, and raise INT1(if CTRL1.bit3 = 1 & CTRL8.bit7 == 0), once it has executed the appropriate function based on the command value. 

4. The host must acknowledge this by writing CTRL_CMD_ACK (0x00) to CTRL9 register, STATUSINT.bit7 (CmdDone) will be reset to 0 on receiving the CTRL_CMD_ACK command. And INT1 is pulled low upon the register read if CTRL1.bit3 = 1 & CTRL8.bit7 == 0. 

5. If any data is expected from the device, it will be available at this time. The location of the data is specified separately for each of the Commands. 

Refer to 5.10.6 for details. 

## 5.10.5 Ctrl9R (CTRL9 Protocol - Read)

1. Write Ctrl9 register 0x0A with the appropriate Command value. 

2. The Device will set STATUSINT.bit7 to 1, and raise INT1 (if CTRL1.bit3 = 1 & CTRL8.bit7 == 0), once it has executed the appropriate function based on the command value. 

3. The host must acknowledge this by writing CTRL_CMD_ACK (0x00) to CTRL9 register, STATUSINT.bit7 (CmdDone) will be reset to 0 on receiving the CTRL_CMD_ACK command. INT1 is pulled low upon the register read if CTRL1.bit3 = 1 & CTRL8.bit7 == 0. 

4. Data is available from the device on the CAL registers. The location of the data is specified separately for each of the Commands. 

Refer to 5.10.6 for details. 

## 5.10.6 CTRL9 Commands in Details

## 5.10.6.1 CTRL_CMD_ACK

Host acknowledges QMI8658A when received the CmdDone information, to end the CTRL9 protocol. 

## 5.10.6.2 CTRL_CMD_RST_FIFO

This CTRL9 command of writing 0x04 to the Ctrl9 register 0x0a allows the host to instruct the device to reset the FIFO. The FIFO data, sample count and flags will be cleared and reset to default status. 

## 5.10.6.3 CTRL_CMD_REQ_FIFO

This CTRL9 Command is issued when the host wants to get data from the FIFO, by writing 0x05 through the CTRL9 process. 

After successfully finish the CTRL9 process, the FIFO read mode will be enabled, the device will direct the FIFO data to the FIFO_DATA register(0x17) until the FIFO is empty. After reading the FIFO data, host must set FIFO_CTRL.FIFO_rd_mode to 0 by write the FIFO_CTRL register, which will cause the FIFO_STATUS.FIFO_WTM/FIFO_FULL to be cleared and/or the INT pin (if enabled) be de-asserted. Refer to ! ! for CTRL9 operation, and refer to 8.8 for details. 

## 5.10.6.4 CTRL_CMD_WRITE_WOM_SETTING

This CTRL9 Command is issued when the host wants to enable/modify the trigger thresholds or blanking interval of the Wake on Motion Feature of the device. Please refer to 12 Wake on Motion (WoM) for details of setting up this feature. Once the specified CAL registers are loaded with the appropriate data, the Command is issued by writing 0x08 to CTRL9 register 0x0A. 

## 5.10.6.5 CTRL_CMD_ACCEL_HOST_DELTA_OFFSET

This CTRL9 Command is issued when the host wants to manually change the accelerometer offset. Each delta offset value should contain 16 bits and the format is signed 4.12 (12 fraction bits, unit is 1 / 2^12). The user must write the offset to the following registers: 

```txt
Accel_Delta_X : {CAL1_H, CAL1_L}
Accel_Delta_Y : {CAL2_H, CAL2_L}
Accel_Delta_Z : {CAL3_H, CAL3_L} 
```

Next, the Command is issued by writing 0x09 to CTRL9 register 0x0A. Note, this offset change is lost when the sensor is power cycled, or the system is reset. 

## 5.10.6.6 CTRL_CMD_GYRO_HOST_DELTA_OFFSET

This CTRL9 Command is issued when the host wants to manually change the gyroscope offset. Each delta offset value should contain 16 bits and the format is signed 11.5 (5 fraction bits, unit is 1 / 2^5). The user must write the offset to the following registers: 

$\mathtt { G y r o \_ D e l t a \_ X : } \{ \mathtt { C A L 1 \_ H } , \mathtt { C A L 1 \_ L } \}$ 

$\mathsf { G y r o \_ D e l t a \_ Y : \{ C A L 2 \_ H , C A L 2 \_ L \} }$ 

$\mathsf { G y r o \_ D e l t a \_ Z : } \{ \mathsf { C A L 3 \_ H } , \mathsf { C A L 3 \_ L j }$ 

Next, the Command is issued by writing 0x0A to CTRL9 register 0x0A. Note, this offset change is lost when the sensor is power cycled, or the system is reset. 

## 5.10.6.7 CTRL_CMD_CONFIGURE_TAP

This CTRL9 command is issued to configure the parameters of Tap detection. Refer to 10.3 Configure Tap for details. 

## 5.10.6.8 CTRL_CMD_CONFIGURE_PEDOMETER

This CTRL9 command is issued to configure the parameters of Pedometer detection. Refer to 11.2 Configure Pedometer for details. 

## 5.10.6.9 CTRL_CMD_CONFIGURE_MOTION

This CTRL9 command is issued to configure the parameters of Motion Detection. Refer to 9.4 Config Motion Detection. 

## 5.10.6.10 CTRL_CMD_RESET_PEDOMETER

This CTRL9 command is issued to clear the step count of Pedometer. Refer to 11.6 Reset Step Count for details. 

## 5.10.6.11 CTRL_CMD_COPY_USID

The USID is the unique ID of every single QMI8658A part. 

This CTRL9 Command copies the following data into UI registers. It is initiated by the host writing 0x10 to CTRL9. After issuing the command, the data will be available for the host to read from the registers shown below: 

$\mathsf { F W \_ V e r s i o n b y t e 0 } \to \mathsf { d Q W \_ L }$ 

$\mathsf { F W \_ V e r s i o n b y t e } 1 \to \mathsf { d Q W \_ H }$ 

$\mathsf { F W \_ V e r s i o n b y t e } 2 \to \mathsf { d Q X \_ L }$ 

$\mathsf { U S I D \_ B y t e \_ 0 } \to \mathsf { d V X \_ L }$ 

$\mathsf { U S I D \_ B y t e \_ 1 } \to \mathsf { d V X \_ H }$ 

$\mathsf { U S I D \_ B y t e \_ 2 } \to \mathsf { d V Y \_ L }$ 

$\mathsf { U S I D \_ B y t e \_ 3 } \to \mathsf { d V Y \_ H }$ 

$\mathsf { U S I D \_ B y t e \_ 4 } \to \mathsf { d V Z \_ L }$ 

$\mathsf { U S I D \_ B y t e \_ 5 } \to \mathsf { d V Z \_ H }$ 

Note that after the successful Power-On Reset or Soft-Reset, the FW_Version and USID will be copied automatically to the according registers once for host to read them. Those registers can be changed after enabling the sensors, so afterwards, the CTRL_CMD_COPY_USID command should be implemented to copy the FW_Version and USID to the according registers before reading. 

## 5.10.6.12 CTRL_CMD_SET_RPU

This CTRL9 Command is issued when the host configure the IO pull-up resistors. Each bit controls a combination of resistors as shown in Table 30: 


Table 30. Pull-Up Resistor Table


<table><tr><td>Bit</td><td>Signal Name</td><td>Pins</td><td>Activity</td></tr><tr><td>0</td><td>aux_rpu_dis</td><td>SDx, SCx, RESV(Pin 10)</td><td>0: enable pull-up resistors(default)1: disable pull-up resistors</td></tr><tr><td>1</td><td>icm_rpu_dis</td><td>SDx</td><td>0: enable pull-up resistor(default)1: disable pull-up resistor</td></tr><tr><td>2</td><td>cs_rpu_dis</td><td>CS</td><td>0: enable pull-up resistor(default)1: disable pull-up resistor</td></tr><tr><td>3</td><td>i2c_rpu_dis</td><td>SCL, SDA</td><td>0: enable pull-up resistors(default)1: disable pull-up resistors</td></tr><tr><td>4:7</td><td>Reserved</td><td>NA</td><td></td></tr></table>

The host writes the appropriate CAL1_L bit by issuing a WCtrl9 command with 0x11. 

By default, all the pull-up resistors are enabled. Write 1 to the bit will disable the pullup resistors accordingly, while write 0 will enable the pull-up resistors. 

## 5.10.6.13 CTRL_CMD_AHB_CLOCK_GATING

When locking Mechanism is set (CTRL7.bit7 == 1(syncSmpl)), the CTRL_CMD_AHB_CLOCK_GATING should be disabled to guarantee the locking mechanism of data reading, to prevent the possible misalignment. Refer to 14 Calibration-On-Demand (COD) for details. 

## 5.10.6.14 CTRL_CMD_ON_DEMAND_CALIBRATION

This CTRL9 Command enables host to recalibrate the gyro sensitivity from time to time. Refer to 14 Calibration-On-Demand (COD). 

## 5.10.6.1 CTRL_CMD_ APPLY_GYRO_GAINS

This CTRL9 Command enables host to restore the saved the gyro gains to QMI8658A, to avoid run the COD again. This is not recommended when there are significant environment changes, like significant PCB stress change. Refer to 14.4 Save and Restore the New Gain Parameters. 

## 6 Interrupts

The QMI8658A has two Interrupt lines, INT1 and INT2. 

Both INT1 and INT2 can be configured as High-Z mode or Push-Pull mode by configure the CTRL1.bit3(INT1) or CTRL1.bit4(INT2). If CTRL1.bit3 (CTRL1.bit4) is set to 0, INT1(INT2) will be set in High-Z mode accordingly. While if CTRL1.bit3 (CTRL1.bit4) is set to 1, INT1(INT2) will be set in Push-Pull mode accordingly. By default, INT1 and INT2 are in High-Z mode. 

If QMI8658A is configured in Wake on Motion (WoM) mode, there is no sensor data generated. The INT pins behavior follows the configuration of WoM, refer to 12 Wake on Motion (WoM). 

If QMI8658A is not in Wake on Motion mode, there are two modes of the interrupt map, as described below. Host can config multiple internal signal/interrupt sources to INT pins (INT1 and/or INT2). If driven to one INT pin, the multiple sources act in LOGIC-OR. 

## 6.1 SyncSample mode

The SyncSample mode supports locking the values during reading process. Refer to 13 Locking Mechanism. For details to the sensor data registers. 

Set CTRL7.bit7(SyncSample) == 1 will enable the SyncSample mode. 

As illustrated in Figure 14. In SyncSample mode, the CTRL9 handshake signal will be routed to INT1. Check 5.10 for details. 

The motion event interrupts (Any Motion, No Motion, Significant Motion, Pedometer, Tap) will be routed to INT1 if enabled. 

FIFO function is not supported in this mode, and DRDY signal will be routed to INT2. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/591bf4b17c800645479516511cd14a2654018c795e6085d934b319de77fb573d.jpg)



Figure 14. Interrupt Map of SyncSample Mode


## 6.2 Non-SyncSample mode

This mode supports FIFO function and free interrupts configuration, as illustrated in Figure 15. 

If CTRL7.bit7(SyncSample) == 0, then bit 1 of STATUSINT register will have the same value as INT1 and bit 0 of STATUSINT register will have the same value as INT2. 

In Non-SyncSample mode, there are two approaches of CTRL9 handshake. Host can check the INT1 pin high level for the handshark if set CTRL8.bit7 = 0; or poll the STATUSINT.bit7 for handshake if set CTRL8.bit7 = 1. 

In Non-SyncSample mode, the motion event interrupt(s) can be configured to INT1 by setting CTRL8.bit6 = 1, or to INT2 by setting CTRL8.bit6 = 0. Note that the motion event engines can be enabled by CTRL8.bit[4:0], refer to Table 22 for details. 

In Non-SycnSample mode, the sensor data can be output through data register or FIFO. Configure the FIFO_CTRL.FIFO_MODE = ‘bypass’ mode, will enable the DRDY function and disable FIFO functionality; configure the FIFO_CTRL.FIFO_MODE = other mode, will enable the FIFO functionality and disable the DRDY function. 

If FIFO mode is enabled, the FIFO interrupt can be configured to INT1 pin if CTRL1.bit2 is set to 1, or INT2 pin if CTRL1.bit2 is set to 0. Refer to 8 FIFO Description for more details of FIFO interrupt behavior. 

If DRDY mode is enabled, the DRDY signal will be routed to INT2 if the CTRL7.bit5(DRDY_DIS) is set to 0, or be blocked from INT2 pin if CTRL7.bit5(DRDY_DIS) is set to 1. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/3c49796c76a468542ab1735ca48472df2bc481545183cbfa7c508984bb310717.jpg)



Figure 15. Interrupt Map of Non-SyncSample Mode


## 6.3 DRDY(Data Ready)

If DRDY mode is enabled (accelerometer and/or gyroscope are/is enabled, and FIFO mode is set to bypass) and DRDY_DIS == 0, DRDY (Data Ready) signal will be driven to the INT2 in edge-trigger mode, means the DRDY signal can be seen on INT2. The Sensor Data Output Registers(refer to 5.6) are updated at the Output Data Rate (ODR), and DRDY signal is pulsed at the ODR frequency. The new data is updated to the data registers during the low level before the rising edge, and a rising edge on DRDY indicates that data is available for host to read. DRDY is cleared automatically after a short duration. The DRDY pulse width is dependent on the sensor ODR set by CTRL2 and/or CTRL3 registers and enabled sensor(s). 

In Non-SyncSample mode, it is the responsibility of the host to detect the rising edge and to read the data out during the high level of the INT2 pulse. Otherwise, there is the possibility that the updating of the new data happens during the host reading process and causes data mismatch. 

In SyncSample mode, it is possible to lock the data in the data registers and read them in an unlimited delay, by following a process. Refer to 6.1 SyncSample mode. 

## 7 Operating Modes

The QMI8658A offers a large number of operating modes that may be used to operate the device in a power efficient manner. These modes are described in Table 31 and are shown in Figure 16; they may be configured using the control (CTRL) registers. 

## 7.1 Operating Modes Descriptions


Table 31. Operating Modes


<table><tr><td>Mode</td><td>Description</td><td>Suggested Configuration</td></tr><tr><td>Power-On Default</td><td>All sensors off, clock is turned on. The current in this mode is typically 15 μA. Note this mode is the default state upon initial power up or after a reset.</td><td>CTRL1 sensorDisable = 0CTRL7 aEN = 0, gEN = 0, CTRL2 aODR =000</td></tr><tr><td>Low Power</td><td>Same as Power-On Default mode, except in this mode the 250 kHz clock is turned on instead of the high-speed clock. The current in this mode is typically 8 μA. To enter this mode requires host interaction to set CTRL2 aODR=11xx.</td><td>CTRL1 sensorDisable =0CTRL7 aEN = 0, gEN = 0, CTRL2 aODR =11xx</td></tr><tr><td>Power-Down</td><td>All QMI8658A functional blocks are switched off to minimize power consumption. Digital interfaces remain on allowing communication with the device. All configuration register values are preserved, and output data register values are maintained. The current in this mode is typically 6 μA. The host must initiate this mode by setting sensorDisable=1.</td><td>CTRL1 sensorDisable =1CTRL7 aEN = 0, gEN = 0</td></tr><tr><td>Normal Accel Only</td><td>Device configured as an accelerometer only.</td><td>CTRL7 aEN =1, gEN =0, CTRL2 aODR !=11xx</td></tr><tr><td>Low Power Accel Only</td><td>Device configured in low power accelerometer mode.</td><td>CTRL7 aEN =1, gEN =0, CTRL2 aODR =11xx</td></tr><tr><td>Snooze Gyro</td><td>Device configured as gyroscope drive only, the gyroscope MEMS will keep running at resonance frequency. Since the Sensing part is not enabled, there is no data from the gyroscope in this mode. This mode enables relative lower current consumption than Gyro-Only mode and can quickly generate data from clearing of gSN.</td><td>CTRL7 gSN=1, aEN =0, gEN =1</td></tr><tr><td>Gyro Only</td><td>Device configured as a gyroscope only.</td><td>CTRL7 gSN=0, aEN =0, gEN =1</td></tr><tr><td>Accel + Gyro (IMU)</td><td>Device configured as an Inertial Measurement Unit, i.e. an accelerometer and gyroscope combination sensors.</td><td>CTRL7 gSN=0, aEN =1, gEN =1CTRL2 aODR != 11xx</td></tr><tr><td>Accel + Snooze Gyro</td><td>Accelerometer and gyroscope snooze are enabled. Only accelerometer data is available.</td><td>CTRL7 gSN=1, aEN =1, gEN =1CTRL2 aODR != 11xx</td></tr><tr><td>Wake on Motion (WoM)</td><td>Very low power mode used to wake-up the host by providing an interrupt upon detection of device motion. Rrefer to 5.10.6.4.</td><td>CTRL7 aEN =1, gEN =0, CTRL2 aODR = 11xx</td></tr><tr><td>Software Reset</td><td>Software Reset asserted</td><td></td></tr><tr><td>No Power</td><td>VDDIO and VDD low</td><td></td></tr></table>

## 7.2 General Mode Transitioning

Upon exiting the No Power state (i.e. on first applying power to the part) or exiting a Software Reset state, the part will enter the Power-On Default state. From there, the sensor can be configured in the various modes described in Table 31 and as shown in Figure 16. The figure illustrates the timing associated with various mode transitions, and values for these times are given in the section below and in Table 7 and Table 8. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/3c9a36559e3d99fba9b6d89820266de6e3a8f9d01400202aeee1ed45bf206e99.jpg)



Figure 16. Operating Mode Transition Diagram


## 7.3 Transition Time

t0 is the System Turn On Time and is the time to enter the Power-On Default state from Software Reset, No Power, or Power down. Time t0 is maximum 15ms. This time only needs to be done once, upon transitioning from either a No Power or Power Down state, or whenever a reset is issued, which should not be done unless the intent is to have the device to go through its entire boot sequence (see the specification System Turn On Time in both Table 7 and Table 8). 

The times t1, t2 and t4, are defined as the time it takes from issuing the enabling-sensor command (when the corresponding sensor is off) to DRDY(INT2) going high (data being present) or STATUSINT.bit0 is set to 1. 

The time t5 is the time it takes to have the internal filtered data to output the correct representation of the inertial state. t5 is variable and is associated with the user selected Output Data Rate (ODR). We have defined minimum $\mathsf { t } 5 = ( 3 / { \mathsf { O D R } } )$ to generally represent that time. 

t6 is the time it takes to go from a sensor powered state to a state where the sensors are off. This time depends on the Output Data Rate (ODR) and ranges from 1/ODR to 2/ODR. 

t7 is the transition time between various states where the sensors are off. 

The Gyro Turn on Time (see Table 8) is comprised of t1 (the gyroscope wakeup time) and t5 (the part’s filter settling time). t1 is typically 150 ms and t5 is defined as 3/ODR, where ODR is the output data rate in Hertz. 

The Accel Turn on Time (see Table 7) is comprised of t2 (the accelerometer wakeup time) and t5 (the part’s filter settling time). t2 is typically 3 ms, and t5 is defined as 3/ODR, where ODR is the output data rate in Hertz. 

The t7 transition is dependent on data transfer rates and is for I2C at 400 kHz is <100 µs for SPI at 11 Mbps is around 40 µs. 

## 7.4 Chip Reset Process

There are two approaches that can trigger the QMI8658A run the Reset process, Power-On Reset and Software Reset. 

Power-On reset is initialized by driving the VDD & VDDIO lines to valid working range from the Power Off status (VDD = 0V, VDDIO = 0V). Refer to 3.2 for details. The Power-On Reset process starts from the release of POR, refer to 3.3 for details. 

The Software Reset (Soft Reset) is generated by writing the 0x0B to RESET register (0x60). Refer to 5.9. 

After the Reset is triggered (Power-On Reset and Software Reset), the QMI8658 will run the reset process. The UI registers, internal RAM, FIFO will be set to default values, Analog and digital circuitries will be disabled, refer to 3.3 for details. 

It takes maximum 15ms for the Reset process to be finished. Refer to Table 7 and Table 8 for details. Note that the VDDIO & VDD power lines are expected to be stable(no sharp pulse) once settled to the Final Value(s), before the System Turn On Time. Otherwise, the QMI8658A initialization may be interfered by the unwanted power pulse, and result in failure of accelerometer or gyroscope startup, refer to 3.3 for details. 

The register 0x4D will present 0x80 if there is a successful Reset (Power-on Reset or Soft Reset) process. Refer to 5.9. Note that the content of register 0x4D could be overwritten after later operations, like enabling the sensor(s) (in CTRL7.bit[1:0]) or implementing the CTRL9 command, so host is expected to read the register immediately after POR or Software Reset to check the Reset result. 

## 8 FIFO Description

## 8.1 FIFO Structure

The QMI8658A contains a programmable 1536-byte FIFO. The FIFO’s operating mode and configuration are set via the FIFO_CTRL register, refer to Table 23. FIFO data may consist of gyroscope and accelerometer data and is accessible via the serial interfaces (SPI/I2C/I3C), in burst reads. Depending on how many sensors are enabled, the host is expected to read increments of 6, 12 bytes, corresponding to one and two sensors active at the same time. This feature helps reduce overall system power consumption by enabling the host processor to read and process the sensor data in bursts and then enter a low-power mode. The interrupt function may be used to alert host when FIFO watermark level is reached. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/02f9205dce2d8e27eeee387052a59979d0be209f93182d9ee068ef367993da50.jpg)



Figure 17. FIFO Data Flow


## 8.2 FIFO Size

There are four levels of FIFO size: 16 samples, 32 samples, 64 samples, 128 samples. The sample stands for 6 bytes of accelerometer data or 6 bytes of gyroscope data if one of them is enabled, or 6 bytes of accelerometer and 6 bytes of gyroscope data (total 12 bytes) if both are enabled. 

The FIFO size is configured using the FIFO_CTRL.FIFO_SIZE[1:0]: 16 samples(0), 32 samples(1), 64 samples(2), 128 samples(3), refer to Table 23. 

When the FIFO is enabled for two sensors (Accelerometer and Gyroscope), the sensors must be set at the same Output Data Rate (ODR), refer to Table 22 for CTRL2 and CTRL3 registers. 

## 8.3 Configure FIFO Mode

The FIFO has multiple operating modes: Bypass, FIFO, Stream. The operating modes are set by the FIFO_CTRL.FIFO_MDOE[1:0] bits in the register, refer to Table 23. 

Configure the FIFO_CTRL.FIFO_MODE to ‘FIFO’(1) or ‘Stream’(2) mode will enable the FIFO functionality. Configure the FIFO_MODE to ‘Bypass’ (0) mode, will disable the FIFO functionality. 

Once FIFO is enabled and the Accelerometer and/or Gyroscope is(are) enabled, the corresponding data will be filled into FIFO. 

In ‘FIFO’ mode, once FIFO is full, the data filling will stop and new data will be discarded until host reads out the FIFO data and release the space for new data to be written to. 

In ‘Stream’ mode, once FIFO is full, the data filling will continue and the oldest data will be discarded, until host reads out the FIFO data and release the space for new data to be written to. 

## 8.4 FIFO Sample Count

The FIFO Sample Count indicates the filled content level of FIFO, which means the quantity of sensor data that was written into FIFO, and to be read out. 

The FIFO Sample Count is a 10-bit value and stored in FIFO_SMPL_CNT and FIFO_STATUS[1:0], unit is word(two bytes). Host need to read the two registers, to calculate the number of bytes of FIFO content data by below formular: 

FIFO_Sample_Count (in byte) = 2 * (fifo_smpl_cnt_msb[1:0] * 256 + fifo_smpl_cnt_lsb[7:0]) 

## 8.5 FIFO Watermark Interrupt

The FIFO_WTM register(0x13) indicates the expected level of FIFO data that host wants to get the FIFO Watermark interrupt. The unit is sample, which means 6 bytes if one of accelerometer and gyroscope is enabled, and 12 bytes if both are enabled. 

Note that the configured FIFO watermark should not be higher than the FIFO size configured by FIFO_CTRL.FIFO_SIZE. 

If the FIFO_WTM is written with non-zero value, the FIFO watermark function is enabled. The internal FIFO watermark interrupt will be triggered if the FIFO content level reaches or is higher than the configured FIFO watermark level. The FIFO_STATUS.FIFO_WTM flag will be set to 1, and will be cleared to 0 if FIFO content drops lower than the FIFO watermark level. 

The internal FIFO watermark interrupt signal can drive the INT1 pin if CTRL1.bit2 = 1, or INT2 pin if CTRL1.bit2 = 0. Note that the CTRL1.bit3(INT1) and CTRL1.bit4(INT2) controls the high-Z or push-pull mode of the INT pins, refer to 6 Interrupts for details. 

Once the corresponds INT pin is configured to the push-pull mode, the FIFO watermark interrupt can be seen on the corresponds INT pin. It will keep high level as long as the FIFO filled level is equal to or higher than the watermark, will drop to low level as long as the FIFO filled level is lower than the configured FIFO watermark after reading out by host and FIFO_RD_MODE is cleared. 

## 8.6 FIFO Full

The FIFO_STATUS.FIFO_FULL flag is set if the FIFO filled level (samples) equals to the FIFO size configured by FIFO_CTRL.FIFO_SIZE. And is cleared if FIFO filled level is lower than the FIFO size. 

## 8.7 FIFO Read Mode

To read out the FIFO data, host need to set the FIFO into Read Mode, by issuing CTRL_CMD_REQ_FIFO command through CTRL9 process, refer to 5.10 CTRL 9 Functionality (Executing Pre-defined Commands). 

Once FIFO Read Mode is enabled, FIFO data will be directed to the FIFO_DATA register, then host can possibly read data from the FIFO_DATA register. 

Note that once FIFO Read Mode is enabled, the new data won’t be filled into FIFO, and will be discarded. Host is expected to read out the FIFO data and disable the FIFO Read Mode before the new data come. Otherwise, data dropping will happen. The time for the new data come is defined by the sensor ODR, which is 1 / ODR. 

## 8.8 Read FIFO Data

The FIFO data is read through the I2C/I3C/SPI interface by reading the FIFO_DATA register. Any time the FIFO_DATA register is read, data is erased from the FIFO memory, corresponds to First-In-First-Out concept. 

Host is expected to read out the FIFO data, following below sequence: 

1. Got FIFO watermark interrupt by INT pin or polling the FIFO_STATUS register (FIFO_WTM and/or FIFO_FULL). 

2. Read the FIFO_SMPL_CNT and FIFO_STATUS registers, to calculate the level of FIFO content data, refer to 8.4 FIFO Sample Count. 

3. Send CTRL_CMD_REQ_FIFO (0x05) by CTRL9 command, to enable FIFO read mode. Refer to CTRL_CMD_REQ_FIFO for details. 

4. Read from the FIFO_DATA register per FIFO_Sample_Count. 

5. Disable the FIFO Read Mode by setting FIFO_CTRL.FIFO_rd_mode to 0. New data will be filled into FIFO afterwards. 

## 8.9 FIFO Data Pattern

Note that when only the accelerometer or gyroscope is enabled, the sensor data format at the host interface is: 

$$
A X \_ L [ 0 ] \rightarrow A X \_ H [ 0 ] \rightarrow A Y \_ L [ 0 ] \rightarrow A Y \_ H [ 0 ] \rightarrow A Z \_ L [ 0 ] \rightarrow A Z \_ H [ 0 ] \rightarrow A X \_ L [ 1 ] \rightarrow \dots
$$

or 

$$
\mathrm {GX\_L[0]} \rightarrow \mathrm {GX\_H[0]} \rightarrow \mathrm {GY\_L[0]} \rightarrow \mathrm {GY\_H[0]} \rightarrow \mathrm {GZ\_L[0]} \rightarrow \mathrm {GZ\_H[0]} \rightarrow \mathrm {GX\_L[1]} \rightarrow \dots
$$

When both accelerometer and gyroscope are enabled, the sensor data format is: 

$$
A X \_ L [ 0 ] \rightarrow A X \_ H [ 0 ] \rightarrow A Y \_ L [ 0 ] \rightarrow A Y \_ H [ 0 ] \rightarrow A Z \_ L [ 0 ] \rightarrow A Z \_ H [ 0 ] \rightarrow
$$

$$
G X \_ L [ 0 ] \rightarrow G X \_ H [ 0 ] \rightarrow G Y \_ L [ 0 ] \rightarrow G Y \_ H [ 0 ] \rightarrow G Z \_ L [ 0 ] \rightarrow G Z \_ H [ 0 ] \rightarrow
$$

$$
A X \_ L [ 1 ] \rightarrow A X \_ H [ 1 ] \rightarrow \dots
$$

## 8.10 Reset FIFO

The FIFO content can be cleared/emptied by inserting CTRL_CMD_RST_FIFO through CTRL9 process, refer to 5.10 CTRL 9 Functionality (Executing Pre-defined Commands) for details. 

## 9 Motion Detection

The calculation of the Motion Detection (No-, Any-, or Significant-Motion) is based on the accelerometer ODR defined by CTRL2.aODR, refer to Table 22 for details. 

The Motion Detection can only work in Non-SyncSample mode, refer to 6.2 Non-SyncSample mode for details. 

No-, Any-, or Significant-Motion interrupts can be issued to host. No-Motion interrupt indicates that the device is in idle/quiet status, host can run into sleep or low-power mode. Any-Motion interrupt indicates that the device is in movement, host can be awakened from sleep/low-power mode. Significant-Motion indicates host that the device is in significant and continuous movement. 

## 9.1 Motion Detection Principle

Any-/No-/Significant-Motion detection is calculating on the slope of the acceleration of enabled axis: 

Slope(n) = Acc(n) – Acc(n-1), for enabled axis/axes of accelerometer 

## 9.1.1 Any-Motion Detection Principle

Any-Motion Detection is detecting the absolute of slope that is higher than defined threshold (AnyMotionXThr, AnyMotionYThr, AnyMotionZThr), and last consecutively for AnyMotionWindow or more samples. As shown in Figure 18. As long as the conditions (threshold and duration) are fulfilled, Any-Motion event flag is set, and can be configured to drive the interrupt pin (INT1 or INT2), synced to the DRDY. When the slope falls within the range of (-Threshold, +Threshold), the Any-Motion conditions are not fulfilled, and the corresponding interrupt is reset. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/ef122707281a9f9b9164bfa080efe827c4ac257b74dd3088cdf7674606f6df90.jpg)



Figure 18. Any-Motion Detection Interrupt


## 9.1.2 No-Motion Detection Principle

No-Motion Detection is detecting the absolute of slope that is lower than defined threshold (NoMotionXThr, NoMotionYThr, NoMotionZThr), and last consecutively for NoMotionWindow or more samples. As shown in Figure 19. If the conditions (threshold and duration) are fulfilled, No-Motion event flag is set, and can be configured to drive the interrupt pin (INT1 or INT2), synced to the DRDY. When the slope falls out of the range of (-Threshold, +Threshold), the No-Motion conditions are not fulfilled, and the corresponding interrupt is reset. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/5fa55255cf1950cd90f28ca31d3743bcf033ac76b9739a07af93e97593128e8b.jpg)



Figure 19. No-Motion interrupt detection


## 9.1.3 Significant-Motion Detection Principle

Significant-Motion Detection works based on Any-Motion and No-Motion events, so both Any-Motion and No-Motion should be configured and enabled to make sure Significant-Motion Detection run properly. As shown in Figure 20. 

After valid Any-Motion is detected, if a further Any-Motion is detected after SigMotionWaitWindow and before the SigMotionConfirmWindow time, the Significant-Motion event flag will be raised to the interrupt path. The Significant-Motion interrupt is cleared when No-Motion is detected. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/bc0f3e1c4c306beaabc8e013f55a4a2f5c41194f00e1027a2e29bdd2da6a30ec.jpg)



Figure 20. Significant-Motion interrupt detection


## 9.2 Motion Detection Flow

The flow chart of No-Motion Detection is show in Figure 21 No-Motion Detection Flow. 

The flow chart of Any-Motion Detection is show in Figure 22 Any-Motion Detection Flow. 

The flow chart of Significant-Motion Detection is show in Figure 23 Significant-Motion Detection Flow. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/fd099b4840bdebc7e15058d0b8f4663df618ffc7f808523540bc2260e0618d75.jpg)



Figure 21. No-Motion Detection Flow


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/4c617c27523c62bad1f3e4e41a8d14ce3da80c966b872a5ff0647f7e4e0913a8.jpg)



Figure 22. Any-Motion Detection Flow


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/5266a00c1dcd20db5491e2d26fc9e48975b7e5adba3f73528cb779a8b1edde5d.jpg)



Figure 23. Significant-Motion Detection Flow


## 9.3 Parameters of Motion Detection


Table 32. Motion Detection Parameters


<table><tr><td>Parameter Name</td><td>Format</td><td>Resolution</td><td>Description</td></tr><tr><td>NoMotionAxisLogic</td><td>1-bit integer</td><td>1</td><td>0: Logic-OR between events of enabled axes for No-Motion detection1: Logic-AND between events of enabled axes for No-Motion detection</td></tr><tr><td>NoMotionEnXNoMotionEnYNoMotionEnZ</td><td>1-bit integer</td><td>1</td><td>0: the corresponding axis is not involved for calculation1: the corresponding axis data is calculated for No-Motion detection</td></tr><tr><td>NoMotionXThrNoMotionYThrNoMotionZThr</td><td>1-byte format unsigned,5-bits fraction</td><td>0.03125g(1 / 32)</td><td>Defines the slope threshold of the corresponding axis for No-Motion detection</td></tr><tr><td>NoMotionWindow</td><td>1-byte integer</td><td>1 sample</td><td>Defines the minimum number of consecutive samples (duration) that the absolute of the slope of the enabled axis/axes data should keep lower than the threshold</td></tr><tr><td>AnyMotionAxisLogic</td><td>1-bit integer</td><td>1</td><td>0: Logic-OR between events of enabled axes for Any-Motion detection1: Logic-AND between events of enabled axes for Any-Motion detection</td></tr><tr><td>AnyMotionEnXAnyMotionEnYAnyMotionEnZ</td><td>1-bit integer</td><td>1</td><td>0: the corresponding axis is not involved for calculation1: the corresponding axis data is calculated for Any-Motion detection</td></tr><tr><td>AnyMotionXThrAnyMotionYThrAnyMotionZThr</td><td>1-byte format unsigned,5-bits fraction</td><td>0.03125g(1 / 32)</td><td>Defines the slope threshold of the corresponding axis for Any-Motion detection</td></tr><tr><td>AnyMotionWindow</td><td>1-byte format unsigned,5-bits fraction</td><td>1 sample</td><td>Defines the minimum number of consecutive samples (duration) that the absolute of the slope of the enabled axis/axes data should keep higher than the threshold</td></tr><tr><td>SigMotionWaitWindow</td><td>2-bytes integer</td><td>1 sample</td><td>Defines the wait window (idle time) starts from the first Any-Motion event until starting to detecting another Any-Motion event for confirmation</td></tr><tr><td>SigMotionConfirmWindow</td><td>2-bytes integer</td><td>1 sample</td><td>Defines the maximum duration for detecting the other Any-Motion event to confirm Significant-Motion, starts from the first Any-Motion event</td></tr></table>


Note: Once the Logic-OR is selected, the calculation on any enabled axis and reported one valid event, will trigger the corresponding Motion Detection(Any-Motion or No-Motion). Once the Logic-AND is selected, the calculation on all enabled axes should report valid event, to trigger the corresponding Motion Detection(Any-Motion or No-Motion). 


The MOTION_MODE_CTRL byte consists of the parameters shown in Table 33. 


Table 33. MOTION_MODE_CTRL Content


<table><tr><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr><tr><td>NoMotionAxisLogic</td><td>NoMotionEnZ</td><td>NoMotionEnY</td><td>NoMotionEnX</td><td>AnyMotionAxisLogic</td><td>AnyMotionEnZ</td><td>AnyMotionEnY</td><td>AnyMotionEnX</td></tr></table>

## 9.4 Config Motion Detection

The Motion Detection parameters are divided into two sets and can be passed to the QMI8658A internal algorithm through two callings of CTRL9 command. As shown in Table 34. 

Host should write the parameters to the corresponding registers, according to Table 34. Especially, write 0x01 to CAL4_H register for the first set of parameters, while write 0x02 to CAL4_H for the second set of parameters. Refer to Table 29 for address of CAL registers. 

Then trigger the CTRL9 command with 0x0E(CTRL_CMD_CONFIGURE_MOTION). Refer to Table 28 for details. 


Table 34. Write Motion Detection Parameters to QMI8658A


<table><tr><td>Register (bits)</td><td>First CTRL9 Command</td><td>Second CTRL9 Command</td></tr><tr><td>CAL1_L (7:0)</td><td>AnyMotionXThr.</td><td>AnyMotionWindow.</td></tr><tr><td>CAL1_H (7:0)</td><td>AnyMotionYThr.</td><td>NoMotionWindow</td></tr><tr><td>CAL2_L (7:0)</td><td>AnyMotionZThr.</td><td>SigMotionWaitWindow[7:0]</td></tr><tr><td>CAL2_H (7:0)</td><td>NoMotionXThr.</td><td>SigMotionWaitWindow [15:8]</td></tr><tr><td>CAL3_L (7:0)</td><td>NoMotionYThr.</td><td>SigMotionConfirmWindow[7:0]</td></tr><tr><td>CAL3_H (7:0)</td><td>NoMotionZThr.</td><td>SigMotionConfirmWindow[15:8]</td></tr><tr><td>CAL4_L (7:0)</td><td>MOTION_MODE_CTRL</td><td>NA</td></tr><tr><td>CAL4_H (7:4)</td><td>0x01(means 1st command)</td><td>0x02(means 2nd command)</td></tr><tr><td>CTRL9 code</td><td>0x0E(CTRL_CMD_CONFIGURE_MOTION)</td><td>0x0E(CTRL_CMD_CONFIGURE_MOTION)</td></tr></table>


Note: Configuration should be done when accelerometer and gyroscope are disabled(CTRL7.aEN = CTRL7.gEN =0). 


## 9.5 Enabling Motion Detection

After successfully passing the parameters to QMI8658A Motion Detection engine, host need to enable the Any-Motion Detection engine by setting CTRL8.bit1 to 1, enable the No-Motion Detection engine by setting CTRL8.bit2 to 1, enable the Significant-Motion Detection engine by setting CTRL8.bit3 to 1. If the accelerometer is configured and enabled too, the enabled engines will be started to detect the corresponding events. 

On contrary, set the bit(s) in CTRL8.bit[3:1] to 0, will disable the corresponding engine. This can be done when accelerometer and/or gyroscope are(is) enabled or disabled. Refer to 5.3 Configuration Registers for the details of CTRL8. 

## 9.6 Motion Interrupt

Once the Any-Motion, No-Motion and/or Significant-Motion event(s) is/are reported, the corresponding Motion Detection Event will be generated. 

The Motion Event (internal signal) can be selected to drive INT1(CTRL8.bit6 = 1) or INT2(CTRL8.bit6 = 0). And once the corresponding INT pin is enabled (by CTRL1.bit3 for INT1, or CTRL1.bit4 for INT2), the Motion interrupt can be seen on the INT pin (synced with DRDY). Refer to 5.3 Configuration Registers for details. 

At the meantime, the STATUS1.bit[7:5] is updated and host can read these bits to confirm the Motion interrupt status. Note that the STATUS1.bit[5] = 1 represents the Any-Motion was detected, STATUS1.bit[6] = 1 represents the No-Motion was detected, STATUS1.bit[7] = 1 represents the Significant-Motion was detected. Refer to 5.3 Configuration Registers for details. 

## 10 Tap

The Tap engine detects the Single-Tap or Double-Tap, if enabled. 

The calculation of the Tap Detection is based on the accelerometer ODR defined by CTRL2.aODR, refer to Table 22 for details. 

The Tap detection can only work in Non-SyncSample mode, refer to for details 6.2 Non-SyncSample mode. 

## 10.1 Tap Detection Principle

Figure 24 shows the principle of Tap detecting, includes Single Tap and Double Tap. 

The acceleration data of the three axes (x, y, z) is dynamically averaged to get the Average of Acceleration. Alpha parameter defines the ratio/weight of the averaging calculation. 

The Linear Acceleration is calculated: Linear Acceleration = Acceleration – Average of Acceleration. 

The Average Movement Magnitude indicates the movement energy level, is used to detect the Quiet status. It is calculated with Gamma. 

If the square sum of the Linear Acceleration of three axes is higher than the PeakMagThr, the peak detecting is started. If later at the end of PeakWindow, the Average of the Movement Magnitude is lower than the UDMThr (Undefined Motion Threshold), it means the vibration is low and return to Quiet status, it is considered a valid Peak is detected. 

If a valid Peak is detected, and no further significant vibration within the TapWindow (be quiet after the Peak, no further Tap), a valid Tap is detected. 

Once a valid Tap (first Tap) is detected, the second Tap of the Double-Tap should be detected after the TapWindow (Quiet time after first Tap) and before the DTapWindow (timeout of the Double Tap detection), and the Double-Tap event will be reported. 

If the second Tap is detected within the TapWindow, it is considered as the Undefined Motion, and will reset the full Tap process, no Tap event will be reported. 

If no further Tap is detected within the DTapWindow, Single-Tap event will be reported. The Tap detected after DTapWindow, is considered as the First Tap of the new round of detecting. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/4c8df34f9147a76fa9df8bbefef6f80ac3d2b23e20d29020b847bc15409f8c48.jpg)



Figure 24. Tap Detect Principle


## 10.2 Tap Detection Parameters

The parameters listed in Table 35 are used to configure the Tap Engine. 


Table 35. Tap Parameters


<table><tr><td>Parameter Name</td><td>Format</td><td>Resolution</td><td>Description</td></tr><tr><td>Priority</td><td>1-byte integer</td><td>NA</td><td>Priority definition between the x, y, z axes of acceleration. Only Priority[2:0] bits are used.The axis that output the first peak of Linear Acceleration in a valid Tap detection, will be consider as the Tap axis. However, there is possibility that two or three of the axes shows same Linear Acceleration at exactly same time when reach (or be higher than) the PeakMagThr. In this case, the defined priority is used to judge and select the axis as Tap axis.</td></tr><tr><td>PeakWindow</td><td>1-byte integer</td><td>1 sample</td><td>Defines the maximum duration (in sample) for a valid peak. In a valid peak, the linear acceleration should reach or be higher than the PeakMagThr and should return to quiet (no significant movement) within UDMThr, at the end of PeakWindow.E.g., 20 @500Hz ODR</td></tr><tr><td>TapWindow</td><td>2-bytes integer</td><td>1 sample</td><td>Defines the minimum quiet time before the second Tap happen. After the first Tap is detected, there should be no significant movement (defined by UDMThr) during the TapWindow. The valid second tap should be detected after TapWindow and before DTapWindow.E.g., 50 @500Hz ODR</td></tr><tr><td>DTapWindow</td><td>2-bytes integer</td><td>1 sample</td><td>Defines the maximum time for a valid second Tap for Double Tap, count start from the first peak of the valid first Tap.E.g., 250 @500Hz ODR</td></tr><tr><td>Alpha</td><td>1-byte unsigned,7-bits fraction</td><td>0.0078(1/128)</td><td>Defines the ratio for calculation the average of the acceleration. The bigger of Alpha, the bigger weight of the latest data.E.g., 0.0625</td></tr><tr><td>Gamma</td><td>1-byte unsigned,7-bits fraction</td><td>0.0078(1/128)</td><td>Defines the ratio for calculating the average of the movement magnitude. The bigger of Gamma, the bigger weight of the latest data.E.g., 0.25</td></tr><tr><td>PeakMagThr</td><td>2-bytes unsigned,10-bits fraction</td><td><eq>0.001g^2</eq>(1/ 1024)</td><td>Threshold for peak detection.E.g, <eq>0.8g^2</eq> (0x0320)</td></tr><tr><td>UDMThr</td><td>2-bytes unsigned,10-bits fraction</td><td>0.001g(1/ 1024)</td><td>Undefined Motion threshold. This defines the threshold of the Linear Acceleration for quiet status.E.g., <eq>0.4g^2</eq> (0x0190)</td></tr></table>


The Priority[2:0] is defined as below: 



0: (X > Y> Z) 



1: (X > Z > Y) 



2: (Y > X > Z) 



3: (Y > Z > X) 



4: (Z > X > Y) 



5: (Z > Y > X) 



6, 7: (Same as 0) 


## 10.3 Configure Tap

The Tap parameters are divided into two sets and can be passed to the QMI8658A internal algorithm through two callings of CTRL9 command. As shown in Table 36. 

Host should write the parameters to the corresponding registers, according to Table 36. Especially, write 0x01 to CAL4_H register for the first set of parameters, while write 0x02 to CAL4_H for the second set of parameters. Refer to Table 29 for address of CAL registers. 

Then trigger the CTRL9 comment with 0x0C(CTRL_CMD_CONFIGURE_TAP). Refer to 5.10 for details. 


Table 36. Write Tap Parameters to QMI8658A


<table><tr><td>Register (bits)</td><td>First Command Set</td><td>Second Command Set</td></tr><tr><td>CAL1_L (7:0)</td><td>PeakWindow[7:0]</td><td>Alpha[7:0]</td></tr><tr><td>CAL1_H (7:0)</td><td>Priority[7:0] (actually only [2:0] is used)</td><td>Gamma[7:0]</td></tr><tr><td>CAL2_L (7:0)</td><td>TapWindow[7:0]</td><td>PeakMagThr[7:0]</td></tr><tr><td>CAL2_H (7:0)</td><td>TapWindow[15:8]</td><td>PeakMagThr[15:8]</td></tr><tr><td>CAL3_L (7:0)</td><td>DTapWindow[7:0]</td><td>UDMThr[7:0]</td></tr><tr><td>CAL3_H (7:0)</td><td>DTapWindow[15:8]</td><td>UDMThr[15:8]</td></tr><tr><td>CAL4_L (7:0)</td><td>NA</td><td>NA</td></tr><tr><td>CAL4_H (7:4)</td><td>0x01(means 1st command)</td><td>0x02(means 2nd command)</td></tr><tr><td>CTRL9 code</td><td>0x0C(CTRL_CMD_CONFIGURE_TAP)</td><td>0x0C(CTRL_CMD_CONFIGURE_TAP)</td></tr></table>


Note: Configuration should be done when accelerometer and gyroscope are disabled(CTRL7.aEN = CTRL7.gEN =0). 


## 10.4 Enable Tap Detection

After successfully passing the parameters to QMI8658A Tap engine, host need to enable the Tap engine by setting CTRL8.bit0 to 1. If the accelerometer is properly configured and enabled (CTRL7.aEN = 1), Tap engine will be started to detect the taps. To detecting the Tap activity, it is recommended to set accelerometer ODR to higher than 200Hz (defined by CTRL2.aODR). 

If CTRL8.bit0 is set to 0, or CTRL7.aEN = 0, Tap engine will be stopped to detect the taps. 

## 10.5 Tap Interrupt

Once the Tap event is reported (Single- or Double-Tap), it can generate the Tap interrupt. 

The Tap event (internal signal) can be selected to drive INT1(CTRL8.bit6 = 1) or INT2(CTRL8.bit6 = 0). And once the corresponding INT pin is enabled (by CTRL1.bit3 for INT1, or CTRL1.bit4 for INT2), the Tap interrupt will be seen on the INT pin (synced with DRDY). Refer to to 5.3 Configuration Registers for details. 

At the meantime, the STATUS1.bit1 is set and host can read this bit to confirm the Tap (Single- or Double-Tap) interrupt is generated. 

## 10.6 Tap Detection Output

When the Tap (Single- or Double-Tap) is detected, the information of the Tap is presented in TAP_STATUS (register 0x59). The TAP_STATUS is updated and valid after the Tap event is detected (STATUSINT.bit1 = 1). 

The TAP_NUM indicate the Single-Tap (TAP_STATUS.TAP_NUM = 1) or Double-Tap (TAP_STATUS.TAP_NUM = 2) was detected. 

The TAP_STATUS.TAP_AXIS indicates the first valid peak of the Tap happens on X axis (TAP_STATUS.TAP_AXIS = 1), or Y axis (TAP_STATUS.TAP_AXIS = 2), or Z axis (TAP_STATUS.TAP_AXIS = 3). Note the TAP_AXIS judgement follows the definition of the Tap Priority. Refer to Table 35 for details. 

The TAP_STATUS.TAP_POLARITY indicates the direction from which the Tap moves towards the 8658A. Note that, this direction is derived from the value of the linear acceleration of first valid Peak. 

## 11 Pedometer

The Pedometer Engine detects the steps and output the step count to the UI registers for host to read, as well as generate the Pedometer interrupt. 

The calculation of the Pedometer Detection is based on the accelerometer ODR defined by CTRL2.aODR, refer to Table 22 for details. 

The Pedometer can only work in Non-SyncSample mode, refer to 6.2 Non-SyncSample mode for details. 

## 11.1 Pedometer Parameters

The parameters listed in Table 37 are used to configure the Pedometer Engine. 


Table 37. Pedometer Parameters


<table><tr><td>Parameter Name</td><td>Format</td><td>Resolution</td><td>Description</td></tr><tr><td>ped_sample_cnt</td><td>2-bytes integer</td><td>1 sample</td><td>Indicates the count of sample batch/window for calculation</td></tr><tr><td>ped_fix_peak2peak</td><td>2-bytes format unsigned, 10-bits fraction</td><td>(1 / 2^10) g = 1 mg</td><td>Indicates the threshold of the valid peak-to-peak detectionE.g., 0x00CC means 200mg</td></tr><tr><td>ped_fix_peak</td><td>2-bytes format unsigned, 10-bits fraction</td><td>1 mg</td><td>Indicates the threshold of the peak detection comparing to averageE.g., 0x0066 means 100mg</td></tr><tr><td>ped_time_up</td><td>2-bytes integer</td><td>1 sample</td><td>Indicates the maximum duration (timeout window) for a step. Reset counting calculation if no peaks detected within this duration.E.g., 80 means 1.6s @ ODR = 50Hz</td></tr><tr><td>ped_time_low</td><td>1-byte integer</td><td>1 sample</td><td>Indicates the minimum duration for a step. The peaks detected within this duration (quiet time) is ignored.E.g., 12 means 0.25s @ ODR = 50Hz</td></tr><tr><td>ped_time_cnt_entry</td><td>1-byte integer</td><td>1 step</td><td>Indicates the minimum continuous steps to start the valid step counting. If the continuously detected steps is lower than this count and timeout, the steps will not be take into account; if yes, the detected steps will all be taken into account and counting is started to count every following step before timeout. This is useful to screen out the fake steps detected by non-step vibrations.The timeout duration is defined by ped_time_up.E.g., 10 means 10 steps entry count</td></tr><tr><td>ped_fix_precision</td><td>1-byte integer</td><td>1 sample</td><td>0 is recommended</td></tr><tr><td>ped_sig_count</td><td>1-byte integer</td><td>1 sample</td><td>The amount of steps when to update the pedometer output registers.E.g., ped_sig_count = 4, every 4 valid steps is detected, update the registers once (added by 4).</td></tr></table>

In the example of parameters listed below, the calculation window is 50 samples (1s duration); 

The slowest step that can be detected is 4s (0.25 step per 1 second); the fastest step that can be detected is 0.4s (2.5 steps per 1 second). 

The steps detected lower than 10 and timeout (defined by ped_time_up), will be ignored. The step counting starts if 10 continues steps detected (internal steps: 10, 11, 12, ...), while the step count registers is updated with 10, 14, 18… (update once after every 4 steps detected). 

It is recommended to select the proper ped_fix_peak2peak and ped_fix_peak values depend on the movement amplitude and movement pattern, for example, if places QMI8658A in a watch or bend, running normally shows more significant peaks than walking. 

```python
ODR = 50Hz
ped_sample_cnt: 50 samples (0x0032)
ped_fix_peak2peak: 200mg (0x00CC in u6.10 format)
ped_fix_peak: 100mg (0x0066 in u6.10 format)
ped_time_up: 200 samples (0x00C8, 4s)
ped_time_low: 20 samples (0x14, 0.4)
ped_time_cnt_entry: 10 samples (0x0A)
ped_fix_precision: 0 (0x00)
ped_sig_count: 4 samples (0x04) 
```

## 11.2 Configure Pedometer

The Pedometer parameters are divided into two sets and can be passed to the QMI8658A internal algorithm through two callings of CTRL9 command. As shown in Table 38. 

Host should write the parameters to the corresponding registers, according to Table 38. Especially, write 0x01 to CAL4_H register for the first CTRL9 command of parameters, while write 0x02 to CAL4_H for the second CTRL9 command of parameters. Then trigger the CTRL9 command with 0x0D(CTRL_CMD_CONFIGURE_PEDOMETER). Refer to Table 28 for details. 


Table 38. Write Pedometer Parameters to QMI8658A


<table><tr><td>Register (bits)</td><td>First CTRL9 Command</td><td>Second CTRL9 Command</td></tr><tr><td>CAL1_L (7:0)</td><td>ped_sample_cnt[7:0]</td><td>ped_time_up[7:0]</td></tr><tr><td>CAL1_H (7:0)</td><td>ped_sample_cnt[15:8]</td><td>ped_time_up [15:8]</td></tr><tr><td>CAL2_L (7:0)</td><td>ped_fix_peak2peak [7:0]</td><td>ped_time_low</td></tr><tr><td>CAL2_H (7:0)</td><td>ped_fix_peak2peak[15:8]</td><td>ped_cnt_entry</td></tr><tr><td>CAL3_L (7:0)</td><td>ped_fix_peak [7:0]</td><td>ped_fix_precision</td></tr><tr><td>CAL3_H (7:0)</td><td>ped_fix_peak[15:8]</td><td>ped_sig_count</td></tr><tr><td>CAL4_L (7:0)</td><td>NA</td><td>NA</td></tr><tr><td>CAL4_H (7:4)</td><td>0x01(means 1st command)</td><td>0x02(means 2nd command)</td></tr><tr><td>CTRL9 code</td><td>0x0D(CTRL_CMD_CONFIGURE_PEDOMETER)</td><td>0x0D(CTRL_CMD_CONFIGURE_PEDOMETER)</td></tr></table>


Note: Configuration should be done when accelerometer and gyroscope are disabled(CTRL7.aEN = CTRL7.gEN =0). 


## 11.3 Enable Pedometer

After successfully passed the parameters to QMI8658 Pedometer engine, host need to enable the Pedometer engine by setting CTRL8.bit4 to 1. If the accelerometer is enabled (CTRL7.aEN = 1), the Pedometer engine will start to detect the steps. 

If CTRL8.bit4 is set to 0, or CTRL7.aEN = 0, Pedometer engine will stop to detect the steps. 

## 11.4 Pedometer Interrupt

Once the detected steps (count from last pedometer event) reach the ped_sig_count defined by host, it will generate the Pedometer interrupt once. For example, if ped_sig_count = 4, then every 4 valid steps detected, there is one Pedometer interrupt (internal signal). 

At the meantime, the STATUS1.bit4 is set and host can read this bit to confirm Pedometer event is detected. 

The Pedometer interrupt(internal signal) can be selected to drive INT1(CTRL8.bit6 = 1) or INT2(CTRL8.bit6 = 0). And once the corresponding INT pin is enabled(by CTRL1.bit3 for INT1, or CTRL1.bit4 for INT2), the Pedometer interrupt will be seen on the INT pin(synced with DRDY). Refer to to 5.3 Configuration Registers for details. 

## 11.5 Read Step Count

When the Pedometer is enabled, and the Pedometer event is detected (STATUS1.bit4 = 1), the step count is updated to the Step Count registers of STEP_CNT_LOW, STEP_CNT_MIDL, STEP_CNT_HIGH, in 24-bits value. Note that the step count is updated per the ped_sig_count definition. For example, if ped_sig_count = 4, the step count is be updated every 4 steps. The 24-bits step count is cycling counted, will return to 0 after 0xFFFFFF. 

Host can read from the three registers, to get the latest step count. 

## 11.6 Reset Step Count

The Step Count can be reset by issuing Power-On Reset, Soft Reset, setting CTRL8.bit4 to 1 from 0, and CTRL9 command (CTRL_CMD_RESET_PEDOMETER). 

After reset the Step Count registers (STEP_CNT_LOW, STEP_CNT_MIDL, STEP_CNT_HIGH) are cleared to 0. 

When CTRL8.bit4 (Pedo_EN) = 0, set CTRL8.bit4 to 1 will reset the Setp Count registers. Host can simply clear the CTRL8.bit4 and then set it to restart the Pedometer engine and reset the Step Count registers. Refer to Table 22 for CTRL8 register. 

Send 0x0F (CTRL_CMD_RESET_PEDOMETER) by CTRL9 command protocol will reset the Step Count without reset the QMI8658A. Refer to Table 28 for CTRL9 operation. 

Note that enalbe and/or disable the accelerometer by configure CTRL7.bit1(aEN), will not influence the content of the Step Counter registers. 

## 12 Wake on Motion (WoM)

## 12.1 Wake on Motion Principle

The purpose of the Wake on Motion (WoM) functionality is to allow a system to enter a low power sleep state while the system is static and then to automatically awaken when moved. In this mode the system should use very little power, yet still respond quickly to motion. 

It is assumed that the system host processor is responsible for configuring the QMI8658A correctly to place it into Wake on Motion mode, and then reconfigure the QMI8658A as necessary following a WoM interrupt. 

Note that, Wake on Motion function works similar to Any-Motion detection but without sensor data output and can be configure more flexibly on interrupt behavior. Besides, once QMI8658A is configured into Wake on Motion mode, there is WoM interrupt to awaken the host if movement is detected. 

## 12.2 Wake on Motion Parameters

Wake on Motion is configured through the CTRL9 command interface (refer to Table 28 for details for CTRL_CMD_WRITE_WOM_SETTING ). 


Table 39. Registers used for WoM


<table><tr><td>Register(bits)</td><td>Format</td><td>Resolution</td><td>Function</td></tr><tr><td>CAL1_L[7:0]</td><td>1-byte unsigned integer</td><td>1mg</td><td>WoM Threshold:Indicate the threshold of absolute of slope of a valid movement0x00 must be used to indicate that WoM mode is disabled</td></tr><tr><td>CAL1_H[7:6]</td><td>2-bits unsigned integer</td><td>1</td><td>WoM Interrupt Initial Value select:01 – INT2 (with initial value 0)11 – INT2 (with initial value 1)00 – INT1 (with initial value 0)10 – INT1 (with initial value 1)</td></tr><tr><td>CAL1_H[5:0]</td><td>6-bits unsigned integer</td><td>1</td><td>Interrupt Blanking Time (in number of accelerometer samples), the number of consecutive samples that will be ignored after enabling the WoM, to screen out unwanted fake detection</td></tr><tr><td>CTRL9[7:0]</td><td>1-byte unsigned integer</td><td>1</td><td>CTRL_CMD_WRITE_WOM_SETTING (0x08): to send this pre-defined command to QMI8658A to enable the WoM mode</td></tr></table>

The WoM Threshold value is configurable to make the amount of motion required to wake QMI8658A and the host. The special threshold value of 0x00 can be used to disable the WoM mode, returning the interrupt pins to their normal functionality. Refer to 6 Interrupts. 

The Interrupt Initial Value (1 or 0) and the interrupt pin used for signaling (INT1 or INT2) are selectable to make it easy for system integrators to use the WoM motion mode to wake the host processor from its deepest sleep level. The selected INT pin can be configured as High-active or Low-active, if enabled by CTRL1.bit3 for INT1, or CTRL1.bit4 for INT2, the WoM interrupt can be seen on the INT pin and interrupt host. 

The Interrupt Blanking Time is a programmable number of accelerometer samples to ignore when starting WoM mode so that no spurious wake-up events are generated by startup transients. 

## 12.3 Accelerometer Configuration

Host is expected to configure the proper ODR and full-scale range of accelerometer by configure CTRL2 register. Refer to Table 22. 

## 12.4 Wake on Motion Event

When a Wake on Motion event is detected the QMI8658A will set STATUS1.WoM = 1 (bit2). Reading STATUS1 by the host will clear the WoM bit and will reset the chosen interrupt line (INT1 or INT2) to the value given by the WoM Interrupt initial value (refer to 12.2). 

For each WoM event, the state of the selected interrupt line is toggled. This ensures that while the system is moved, the host processor will receive wakeup interrupts regardless of whether it uses high, low, positive- or negative-edge interrupts. 

The QMI8658A stays in WoM mode until commanded to enter a new mode by the host processor. 

## 12.5 Configuration Procedure

To configure QMI8658A into WoM mode, the sequence ilustated in Figure 25 should be followed and implemented. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/1121bcd32a1db1b799b34f961bc37f711d74f165908c6df5e7233e6cd26e7437.jpg)



Figure 25. WoM Configuration Commands and Sequence


The STATUS1.WoM bit is cleared upon setting the WoM threshold to a non-zero value, and the selected interrupt pin is configured according to the settings. The interrupt blanking time is included that prevents spurious interrupts to propagate after the enabling of WoM. 

## 12.6 Exit Wake on Motion Mode

To exit WoM mode the host must first clear CTRL7[1:0] to disable all sensors, and then write a threshold value of 0x0 for the WoM Threshold (see Table 39, Registers used for WoM) and execute the WoM configuration CTRL9 command (refer to Table 28 for details for CTRL_CMD_WRITE_WOM_SETTING). On doing this the interrupt pins will return to their normal function. After zeroing the WoM Threshold the host processor can proceed to reconfigure the QMI8658A as normal. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/5d8f8c94820c4547589837d3eb5ae00906f5da8ab53f08a72c3a1fa287e4ccbf.jpg)



Figure 26. WoM Example Diagram


## 13 Locking Mechanism

## 13.1 Locking Mechanism Principle

Locking Mechanism function is enabled in SyncSample mode, can lock the sensor data and keep the values in data registers after a proper locking process. 

As the concept of “shadow register”, which enables host to read the locked data in unlimited delay without the risk of mixing the two consecutive data if the new data comes and updates to the sensor data registers during the host read the sensor data registers. Refer to 6.1 SyncSample mode. 

## 13.2 Locking Mechanism Data Reading Process


Figure 27 shows the process of reading data in Locking Mechanism.


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/0f1a975b4532ad8a4d290778b0fe3587ea9671cd7bf49da7bac99cb0c8a120f5.jpg)



Figure 27. Data Reading Process in Locking Mechanism


## 13.2.1 Disable/Enable AHB Clock Gating

First host need to disable the internal AHB clock gating, this will fully screen out the possibility of misalignment of the two consecutive data samples. Note this is ONLY needed when using SPI (accel only mode with ODR less than 500Hz), I2C or I3C interfaces (in all ODRs). 

Host can disable the internal AHB clock gating by applying: 

1- write 0x01 to CAL1_L register. 

2- write 0x12 (CTRL_CMD_AHB_CLOCK_GATING) in CTRL9 protocol, refer to 5.9 and 5.10.6.13. 

After disabling the sensor, enable back the clock gating by: 

1- write 0x00 to CAL1_L register. 

2- write 0x12 (CTRL_CMD_AHB_CLOCK_GATING) in CTRL9 protocol, refer to 5.9 and 5.10.6.13. 

## 13.2.2 Enable Locking Mechanism

The locking Mechanism is enabled when setting CTRL7.bit7 to “1” (syncSmpl) and enable accelerometer and/or gyroscope. 

1- Enable 6DOF in this mode, write 0x83 to CTRL7. 

2- Enable only Accel in this mode, write 0x81 to CTRL7. 

3- Enable only Gyro in this mode, write 0x82 to CTRL7. 

## 13.2.3 Reading Sensor Data

When the Locking Mechanism is enabled, the reading to the STATUSINT register when new sensor data is available (STATUSINT.Avail = 1) will trigger the locking of the current sensor data sample. Once the data sample is locked, new data will be dropped, until the release of the locking. The Locking Mechanism is automatically released after host reads GZ_H if gyroscope is enabled or AZ_H if accelerometer only is enabled. 

Process of data reading in Locking Mechanism mode: 

Read STATUSINT register, if STATUSINT.Avail = 1, the locking mechanism is started to take effect, go to step 2. If STATUSINT.Avail = 0, repeat step 1. 

If STATUSINT.Avail = 1 and STATUSINT.Locked = 0, means data looking is in progress, and will be locked within Data_Lock_Delay. If STATUSINT.Locked = 1, go to step 4. 

1- Wait for Data_Lock_Delay duration (refer to 13.3), go to step 4. 

Burst read the sensor data until the last byte of enabled sensor(s) (to release the locking), Refer to Table 25. 

2- Repeat step 1 – 4, until Exit 

## 13.3 Data_Lock_Delay

When the gyroscope is enabled the duration of Data_Lock_Delay is shown in Table 40. 


Table 40. Data_Lock_Delay When Gyroscope Is Enabled


<table><tr><td>ODR setting</td><td>ODR(Hz)</td><td>Data_Lock_Delay (usec)</td></tr><tr><td>0</td><td>7174.4</td><td>2</td></tr><tr><td>1</td><td>3587.2</td><td>2</td></tr><tr><td>2</td><td>1793.6</td><td>4</td></tr><tr><td>3</td><td>896.8</td><td>6</td></tr><tr><td>4</td><td>448.4</td><td>12</td></tr><tr><td>5</td><td>224.2</td><td>12</td></tr><tr><td>6</td><td>112.1</td><td>12</td></tr><tr><td>7</td><td>56.05</td><td>12</td></tr><tr><td>8</td><td>28.025</td><td>12</td></tr></table>

When the gyro is not enabled (accel only mode) the value of Data_Lock_Delay is shown in Table 41. 


Table 41. Data_Lock_Delay When Gyroscope Is Disabled


<table><tr><td>ODR setting</td><td>ODR(Hz)</td><td>Data_Lock_Delay (usec)</td></tr><tr><td>3</td><td>1000</td><td>6</td></tr><tr><td>4</td><td>500</td><td>12</td></tr><tr><td>5</td><td>250</td><td>24</td></tr><tr><td>6</td><td>125</td><td>48</td></tr><tr><td>7</td><td>62.5</td><td>48</td></tr><tr><td>8</td><td>31.25</td><td>48</td></tr><tr><td>12</td><td>128</td><td>40</td></tr><tr><td>13</td><td>21</td><td>100</td></tr><tr><td>14</td><td>11</td><td>200</td></tr><tr><td>15</td><td>3</td><td>270</td></tr></table>

## 13.4 Exit Locking Mechanism

Once pulling sensor data from QMI8658 is finished, it can be configured to exit the Locking Mechanism mode, by disabling the sensors by setting CTRL7.bit[1:0] to 0, and enabling the AHB Clock Gating (refer to 13.2.1). 

## 13.5 On-The-Fly ODR Change in Locking Mechanism

The on-the-fly ODR changing is supported, so host can change the ODR of sensor without disable the sensor. 

An example sequence of changing ODR without disabling the sensor in locking mechanism is shown below: 

1- Write CTRL2/CTRL3 to set the ODR’s of accelerometer and gyroscope and full scales. 

2- Write 0x81 / 0x82 / 0x83 to CTRL7. 

3- Read Sensor Data according 13.2.3. 

4- Changing ODR on-the-fly: 

a- Write 0x01/0x02/0x03 to CTRL7(clear the syncSmpl bit). 

b- Wait 1ms. 

c- Clear the Locking Mechanism in case if the data is still locked from previous ODR by reading GZ_H if gyroscope is enabled or AZ_H if accelerometer only is enabled. 

d- Write CTRL2 / CTRL3 with the new ODR’s. 

e- Write 0x81/0x82/0x83 to CTRL7. 

5- 1S Start poll and read Sensor Data using the new ODRs based on 13.2.3. 

Note that, the new data will be stable in at least 3 samples for filter to settle down, therefore, it is recommended to discard the first several samples at host side. 

# 14 Calibration-On-Demand (COD)

## 14.1 COD Principle

The Calibration-On-Demand supports the on-demand calibration of Gyro X and Y axes. Based the internal integrated functionality, the QMI8658A can calibrate the internal gain of X & Y axes of gyroscope, result in a more precise sensitivity, and a tighter distribution of the X & Y axes sensitivity over QMI8658A chips. 

Note that the Z axis of gyroscope is not influenced by COD. 

## 14.2 Run COD

To run the COD, host need to 

1. Set CTRL7.aEN = 0 and CTRL7.gEN = 0, to disable the accelerometer and gyroscope. 

2. Issue the CTRL_CMD_ON_DEMAND_CALIBRATION (0xA2) by CTRL9 command. 

3. And wait about 1.5 seconds for QMI8658A to finish the CTRL9 command. 

4. Read the COD_STATUS register (0x46) to check the result/status of the COD implementation. 

During the process, it is recommended to place the device in quiet, otherwise, the COD might fail and report error. 

If succeeds, the recalibrated gain parameters will be applied to the sensor data afterwards. The updated gains are output to the UI registers and can be read by host, refer to 14.3. The recalibrated gain parameters will be lost if a power on reset or soft reset is implemented, QMI8658A will then use the on-chip default gain parameters. 

If failed, there is no influence on the operation of gyroscope, QMI8658A will keep using the previous workable parameters (last successful COD parameters or the on-chip default parameters). 

## 14.3 COD Status

If the COD command is successfully implemented, the COD_STATUS register will output 0x00 for the indication. 

The non-zero value of COD_STATUS indicates different modes of failure. Refer to 5.7 Calibration-On-Demand (COD) Status Register for details. 

## 14.4 Save and Restore the New Gain Parameters

After a successful COD, the new gains with COD correction will be applied to the future data of X and Y axes of Gyroscope. At the meantime, the new parameters are updated to the registers below, for host to read and save. 

1. Gyro-X gain (16 bits) will be in dVX_L and dVX_H registers (0x51, 0x52) 

2. Gyro-Y gain (16 bits) will be in dVY_L and dVY_H registers (0x53, 0x54) 

3. Gyro-Z gain (16 bits) will be in dVZ_L and dVZ_H registers (0x55, 0x56) 

If the host saved those gain parameters, it is possible to pass them back to the QMI8658A (without invoking again the COD routine), using the CTRL9 command CTRL_CMD_APPLY_GYRO_GAINS (0xAA) as follow: 

1. Disable Accelerometer and Gyroscope by setting CTRL7.aEN = 0 and CTRL7.gEN = 0 

2. write Gyro-X gain (16 bits) to registers CAL1_L and CAL1_H registers (0x0B, 0x0C) 

3 write Gyro-Y gain (16 bits) to registers CAL2_L and CAL2_H registers (0x0D, 0x0E) 

4. write Gyro-Z gain (16 bits) to registers CAL3_L and CAL3_H registers (0x0F, 0x10) 

5. Write 0xAA to CTRL9 and follow CTRL9 protocol 

Once the CTRL9 command is successfully finished, the restored gains will take effects for future data of Gyroscope. 

Note that it is always recommended to run the COD from time to time to apply the precise and up-to-date correction of the Gyro-X and Gyro-Y sensitivity. Designer should be careful to restore the out-of-date gain parameters, especially when there is significant change of PCB stress. 

## 15 Self-Test (Check-Alive)

## 15.1 Accelerometer Self-Test

The accelerometer Self-Test (Check-Alive) is used to determine if the accelerometer is functional and working within acceptable parameters. 

It is implemented by applying an electrostatic force to actuate each of the three X, Y, and Z axis of the accelerometer. If the accelerometer mechanical structure responds to this input stimulus by sensing at least 200 mg, then the accelerometer can be considered functional. 

The accelerometer Self-Test data is available to be read at registers dVX_L, dVX_H, dVY_L, dVY_H, dVZ_L and dVZ_H. The Host can initiate the Self-Test at any time with the following procedure. 

Procedure for accelerometer Self-Test: 

1- Disable the sensors (CTRL7 = 0x00). 

2- Set proper accelerometer ODR (CTRL2.aODR) and bit CTRL2.aST (bit7) to 1 to trigger the Self-Test. 

3- Wait for QMI8658A to drive INT2 to High, if INT2 is enabled (CTRL1.bit4 = 1), or STATUSINT.bit0 is set to 1. 

4- Set CTRL2.aST(bit7) to 0, to clear STATUSINT1.bit0 and/or INT2. 

5- Check for QMI8658A drives INT2 back to Low, and sets STATUSINT1.bit0 to 0. 

6- Read the Accel Self-Test result: 

X channel: dVX_L and dVX_H (registers 0x51 and 0x52) 

Y channel: dVY_L and dVY_H (registers 0x53 and 0x54) 

Z channel: dVZ_L and dVZ_H (registers 0x55 and 0x56) 

The results are 16-bits in format signed U5.11, resolution 0.5mg (1 / 2^11 g). 

If the absolute results of all three axes are higher than 200mg, the accelerometer can be considered functional. Otherwise, the accelerometer cannot be considered functional. 

Note that, the Self-Test function will automatically set the full-scall to 16g and use the aODR set by user (CTRL2.aODR). At the end of Self-Test, QMI8658A will update CTR2 with the original value user set before starting the Check-Alive) routine. 

The typical time for Self-Test (from setting aST to 1, until the rising edge of INT2 if enabled, or STATUSINT.bit0 is set to 1) costs about 25 ODRs: 

25ms @ 1KHz ODR 

800ms @ 32Hz ODR 

2.2s @ 11Hz ODR 

## 15.2 Gyroscope Self-Test

The gyroscope Self-Test (Check-Alive) is used to determine if the gyroscope is functional. 

It is implemented by applying an electrostatic force to actuate each of the three X, Y, and Z axis of the gyroscope and measures the mechanical response on the corresponding X, Y, and Z axis. If the equivalent magnitude of the gyroscope output is greater than 300dps for each axis, the gyroscope can be considered as functional. 

The gyroscope Self-Test data is available to be read at output registers dVX_L, dVX_H, dVY_L, dVY_H, dVZ_L & dVZ_H. The Host can initiate the Self-Test anytime with the following procedure. 

Procedure for gyroscope Self-Test: 

1- Disable the sensors (CTRL7 = 0x00). 

2- Set the bit gST to 1. (CTRL3.bit7 = 1’b1). 

3- Wait for QMI8658A to drive INT2 to High, if INT2 is enabled, or STATUSINT.bit0 is set to 1. 

4- Set CTRL3.aST(bit7) to 0, to clear STATUSINT1.bit0 and/or INT2. 

5- Check for QMI8658A drives INT2 back to Low, or sets STATUSINT1.bit0 to 0. 

6- Read the Gyro Self-Test result: 

X channel: dVX_L and dVX_H (registers 0x51 and 0x52) 

Y channel: dVY_L and dVY_H (registers 0x53 and 0x54) 

Z channel: dVZ_L and dVZ_H (registers 0x55 and 0x56) 

Read the 16 bits result in format signed U12.4, resolution is 62.5mdps (1 / 2^4 dps). 

If the absolute results of all three axes are higher than 300dps, the gyroscope can be considered functional. Otherwise, the gyroscope cannot be considered functional. 

Note that, the Self-Test function will automatically set the full-scale (gFS) and ODR (gODR) of CTRL3. At the end of Self-Test, QMI8658A will update CTR3 with the original value user set before starting the Self-Test routine. 

The typical time (from writing gST to 1, until the rising edge of INT2 if enabled, or STATUSINT.bit0 set to 1) cost for the Self-Test process is about 400ms. 

## 16 Host Serial Interface

QMI8658A Host Serial Interface supports slave interfaces of I2C, MIPI I3C and SPI. For SPI, it supports both 3-wire and 4-wire modes. The basic timing characteristics for the interface are described below. Through the QMI8658A Host Serial Interface, the host can access, setup and control the QMI8658A UI Registers. 

## 16.1 Address Auto Increment

During burst reads and writes, the target address can be configured in Auto-Increment or Non-Increment, by CTRL1.ADDR_AI(bit 6), refer to 5.3. 

For example, during burst read(burst write is similar) started from register 0x0B(CAL1_L): 

If ADDR_AI = 0, the register address will not increase, and QMI8658A will output the content of CAL1_L, CAL1_L, CAL1_L …, as long as host continues the burst read clock; 

If ADDR_AI = 1, the register address will automatically increase, and QMI8658A will output the content of CAL1_L, CAL1_H, CAL2_L …, as long as host continues the burst read clock. 

Note that the default value of ADDR_AI is 0, so it is recommended to set it to 1 from beginning, in case of burst read/write is required. 

Note that, burst writes to Configuration Registers(refer to Table 22) are NOT supported. These registers should be written in single cycle mode only. 

## 16.2 Serial Peripheral Interface (SPI)

## 16.2.1 SPI Features

QMI8658A supports both 3-wire and 4-wire modes in the SPI slave interface. The SPI 4-wire mode uses two control lines (CS, SPC) and two data lines (SDI, SDO). The SPI 3-wire mode uses the same control lines and one bi-directional data line (SDIO). The SDI /SDIO pin is used for both 3- and 4-wire modes and is configured based on the mode selected. 

SPI transactions can be done in either Mode 0 (CPOL=0, CPHA=0) or Mode 3 (CPOL=1, CPHA=1). The QMI8658A interface automatically detects which mode is in use and configures clocking accordingly. 

SPI 3- or 4-wire modes are configured by setting CTRL1.bit7. SPI 3-wire mode is selected when CTRL1.bit7 = 1. The default configuration is SPI 4-wire mode, that CTRL1.bit7 is 0. 

Figure 28 shows the SPI address and data formats. 

## SPI Features

Data is latched on the rising edge of the clock 

Data should change on falling edge of clock 

Maximum frequency is 15 MHz 

Data is delivered MSB first 

Support single read/writes and multi cycle (Burst) read/writes. 

NOTE: burst writes to Configuration Registers(refer to Table 22) are NOT supported. These registers should be written in single cycle mode only. 

Supports 7-bit Address format and 8-bit data format 


Address Format


<table><tr><td>MSB</td><td></td><td></td><td></td><td></td><td></td><td></td><td>LSB</td></tr><tr><td>Read</td><td>A6</td><td>A5</td><td>A4</td><td>A3</td><td>A2</td><td>A1</td><td>A0</td></tr></table>


Read-indicates read (1) or write (O) transaction relative to the SPl master 



Data Format


<table><tr><td>MSB</td><td></td><td></td><td></td><td></td><td></td><td></td><td>LSB</td></tr><tr><td>D7</td><td>D6</td><td>D5</td><td>D4</td><td>D3</td><td>D2</td><td>D1</td><td>D0</td></tr></table>


Figure 28. SPI Address and Data Format


## 16.2.2 SPI Interface Connection

In a typical SPI Master and Multi-Slave configuration, the SPI master shares the SPI clock (SPC), the serial data input (SDI), and the Serial Data Output (SDO) with all the connected SPI slave devices. Unique Chip Select (CS) lines connect each SPI slave to the master. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/17f8ae53888538261aa06abc3460a11cf624046c83cae6f775fad0f86a5b0176.jpg)



Figure 29. Typical SPI 4-Wire Connection


Figure 29 and Figure 30 show typical multi-slave 4- and 3-wire configurations. The primary difference between the two configurations is that the SDI and SDO lines are replaced by the bi-directional SDIO line. The SDIO line is driven by the master with both address and data when it is configured for write mode. During read mode, the SDIO line is driven by the master with the address, and subsequently driven by the “addressed” slave with data. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/b24693187296f47ea20962e053263e9019e9f1d846b8ede32c8b158d3579041f.jpg)



Figure 30. Typical SPI 3-Wire Connection


## 16.2.3 SPI Transaction Protocol


Figure 31 and Figure 32 illustrate the waveforms for both 4-wire and 3-wire SPI read and write transactions. Note that CS is active during the entire transaction.


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/118984290781942464761808f898b81885c148b2068db892404a20fd03dbd71e.jpg)



Figure 31. SPI 4-Wire Single Byte Read and Write (Mode 0 and Mode 3)


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/bcbe927fd96d1473d8df1c77c328db9a034711664028f16fbcc6dc0ac3ce5301.jpg)



Figure 32. SPI 4-Wire Multi-Byte Read and Write Transactions


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/889ea22fa0a570a1df61972e563e9c52aeab5fe3cc361cd834c48024c95384f3.jpg)



Figure 33. SPI 3-Wire Single Byte Read and Write Transactions


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/08d74c50c2878fe7e4d5c0becf3586bf9f1f85ba71b6d79859fa668c59eaddf5.jpg)



Figure 34. SPI 3-Wire Multi-Byte Read and Write Transactions


## 16.2.4 SPI Timing Characteristics

The typical operating conditions for the SPI interface are provided in Table 42. Please refer to Table 6 for the VIL, VIH, VOL, VOH definition to define the rising and falling edge condition of the timing symbols. 

VDDIO = 1.8 V, T = 25°C unless otherwise noted. 


Table 42. SPI Interface Timing Characteristics


<table><tr><td>Symbol</td><td>Parameter</td><td>Min.</td><td>Max.</td><td>Unit</td></tr><tr><td><eq>t_{SPC}</eq></td><td>SPI Clock Cycle</td><td>66.6</td><td></td><td>ns</td></tr><tr><td><eq>f_{SPC}</eq></td><td>SPI Clock Frequency</td><td></td><td>15</td><td>MHz</td></tr><tr><td><eq>t_{SCS}</eq></td><td>CS Setup Time</td><td>6</td><td></td><td>ns</td></tr><tr><td><eq>th_{CS}</eq></td><td>CS Hold Time</td><td>8</td><td></td><td>ns</td></tr><tr><td><eq>t_{SSDI}</eq></td><td>SDI Input Setup Time</td><td>5</td><td></td><td>ns</td></tr><tr><td><eq>th_{SDI}</eq></td><td>SDI Input Hold Time</td><td>15</td><td></td><td>ns</td></tr><tr><td><eq>tv_{SDO}</eq></td><td>SDO Time for Valid Output</td><td></td><td>50</td><td>ns</td></tr><tr><td><eq>th_{SDO}</eq></td><td>SDO Hold Time for Output</td><td>9</td><td></td><td>ns</td></tr><tr><td><eq>td_{SDO}</eq></td><td>SDO Disable Time for Output</td><td></td><td>50</td><td>ns</td></tr><tr><td><eq>t_{SSDIO}</eq></td><td>SDIO Address Setup Time</td><td>5</td><td></td><td>ns</td></tr><tr><td><eq>th_{SDIO}</eq></td><td>SDIO Address Hold Time</td><td>15</td><td></td><td>ns</td></tr><tr><td><eq>tv_{SDIO}</eq></td><td>SDIO Time for Valid Data</td><td></td><td>50</td><td>ns</td></tr><tr><td><eq>tcz_{SDIO}</eq></td><td>SDIO Time from SPC to High Z</td><td></td><td>50</td><td>ns</td></tr><tr><td><eq>tz_{SDIO}</eq></td><td>SDIO Time from CS to High Z</td><td></td><td>50</td><td>ns</td></tr></table>

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/64bd7fdf1d792e4ae700026330811ed9e92b3fe4400f2e5f333109f87faa0ca0.jpg)



SPI : Mode 0


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/0c443f9f243cf766b0a6feee3980035d689da41dabfb45cc7c4a253225dbbb49.jpg)



SPI : Mode 3



Figure 35. Timing Characteristics for SPI 3- and 4-Wire Interfaces


## 16.3 I2C Interface

## 16.3.1 I2C Slave Address Selection

During the slave device selection phase, the I2C master supplies the 7-bit I2C slave device address to enable the QMI8658A. When SA0 is pulled down externally, the 7-bit device address becomes 0x6B (0b1101011). The 7-bit device address for the QMI8658A is 0x6A (0b1101010) if SA0 is pulled up or left unconnected. Note that internally there is a weak pull-up of 200K and this pull-up resistor will be automatically disabled after the detection of I2C slave address during the Reset Process, refer to 7.4. 

## 16.3.2 I2C Interface Characteristics

Table 43 provides the I2C interface timing characteristics while Figure 36 and Figure 37 illustrate the I2C timing for both fast and standard modes, respectively. Please refer to Table 6 for the VIL, VIH, VOL, VOH definition to define the rising and falling edge condition of the timing symbols. 

For additional technical details about the I2C standard, such as pull-up resistor sizing the user is referred to “UM10204 I2C-bus specification and user manual,” published by NXP B.V. 


Table 43. I2C Timing Characteristics


<table><tr><td>Symbol</td><td>Parameter</td><td>Conditions</td><td>Min.</td><td>Typ.</td><td>Max.</td><td>Unit</td></tr><tr><td><eq>f_{SCL}</eq></td><td>SCL Clock Frequency</td><td></td><td>0</td><td></td><td>400</td><td>KHz</td></tr><tr><td><eq>t_{BUF}</eq></td><td>Bus-Free Time between STOP and START Conditions</td><td></td><td>1300</td><td></td><td></td><td>ns</td></tr><tr><td><eq>t_{HD;STA}</eq></td><td>START or Repeated START Hold Time</td><td></td><td>600</td><td></td><td></td><td>ns</td></tr><tr><td><eq>t_{LOW}</eq></td><td>SCL LOW Period</td><td></td><td>1300</td><td></td><td></td><td>ns</td></tr><tr><td><eq>t_{HIGH}</eq></td><td>SCL HIGH Period</td><td></td><td>600</td><td></td><td></td><td>ns</td></tr><tr><td><eq>t_{SU;STA}</eq></td><td>Repeated START Setup Time</td><td></td><td>600</td><td></td><td></td><td>ns</td></tr><tr><td><eq>t_{SU;DAT}</eq></td><td>Data Setup Time</td><td></td><td>100</td><td></td><td></td><td>ns</td></tr><tr><td rowspan="2"><eq>t_{HD;DAT}</eq></td><td rowspan="2">Data Hold Time</td><td>Standard Mode</td><td>0</td><td></td><td>3450</td><td rowspan="2">ns</td></tr><tr><td>Fast Mode</td><td>0</td><td></td><td>900</td></tr><tr><td rowspan="2"><eq>t_{RCL,t_R}</eq></td><td rowspan="2">SCL Rise Time</td><td>Standard Mode</td><td></td><td></td><td>1000</td><td rowspan="2">ns</td></tr><tr><td>Fast Mode</td><td colspan="2">20 + 0.1 * <eq>C_B^{(14)}</eq></td><td>300</td></tr><tr><td rowspan="2"><eq>t_{FCL}</eq></td><td rowspan="2">SCL Fall Time</td><td>Standard Mode</td><td></td><td></td><td>300</td><td rowspan="2">ns</td></tr><tr><td>Fast Mode</td><td colspan="2">20 + 0.1 * <eq>C_B^{(14)}</eq></td><td>300</td></tr><tr><td rowspan="2"><eq>t_{RDA,t_{RCL1}}</eq></td><td rowspan="2">SDA Rise Time.Rise Time of SCL after a Repeated START Condition and after ACK Bit</td><td>Standard Mode</td><td></td><td></td><td>1000</td><td rowspan="2">ns</td></tr><tr><td>Fast Mode</td><td colspan="2">20 + 0.1 * <eq>C_B^{(14)}</eq></td><td>300</td></tr><tr><td rowspan="2"><eq>t_{FDA}</eq></td><td rowspan="2">SDA Fall Time</td><td>Standard Mode</td><td></td><td></td><td>300</td><td rowspan="2">ns</td></tr><tr><td>Fast Mode</td><td colspan="2">20 + 0.1 * <eq>C_B^{(14)}</eq></td><td>300</td></tr><tr><td><eq>t_{SU;STO}</eq></td><td>Stop Condition Setup Time</td><td></td><td>600</td><td></td><td></td><td>ns</td></tr></table>

## Note:

14. CB is the bus capacitance. 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/761270bfaf7c168dc47a3b2c1c8b68d0c46cce7bfad9c201b998969f02fc8eb4.jpg)



Figure 36. I2C Standard Mode Interface Timing


![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/a8654eb490186351526d27f1d0e5202256efecb1cfe5c6600e4cb0eef3c3ad85.jpg)



Note A: First rising edge of SCLH after Repeated Start and after each ACK bit


Figure 37. I2C Fast Mode Interface Timing 

## 16.4 I3C Interface

The QMI8658A is compliant with the MIPI Alliance Basic Specification for I3C, version 1.0. 

Note that HDR, IBI are not supported by QMI8658A. 

The PID of 8658A is 0x086E00051000. 

MIPI Manufacturer ID(VID) for QST Corp. is 0x0437. 

The static address of I3C follows the descriptions in I2C, refer to 16.3.1. 

## 17 Package and Handling

## 17.1 Package Drawing

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/ef3b7d1cc8d1a3e021555966b2d61f6e081b6fec7d35c0d1ffa0fd1130d01513.jpg)



Figure 38. 14 Pin LGA 2.5 x 3.0 x 0.86 mm Package


## 17.2 Reflow Specification

![image](https://cdn-mineru.openxlab.org.cn/result/2026-06-13/d42292b2-1dbd-415e-8eb8-03a9dcaff36f/f4458c9ad281426d171187580ce335bc88767fa3c2dc869a000a81e5514c890b.jpg)



Note:



15. Figure from JEDEC J-STD-020


<table><tr><td>Profile Feature</td><td>Pb-Free Assembly Profile</td></tr><tr><td>Temperature Min. (<eq>T_{smin}</eq>)</td><td>150°C</td></tr><tr><td>Temperature Max. (<eq>T_{smax}</eq>)</td><td>200°C</td></tr><tr><td>Time (ts) from (<eq>T_{smin}</eq> to <eq>T_{smax}</eq>)</td><td>60-120 seconds</td></tr><tr><td>Ramp-up Rate (<eq>T_L</eq> to <eq>T_P</eq>)</td><td>3°C/second max.</td></tr><tr><td>Liquidous Temperature (<eq>T_L</eq>)</td><td>217°C</td></tr><tr><td>Time (tL) Maintained above (<eq>T_L</eq>)</td><td>60-150 seconds</td></tr><tr><td>Peak Body Package Temperature (<eq>T_P</eq>)</td><td>260°C +0°C / -5°C</td></tr><tr><td>Time (tp) within 5°C of 260°C</td><td>30 seconds</td></tr><tr><td>Ramp-down Rate (<eq>T_P</eq> to <eq>T_L</eq>)</td><td>6°C/second max.</td></tr><tr><td>Time 25°C to Peak Temperature</td><td>8 minutes max.</td></tr></table>


Figure 39. Reflow Profile


## 17.3 Storage Specifications

QMI8658A storage specification conforms to IPC/JEDEC J-STD-020D.01 Moisture Sensitivity Level (MSL) 3. Floor life after opening the moisture-sealed bag is 168 hours with storage conditions: Temperature: ambient to ≤30°C and Relative Humidity: 60%RH. 

## 18 Document Information

## 18.1 Revision History

<table><tr><td>Revision</td><td>Revision Date</td><td>Description</td></tr><tr><td>0.4</td><td>April 22, 2020</td><td>Initial release of Advance Information datasheet</td></tr><tr><td>0.5</td><td>July 7, 2020</td><td>Updated CAL Register Addresses, CTRL9 Commands and Descriptions, Current Consumption, Accelerometer and Gyroscope Filter Characteristics, Low Power Mode ODR, Wake on Motion, Magnetometer Sensors supported</td></tr><tr><td>0.6</td><td>Jan 13, 2021</td><td>Updated SPI description and diagrams, SPI modes, product performance specifications, and register map and descriptions</td></tr><tr><td>0.7</td><td>17 May, 2021</td><td>Updated the pin name and function name map, typical SPI connections, soft reset, Self-Test, On-Demand Calibration, I2C timing parameter table, ADDR_AI, BE, VDDIO rising limitation, recommended I2C pull-up resistance</td></tr><tr><td>0.8</td><td>10 Sep, 2021</td><td>Added the maximum limitation of VIH, updated FIFO read command and sequence, updated ODR &amp; filter bandwidth configurations, updated CTRL2, CTRL3, CTRL4, CTRL5 descriptions, deleted descriptions of magnetometer, updated Electro-Mechanical Specifications, updated the internal block diagram &amp; interface operating modes, deleted the specifications, registers, and application diagrams that relative to I2CM interface, updated the Features, Descriptions &amp; Applications, updated the marking information, updated the disabling of the internal pull-up resistors in los.</td></tr><tr><td>0.9</td><td>10 Jan, 2022</td><td>updated the INT1/INT2 enable bit in CTRL1, updated Wake on Motion, added locking mechanism.</td></tr><tr><td>0.94</td><td>29 Apr, 2022</td><td>Updated the parameters of Acceleration and Gyroscope, remove sys_hs, removed AltitudeEngine relative information, updated FIFO relative descriptions, updated Interrupt relative descriptions, updated operating mode and transition diagram, updated CTRL1, CTRL7, CTRL8 register map, added Motion Detection, Tap, Pedometer descriptions.</td></tr><tr><td>0.95</td><td>6 May, 2022</td><td>Added the Reset process, updated the time cost for Self-Test process and correct the description of aODR used for Self-Test, added the COD chapter, removed the CTRL_CMD_GYRO_BIAS CTRL9 command, updated the Locking Mechanism, WoM, COD, Self-Test, SPI, I2C, I3C interface descriptions, adjusted the format, remove CTRL4 &amp; CTRL6, remove tr requirement, added power on sequence of VDDIO and VDD, added noise density of gyroscope over ODR.</td></tr><tr><td>1.0</td><td>20 June, 2022</td><td>Updated the gyro noise density, changed the name of RESV-NC(pin 10) to RESV, updated the power on sequence.</td></tr></table>