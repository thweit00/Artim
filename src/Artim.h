/*
    Arduino Runtime Control and Measurement.
    Copyright (C) 2021 Thomas Wendel
    https://github.com/thweit00/Artim

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef ARTIM_H
#define ARTIM_H

#include <Arduino.h>


// Array size for histogram data.
// Current implementation is fixed to 10.
#define ARTIM_NUM_HIST  (10)


class Artim {
  private:
    
    // ------------- private members -------------
    
    // Loop time of main loop in ms. Artim will suspend execution
    // at the end of the loop and wait till loopTime is expired.
    unsigned int    loopTime;
    
    // Private variable that holds the start time of the loop in ms.
    // Used for runtime calculation.
    unsigned long   loopStartTime;
    
    // Private variable that holds the end time of the loop in ms.
    // Used for runtime calculation.
    unsigned long   loopEndTime;
    
    // Private variable that holds the calculation result (ms) for the
    // wait time that needs to be applied to meet the loopTime.
    // Can be negative if a loop overrun happened (execution took longer
    // than the configured time)
    long            loopWaitTime;
    
    // Private counter that holds the number of load samples (number of executions)
    // Basis for average calculation
    unsigned long   numLoadSamples;
    
    // Load average in % (valid for current power cycle)
    float           loadAvg;

    // Load minimum in % (valid for current power cycle)
    float           loadMin;

    // Load maximum in % (valid for current power cycle)
    float           loadMax;

    // Historgram data array. Will be updated each cycle.
    // Each element represents a load range (0%-9%, 10% - 19%, 20% - 29%, ...)
    // E.g. if current runtime is 21%, the counter at index 2 will be incremented.
    unsigned int    loadHistogram[ARTIM_NUM_HIST];

    // Overload counter. Same input as histogram array but will be incremented
    // each time a loop overrun was detected (negative wait time).
    unsigned int    overloadHistogram;

    // Private flag used to start average calculation
    boolean         firstCall;
    
    // ------------- private operations -------------

    // Private operation to calculate load of the last loop.
    // Called at the beginning of each loop (called by loopBegin).
    void calcStat();

    // Private operation to calculate the wait time.
    // Called at the end of each loop (called by loopEnd).
    void calcWait();

    // Private operation to execute the wait operation (delay)
    // based on the wait time calculated before (called by loopEnd).
    void waitNow();

  
  public:

    // ------------- public operations -------------
    // Constructor.
    // Takes the loop time in ms as input to calculate wait time and load.
    Artim(unsigned int loopTime_ms);
    
    // loopBegin needs to be invoked as very first operation of the loop.
    // It takes the start timestamp for load calculation and calculates
    // the load of the last cycle.
    void loopBegin();

    // loopEnd needs to be invoked as very last operation of the loop.
    // It takes the end time stamp for load calculation and triggers
    // the calculation of the wait time and executes the wait.
    void loopEnd();

    // API function that provides the average load in %.
    float getAvgLoad(void);

    // API function that provides the minimum load in %.
    float getMinLoad(void);

    // API function that provides the maximum load in %.
    float getMaxLoad(void);

    // API function that provides the histogram data for a range
    // provided as input parameter.
    // Valid range: 0... ARTIM_NUM_HIST-1.
    // In case of input out of range, it returns 0.
    unsigned int getLoadHist(byte rangeIdx);

    // API function that provides the overload historgram data.
    // (number of loops that exceeded the configured loop time)
    unsigned int getOverloadHist(void);
};


#endif // ARTIM_H

