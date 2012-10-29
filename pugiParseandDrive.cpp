/****************
Make sure to add the pugiXML include to the main file.
****************/
#include <iostream>
#include <vector>
#include "pugixml.hpp"
#include "Control.h"
#include "Button.h"
#include "TextField.h"

using namespace std;

/**********************
Begin section of code that will be needed in the final tester

 **********************/
#define FILE_LOC "fuzzyPanel.xml"

int parseXml(vector< Control* > &controls){
  //clear control list
  for(int i = 0; i < controls.size(); i++){
    delete controls[i];
  }
  controls.clear();

  //open XML document
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(FILE_LOC);

  //return error if document fails to read
  if(!result){
    cerr << "Error parsing XML" << endl;
    return 1;

  }else{
    pugi::xml_node panel = doc.child("panel");
    for(pugi::xml_node object = panel.child("object"); object; object = object.next_sibling("object")){
      string name = object.attribute("name").value();
      string objectType = object.attribute("type").value();
      int x = object.attribute("x").as_int();
      int y = object.attribute("y").as_int();
      int width = object.attribute("width").as_int();
      int height = object.attribute("height").as_int();

      if(name == "button"){
	controls.push_back(new Button(name, x, y, width, height));

      }else if(name == "textField"){
	controls.push_back(new TextField(name, x, y, width, height));

      }else{
	cerr<< "ERROR: Bad control type present in XML" << endl; 
	return 1;
      }
    }
  }  

  return 0;

}


/**********************
End section of code that will be needed in the final tester

Begin test code for Parser
 **********************/


int main(){
  return 0;
}
