# meteocs-cansat
The flow block diagram of the code can be found here: https://drive.google.com/file/d/1hDMlhlLcdeDGHhbZly0Bg1h5nYsYx1YS/view?usp=sharing
And the final presentation can be viewed here: https://fb.watch/ltn5Kg6NAa/    (3h 38min)

# MainSat
Is responsible for gathering and logging data and transmitting it over 433 Lora to the base station. Also, the MainSat ejects NanoSat and opens the air brake by firing the MOSFET at the appropriate time after launch.

Currently, most of the parameters can be changed in the config file, except for some Radio and pre-launch buffer settings which need to be changed in the relevant code section manually

The MainSat can be controlled either through USB by manually sending commands through a console or remotely with the help of a base station that sends the same commands over 433 Lora

# NanoSat
Almost the exact same code base but with some functionality removed and the config changed to fit the pinout

# SensorTests
There should be a different .cpp file for each of the sensors that are on the CanSat in the MainSat-SensorTests. The NanoSat sensor test folder stopped being used after we switched MCU to the same one as the MainSat.

# Getting data from the MainSat/NanoSat
Quite straightforward:
  If the data is on the Pico flash the Flash dumper cpp file
  Connect to the Pico through USB
  According to the commands specified in the code type the needed sequence in the console 
  Data will be sent over a console so you might need to have a console that can either show a long history of coms or have a console that logs2file. To do that in platform.io add log2file in the monitor_filers tab

# Ballon payload
block diagram: https://drive.google.com/file/d/1JWPtqqIjti7fqFCMI8yH3zY4sfTKfGcq/view?usp=sharing



