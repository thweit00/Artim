# Arduino Runtime Control and Measurement
[![Arduino Lint](https://github.com/thweit00/Artim/actions/workflows/arduino-lint-action.yml/badge.svg)](https://github.com/thweit00/Artim/actions/workflows/arduino-lint-action.yml)
[![Compile Examples](https://github.com/thweit00/Artim/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/thweit00/Artim/actions/workflows/compile-examples.yml)
[![Doxygen Action](https://github.com/thweit00/Artim/actions/workflows/doxygen.yml/badge.svg)](https://github.com/thweit00/Artim/actions/workflows/doxygen.yml)

## Introduction
The Arduino Runtime Contro and Measurement library is a piece of code for the specific use-case of time discrete execution of the main loop e.g. for time triggered operations. Please check the [Use Case](#use-case) section to see if this use case fits your needs.  
In general there are two features supported:
- Timing Control
- Load Measurement

### Timing Control
Artim checks the execution time consumed by the loop function. It compares the time with a configured desired loop cycle time (e.g. 200ms). If the desired loop time is not reached yet, it will wait till the desired loop time is reached. This helps you to have a time discrete behaviour of your loop function.
If the desired loop time is already reached or the loop took longer, there will be no delay.

### Load Measurement
Base on the desired loop time and the actual time needed, the load is calculated. Artim provides several API functions to read the load values.

- **Average Load**  
The average load in %. The load for the last loop is calculated at the beginning of each new loop and the overall average is calculated.

- **Min Load**  
Minimum observed load in % for a loop execution.

- **Max Load**  
Maximum observed load in % for a loop execution. 

- **Load Histogram**  
The load histogram is a nice feature that gives you an overview in which load ranges your application runs.  
For each loop execution, the load is checked and a interval counter is incremented for the given load range.
The range of 100% is diveded in 10% segments (0..9,9%, 10..19,9%, 20..20,9%, ...).
There is also a counter for overload events available, when the acutal loop time exceeds the desired loop time.

## Use Case
- You want to execute your arduino loop-function in a fixed time interval (configurable at startup).
E.g. you are in need of a fixed timing (e.g. 200ms, 1000ms, etc.).  


## API description
The API description can be found in the generated doxygen files:  
[API Documentation](https://thweit00.github.io/Artim/classArtim.html)  
[Doxygen Documentation Start Page](https://thweit00.github.io/Artim/index.html)  

## Installation
To install the library, download the zip-file of the latest version and import it in the Arduino IDE.  
The official Arduino documentation might help you: [Arduino Tutorial](https://www.arduino.cc/en/guide/libraries#toc4)  
After you installed the library, you need to include the Artim.h header file. Have a look at the example code, provided with the library.

## Closing Words
This code is the result of a hobby project. It was never inteded to be perfect. Nevertheless, I hope that it will help one or the other to achieve their goals with a little less effort.  
Improvement proposals or pull requests are welcome.



