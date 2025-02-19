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

using namespace std;

#include "Control.h"
#include "Network.h"
#include "TextField.h"
#include "Button.h"
#include "pugixml.hpp"
#include "globals.h"

vector <Control*> controls;
//Prototypes

/*Pre: Send valid vector of controls. Will pick one of the vector indexes
 * that control will be the index of the control to choose.
 *
 * Post: Returns the index in the vector of which control to activate
 */
int select_control() {
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
int parseXml() {
    //clear control list
    for (int i = 0; i < controls.size(); i++) {
        delete controls[i];
    }
    controls.clear();

    //open XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("file.xml");

    //return error if document fails to read
    if (!result) {
        cerr << "Error parsing XML" << endl;
        return 1;

    } else {
        pugi::xml_node panel = doc.child("panel");
        //cout << panel.attribute("viewname").value();
        for (pugi::xml_node object = panel.child("object"); object; object = object.next_sibling("object")) {
            //cout << object.value();
            string name = object.child_value("name");
            string objectType = object.child_value("type");
            //cout << objectType;
            int x = atoi(object.child_value("x"));
            int y = atoi(object.child_value("y"));
            int width = atoi(object.child_value("width"));
            int height = atoi(object.child_value("height"));

            if (objectType == "Button") {
                controls.push_back(new Button(name, x, y, width, height));

            } else if (objectType == "TextField") {
                controls.push_back(new TextField(name, x, y, width, height));

            } else {
                //cerr << "ERROR: Bad control type present in XML" << endl;
                //return 1;
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
            MAX_ACTIONS = atoi(argv[i++]);
        }
        if (arg == "-s") {
            ipaddr = argv[i++];
        }
    }

    Network *localNetwork = new Network("128.163.146.74");

    int noResponseCounter = 0, choice = -1; //choice is the index of the control option to select. noResponseCounter is used to count possible crashes/hangs
    ofstream outputFile(logFileName.c_str());
    for (int actionsMade = 0; actionsMade < MAX_ACTIONS; actionsMade++) {
        //if (!localNetwork->get_query("/webservices/automation/data/panel.xml")) { //calls Network get_query to get the XML page
          //  crashFlag = true;
            //break;
        //}
        int corParse = 1; //assume the parse has failed.
        corParse = parseXml(); //returns 0 if correctly parses, 1 is failed
        if (corParse == 1) {
            /* For loop used to double check and make sure it wasn't just a messup.
             * Could potentially move later
             */
            for (int i = 0; i < 5; i++) {
                corParse = parseXml();
                if (corParse == 0) { //page is loaded and parsed correctly
                    break;
                }
            }
        }

        choice = select_control();
        string activateString = controls[choice]->getActivateString();
        cout << activateString;
        bool responseReceived = localNetwork->get_query(activateString);
        if (usingStdOut) {
            cout << actionsMade << " " << controls[choice]->getname() << " " << activateString << " ";
            if (responseReceived) {
                cout << "True\n";
            } else {
                cout << "False\n";
            }
        } else //if not using stdOut
        {
            outputFile << actionsMade << " " << controls[choice]->getname() << " " << activateString << " ";
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

