#include "batt.h"

int set_batt_from_ports(batt_t *batt)
{
  if (BATT_VOLTAGE_PORT < 0)
  { // when BATT_VOLTAGE_PORT is negative, then return 1
    return 1;
  }
  batt->mlvolts = BATT_VOLTAGE_PORT / 2;    // set batt->mlvolts
  int percent = (batt->mlvolts - 3000) / 8; // calcuate the value of percent
  if (percent >= 100)
  { // check percent boundary situations
    batt->percent = 100;
  }
  else if (percent < 0)
  {
    batt->percent = 0;
  }
  else
  {
    batt->percent = percent; // set batt -> percent
  }

  if (BATT_STATUS_PORT & 1 << 4)
  {                 // find the index 4 and set batt -> mode
    batt->mode = 1; // percent
  }
  else
  {
    batt->mode = 2; // volt
  }
  return 0;
}
// Uses the two global variables (ports) BATT_VOLTAGE_PORT and
// BATT_STATUS_PORT to set the fields of the parameter 'batt'.  If
// BATT_VOLTAGE_PORT is negative, then battery has been wired wrong;
// no fields of 'batt' are changed and 1 is returned to indicate an
// error.  Otherwise, sets fields of batt based on reading the voltage
// value and converting to precent using the provided formula. Returns
// 0 on a successful execution with no errors. This function DOES NOT
// modify any global variables but may access global variables.
//
// CONSTRAINT: Avoids the use of the division operation as much as
// possible. Makes use of shift operations in place of division where
// possible.
//
// CONSTRAINT: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU.
//
// CONSTRAINT: Limit the complexity of code as much as possible. Do
// not use deeply nested conditional structures. Seek to make the code
// as short, and simple as possible. Code longer than 40 lines may be
// penalized for complexity.

int set_display_from_batt(batt_t batt, int *display)
{
  char masks[10] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111}; // creat masks of digits 0-9
  int right, middle, left;
  *display = 0; // reset display to 0
  if (batt.mode == 2)
  { // volt mode
    // use modulo to determine the interger value for right, middle and left
    right = ((batt.mlvolts + 5) / 10) % 10; //+5 to round up the lowest digit
    middle = (batt.mlvolts / 100) % 10;     // middle digit
    left = (batt.mlvolts / 1000) % 10;      // left digit
    // move bit and set display
    *display |= masks[right] << 3;
    *display |= masks[middle] << 10;
    *display |= masks[left] << 17;

    *display |= 1 << 1; // v
    *display |= 1 << 2; // decimal
  }
  else
  {                                    // percent mode
    right = (batt.percent % 10);       // right digit
    middle = (batt.percent / 10) % 10; // middle digit
    left = (batt.percent / 100) % 10;  // left digit

    *display |= masks[right] << 3; // set display
    if (left == 0)
    { // check whether left or middle is 0. then not show on display screen
      if (middle != 0)
      {
        *display |= masks[middle] << 10;
      }
    }
    else
    {
      *display |= masks[left] << 17;
      *display |= masks[middle] << 10;
    }
    *display |= 1; //%
  }

  int levelMask = 0b00000; // mask for battery bars
  if (batt.percent >= 90)
  {
    levelMask |= 0b11111;
  }
  else if (batt.percent >= 70 && batt.percent < 90)
  {
    levelMask |= 0b01111;
  }
  else if (batt.percent >= 50 && batt.percent < 70)
  {
    levelMask |= 0b00111;
  }
  else if (batt.percent >= 30 && batt.percent < 50)
  {
    levelMask |= 0b00011;
  }
  else if (batt.percent >= 5 && batt.percent < 30)
  {
    levelMask |= 0b00001;
  }
  *display |= levelMask << 24;
  return 0;
}
// Alters the bits of integer pointed to by 'display' to reflect the
// data in struct param 'batt'.  Does not assume any specific bit
// pattern stored at 'display' and completely resets all bits in it on
// successfully completing.  Selects either to show Volts (mode=1) or
// Percent (mode=2). If Volts are displayed, only displays 3 digits
// rounding the lowest digit up or down appropriate to the last digit.
// Calculates each digit to display changes bits at 'display' to show
// the volts/percent according to the pattern for each digit. Modifies
// additional bits to show a decimal place for volts and a 'V' or '%'
// indicator appropriate to the mode. In both modes, places bars in
// the level display as indicated by percentage cutoffs in provided
// diagrams. This function DOES NOT modify any global variables but
// may access global variables. Always returns 0.
//
// CONSTRAINT: Limit the complexity of code as much as possible. Do
// not use deeply nested conditional structures. Seek to make the code
// as short, and simple as possible. Code longer than 65 lines may be
// penalized for complexity.

int batt_update()
{
  batt_t batt;
  if (set_batt_from_ports(&batt) != 0)
  { // check if set_batt_from_ports indicate an error
    return 1;
  }
  set_display_from_batt(batt, &BATT_DISPLAY_PORT); // change the display

  return 0;
}
// Called to update the battery meter display.  Makes use of
// set_batt_from_ports() and set_display_from_batt() to access battery
// voltage sensor then set the display. Checks these functions and if
// they indicate an error, does NOT change the display.  If functions
// succeed, modifies BATT_DISPLAY_PORT to show current battery level.
//
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
