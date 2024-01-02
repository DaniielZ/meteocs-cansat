# meteocs-cansat
The flow block diagram of the code can be found here: https://drive.google.com/file/d/1hDMlhlLcdeDGHhbZly0Bg1h5nYsYx1YS/view?usp=sharing
And the final presentation can be viewed here: https://fb.watch/ltn5Kg6NAa/    (3h 38min)

# MainSat
- Is responsible for gathering and logging data and transmitting it over 433 Lora to the base station. Also, the MainSat ejects NanoSat and opens the air brake by firing the MOSFET at the appropriate time after launch.
- Currently, most of the parameters can be changed in the config file, except for some Radio and pre-launch buffer settings which need to be changed in the relevant code section manually
- The MainSat can be controlled either through USB by manually sending commands through a console or remotely with the help of a base station that sends the same commands over 433 Lora
- Simplified operation block diagram
<img src="https://github.com/DaniielZ/meteocs-cansat/assets/34136852/bf4655c7-c464-4397-bf61-0909ba084082" alt="drawing" width="600"/>

# NanoSat
Almost the exact same code base but with some functionality removed and the config changed to fit the pinout
<img src="https://github.com/DaniielZ/meteocs-cansat/assets/34136852/9a19247a-f090-4aa7-8a11-915771378c65" alt="drawing" width="600"/>

# Base station
- 2-way communication with MainSat and NanoSat over 433mhz LoRa modules (telemetry and control commands from the user)
- Ranging (salves) for MainSat and NanoSat so they can gather necessary data to calculate their relative position from the 3 base stations (see diagram below)
<img src="https://github.com/DaniielZ/meteocs-cansat/assets/34136852/5fa058f8-4b54-4b97-844b-170aa3f75e04" alt="drawing" width="600"/>

# SensorTests
There should be a different .cpp file for each of the sensors that are on the CanSat in the MainSat-SensorTests. The NanoSat sensor test folder stopped being used after we switched MCU to the same one as the MainSat.

# Getting data from the MainSat/NanoSat
Quite straightforward:
  If the data is on the Pico flash the Flash dumper cpp file
  Connect to the Pico through USB
  According to the commands specified in the code type the needed sequence in the console 
  Data will be sent over a console so you might need to have a console that can either show a long history of coms or have a console that logs2file. To do that in platform.io add log2file in the monitor_filers tab

# Ballon payload (outdated)
__Code-base has been moved to https://github.com/RTU-HPR/Dev-Payload-Rockoon__
block diagram: https://drive.google.com/file/d/1JWPtqqIjti7fqFCMI8yH3zY4sfTKfGcq/view?usp=sharing




