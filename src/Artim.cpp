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

#include "Artim.h"


Artim::Artim(unsigned int loopTime_ms)
{
  // Set the desired loop time in ms.
  this->loopTime = loopTime_ms;
  // Init flags and load variables.
  this->firstCall = true;
  this->numLoadSamples = 0;
  this->loadMin = 100.0f;
  this->loadMax = 0.0f;
  this->overloadHistogram = 0;
}


void Artim::loopBegin()
{
  // Set the start time for the loop by reading the ms counter.
  this->loopStartTime = millis();
  // Calculate statistics for last execution cycle.
  this->calcStat();
}

void Artim::loopEnd()
{
  // Set the loop end time by reading the ms counter.
  this->loopEndTime = millis();
  // Calculate the wait time that is need to reach the
  // desired loop time.
  this->calcWait();
  // Now wait...
  // If the desired loop time is already reached,
  // the function will return immediately.
  this->waitNow();
}


void Artim::calcStat()
{
  float currentLoad;
  byte histogramIdx;

  // On the first execution there are no data available.
  // Therefore we do nothing but set the flag.
  if (this->firstCall)
  {
    this->firstCall = false;
  }
  else
  {
    // calculate load
    currentLoad = 100.0f * (this->loopTime - this->loopWaitTime) / this->loopTime;

    // increment the load samples counter (needed for avg calculation).    
    this->numLoadSamples++;

    // Calculate overall average
    if (this->numLoadSamples < 0xFFFFFFFF)
    {
      this->loadAvg += (currentLoad - this->loadAvg) / this->numLoadSamples;
    }
    else
    {
      // Reset for next cycle to avoid division by zero.
      this->numLoadSamples = 0;
    }
    // Calculate max
    if (currentLoad > this->loadMax)
    {
      this->loadMax = currentLoad;
    }
    // Calculate min
    if (currentLoad < this->loadMin)
    {
      this->loadMin = currentLoad;
    }

    // Create historgram
    // This is done by doing a integer division where the remainder is ignored.
    histogramIdx = (byte)(currentLoad / 10);
    if (histogramIdx <  ARTIM_NUM_HIST)
    {
      this->loadHistogram[histogramIdx]++;
    }
    else
    {
      // If the value is above 9, it means we have a overload situation.
      this->overloadHistogram++;
    }
  }
}


void Artim::calcWait()
{
  // negative wait time means overload.
  this->loopWaitTime = this->loopTime - (this->loopEndTime - this->loopStartTime);
}


void Artim::waitNow()
{
  // Negative value means overflow. We don't wait in that case.
  if (this->loopWaitTime >= 0)
  {
    delay(this->loopWaitTime);
  }
}


float Artim::getAvgLoad(void)
{
  return this->loadAvg;
}


float Artim::getMinLoad(void)
{
  return this->loadMin;
}


float Artim::getMaxLoad(void)
{
  return this->loadMax;
}


unsigned int Artim::getLoadHist(byte rangeIdx)
{
  // Check if range is valid.
  if (rangeIdx < ARTIM_NUM_HIST)
  {
    // Return data element
    return this->loadHistogram[rangeIdx];
  }
  else
  {
    // Return 0 in case of invalid index.
    return 0;
  }
}


unsigned int Artim::getOverloadHist(void)
{
  return this->overloadHistogram;
}
