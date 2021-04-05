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

///@brief Arduino Runtime Control and Measurement class.
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
    void calcStat(void);

    // Private operation to calculate the wait time.
    // Called at the end of each loop (called by loopEnd).
    void calcWait(void);

    // Private operation to execute the wait operation (delay)
    // based on the wait time calculated before (called by loopEnd).
    void waitNow(void);

  
  public:

    // ------------- public operations -------------
    /// @brief      Constructor.
    /// @param      loopTime_ms Desired loop time in ms as input to calculate wait time and load.
    Artim(unsigned int loopTime_ms);
    
    /// @brief      loopBegin needs to be invoked as very first operation of the loop.
    /// @details    It takes the start timestamp for load calculation and calculates
    ///             the load of the last cycle.
    void loopBegin(void);

    /// @brief      loopEnd needs to be invoked as very last operation of the loop.
    /// @details    It takes the end time stamp for load calculation and triggers
    ///             the calculation of the wait time and executes the wait.
    void loopEnd(void);

    /// @brief      API function that provides the average load in %.
    /// @details    The average load is calculated for the whole power-cycle.
    /// @return     Average load in %.
    float getAvgLoad(void);

    /// @brief      API function that provides the minimum load in %.
    /// @details    The minimum load observed for one loop execution.
    ///             The minimum load is calculated for the whole power cycle.
    /// @return     Minimum load in %.
    float getMinLoad(void);

    /// @brief      API function that provides the maximum load in %.
    /// @details    The maximum load observed for one loop execution.
    ///             The maximum load is calculated for the whole power cycle.
    /// @return     Maximum load in %.
    float getMaxLoad(void);

    /// @brief      API function that provides the histogram data for a range
    ///             provided as input parameter.
    /// @details    The histogram provides the information about the load distribution
    ///             during runtime. Each loop execution is considered.\n
    ///             - Index 0: Number of loop executions with load of 0...9,9%
    ///             - Index 1: Number of loop executions with load of 10...19,9%
    ///             - Index 2: Number of loop executions with load of 20...29,9%
    ///             - Index 3: Number of loop executions with load of 30...39,9%
    ///             - Index 4: Number of loop executions with load of 40...49,9%
    ///             - Index 5: Number of loop executions with load of 50...59,9%
    ///             - Index 6: Number of loop executions with load of 60...69,9%
    ///             - Index 7: Number of loop executions with load of 70...79,9%
    ///             - Index 8: Number of loop executions with load of 80...89,9%
    ///             - Index 9: Number of loop executions with load of 90...99,9%
    /// @param      rangeIdx Histogram index. Valid range: 0...9.
    /// @return     Returns the current histogram data for the given index.
    ///             If the input is out of range, it returns 0.
    unsigned int getLoadHist(byte rangeIdx);

    /// @brief      API function that provides the overload historgram data.
    /// @details    In case the desired loop time is exceeded before loopEnd is called,
    ///             an overload is detected. The overload will be counted in a separate
    ///             histogram variable. The function returns the overload count.
    /// @return     Returns the number of overload events
    ///             (number of loop executions that exceeded the configured loop time).
    unsigned int getOverloadHist(void);
};


#endif // ARTIM_H

