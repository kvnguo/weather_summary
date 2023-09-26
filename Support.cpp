/*
Implementation of all structs and classes declared in Support.h
*/
#include <string> 
#include <iostream>
#include <iomanip>

#include "Support.h"
using namespace std;

//Overloaded the < operator to allow us to compare two locations and sort alphabetically by state then by city 
bool location::operator<(const location &second) const {
	if(state < second.state) {
		return true; 
	}
	//Should only sort the city if the states are the same
	else if(city < second.city && state == second.state){
		return true;
	}
	return false; 
}

//Overloaded the == operator to check if two cities are identical 
bool location::operator==(const location &second) const {
	return geocode == second.geocode;	
}

//Dynamically allocated memory for all variables of the node struct
//Initialized all N to equal zero for later calculations 
list::node::node(const location &n_station=location()) {
	station = n_station;
	
    N = new int[12];
	for (int i = 0; i < 12; i++) {
		N[i] = 0;
	}        
    total_precip = new float[12];
    max_precip = new float[12];
    min_precip = new float[12];

    total_temp = new int[12];
    max_temp = new int[12];
    min_temp = new int[12];

	next = nullptr; 
}

//Deletes all dynamically allocated variables constructed in the node constructor 
list::node::~node() {
	delete[] N; 
	delete[] total_precip;
    delete[] max_precip;
    delete[] min_precip;
    delete[] total_temp;
    delete[] max_temp;
    delete[] min_temp;
}

//Prints the location of the node 
void list::node::print_station() {
	printDashes(); 
	cout << station.city << ", " << station.state << " (" << station.geocode << ")" << endl;
	printDashes();			
}

//Prints all data of the node 
void list::node::print_data() {
	for(int i = 0; i < 12; i++) {
		cout << right << setw(3) << convertMonth(i) << ":"
			 << fixed << setprecision(2) << setw(6) << total_precip[i]
			 << fixed << setprecision(2) << setw(6) << total_precip[i] / N[i]
			 << fixed << setprecision(2) << setw(6) << max_precip[i]
			 << fixed << setprecision(2) << setw(6) << min_precip[i] << " :" 
			 << setw(4) << total_temp[i] / N[i]
	  		 << setw(4) << max_temp[i]
			 << setw(4) << min_temp[i]
			 << endl;
	}
}

//Incorporate data into the node 
void list::node::incorporate_data(const data &dat) {
    //For zero based indexing 
	int i = dat.month - 1;
    //If N[i] == 0, there is no data yet, meaning you have to set it initially 
	if(N[i] == 0) {
        total_precip[i] = dat.precip;
        total_temp[i] = dat.temp;
        max_precip[i] = dat.precip;
        min_precip[i] = dat.precip;
        max_temp[i] = dat.temp;
        min_temp[i] = dat.temp;
    }
	//Otherwise you update the data 
    else {
        total_precip[i] += dat.precip;
        total_temp[i] += dat.temp;
        if (dat.precip > max_precip[i]) {
            max_precip[i] = dat.precip;
        }
        if (dat.precip < min_precip[i]) {
            min_precip[i] = dat.precip;
        }
        if (dat.temp > max_temp[i]) {
            max_temp[i] = dat.temp;
        }
        if (dat.temp < min_temp[i]) {
            min_temp[i] = dat.temp;
        }
     }
    N[i]++;
}

//Creates a sentinel node when a list object is created 
list::list() {
	head = new node(); 
} 

//Deletes all the nodes within the list  
list:: ~list() {
	while(head->next != nullptr) {
		node* temp = head->next; 
		head->next = temp->next; 
		delete temp; 
	}
	delete head; 
}

//Inserts nodes into the linked list alphabetically 
void list::insert(const location &loc, const data &dat) {
	//I used two nodes for iterating since you can't access the prev node in a singlely linked list  
	node *current = head->next;
	node *prev = head;
	//Iterate the two pointers until the current pointer points to the node that is either greater than, or equal to, the new location  
	while(current != nullptr && current->station < loc) {
		current = current->next; 
		prev = prev->next; 
	}
	//If the current node is not null and the station is equal to the location, that means the location is already inside the linked list, so you only have to update the data 
	if(current != nullptr && current->station == loc) {
		current->incorporate_data(dat);
	}
	//Otherwise, the location isn't in the list yet so we have to make a new node, populate it with data, and then insert it into the list 
	else {
		node *n = new node(loc);
		n->incorporate_data(dat);
		n->next = current; 
		prev->next = n;
	}
}

//If there is a command line argument, we will iterate until we get to the location and print out the summary
//Otherwise, we print out everything 
void list::print(const char *s) {
	node *it = head->next;  
	if(s != NULL) {
		while(it->station.geocode != s) {
			it = it->next; 
		}
		it->print_station();
		it->print_data();	
		return; 
	}
	while(it != nullptr) {
		it->print_station();
		it->print_data();
		it = it->next;
	}
}


//Prints dashes  
void printDashes() {
	for(int i = 0; i < 42; i++){
		cout << '-'; 
	}
	cout << endl; 
}

//Converts months from numeric to alphabet
string convertMonth(int month) {
    string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
							"Jul","Aug", "Sep", "Oct", "Nov", "Dec"};
    return months[month];
}

