/*
    Arduino Runtime Control and Measurement. Example file.
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
#include <Artim.h>


// Create an Artim instance.
// Define the desired loop time as 200ms.
Artim myArtimInstance(200);



void setup()
{
  // Init serial interface to print some information
  Serial.begin(115200);

  // Initialize the random number generator.
  // This is needed for the later part of this example.
  randomSeed(1);
}


void loop()
{
  long randomNumber;

  // Place the loopBegin call at the beginning of your loop.
  // It will take a timestamp to calculate the load of the loop execution
  // and it will calculate the load of the last loop.
  myArtimInstance.loopBegin();

  // Place the code within your loop...
  // to give you an example, the code here is just randomly waiting
  // and printing your current load data to the serial interface.

  // print the statistical load data
  Serial.println("\n======================================");
  Serial.println("loadAvg:     " + String(myArtimInstance.getAvgLoad()) + "%");
  Serial.println("loadMin:     " + String(myArtimInstance.getMinLoad()) + "%");
  Serial.println("loadMax:     " + String(myArtimInstance.getMaxLoad()) + "%");
  // The following loop will print the historgram data.
  // The histogram data shows the load peaks. Each cycle, the current load is calculated and
  // the counter of the corresonding range is incremented.
  // E.g. a load of 21% will lead to an increment of index 2 (20%...29%).
  // A load of 59% will lead to an increment of index 5 (50%...59%).
  for (unsigned int i = 0; i < 10; i++)
  {
    Serial.println("loadHist[" + String(i) + "]  " + String(myArtimInstance.getLoadHist(i)));
  }
  // If load exceeds 99,9% it will be reported to the OverloadHistogram.
  Serial.println("Overload     " + String(myArtimInstance.getOverloadHist()));

  // Now simulate some load for this cycle:
  // Generate a random number between 0 and 200 that will be used as wait time.
  // 200ms is also the desired loop time. So due to additional code execution
  // we will be able to generate some overloads that can be seen in the statistical data.
  randomNumber = random(0, 200);

  // delay the execution of the loop.
  delay(randomNumber);


  // Place the loopEnd call at the end of your loop.
  // It will check the actual runtime and wait till your desired loop time
  // you provided at init is reached. If the loop time is already
  // over the desired loop time it will continue immediately.
  myArtimInstance.loopEnd();
}
