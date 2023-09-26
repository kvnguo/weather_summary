/*
This is a Program that calculates monthly per-city summaries of statistics from weather data read from a CSV file
*/
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Support.h"

//Parse line input and extract info from weather file 
void extract_values(string &text, location &n_station, data &n_data) {
	for (long unsigned int i = 0; i < text.length(); i++) {
		if (text[i] == ' ') {
			text[i] = '_';
		} 
		else if (text[i] == ',') {
			text[i] = ' ';
		}
	}
	istringstream sin(text);
	sin >> n_data.month >> n_station.city >> n_station.state 
		>> n_station.geocode >> n_data.precip >> n_data.temp;
}

int main(int argc, char *argv[])
//Error checking 
{
  if (argc != 1 && argc != 2) {
    cerr << "usage: " << argv[0] << " [location] < datafile\n";
    return 1;
  }

  char *target_location = NULL;
  if (argc == 2)
    target_location = argv[1];

  list city_list;

  string n_text;
  location n_station;
  data n_data;

//Extract all the values 
  while (getline(cin, n_text)) {
    extract_values(n_text, n_station, n_data);
	city_list.insert(n_station, n_data);
  }

//Prints out summary of a city or all summaries accordingly 
  city_list.print(target_location);
}
