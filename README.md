# Weather Data Processor

## Overview

This program is designed to process and analyze weather data from a CSV file. It enables users to quickly look up weather statistics and generate reports based on the input data.

## Features

- Parses weather data from a CSV file.
- Provides options for viewing raw data or summarized information.
- Interactive prompt for user input to display summarized weather data.

## Requirements

- C++ compiler (e.g., g++, clang++)
- Standard C++ library

## Compilation

Compile the program using a C++ compiler by running:

```bash
g++ -o weather_processor main.cpp Support.cpp -std=c++11
```

## Usage 

Run the program with the following command:

```bash 
./weather_processor [-rawdata] <datafile.csv>
```
- -rawdata: Optional flag to display raw data instead of a summary.
- <datafile.csv>: The CSV file containing weather data.
