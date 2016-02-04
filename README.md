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

Development Enviroment Settings
-----------
In order to get the whole development enviroment ready (over Windows 8), these steps should be followed:
- Install Visual Studio 2013
- Install WDK 8.0
- Install Qt 5.5.1 MSVC201364bits
  -**It's important to follow this installation order, due to several dependencies between the products**
- Verify Auto-detected "Kits" [Desktop Qt 5.5.1 MSVC2013 64bit], "Compilers" [Microsoft Visual C++ Compiler 12.0 (amd64--> depends on your PC)] and "Debbugers" [CDB at C:\Program Files (x86)\Windows Kits\8.0\Debuggers\x86\cdb.exe]
  - **If it fails, write by hand all empty fields. Look for help on the Internet if needed**
- Go to: Tools>>Options>>Debbuger>>CDB Paths and set path to installed CDB (C:\Program Files (x86)\Windows Kits\8.0\Debuggers\x64)

License
-------
This project is under MIT license as described in the [LICENSE](https://github.com/Mandros7/EOG-Application/blob/master/LICENSE) file.

Authors
-----------
Hector Rodriguez Campo
