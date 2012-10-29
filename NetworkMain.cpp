#include <iostream>
#include <string>

using namespace std;

#include "Network.h"

int main() {

    // Establish an initial network connection
    Network myNetwork = Network("128.163.146.74");

    // Send the request to get the XML panel
    bool success = myNetwork.get_query("/webservices/automation/data/panel.xml");

    // Touch and release for a button from the home screen
    // Implementation idea:
    //  Use ButtonObject.toString() to generate this parameter, and likewise for any actionable object
    success = myNetwork.get_query("/webservices/automation/request/touchAndRelease?x=48&y=228");

    return 0;
}
