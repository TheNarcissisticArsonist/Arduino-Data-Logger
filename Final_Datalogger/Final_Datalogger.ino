/*
 * Code written by Thomas Cohn.
 * Insert more personalized comments and such here...
 */

/*
 * Do not worry!
 * Comments are removed during compiling, and
 * therefore have no effect on memory usage.
 */

//Include statements
#include <SD.h>       //Used for the SD card
#include <SPI.h>      //Used by WiFi and the SD card -- they both use SPI to communicate
#include <WiFi.h>     //Used for WiFi connectivity
/* WARNING: WRONG WIFI LIBRARY! */
/* Use this one instead: https://learn.adafruit.com/adafruit-cc3000-wifi/cc3000-library-software */

/*
----- Values unique to each sensor package...
----- These will need to be changed for each one.
*/
const int systemId = 12345; //The ID tag of the system
const String key = "super secret password" //The password to get into the server