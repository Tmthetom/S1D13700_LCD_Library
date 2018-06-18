#include <S1D13700.h>

/*create our S1D13700 LCD object and name it
 glcd. Since no options are specified, the
 default (and only) constructor is executed. */
S1D13700 glcd;

void setup()
{
  /*Specify control pin connections*/
  glcd.pins.d0 = 14;
  glcd.pins.d1 = 15;
  
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
  
}
