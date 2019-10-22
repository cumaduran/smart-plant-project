







//=======================================================================================================//|
//                                                                                                       //|
//                                        Program Description Begin                                      //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//  This header file contains the class declarations for the employment of a DS1302 Real Time Clock      //|
//  library.  This file is written for compatibility for Arduino Boards, as well as Versilino boards.    //|
//  These functions will work on any pin type available on the boards (PWM, COMMUNICATION, DIGITAL, or   //|
//  ANALOG).                                                                                             //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                        Program Description End                                        //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                 This is the Revision Section Begin                                    //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//    Date                  Programmer                  Revisions Made                                   //|
//                                                                                                       //|
//    XX June 2012          Krodal                      Original                                         //|
//    XX March 2013         Krodal                      Added bcd2bin, bin2bcd_h, and bin2bcd_l functions//|
//    07 January 2014       Joseph Datillo              Converted file into library format.              //|
//                          T. Daniel Murphy            Added functions to allow for user implentation   //|
//                                                        as a library, vice a stand alone program.      //|
//                                                      Added ability to access individual elements of   //|
//                                                        the time structure.                            //|
//                                                      Added functions for printing, and element access.//|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                        Revision Section End                                           //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                        Acknowledgements Begin                                         //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//  The original program was provided by Arduino.cc user Kodal on URL:                                   //|
//  http://playground.arduino.cc/Main/DS1302                                                             //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//                                          Acknowledgements End                                         //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                            Reference Links Begin                                      //|
//                                                                                                       //|
//=======================================================================================================//|
                                                                                                         //|
//  Datasheet:  http://www.hobbytronics.co.uk/datasheets/DS1302.pdf                                      //|
//  GET UPDATES: https://www.virtuabotix.com/resources.php?product=versalino                             //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                                            Reference Links End                                        //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                          Include files required for Program Function Begin                            //|
//                                                                                                       //|
//=======================================================================================================//|
                                                                                                         //|
#include "stdio.h"                                                                                        //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                          Include files required for Program Function End                              //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                         Include Guards Begin                                          //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//  This will pull the topmost  library for the BUS layouts, VersalinoPINS and VersalinoBUS structures   //|
//  that you will be using in this library.  A version of the Versalino.h should always be included      //|
//  in  your library folder just in case the user has not installed a Versalino library. (This will      //|
//  prevent compile errors, and/or lower the complexity of the code you need to write for a cross        //|
//  compatible Library) using <Versalino.h> as apposed to "Versalino.h" is what makes this code          //|
//  automatically pull from a higher level if the library is already present in the users libraries      //|
//  folder.                                                                                              //|
//                                                                                                       //|
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//|
                                                                                                         //|
#ifndef virtuabotixRTC_H                                                                                 //|
#define virtuabotixRTC_H                                                                                 //|
                                                                                                         //|
#include <stddef.h>                                                                                      //|
  #include <string.h>                                                                                    //|
                                                                                                         //|
#if defined(ARDUINO) && ARDUINO >= 100                                                                   //|
  #include "Arduino.h"                                                                                   //|
  #else                                                                                                  //|
    #include "WProgram.h"                                                                                //|
    #include <pins_arduino.h>                                                                            //|
#endif                                                                                                   //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                                          Include Guards Break                                         //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//                                   Defines Required for Library Begin                                  //|
//                                                                                                       //|
//=======================================================================================================//|
                                                                                                         //|
#define DS1302_ENABLE            0x8E                                                                    //|
#define DS1302_TRICKLE           0x90                                                                    //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                                     Defines Required for Library End                                  //|
//                                                                                                       //|
//=======================================================================================================//|


//=======================================================================================================//|
//                                                                                                       //|
//			             virtuabotixRTC Class Declaration Begin                              //|
//                                                                                                       //|
//=======================================================================================================//|
//                                                                                                       //|
//  virtuabotixRTC - Creates the Real Time Clock object, and contains all of the functions that are      //|
//                   required for setting, and tracking the time, as well as converting that time into   //|
//                   the current UNIX Time.                                                              //|
//                                                                                                       //|
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//|
                                                                                                         //|
class virtuabotixRTC  {                                                                                  //|
  public:
    virtuabotixRTC();                                                                                            //|
//+++++++++++++++++++++++++++++++++++++++++++++ Class Constructor +++++++++++++++++++++++++++++++++++++++//|
    virtuabotixRTC(uint8_t inSCLK, uint8_t inIO, uint8_t inC_E);                                                     //|
                                                                                                         //|
//++++++++++++++++++++++++++++++++++++++++++++++ Class Functions ++++++++++++++++++++++++++++++++++++++++//|
    void initRTC(uint8_t CLK, uint8_t IO, uint8_t ENABLE);     // Sets the pins and enable them          //|
    void DS1302_clock_burst_read( uint8_t *p);                 // Reads clock data, and sets pinmode     //|
    void DS1302_clock_burst_write( uint8_t *p);                // Writes clcok data, and sets pinmode    //|
    uint8_t DS1302_read(int address);                          // Reads a byte from DS1302, sets pinmode //|
    void DS1302_write( int address, uint8_t data);             // Writes a byte to DS1302, sets pinmode  //|
    void _DS1302_start( void);                                 // Function to help setup start condition //|
    void _DS1302_stop(void);                                   // Function to help stop the communication//|
    uint8_t _DS1302_toggleread( void);                         // Function to help read byte with bit    //|
    void _DS1302_togglewrite( uint8_t data, uint8_t release);  // Function to help wrtie byte with bit   //|
    void setDS1302Time(uint8_t seconds, uint8_t minutes,       // This function sets the time on the     //|
                       uint8_t hours, uint8_t dayofweek,       // DS1302                                 //|
                       uint8_t dayofmonth, uint8_t month,                                                //|
                       int year);                                                                        //|
    void updateTime();                                         // This function simply updates the time  //|
                                                                                                         //|
//++++++++++++++++++++++++++++++++++++++++++++++ Class variables ++++++++++++++++++++++++++++++++++++++++//|
    uint8_t SCLK;                                                                                        //|
    uint8_t IO;                                                                                          //|
    uint8_t C_E;                                                                                         //|
    uint8_t seconds;                                                                                     //|
    uint8_t minutes;                                                                                     //|
    uint8_t hours;                                                                                       //|
    uint8_t dayofweek;                                                                                   //|
    uint8_t dayofmonth;                                                                                  //|
    uint8_t month;                                                                                       //|
    int year;                                                                                            //|
};                                                                                                       //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                                  virtuabotixRTC Class Declaration End                                 //|
//                                                                                                       //|
//=======================================================================================================//|
                                                                                                         //|
#endif                                                         // versalinoRTC_H                         //|
                                                                                                         //|
//=======================================================================================================//|
//                                                                                                       //|
//                                          Include Guards End                                           //|
//                                                                                                       //|
//=======================================================================================================//|
