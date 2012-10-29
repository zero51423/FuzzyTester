#ifndef CONTROL_H
#define CONTROL_H

/*********************************
Filename: Control.h
Authors: Alex Williams, Josh Estus, Cory Fowler 

Date: Oct 25, 2012

Description: Declares an abstract class that will be the 
super class for the various types of controls that the simulation can have.

caveats: CLASS IS PURE VIRTUAL AND CANNOT BE INSTANTIATED
 *********************************/

#include <string>

class Control{
public:
  Control() {}
  Control(std::string n, int x, int y, int w, int h){ 
    name = n;
    xLocation = x;
    yLocation = y;
    width = w;
    height = h;
  }

  void setXLoaction(int x) {xLocation = x;}
  int getXLocation() {return xLocation;}

  void setYLoaction(int y) {yLocation = y;}
  int getYLocation() {return yLocation;}

  void setWidth(int w) {width = w;}
  int getWidth() {return width;}

  void setHeight(int h) {height = h;}
  int getHeight() {return height;}

  void setName(std::string n) {name = n;}
  std::string getname() {return name;}

  virtual std::string getActivateString() = 0;

private:
  int xLocation, yLocation;
  int width, height;
  std::string name;
};

#endif
