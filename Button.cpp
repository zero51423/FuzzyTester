/*********************************
Filename: Button.cpp
Authors: Alex Williams, Josh Estus, Cory Fowler 

Date: Oct 25, 2012

Description: Implementation of the Button class.
 *********************************/

#include "Button.h"
#include <sstream>

Button::Button(std::string n, int x, int y, int w, int h) : Control(n, x, y, w, h){}

/*********************************
Returns the back half of the http POST request that will be sent to the
printer simulation to activate this Button instance. Simulalation URL and 
HTTP wrapping will be added by the Networking sendCommand function
*********************************/
std::string Button::getActivateString(){
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
