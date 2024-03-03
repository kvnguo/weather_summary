// Implimentation of the header file 

#include "Support.h"

using namespace std;

/***********************
 *   Location Struct   *
 * *********************/

// Operator overload enabling std::map to sort location objects 
bool location::operator<(const location &other) const
{
    if (state < other.state)
        return true;
    else if (city < other.city && state == other.state)
        return true;
    return false;
}

// Prints out header for a location 
void location::print() const
{
    for (int i = 0; i < 42; i++)
        cout << "-";
    cout << endl;

    cout << city << ", " << state << " (" << geocode << ")" << endl;

    for (int i = 0; i < 42; i++)
        cout << "-";
    cout << endl;
}

/**********************
 *   Rawdata Struct   *
***********************/

// Prints rawdata info 
void rawdata::print()
{
    cout << setw(5) << month << setw(6) << fixed << setprecision(2) << precip << setw(6) << temp << endl;
}


/**********************
 *   Summary Struct   *
 * ********************/

// Constructor for summary class initilizing weather variables 
summary::summary() : N(0), precip_avg(0), precip_max(numeric_limits<int>::min()), precip_min(numeric_limits<int>::max()), temp_avg(0), temp_max(numeric_limits<int>::min()), temp_min(numeric_limits<int>::max()) {}

// Operator overload enabling summary objects to incorperate raw data for calculations 
void summary::operator+=(const rawdata &data)
{
    N++;

    precip_avg += data.precip;
    precip_max = max(precip_max, data.precip);
    precip_min = min(precip_min, data.precip);

    temp_avg += data.temp;
    temp_max = max(temp_max, static_cast<float>(data.temp));
    temp_min = min(temp_min, static_cast<float>(data.temp));
}

// Prints out summary object data 
void summary::print(string month) const
{
    cout << month << ": "
         << setw(5) << fixed << setprecision(2) << precip_min << " "
         << setw(5) << precip_max << " "
         << setw(5) << precip_avg / N << " : "
         << setw(5) << setprecision(1) << temp_min << " "
         << setw(5) << temp_max << " "
         << setw(5) << temp_avg / N << endl;
}

/**********************
 *   Database Class   *
 * ********************/
// Public:
    // Populates location_map and rawdata_cache with data from csv file 
    void database::init_rawdata(const string &filename)
    {
        // Check file
        ifstream fin;
        fin.open(filename);
        if (!fin.is_open())
        {
            cerr << "error: cannot open invalid_file.csv" << endl
                << "usage: ./executable [-rawdata] datafile" << endl;
            return;
        }

        string line;
        while (getline(fin, line))
        {
            extract_rawdata(line); // Parses line 

            // Extract data 
            location loc;
            rawdata data;
            istringstream sin(line);
            sin >> data.month >> loc.city >> loc.state >> loc.geocode >> data.precip >> data.temp;

            // Check if location is in location_map
            // If not, create a new list, add data, then add to the map 
            // Otherwise, just add data in the correct location. 
            if (location_map.find(loc) == location_map.end())
            {
                list<rawdata> cache_element;
                cache_element.push_back(data);
                rawdata_cache.push_back(cache_element);
                location_map[loc] = rawdata_cache.size() - 1;
            }
            else
                rawdata_cache[location_map[loc]].push_back(data);
        }
        fin.close();
    }

    // Prints all raw data in order
    void database::print_rawdata()
    {
        // std::map is already sorted 
        for (map<location, int>::iterator map_it = location_map.begin(); map_it != location_map.end(); ++map_it)
        {
            map_it->first.print(); // Key is location 
            list<rawdata> cache_element = rawdata_cache[map_it->second]; // Access each location's unique index
            for (list<rawdata>::iterator list_it = cache_element.begin(); list_it != cache_element.end(); ++list_it)
                list_it->print();
        }
    }


     // Populates summary_cache, state_map, and geocode_map  with data 
    void database::init_summary()
    {
        summary_cache.resize(rawdata_cache.size(), vector<summary>(12)); // Resize summary_cache to (# of locations) x 12 size 
        for (map<location, int>::iterator map_it = location_map.begin(); map_it != location_map.end(); ++map_it)
        {
            state_map[map_it->first.state].insert(map_it->first.geocode); // Each state and its own set of geocodes for its states
            geocode_map[map_it->first.geocode] = map_it->first; // Every geocode can access its own location object  
            extract_summary(map_it->second); // Inserts data into right cell of summary_cache 
        }
    }

    void database::print_summary(string target)
    {
        // Check if database contains the state or geocode 
        if (state_map.count(target) == 0 && geocode_map.count(target) == 0)
        {
            cerr << "Target not found!" << endl;
            return;
        }

        queue<string> q;
        string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

        // If given a state, add all of its geocode into the queue
        // Overwise, just add the geocode 
        if (target.size() > 3)
        {
            for (set<string>::iterator it = state_map[target].begin(); it != state_map[target].end(); ++it)
                q.push(*it);
        }
        else
            q.push(target);

        // Prints out the each entry of the queue 
        while (!q.empty())
        {
            int index = location_map[geocode_map[q.front()]]; // Gets unique index of the location 
            geocode_map[q.front()].print(); // Prints out location header 
            // Prints out the months 
            for (unsigned long int i = 0; i < summary_cache[index].size(); i++)
                summary_cache[index][i].print(months[i]);
            q.pop(); 
        }
    }

// Private:
    // Repalces spaces with underscores and commas with spaces 
    void database::extract_rawdata(string &line)
    {
        for (long unsigned int i = 0; i < line.length(); i++)
        {
            if (line[i] == ' ')
                line[i] = '_';
            else if (line[i] == ',')
                line[i] = ' ';
        }
    }

    // Accesses correct location and added data to the correct month 
    void database::extract_summary(int index)
    {
        for (list<rawdata>::iterator list_it = rawdata_cache[index].begin(); list_it != rawdata_cache[index].end(); ++list_it)
            summary_cache[index][list_it->month - 1] += *list_it;
    }
