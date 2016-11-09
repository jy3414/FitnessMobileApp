#include <BMI160.h>

#include <Kalman.h>
#include <SerialFlash.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <SPI.h>
#include <CurieIMU.h>
#include "CuriePME.h"
#include <CurieBLE.h>
#include <math.h>
/*
   Copyright (c) 2015 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

/*
   This sketch example demonstrates how the BMI160 accelerometer on the
   Intel(R) Curie(TM) module can be used as a Step Counter (pedometer)
*/

/* To get an interrupt notification for every step detected,
    set stepEventsEnabeled to true. Otherwise, the main loop will
    poll for the current step count.

   By design, the step counter does not immediately update on every step detected.
   Please refer to Section 2.7 of the BMI160 IMU SensorData Sheet
   for more information on this feature.
*/
char *correctData = "-43.4 21.3\n"
                    "-43.3 21.2\n"
                    "-43.7 21.1\n"
                    "-43.9 21.2\n"
                    "-43.3 21.2\n"
                    "-40.7 20.2\n"
                    "-38.3 20.4\n"
                    "-47.7 18.7\n"
                    "-56.9 23.7\n"
                    "-62.2 20.1\n"
                    "-63.5 24.9\n"
                    "-67.9 21.6\n"
                    "-62.8 18.7\n"
                    "-56.7 17.3\n"
                    "-65.1 19.7\n"
                    "-70.5 21.6\n"
                    "-69.7 24.4\n"
                    "-56.7 25.2\n"
                    "-54.3 23.1\n"
                    "-43.8 18.4\n"
                    "-38.0 18.6\n"
                    "-45.4 19.3\n"
                    "-46.2 20.3\n"
                    "-46.6 20.4\n"
                    "-43.0 19.4\n"
                    "-46.5 18.4\n"
                    "-45.7 20.3\n"
                    "-56.2 22.0\n"
                    "-58.1 23.8\n"
                    "-59.8 23.0\n"
                    "-59.2 24.1\n"
                    "-58.2 20.9\n"
                    "-59.0 22.0\n"
                    "-69.7 21.2\n"
                    "-68.1 25.3\n"
                    "-62.2 24.0\n"
                    "-53.4 20.3\n"
                    "-43.9 15.3\n"
                    "-37.4 19.0\n"
                    "-36.6 19.3\n"
                    "-36.0 19.9\n"
                    "-37.4 19.6\n"
                    "-36.7 19.5\n"
                    "-37.2 19.7\n"
                    "-37.5 20.0\n"
                    "-36.8 19.6\n"
                    "-33.9 19.2\n"
                    "-35.8 19.5\n"
                    "-32.0 18.4\n"
                    "-39.0 18.6\n"
                    "-51.0 20.8\n"
                    "-57.4 26.6\n"
                    "-69.3 27.0\n"
                    "-76.3 27.5\n"
                    "-71.5 25.4\n"
                    "-70.5 20.1\n"
                    "-66.8 20.7\n"
                    "-62.1 23.7\n"
                    "-67.6 25.4\n"
                    "-61.7 24.1\n"
                    "-53.9 21.1\n"
                    "-56.2 25.1\n"
                    "-48.4 20.2\n"
                    "-35.6 15.9\n"
                    "-30.9 17.4\n"
                    "-34.2 16.7\n"
                    "-35.9 18.5\n"
                    "-37.6 19.3\n"
                    "-38.0 19.5\n"
                    "-41.0 19.7\n"
                    "-42.6 18.9\n"
                    "-37.5 19.4\n"
                    "-47.0 17.0\n"
                    "-49.1 22.0\n"
                    "-61.3 20.9\n"
                    "-67.2 20.9\n"
                    "-64.8 19.3\n"
                    "-58.2 17.9\n"
                    "-63.7 20.2\n"
                    "-71.1 21.4\n"
                    "-67.2 25.2\n"
                    "-59.2 24.1\n"
                    "-42.6 20.5\n"
                    "-26.9 14.5\n"
                    "-34.7 13.2\n"
                    "-39.0 17.7\n"
                    "-35.7 15.2\n"
                    "-38.3 16.6\n"
                    "-35.9 17.8\n"
                    "-42.1 20.3\n"
                    "-44.3 20.8\n"
                    "-46.8 21.8\n"
                    "-43.1 20.5\n"
                    "-40.4 20.3\n"
                    "-38.9 19.1\n"
                    "-37.3 17.0\n"
                    "-53.5 21.2\n"
                    "-64.4 19.1\n"
                    "-67.8 20.2\n"
                    "-67.5 21.6\n"
                    "-59.8 20.9\n"
                    "-67.9 22.5\n"
                    "-74.2 21.5\n"
                    "-68.6 24.1\n"
                    "-61.0 21.5\n"
                    "-49.2 26.4\n"
                    "-20.6 7.8\n"
                    "-30.1 16.3\n"
                    "-43.8 13.8\n"
                    "-37.3 18.9\n"
                    "-36.7 19.0\n"
                    "-36.8 19.1\n"
                    "-36.5 19.1\n"
                    "-37.7 19.1\n"
                    "-36.8 19.5\n"
                    "-36.9 19.6\n"
                    "-38.5 20.0\n"
                    "-48.8 22.6\n"
                    "-59.4 21.3\n"
                    "-65.3 20.9\n"
                    "-61.9 23.3\n"
                    "-63.3 23.4\n"
                    "-59.2 20.9\n"
                    "-56.2 20.3\n"
                    "-69.5 22.8\n"
                    "-68.8 26.7\n"
                    "-64.8 28.3\n"
                    "-60.2 26.7\n"
                    "-40.8 24.2\n"
                    "-38.7 22.1\n"
                    "-37.3 18.8\n"
                    "-36.3 19.8\n"
                    "-35.5 20.4\n"
                    "-35.3 20.7\n"
                    "-36.6 19.6\n"
                    "-36.5 19.7\n"
                    "-37.5 20.0\n"
                    "-37.8 19.9\n"
                    "-38.9 20.4\n"
                    "-36.8 20.1\n"
                    "-36.6 22.0\n"
                    "-48.9 18.1\n"
                    "-51.6 23.7\n"
                    "-63.4 27.6\n"
                    "-63.5 18.0\n"
                    "-64.2 21.2\n"
                    "-61.7 19.7\n"
                    "-54.6 20.2\n"
                    "-66.3 20.8\n"
                    "-69.5 20.8\n"
                    "-56.4 28.4\n"
                    "-45.3 27.1\n"
                    "-31.1 20.0\n"
                    "-33.3 17.2\n"
                    "-36.5 19.0\n"
                    "-34.7 18.0\n"
                    "-34.4 19.4\n"
                    "-35.9 18.2\n"
                    "-36.1 18.4\n"
                    "-34.6 19.4\n"
                    "-35.2 18.8\n"
                    "-34.2 18.8\n"
                    "-34.9 19.4\n"
                    "-34.3 20.0\n"
                    "-52.2 21.5\n"
                    "-48.6 20.2\n"
                    "-47.3 19.4\n"
                    "-48.0 21.3\n"
                    "-45.7 20.9\n"
                    "-42.8 21.2\n"
                    "-42.0 22.5\n"
                    "-61.3 21.2\n"
                    "-64.1 22.8\n"
                    "-66.0 25.8\n"
                    "-64.5 18.2\n"
                    "-57.6 21.8\n"
                    "-73.5 23.5\n"
                    "-75.4 22.7\n"
                    "-63.9 23.3\n"
                    "-50.8 25.1\n"
                    "-22.0 14.1\n"
                    "-28.8 15.8\n"
                    "-33.3 16.9\n"
                    "-35.7 17.9\n"
                    "-34.6 19.8\n"
                    "-36.0 19.8\n"
                    "-36.5 20.0\n"
                    "-36.5 19.7\n"
                    "-35.2 19.4\n"
                    "-31.2 19.1\n"
                    "-32.1 19.2\n"
                    "-49.1 16.4\n"
                    "-51.8 25.7\n"
                    "-60.7 22.9\n"
                    "-63.0 23.1\n"
                    "-65.8 23.3\n"
                    "-62.8 20.6\n"
                    "-64.3 21.7\n"
                    "-57.0 21.0\n"
                    "-64.3 23.1\n"
                    "-71.7 21.6\n"
                    "-70.4 26.2\n"
                    "-50.8 26.7\n"
                    "-28.2 18.4\n"
                    "-32.9 16.9\n"
                    "-35.8 21.1\n"
                    "-32.8 20.8\n"
                    "-37.0 19.8\n"
                    "-36.0 19.7\n"
                    "-37.0 20.3\n"
                    "-36.9 20.0\n"
                    "-37.4 20.4\n"
                    "-34.1 19.6\n"
                    "-49.5 21.4\n"
                    "-44.1 19.2\n"
                    "-59.4 22.9\n"
                    "-64.9 22.5\n"
                    "-64.9 21.2\n"
                    "-63.9 21.2\n"
                    "-57.2 17.7\n"
                    "-60.3 20.3\n"
                    "-75.3 17.4\n"
                    "-72.6 28.8\n"
                    "-63.6 26.2\n"
                    "-48.6 25.7\n"
                    "-30.5 11.1\n"
                    "-33.0 11.2\n"
                    "-42.7 15.5\n"
                    "-36.2 16.2\n"
                    "-40.5 16.2\n"
                    "-38.3 17.7\n"
                    "-38.0 17.6\n"
                    "-53.1 7.9\n"
                    "-51.2 8.5\n"
                    "-51.0 8.4\n"
                    "-50.9 8.3\n"
                    "-50.6 8.3\n"
                    "-51.0 7.9\n"
                    "-50.8 8.2\n"
                    "-49.4 9.5\n"
                    "-37.0 -14.1\n"
                    "-70.4 0.8\n"
                    "-72.0 9.2\n"
                    "-71.9 5.8\n"
                    "-68.1 4.4\n"
                    "-64.8 4.5\n"
                    "-67.3 5.7\n"
                    "-72.0 3.0\n"
                    "-72.2 0.5\n"
                    "-65.3 3.6\n"
                    "-47.4 0.2\n"
                    "-55.6 10.6\n"
                    "-51.2 8.8\n"
                    "-53.4 9.2\n"
                    "-52.5 9.2\n"
                    "-53.2 9.2\n"
                    "-52.9 9.2\n"
                    "-51.0 9.3\n"
                    "-47.9 9.6\n"
                    "-33.6 12.5\n"
                    "-51.1 8.1\n"
                    "-77.7 7.3\n"
                    "-67.2 7.5\n"
                    "-65.1 9.3\n"
                    "-65.4 6.1\n"
                    "-77.3 5.8\n"
                    "-80.6 7.2\n"
                    "-74.4 17.4\n"
                    "-92.8 11.6\n"
                    "-67.7 4.0\n"
                    "-61.5 6.8\n"
                    "-51.7 8.8\n"
                    "-52.3 8.1\n"
                    "-51.7 8.1\n"
                    "-51.3 8.3\n"
                    "-50.9 8.3\n"
                    "-51.1 8.8\n"
                    "-38.4 10.0\n"
                    "-55.1 5.7\n"
                    "-69.3 4.8\n"
                    "-75.5 11.9\n"
                    "-75.8 8.6\n"
                    "-65.1 8.1\n"
                    "-74.6 6.8\n"
                    "-75.9 6.1\n"
                    "-71.0 2.5\n"
                    "-38.9 6.0\n"
                    "-44.9 10.9\n"
                    "-51.5 10.1\n"
                    "-49.8 9.9\n"
                    "-50.3 9.9\n"
                    "-49.0 10.2\n"
                    "-49.7 10.0\n"
                    "-49.7 10.3\n"
                    "-47.4 9.7\n"
                    "-48.0 10.0\n"
                    "-42.8 5.3\n"
                    "-57.1 8.7\n"
                    "-75.8 2.3\n"
                    "-65.5 7.6\n"
                    "-69.8 5.9\n"
                    "-52.5 4.2\n"
                    "-68.8 6.5\n"
                    "-66.9 3.7\n"
                    "-65.1 6.9\n"
                    "-60.1 7.2\n"
                    "-62.0 8.1\n"
                    "-49.8 1.3\n"
                    "-54.3 7.8\n"
                    "-52.4 10.6\n"
                    "-52.2 10.8\n"
                    "-51.9 10.6\n"
                    "-51.9 10.8\n"
                    "-51.9 10.8\n"
                    "-48.8 11.7\n"
                    "-47.9 0.9\n"
                    "-66.4 7.5\n"
                    "-67.1 4.5\n"
                    "-64.0 5.8\n"
                    "-69.2 2.4\n"
                    "-62.6 6.7\n"
                    "-65.6 6.4\n"
                    "-61.3 4.8\n"
                    "-58.7 6.7\n"
                    "-56.8 4.2\n"
                    "-58.0 8.8\n"
                    "-48.7 12.1\n"
                    "-53.1 10.8\n"
                    "-54.8 12.2\n"
                    "-50.3 10.7\n"
                    "-50.1 11.0\n"
                    "-50.8 10.4\n"
                    "-49.9 10.9\n"
                    "-46.8 11.9\n"
                    "-46.4 1.7\n"
                    "-72.2 0.8\n"
                    "-77.7 4.1\n"
                    "-71.5 4.7\n"
                    "-67.9 5.4\n"
                    "-64.5 2.5\n"
                    "-73.8 4.6\n"
                    "-78.9 3.7\n"
                    "-64.5 7.7\n"
                    "-62.0 7.7\n"
                    "-61.3 7.2\n"
                    "-56.2 8.5\n"
                    "-52.7 12.4\n"
                    "-54.1 10.4\n"
                    "-53.1 11.0\n"
                    "-54.0 11.3\n"
                    "-54.1 11.5\n"
                    "-53.2 11.3\n"
                    "-53.1 11.2\n"
                    "-51.1 11.0\n"
                    "-45.7 3.2\n"
                    "-55.0 10.0\n"
                    "-68.5 1.7\n"
                    "-67.1 8.9\n"
                    "-74.7 6.6\n"
                    "-72.8 7.5\n"
                    "-69.3 4.3\n"
                    "-53.1 8.0\n"
                    "-63.6 7.0\n"
                    "-57.8 8.2\n"
                    "-51.4 9.8\n"
                    "-50.2 9.8\n"
                    "-50.3 9.8\n"
                    "-50.8 10.4\n"
                    "-51.0 9.8\n"
                    "-45.9 10.8\n"
                    "-45.0 7.3\n"
                    "-43.9 11.4\n"
                    "-74.0 0.2\n"
                    "-71.5 8.6\n"
                    "-65.9 11.6\n"
                    "-73.4 9.4\n"
                    "-67.8 11.7\n"
                    "-72.8 8.0\n"
                    "-59.4 12.1\n"
                    "-67.8 5.6\n"
                    "-42.2 4.5\n"
                    "-54.4 5.7\n"
                    "-55.3 12.2\n"
                    "-55.0 10.9\n"
                    "-54.9 11.5\n"
                    "-52.1 10.2\n"
                    "-50.1 10.1\n"
                    "-48.9 10.1\n"
                    "-48.5 10.5\n"
                    "-46.8 11.2\n"
                    "-38.3 2.8\n"
                    "-74.6 6.6\n"
                    "-77.6 6.9\n"
                    "-64.4 8.1\n"
                    "-62.4 5.6\n"
                    "-72.3 4.4\n"
                    "-81.6 3.0\n"
                    "-66.5 18.4\n"
                    "-77.6 -7.9\n"
                    "-61.4 5.7\n"
                    "-62.0 7.6\n"
                    "-49.1 11.5\n"
                    "-49.3 11.6\n"
                    "-49.3 10.8\n"
                    "-50.6 11.0\n"
                    "-48.6 11.3\n"
                    "-41.3 12.0\n"
                    "-36.9 6.0\n"
                    "-81.8 3.7\n"
                    "-74.0 6.6\n"
                    "-75.0 6.4\n"
                    "-79.4 6.8\n"
                    "-82.9 2.9\n"
                    "-73.7 7.4\n"
                    "-64.9 5.9\n"
                    "-44.7 7.3\n";
char *wrongData = "-39.8 20.1\n"
                  "-39.1 19.8\n"
                  "-40.1 19.8\n"
                  "-38.6 19.6\n"
                  "-30.4 19.0\n"
                  "-33.9 16.5\n"
                  "-67.6 22.8\n"
                  "-90.1 21.1\n"
                  "-96.0 22.0\n"
                  "-97.9 20.8\n"
                  "-103.0 18.1\n"
                  "-102.0 19.0\n"
                  "-113.7 28.5\n"
                  "-110.7 18.3\n"
                  "-109.0 19.7\n"
                  "-108.3 28.0\n"
                  "-110.5 20.5\n"
                  "-104.7 21.9\n"
                  "-100.1 20.6\n"
                  "-89.0 27.4\n"
                  "-54.2 26.6\n"
                  "-37.0 20.1\n"
                  "-37.7 20.1\n"
                  "-37.4 20.6\n"
                  "-37.6 20.4\n"
                  "-36.4 19.6\n"
                  "-33.4 19.5\n"
                  "-32.2 18.9\n"
                  "-31.3 21.9\n"
                  "-61.6 20.7\n"
                  "-79.8 20.2\n"
                  "-93.5 18.7\n"
                  "-98.7 22.9\n"
                  "-97.4 21.2\n"
                  "-96.4 22.0\n"
                  "-101.0 28.2\n"
                  "-106.3 25.0\n"
                  "-106.9 29.9\n"
                  "-106.2 24.4\n"
                  "-96.9 22.2\n"
                  "-90.0 16.2\n"
                  "-47.5 12.5\n"
                  "-38.9 16.9\n"
                  "-35.7 16.8\n"
                  "-37.7 20.0\n"
                  "-37.4 19.0\n"
                  "-38.1 20.0\n"
                  "-36.7 19.7\n"
                  "-35.6 20.0\n"
                  "-34.9 18.4\n"
                  "-34.0 18.5\n"
                  "-35.8 20.9\n"
                  "-58.4 13.6\n"
                  "-90.9 13.6\n"
                  "-95.5 17.2\n"
                  "-90.8 17.3\n"
                  "-97.5 20.1\n"
                  "-98.2 20.3\n"
                  "-97.8 25.2\n"
                  "-100.9 21.0\n"
                  "-104.6 19.4\n"
                  "-96.1 17.9\n"
                  "-90.5 21.4\n"
                  "-37.7 15.1\n"
                  "-31.9 18.8\n"
                  "-37.1 19.5\n"
                  "-38.1 20.4\n"
                  "-38.1 20.0\n"
                  "-37.6 19.2\n"
                  "-32.7 19.2\n"
                  "-50.6 19.7\n"
                  "-80.4 20.2\n"
                  "-89.6 19.8\n"
                  "-100.0 18.2\n"
                  "-97.6 15.7\n"
                  "-96.5 19.4\n"
                  "-101.4 17.8\n"
                  "-97.2 17.6\n"
                  "-97.7 23.1\n"
                  "-100.2 22.3\n"
                  "-103.3 20.9\n"
                  "-97.6 17.3\n"
                  "-90.9 17.4\n"
                  "-81.7 20.0\n"
                  "-45.1 18.6\n"
                  "-49.2 20.6\n"
                  "-39.1 19.9\n"
                  "-51.0 21.5\n"
                  "-51.9 22.2\n"
                  "-50.9 21.1\n"
                  "-47.5 21.3\n"
                  "-41.8 21.9\n"
                  "-51.5 17.3\n"
                  "-71.6 20.1\n"
                  "-82.4 19.7\n"
                  "-86.3 16.9\n"
                  "-88.4 18.0\n"
                  "-91.8 18.4\n"
                  "-104.3 22.9\n"
                  "-99.8 15.8\n"
                  "-112.7 13.9\n"
                  "-115.4 14.0\n"
                  "-99.2 19.9\n"
                  "-91.6 14.0\n"
                  "-75.0 13.9\n"
                  "-37.2 14.0\n"
                  "-37.9 18.9\n"
                  "-38.9 19.3\n"
                  "-36.1 20.3\n"
                  "-36.9 20.2\n"
                  "-38.6 20.1\n"
                  "-38.3 20.5\n"
                  "-35.2 19.9\n"
                  "-31.5 19.6\n"
                  "-34.7 19.7\n"
                  "-33.9 18.6\n"
                  "-68.4 21.4\n"
                  "-74.2 17.2\n"
                  "-92.0 20.1\n"
                  "-94.1 20.8\n"
                  "-97.5 16.5\n"
                  "-96.8 21.5\n"
                  "-104.1 19.5\n"
                  "-99.9 22.9\n"
                  "-102.9 23.3\n"
                  "-101.2 23.4\n"
                  "-107.4 21.0\n"
                  "-107.7 19.5\n"
                  "-98.6 21.8\n"
                  "-81.3 19.4\n"
                  "-56.1 21.0\n"
                  "-44.6 22.0\n"
                  "-36.1 21.1\n"
                  "-36.2 21.5\n"
                  "-35.9 21.6\n"
                  "-35.4 21.7\n"
                  "-31.8 21.3\n"
                  "-30.1 20.7\n"
                  "-32.3 20.5\n"
                  "-31.5 20.8\n"
                  "-60.0 12.4\n"
                  "-90.1 20.8\n"
                  "-87.3 18.5\n"
                  "-95.9 25.3\n"
                  "-97.9 25.8\n"
                  "-101.7 20.8\n"
                  "-104.0 26.2\n"
                  "-103.1 22.3\n"
                  "-102.7 21.9\n"
                  "-100.8 20.4\n"
                  "-98.3 25.6\n"
                  "-87.8 26.0\n"
                  "-74.1 26.2\n"
                  "-66.4 29.8\n"
                  "-45.7 17.4\n"
                  "-41.5 18.8\n"
                  "-43.4 17.3\n"
                  "-35.4 17.5\n"
                  "-36.8 21.3\n"
                  "-36.4 21.3\n"
                  "-36.1 21.5\n"
                  "-36.0 21.5\n"
                  "-32.1 21.0\n"
                  "-31.3 21.0\n"
                  "-29.8 19.9\n"
                  "-31.3 21.4\n"
                  "-69.9 24.2\n"
                  "-85.5 22.4\n"
                  "-96.5 30.5\n"
                  "-98.9 25.2\n"
                  "-100.1 25.4\n"
                  "-106.3 23.8\n"
                  "-95.5 22.1\n"
                  "-99.7 19.2\n"
                  "-101.5 20.2\n"
                  "-101.4 21.4\n"
                  "-101.8 18.9\n"
                  "-91.0 16.5\n"
                  "-77.8 18.4\n"
                  "-40.2 12.8\n"
                  "-38.2 17.4\n"
                  "-38.3 21.9\n"
                  "-33.7 20.6\n"
                  "-33.6 20.3\n"
                  "-32.9 19.8\n"
                  "-33.9 20.4\n"
                  "-34.7 20.5\n"
                  "-32.8 20.6\n"
                  "-30.2 20.3\n"
                  "-32.2 19.8\n"
                  "-55.1 19.2\n"
                  "-74.5 18.9\n"
                  "-83.3 23.1\n"
                  "-94.3 26.3\n"
                  "-95.2 29.5\n"
                  "-93.4 28.6\n"
                  "-97.4 24.2\n"
                  "-101.2 23.1\n"
                  "-104.7 22.5\n"
                  "-97.6 18.5\n"
                  "-107.8 18.5\n"
                  "-99.1 25.6\n"
                  "-76.9 20.9\n"
                  "-45.9 3.2\n"
                  "-29.3 14.7\n"
                  "-21.7 13.9\n"
                  "-29.7 17.2\n"
                  "-26.7 17.9\n"
                  "-32.1 20.1\n"
                  "-33.8 20.2\n"
                  "-33.6 20.2\n"
                  "-33.4 20.3\n"
                  "-33.3 20.4\n"
                  "-30.1 19.9\n"
                  "-30.2 19.4\n"
                  "-46.3 18.9\n"
                  "-62.6 19.1\n"
                  "-77.4 14.5\n"
                  "-88.3 18.0\n"
                  "-97.6 20.9\n"
                  "-103.9 24.1\n"
                  "-104.5 17.0\n"
                  "-119.2 28.0\n"
                  "-108.0 13.2\n"
                  "-116.5 23.0\n"
                  "-117.9 30.6\n"
                  "-114.9 23.9\n"
                  "-111.8 26.5\n"
                  "-103.5 27.8\n"
                  "-95.3 24.2\n"
                  "-85.0 19.8\n"
                  "-75.7 21.8\n"
                  "-54.0 17.7\n"
                  "-40.4 15.9\n"
                  "-52.5 10.8\n"
                  "-50.4 11.5\n"
                  "-50.0 10.8\n"
                  "-51.5 11.1\n"
                  "-50.2 10.8\n"
                  "-50.1 10.3\n"
                  "-44.4 11.0\n"
                  "-41.0 8.2\n"
                  "-124.2 -3.6\n"
                  "-109.7 2.2\n"
                  "-121.8 -2.9\n"
                  "-116.4 -6.7\n"
                  "-114.6 -7.5\n"
                  "-122.5 -5.6\n"
                  "-117.9 -5.3\n"
                  "-100.2 -3.1\n"
                  "-106.8 0.0\n"
                  "-101.1 0.2\n"
                  "-85.2 1.9\n"
                  "-49.4 5.2\n"
                  "-50.7 11.2\n"
                  "-45.4 8.4\n"
                  "-53.4 9.9\n"
                  "-52.7 10.2\n"
                  "-53.5 10.3\n"
                  "-53.4 10.7\n"
                  "-53.8 10.9\n"
                  "-52.8 10.6\n"
                  "-51.6 10.3\n"
                  "-51.7 5.9\n"
                  "-91.7 8.4\n"
                  "-108.2 -1.4\n"
                  "-115.0 0.5\n"
                  "-120.8 -6.2\n"
                  "-117.8 -4.3\n"
                  "-118.7 -7.0\n"
                  "-124.7 -6.2\n"
                  "-129.8 -13.1\n"
                  "-114.7 -6.4\n"
                  "-111.2 -6.4\n"
                  "-85.2 0.0\n"
                  "-66.2 5.8\n"
                  "-50.2 10.9\n"
                  "-49.6 10.8\n"
                  "-49.4 10.6\n"
                  "-49.4 10.8\n"
                  "-49.0 10.9\n"
                  "-49.2 11.3\n"
                  "-50.5 11.5\n"
                  "-45.4 11.7\n"
                  "-45.1 10.8\n"
                  "-46.5 10.7\n"
                  "-40.9 8.3\n"
                  "-89.7 -3.9\n"
                  "-109.1 -3.1\n"
                  "-119.7 -2.9\n"
                  "-120.9 -3.6\n"
                  "-124.3 -4.2\n"
                  "-126.6 0.5\n"
                  "-133.3 -3.8\n"
                  "-132.5 -6.8\n"
                  "-118.1 -2.2\n"
                  "-104.0 -2.7\n"
                  "-87.3 6.9\n"
                  "-45.9 4.7\n"
                  "-47.7 4.9\n"
                  "-48.7 5.5\n"
                  "-46.8 9.7\n"
                  "-50.0 10.5\n"
                  "-49.6 10.4\n"
                  "-49.8 10.6\n"
                  "-49.4 10.8\n"
                  "-48.6 10.9\n"
                  "-48.8 11.2\n"
                  "-50.1 11.1\n"
                  "-63.4 1.8\n"
                  "-106.9 0.7\n"
                  "-101.8 -5.3\n"
                  "-114.8 -8.7\n"
                  "-111.0 -6.4\n"
                  "-119.6 -3.0\n"
                  "-122.1 -4.2\n"
                  "-121.8 -3.0\n"
                  "-109.6 2.6\n"
                  "-95.0 0.5\n"
                  "-66.2 4.2\n"
                  "-44.1 5.5\n"
                  "-50.8 6.0\n"
                  "-51.5 10.5\n"
                  "-50.1 13.0\n"
                  "-50.5 13.4\n"
                  "-49.5 12.8\n"
                  "-47.0 12.8\n"
                  "-22.1 11.7\n"
                  "-113.7 0.8\n"
                  "-113.3 0.3\n"
                  "-113.2 -6.9\n"
                  "-122.2 -3.8\n"
                  "-119.9 0.9\n"
                  "-118.2 -2.7\n"
                  "-114.6 0.9\n"
                  "-107.3 -3.9\n"
                  "-72.8 4.8\n"
                  "-45.7 9.4\n"
                  "-45.2 6.0\n"
                  "-44.7 8.0\n"
                  "-52.5 10.2\n"
                  "-51.5 9.8\n"
                  "-50.6 9.6\n"
                  "-51.3 9.7\n"
                  "-50.8 9.4\n"
                  "-50.3 9.3\n"
                  "-49.3 9.2\n"
                  "-47.4 9.5\n"
                  "-46.0 9.8\n"
                  "-36.9 1.8\n"
                  "-118.6 -7.4\n"
                  "-117.7 -2.6\n"
                  "-120.8 0.8\n"
                  "-122.1 2.1\n"
                  "-125.1 -1.2\n"
                  "-119.6 1.9\n"
                  "-103.7 2.8\n"
                  "-105.9 2.4\n"
                  "-80.8 1.3\n"
                  "-51.2 7.3\n"
                  "-48.5 5.9\n"
                  "-46.3 7.2\n"
                  "-45.2 8.2\n"
                  "-49.0 7.9\n"
                  "-50.4 8.3\n"
                  "-49.7 9.1\n"
                  "-50.0 8.4\n"
                  "-49.0 9.1\n"
                  "-48.6 9.3\n"
                  "-48.9 9.2\n"
                  "-48.5 9.0\n"
                  "-48.1 8.9\n"
                  "-44.3 9.1\n"
                  "-30.0 5.1\n"
                  "-112.8 -8.8\n"
                  "-111.1 1.8\n"
                  "-124.0 -2.1\n"
                  "-123.2 3.6\n"
                  "-126.8 2.3\n"
                  "-123.5 0.7\n"
                  "-109.7 3.0\n"
                  "-103.1 0.9\n"
                  "-56.2 4.8\n"
                  "-48.6 8.6\n"
                  "-48.6 8.6\n"
                  "-48.6 8.5\n"
                  "-48.4 8.9\n"
                  "-48.8 8.7\n"
                  "-48.7 8.9\n"
                  "-47.5 8.6\n"
                  "-43.9 8.8\n"
                  "-47.3 7.7\n"
                  "-107.8 -1.7\n"
                  "-116.6 1.0\n"
                  "-117.2 0.5\n"
                  "-122.3 -2.2\n"
                  "-125.3 -1.7\n"
                  "-128.4 -1.0\n"
                  "-127.4 -4.4\n"
                  "-124.8 -2.9\n"
                  "-117.9 -6.4\n"
                  "-58.7 12.6\n"
                  "-58.0 13.2\n"
                  "-56.5 12.4\n"
                  "-55.7 12.3\n"
                  "-52.7 10.5\n"
                  "-54.1 8.6\n"
                  "-50.7 7.5\n"
                  "-50.5 6.4\n"
                  "-47.4 6.7\n"
                  "-61.4 7.2\n"
                  "-110.8 1.8\n"
                  "-120.3 -1.4\n"
                  "-121.1 -3.0\n"
                  "-122.8 -2.8\n"
                  "-120.5 0.9\n"
                  "-121.7 -5.0\n"
                  "-109.3 -1.9\n"
                  "-104.3 1.4\n"
                  "-92.3 2.3\n"
                  "-49.3 8.4\n"
                  "-50.1 8.5\n"
                  "-49.7 8.4\n"
                  "-49.2 7.6\n"
                  "-50.1 7.8\n"
                  "-50.2 8.1\n"
                  "-44.3 9.2\n"
                  "-25.2 13.2\n"
                  "-94.0 0.8\n"
                  "-112.1 -3.3\n"
                  "-124.0 1.1\n"
                  "-125.8 1.0\n"
                  "-128.1 2.6\n"
                  "-124.7 4.8\n"
                  "-124.1 4.7\n"
                  "-120.6 2.9\n"
                  "-112.4 7.4\n"
                  "-87.6 8.3\n"
                  "-64.1 5.0\n";

const int FlashChipSelect = 21; // digital pin for flash chip CS pin
const float HOUR = 3600;
const float KM = 1000;
const int ledPin = 13;
const int HEIGHT = 180;
const int WEIGHT = 50 * 2.20462;
const int AGE = 23;
const int GENDER = 1;
const float MAN_AGE_COFFICIENT = 0.2017;
const float MAN_WEIGHT_COFFICIENT = 0.09036;
const float MAN_HEART_RATE_COFFICIENT = 0.6309;
const float MAN_COFFICIENT = 55.0969;
const float WOMAN_AGE_COFFICIENT = 0.074;
const float WOMAN_WEIGHT_COFFICIENT = 0.05741;
const float WOMAN_HEART_RATE_COFFICIENT = 0.4472;
const float WOMAN_COFFICIENT = 20.4022;
int catL = 2;
const int PAUSE = 2;
const int RESUME = 1;
const int RESTORE = 0;
const int DEFAULT_INPUT = -49;
const char*correctFileName = "correct.txt";
const char*wrongFileName = "wrong.txt";
boolean stepEventsEnabeled = true;   // whether you're polling or using events
long lastStepCount = 0;
int averageHeartRate = random(140, 150);
long randomHeartRate = 0; // step count on previous polling check
boolean blinkState = false;          // state of the LED
int time = 0;
int stepCount = 0;
int signal = PAUSE;
int lastSignal = 100;
BLEPeripheral blePeripheral; // create peripheral instance

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLECharacteristic switchChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify , 14);


//filter
Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter
double pitch, roll;


uint32_t timer;
int axRaw, ayRaw, azRaw;         // raw accelerometer values
//  float normA,normAx,normAy,normAz;
int gxRaw, gyRaw, gzRaw;         // raw gyro values
char gyroArray[3] = { 19, 0, 0};
char accelerator[14];



void setup() {
  Serial.begin(9600);
  // pinMode(13, OUTPUT);
  // set the local name peripheral advertises
  blePeripheral.setLocalName("LEDCB");
  CuriePME.begin();
  CuriePME.setClassifierMode( CuriePME.KNN_Mode );
  // intialize the sensor:
  CurieIMU.begin();
  // turn on step detection mode:
  CurieIMU.setStepDetectionMode(CURIE_IMU_STEP_MODE_NORMAL);
  // enable step counting:
  CurieIMU.setStepCountEnabled(true);

  if (stepEventsEnabeled) {
    // attach the eventCallback function as the
    // step event handler:
    CurieIMU.attachInterrupt(eventCallback);
    CurieIMU.interrupts(CURIE_IMU_STEP);  // turn on step detection

    Serial.println("IMU initialisation complete, waiting for events...");
  }
  // set the UUID for the service this peripheral advertises

  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchChar);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchChar.setEventHandler(BLEWritten, switchCharacteristicWritten);
  //  switchChar.setValue(PAUSE);

  // advertise the service
  blePeripheral.begin();
  Serial.println(("Bluetooth device active, waiting for connections..."));




  //filter initialization
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  timer = micros();
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the accelerometer range to 250 degrees/second
  CurieIMU.setGyroRange(250);

  /*
    SerialFlashFile correctFile;
    SerialFlashFile wrongFile;
    correctFile = SerialFlash.open(correctFileName);
    wrongFile = SerialFlash.open(wrongFileName);
    char correctBuffer[102400];
    char wrongBuffer[102400];
    correctFile.read(correctBuffer,10240);
     wrongFile.read(wrongBuffer,10240);
  */
  tokenizeAndTraining(correctData, 1);
  //  tokenizeAndTraining(wrongData,2);
}

void loop() {
  // signal =Serial.read()-'0';

  if ((signal != lastSignal)) {
    lastSignal = signal;
  }


  run();
  runFilter();

  uint8_t vector[2];

  vector[0] = pitch;
  vector[1] = roll;
  CuriePME.writeVector(vector, 2 );

/*
  while (1) {
    uint16_t distance = CuriePME.getIDX_DIST();
    uint16_t category = CuriePME.getCAT();

    if (category == 0 || category > 127)
      break;

    Serial.print( "Distance = ");
    Serial.print( distance );
    Serial.print("  Category = ");
    Serial.print(category);
    Serial.print("\n");
  }
*/
}

static void run() {


  if (lastSignal == PAUSE) {
    return;
  }
  randomHeartRate = random(140, 150);
  if (lastSignal == RESTORE ) {
    Serial.print("Man Calorie : ");
    Serial.println(calculateCalorieMan());
    Serial.print("Woman Calorie : ");
    Serial.println(calculateCalorieWoman());
    Serial.print("Current Speed : ");
    Serial.println(calculateSpeed());
    time = 0;
    stepCount = 0;
    signal = PAUSE;
  }
  if (!stepEventsEnabeled) {
    updateStepCount();
  }
  digitalWrite(13, blinkState);
  blinkState = !blinkState;
  Serial.print("time: ");
  Serial.println(time);
  Serial.print("heart rate: ");
  Serial.println(randomHeartRate);
  if (time > 1) {
    averageHeartRate = calculateAverageHearRate(randomHeartRate);
    Serial.print("Average rate: ");
    Serial.println(calculateAverageHearRate(randomHeartRate));
  }

  time++;

}

int calculateCalorieMan() {
  return ((AGE * MAN_AGE_COFFICIENT) - (WEIGHT * MAN_WEIGHT_COFFICIENT) + (averageHeartRate * MAN_HEART_RATE_COFFICIENT) - MAN_COFFICIENT) * time / (60 * 4.184);
}
int calculateCalorieWoman() {
  return ((AGE * WOMAN_AGE_COFFICIENT) - (WEIGHT * WOMAN_WEIGHT_COFFICIENT) + (averageHeartRate * WOMAN_HEART_RATE_COFFICIENT) - WOMAN_COFFICIENT) * time / ( 60 * 4.184);
}
float calculateSpeed() {
  return ((float)stepCount / (float)time) * (HOUR / KM);
}
int calculateAverageHearRate(int randomHeartRate) {
  return ((averageHeartRate * (time - 1)) + randomHeartRate) / time;
}

static void eventCallback(void) {
  if (CurieIMU.stepsDetected() && (lastSignal != PAUSE))
    updateStepCount();
}
void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  signal = switchChar.value()[0];

  if (switchChar.value()[0] == 1) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }

}
static void updateStepCount() {
  // get the step count:
  stepCount++;
  // if the step count has changed, print it:
  Serial.print("Step count: ");
  Serial.println(stepCount);
  // save the current count for comparison next check:

}


float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767

  float a = (aRaw * 2.0) / 32768.0;

  return a;
}
void filter() {
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  roll  = atan2(accY, accZ) * RAD_TO_DEG;
  pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
  double dt = (double)(micros() - timer) / 1000000;
  timer = micros();
  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s
#ifdef RESTRICT_PITCH
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } else
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleX) > 90) {
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
  }
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
#else
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } else
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleY) > 90) {
    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
  }
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
#endif

  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
  //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;
}
void runFilter() {

  // read raw accelerometer measurements from device
  CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);

  // convert the raw accelerometer data to G's
  accX = convertRawAcceleration(axRaw);
  accY = convertRawAcceleration(ayRaw);
  accZ = convertRawAcceleration(azRaw);


  gyroArray[0] = (char)accX;
  gyroArray[1] = (char)accY;
  gyroArray[2] = (char)accZ;

  // read raw gyro measurements from device
  CurieIMU.readGyro(gxRaw, gyRaw, gzRaw);

  // convert the raw gyro data to degrees/second
  gyroX = convertRawGyro(gxRaw);
  gyroY = convertRawGyro(gyRaw);
  gyroZ = convertRawGyro(gzRaw);

  char accYChar[7];
  String acc = "";
  filter();
  // display tab-separated accelerometer x/y/z values
  // dtostrf (pitch,100,5,accXChar);
  dtostrf (pitch, 6, 1, accYChar);
  //accelerator=maskData(accX,accY,accZ);

  acc += accYChar;
  acc += ' ';
  dtostrf (roll, 6, 1, accYChar);
  acc += accYChar;
  acc.toCharArray(accelerator, 14);


}
float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767

  float g = (gRaw * 250.0) / 32768.0;

  return g;
}
void training (int category, uint8_t s1, uint8_t s2)
{
  uint8_t vector[2];

  vector[0] = s1;
  vector[1] = s2;

  // give the data, the number of elements and the category it belongs to.
  CuriePME.learn(vector, 2, category);

  Serial.println("Category ");
  Serial.println( category );
  Serial.println(" trained with: ");
  // Serial.print(s1+"\t"+s2);
}
void tokenizeAndTraining(char *buffer, int category) {
  char * pch;
  pch = strtok (buffer, "\n");
  while (pch != NULL)
  {

    String temp(pch);
    int spaceIndex = temp.indexOf(' ');
    int x = temp.substring(0, spaceIndex).toInt();
    int y = temp.substring(spaceIndex + 1).toInt();
    training (category, x, y);
    pch = strtok (NULL, "\n");
  }

  Serial.print("Neurons committed after learning = ");
  Serial.print( CuriePME.getCommittedCount());
  Serial.print("\n");
}

