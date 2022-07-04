# CurveTracer  
A digital curve tracer using Arduino, with source code and binaries for Windows and Linux  
  
__<h2>This repository is currently under construction</h2>__  
11-jan-2022  
  
Used tools:  
&nbsp;&nbsp;&nbsp;&nbsp; Borland C++Builder 6.0  (Windows)  
&nbsp;&nbsp;&nbsp;&nbsp; Code::Blocks 20.03  (Linux)  
&nbsp;&nbsp;&nbsp;&nbsp; Arduino IDE 1.8.11  
&nbsp;&nbsp;&nbsp;&nbsp; LTSpice 17.0.32  
&nbsp;&nbsp;&nbsp;&nbsp; Eagle 7.7  
  
  
15-jan-2022  
  
A working prototype (Mk1):  
<img src="https://github.com/HenniePeters/CurveTracer/blob/main/Mk1/prototype.jpeg?raw=true"/>  
  
<img src="https://github.com/HenniePeters/CurveTracer/blob/main/Mk1/screenshot.png?raw=true"/>  
  
  
4-jul-2022  
  
I am working on the STM32 version. The PC software and source for Linux are available in the MK2 directories.  
I've changed the number of samples from 48 to 50. This rounds to a better number for the delay in microseconds.
I'am currently trying to find a way to publish the source for STM32Cube. The project is 37Mb in size. Not a good idea to post that on github entirely. Any ideas are welcome.  

The circuit connects to a clone of a nucleo board. Pin numbers are available in the schematic.

<img src="https://github.com/HenniePeters/CurveTracer/blob/main/MK2/STM32F072RBT6.png?raw=true"/>  
  
<img src="https://github.com/HenniePeters/CurveTracer/blob/main/MK2/Schematic-MK2.png?raw=true"/>  
