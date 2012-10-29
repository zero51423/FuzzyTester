#ifndef TEXTFIELD_H
#define TEXTFIELD_H

/*********************************
Filename: TextField.h
Authors: Alex Williams, Josh Estus, Cory Fowler 

Date: Oct 25, 2012

Description: Declares a the TextField class, which will
represent a TextField object in the target printer simulation screen

caveats: This class relies on the Control clas as a super class
 *********************************/

#include "Control.h"

class TextField : public Control{
public:
  TextField() {}
  TextField(std::string n, int x, int y, int w, int h);
  std::string getActivateString();
};

#endif
