// Header file defining all classes annd structs for later use 

#ifndef SUPPORT
#define SUPPORT

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

struct location
{
  string city;
  string state;
  string geocode;

  bool operator<(const location &) const;
  void print() const;
};

struct rawdata
{
  int month;
  float precip;
  int temp;

  void print();
};

struct summary
{
  int N;

  float precip_avg;
  float precip_max;
  float precip_min;

  float temp_avg;
  float temp_max;
  float temp_min;

  summary();
  void operator+=(const rawdata &);
  void print(string) const;
};

class database
{
public:
  void init_rawdata(const string &);
  void print_rawdata();

  void init_summary();
  void print_summary(string);

private:
  void extract_rawdata(string &);
  void extract_summary(int);

  map<location, int> location_map;
  vector<list<rawdata>> rawdata_cache;

  unordered_map<string, set<string>> state_map;
  unordered_map<string, location> geocode_map;
  vector<vector<summary>> summary_cache;
};

#endif
