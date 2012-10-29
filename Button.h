#ifndef BUTTON_H
#define BUTTON_H

/*********************************
Filename: Button.h
Authors: Alex Williams, Josh Estus, Cory Fowler 

Date: Oct 25, 2012

Description: Declares a the button class, which will
represent a button object in the target printer simulation screen

caveats: This class relies on the Control clas as a super class
 *********************************/

#include "Control.h"

class Button : public Control{
public:
  Button() {}
  Button(std::string n, int x, int y, int w, int h);
  std::string getActivateString();
};

#endif
