/*********************************
Filename: TextField.cpp
Authors: Alex Williams, Josh Estus, Cory Fowler 

Date: Oct 25, 2012

Description: Implementation of the TextField class.
 *********************************/

#include "TextField.h"
#include <sstream>

TextField::TextField(std::string n, int x, int y, int w, int h): Control(n, x, y, w, h) {
  return;
}

/*********************************
Returns the back half of the http POST request that will be sent to the
printer simulation to activate this TextField instance. Simulation URL and 
HTTP wrapping will be added by the Networking sendCommand function.
*********************************/
std::string TextField::getActivateString(){
	int x = getXLocation();
	int y = getYLocation();

	x = x + (getWidth() / 2);
	y = y + (getHeight() / 2);
	
	std::string activationString = "/webservices/automation/request/touchAndRelease?";
	
	std::ostringstream oss;
	oss << "x=" << x << "&y=" << y;

	activationString += oss.str();

	return activationString;
}
