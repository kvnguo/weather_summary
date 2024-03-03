// This is a program that takes and processes weather data from a cvs file for quick lookup 

#include "Support.h"

using namespace std;

int main(int argc, char *argv[])
{
   // Check command line inputs
   if (argc > 3 || argc < 2)
   {
      cerr << "usage: ./executable [-rawdata] datafile" << endl;
      return 1;
   }
   string filename = argv[argc - 1];
   bool rawdata = string(argv[1]) == "-rawdata";
   if (argc == 3 && !rawdata)
   {
      cerr << "usage: ./executable [-rawdata] datafile" << endl;
      return 1;
   }

   database database;
   database.init_rawdata(filename);
   

   if (rawdata)
      database.print_rawdata();
   else
   {
      database.init_summary();
      while (!cin.eof())
      {
         string userInput;
         cout << "print> ";
         cin >> userInput;
         database.print_summary(userInput);
      }
      cout << endl;
   }

   return 0;
}