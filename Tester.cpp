/* 
 * File:   Tester.cpp
 * Author: Josh Estus, Cory Fowler, Alex Williams
 *
 * Created on October 25, 2012
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <vector>
#include "Control.h"
#include "Network.h"
#include "Textfield.h"
#include "Button.h"
#include "pugixml.hpp"

#define FILE_LOC "fuzzyPanel.xml"

using namespace std;
//Prototypes

/*Pre: Send valid vector of controls. Will pick one of the vector indexes
 * that control will be the index of the control to choose. 
 * 
 * Post: Returns the index in the vector of which control to activate
 */
int select_control(vector <Control*> &controls) {
    srand(time(NULL)); //random seed
    return rand() % controls.size(); //getting a number between 0 and the amount of controls on the screen
}

/*Pre: noReponseCounter and responseRecieved will be used to update
 * the count of if the Simulation has crashed or not
 * Post: will return a count between 0 and 5 for the noResponseCounter. 
 */
int test_for_crash(int noResponseCounter, bool responseRecieved) {
    if (responseRecieved) {
        return 0;
    } else {
        return noResponseCounter + 1;
    }
}

/*Pre: parseXML is given the vector of controls. parseXML is able to retrieve the fuzzypanel.xml
 *  correctly.
 * 
 *  Post: Fills control vector with all possible controls and returns 0 for success 1 for failure. 
 */
int parseXml(vector< Control* > &controls) {
    //clear control list
    for (int i = 0; i < controls.size(); i++) {
        delete controls[i];
    }
    controls.clear();

    //open XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(FILE_LOC);

    //return error if document fails to read
    if (!result) {
        cerr << "Error parsing XML" << endl;
        return 1;

    } else {
        pugi::xml_node panel = doc.child("panel");
        for (pugi::xml_node object = panel.child("object"); object; object = object.next_sibling("object")) {
            string name = object.attribute("name").value();
            string objectType = object.attribute("type").value();
            int x = object.attribute("x").as_int();
            int y = object.attribute("y").as_int();
            int width = object.attribute("width").as_int();
            int height = object.attribute("height").as_int();

            if (name == "button") {
                controls.push_back(new Button(name, x, y, width, height));

            } else if (name == "textField") {
                controls.push_back(new TextField(name, x, y, width, height));

            } else {
                cerr << "ERROR: Bad control type present in XML" << endl;
                return 1;
            }
        }
    }

    return 0;

}

/*
 * 
 */
int main(int argc, char** argv) {

    //setting up defaults
    int MAX_ACTIONS = 100;
    bool output_on_no_crash = false; //Whether or not save if crash
    bool crashFlag = false; //Flag for if the simulation has crashed
    bool usingStdOut = true; //flag for whether or not to use StdOut
    string ipaddr = "0.0.0.0"; //IP address
    string logFileName = "actionLog.txt"; //name of log file default
    string arg = "";
    //parsing the arguments and storing them in each of the different variables
    for (int i = 0; i < argc; i++) {
        arg = argv[i];
        if ((i + 1) == argc) {
            //detect option without a follow up value. Just keep default
            break;
        }
        if (arg == "-o") {
            logFileName = argv[i++]; //log file name will equal the next argument
            usingStdOut = false;
        }
        if (arg == "-dump-log") {
            output_on_no_crash = argv[i++];
        }
        if (arg == "-n") {
            MAX_ACTIONS = argv[i++];
        }
        if (arg == "-s") {
            ipaddr = argv[i++];
        }
    }
    vector <Control> controls;
    int noResponseCounter = 0, choice = -1; //choice is the index of the control option to select. noResponseCounter is used to count possible crashes/hangs
    ofstream outputFile(logFileName.c_str());
    for (int actionsMade = 0; actionsMade < MAX_ACTIONS; actionsMade++) {
        if (!Network.get_query("/webservices/automation/data/panel.xml")) { //calls Network get_query to get the XML page
            crashFlag = true;
            break;
        }
        int corParse = 1; //assume the parse has failed. 
        corParse = parseXml(controls); //returns 0 if correctly parses, 1 is failed
        if (corParse == 1) {
            /* For loop used to double check and make sure it wasn't just a messup. 
             * Could potentially move later          
             */
            for (int i = 0; i < 5; i++) {
                corParse = parseXml(controls);
                if (corParse == 0) { //page is loaded and parsed correctly
                    break;
                }
            }
        }

        choice = select_control(controls);
        string activateString = controls[choice].getActivateString();
        bool responseReceived = Network.get_query(activateString);
        if (usingStdOut) {
            cout << actionsMade << " " << controls[choice].getname() << " " << activateString << " ";
            if (responseReceived) {
                cout << "True\n";
            } else {
                cout << "False\n";
            }
        } else //if not using stdOut
        {
            outputFile << actionsMade << " " << controls[choice].getname() << " " << activateString << " ";
            if (responseReceived) {
                outputFile << "True\n";
            } else {
                outputFile << "False\n";
            }
        }

        noResponseCounter = test_for_crash(noResponseCounter, responseReceived);
        if (noResponseCounter >= 5) {
            crashFlag = true;
            break;
        }
    }
    if (crashFlag) {
        if (usingStdOut) {
            cout << "POSSIBLE SIMULATION CRASH DETECTED";
        }
        else //if not using stdOut
        {
            outputFile << "POSSIBLE SIMULATION CRASH DETECTED";
            outputFile.close();
            cout << "The file is located within this directory at " << logFileName;
        }

    } else if (output_on_no_crash) {
        if (usingStdOut) {
            cout << "TESTING COMPLETED SUCCESSFULLY";
        }
        else //if not using stdOut
        {
            outputFile << "TESTING COMPLETED SUCCESSFULLY";
            outputFile.close();
            cout << "The file is located within this directory at " << logFileName;
        }

    }

    return 0;
}

