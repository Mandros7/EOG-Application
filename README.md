# TFG

An End of Degree project for a BEng in Telecomunications at [University of Oviedo](http://www.uniovi.es). Design of a C++/Qt application with the purpose of receiving EOG sygnals through Bluetooth or NIDAQmx, and used them to control mouse movement in a simple interface.


Source code
-----------
This project's code is written in English, but commented in Spanish due to the fact that it's initially meant to be shared with Oviedo's University staff. 
Functionalities are divided in several blocks of code, running in different Threads:
- **GUI**. Uses a basic Qt interface arquitecture.
- **Data acquisition block**. Allowed inputs are: Bluetooth, NIDAQmx.
- **Data parsing block**. Initial version separates both eye channels.
- **Data treatment block**. Identifies basic signals like "Blink", "Up", "Down", "Left", "Right", "UpRight"...
  - Noise treatment, convolution system, thresholds are some of the possible treatment systems.
- **Decision block**. Decides over identified signals and moves the cursor.


Authors
-----------
Hector Rodriguez Campo
