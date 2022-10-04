# LoRaUWB_demo_single_master_slave

This Arduino sketch operates the LoRa-UWB Board (UCA - LEAT) in Master & Slave modes for Ultra-wideband measurement senarios. The measurement results including distance & link quality betweem the Master and Slave are sent to The Things Network via LoRaWAN Network.

The board starts in Slave Mode by default. To change to Master mode, press UserButton 3 times (triple click). In Master Mode, press UserButton 1 time (single click) to trigger the measurement. Measurement result will be send to LoRaWAN network.

<i>Contact project leaders in Credits section below for further details of the project, the hardware/antenna design & the measurement results.</i>

# Requirements

* [Arduino IDE](https://www.arduino.cc/en/software)
* [nguyenmanhthao996tn/LoRa-UWB-Core](https://github.com/nguyenmanhthao996tn/LoRa-UWB-Core)

<i><b>Arduino Legacy IDE (1.8.X) is recommended. Both LoRa-UWB-Core & this sketch are not tested on Arduino IDE version >= 2.0.</b></i>

# Setup / Getting Started

1. <b>Setup [Arduino IDE](https://www.arduino.cc/en/software) & [LoRa-UWB-Core](https://github.com/nguyenmanhthao996tn/LoRa-UWB-Core)</b>

Follow instructions on LoRa-UWB-Core's Github.

2. <b>Config Board options</b>

Open ArduinoIDE, configure the board options (Tool Menu) as the following image:

![Board Options](docs/pic0_board_options.png)

3. <b>Plug your LoRa-UWB Board to your PC via USB</b>

4. <b>Compile & Upload the sketch</b>

<i>Remember to choose the right COM Port that connect to your LoRa-UWB Board.</i>

If the COM Port of the LoRa-UWB Board does NOT show up in the list, the USB Type may be set as No USB in the previous upload. In this case, trigger the Bootloader Mode of the LoRa-UWB Board to uploader the code by holding BOOT button and press RESET Button.

Ensure that there is NO COM Port selected in Tool Menu (unless it will be failed to upload).

![no_com_port](docs/pic1_no_com_port.png)

# Program States Diagram

## NEED DIAGRAM

# LEDs behaviours
There are 2 LEDs on the LoRa-UWB Board: 1x Red LED & 1x Blue LED.

1. <b>LED Blue blinks 2 times -> LED Red blinks 2 times -> LED Blue blinks 2 times -> LED Red blinks 2 times</b>: The initialization is done without error.

2. <b>LED Red blinks 2 times -> LED Red is ON for 0.5 second</b>: The Slave Mode started.

3. <b>LED Blue blinks 2 times -> LED Blue is ON for 0.5 second</b>: The Master Mode started.

4. <b>LED Blue is ON for 1 second</b>: The Master Mode Measurement is started. You have 3 seconds (including 1 second of LED Blue ON) to put the board in the position.

5. <b>LED Blue blinks 5 times</b>: The Master Mode Measurement is done.

5. <b>LED Red blinks 5 times</b>: The Master Mode Measurement is failed.

# USER Button behaviours

## Single Click

In Master Mode, it triggers a measurement. The result will be sent to LoRaWAN network (TTN) if it's available.

In Slave Mode, it do nothing.

## Double Click

<i>(No more than 400 milliseconds between 2 clicks)</i>

Do nothing.

## Triple Click

<i>(No more than 400 milliseconds between 2 clicks)</i>

Switch between Master Mode & Slave Mode.

# Credits
This project is led by Amnia Benouakta & Prof. Fabien Ferrero at LEAT. Laboratoire d'Electronique, Antennes et Télécommunications. Université Cote d'Azur, CNRS UMR 7248.
