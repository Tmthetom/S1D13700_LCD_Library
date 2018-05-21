/*
******************************************************************
Copyright (c) 2010, Matt Bommicino
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of Cafe Logic nor the names of its contributors 
	  may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

******************************************************************
*/
/* This is an example sketch that illustrates
  how to use the S1D13700 library. The sketch
  itizializes the display, outputs some text,
  draws a coordinate grid, then graphs a sin
  wave. This is not intended to represent the
  most effiecent way of implementing any of
  the above goals. The sketch assumes that you
  are using the default pin connections as
  described in the documentation. */

#include <S1D13700.h>

/*create our S1D13700 LCD object and name it
 glcd. Since no options are specified, the
 default (and only) constructor is executed. */
S1D13700 glcd;

void setup()
{
  /*Call the setup routine */
  glcd.initLCD();

  /*Create a string variable */
  char buf[] = "Hello World!";

  /*Clear the screen */
  glcd.clearText();
  glcd.clearGraphic();

  /*specify the column and row address
   where the text should be output */
  glcd.textGoTo(10,1);

  /*Write our string variable to the screen */
  glcd.writeText(buf);

  /*Write the same string in a different place */
  glcd.textGoTo(20,28);
  glcd.writeText(buf);

  /*Draw a box on the screen to 
  contain out upcoming sin wave.
  We use two command to double the 
  border width*/
  glcd.drawBox(0,24,319,216);
  glcd.drawBox(1,25,318,215);
  
  /*Now we will draw a grid inside the
    box */
  int i;
  for (i = 16; i < 320; i+=16)
  {
    glcd.drawLine(i,24,i,216);
  }
  
  for (i = 40; i < 216; i+=16)
  {
    glcd.drawLine(0,i,319,i);
  }
  
  /*Thicken the center lines on both axis
    to make it look like a coordinate grid */
  glcd.drawLine(0,119,319,119);
  glcd.drawLine(0,121,319,121);
  glcd.drawLine(159,24,159,216);
  glcd.drawLine(161,24,161,216);

  

  /*Create a sine wave using a loop
   and the sine function. I created these 
   defines so you would know where all the
   numbers come from. Keep in mind, however,
   this is not a tutorial on sin fucntions. */
   #define SCREEN_WIDTH 320
   #define SCREEN_HEIGHT 240
   #define SIN_HEIGHT 60
   #define NUM_OF_OSCILLATIONS 2
   #define LOOP_INCREMENT (NUM_OF_OSCILLATIONS * 2 * PI) / SCREEN_WIDTH
  float currentSin = 0;
  int x = 0;
  int y = 0;
  int oldX = 0;
  int oldY = (SCREEN_HEIGHT / 2);
  for (x = 0; x < SCREEN_WIDTH; x++)
  {
    currentSin += LOOP_INCREMENT;
    y = (int) (SIN_HEIGHT * sin(currentSin)) + (SCREEN_HEIGHT / 2);
    glcd.drawLine(oldX, oldY, x, y);
    
    /*Here we draw extra lines to thicken the sin 
      function but we need to make sure we don't
      try to draw outside the screen boundries or
      we could have a crash */
    if ((x < SCREEN_WIDTH) && (oldX < SCREEN_WIDTH))
    {
       glcd.drawLine(oldX + 1, oldY, x + 1, y);
    }
    if ((x > 0) && (oldX > 0))
    {
       glcd.drawLine(oldX - 1, oldY, x - 1, y);
    }
    oldX = x;
    oldY = y;
  }

}

void loop()
{


  /*Blink the LED for verification that
   the program is executing succesfully */
  pinMode(13,OUTPUT);
  while(1)
  {

    digitalWrite(13,1);
    delay(500);
    digitalWrite(13,0);
    delay(500);

  }
}







