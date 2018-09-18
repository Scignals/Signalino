# Get Started (Linux)

## Software 

- Download last version of signalino software:

    $ git clone https://github.com/Scignals/Signalino.git

It includes firmware of the shield and the software Scignals visor. This one depends on Processing, a java development environment. 

- Download last version of Processing software from https://processing.org/download/ and install it

 
## Recording your first EKG

1. Attach the shield to the arduino DUE
2. Connect the programming port of the DUE (the usb port nearest to the power connector) to a serial port in a PC, better if it is a notebook running on batteries
3. The PC should identify the signalino as a new device.
3. Now, open Processing. In linux
   `$ sudo ./processing  `
   In windows, click in the icon.
    
4. Open the Scignals visor (sgnlino_visor/sgnlino_visor.pde) and start playing. If everything is okay, you will see a /dev/ACM0 between the list of devices at the start. Connect!

5. Attach EKG electrodes to your fingers, connect to channel 1 in  P and N, and enjoy.


## Bluetooth connection

1. first of all, you have to power up (put it in the socket of the shield) and pair the HC-06 bluetooth, using the UBUNTU bluetooth manager (if you got a preloaded-HC06, PIN number is 6666).

2. You have to find the MAC adress of your HC-06 (mine is 98:D3:32:10:5F:6E)

	$ hcitools scan
	Scanning ...
	98:D3:32:10:5F:6E	sigino_115kbt

3. use that to create the /etc/bluetooth/rfcomm.conf file; you'll note that I chose 0 for the rfcomm device, 
   we need that for connection later:        
   Copy code

    $ cat /etc/bluetooth/rfcomm.conf
    rfcomm0 {
        bind yes;
        device 98:D3:32:10:5F:6E;
        channel    1;
        comment "HC06-signalino Bluetooth Module";
    }

4. Now, create a new serial device connected to the HC-06. From the terminal, and KEEP OPEN the terminal!

	$ sudo rfcomm connect /dev/rfcomm0 98:D3:32:10:5F:6E 1 &
	Connected /dev/rfcomm0 to 98:D3:32:10:5F:6E on channel 1
	Press CTRL-C for hangup

5. Now, open Processing

	$ sudo ./processing
    
6. Open the Scignals visor and start playing. If everything is okay, you will see a /dev/rfcomm0 between the list of devices at the start. Connect!






