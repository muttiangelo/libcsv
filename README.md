# libcsv
Project building a .so library for reading and filtering a .csv file; Written in C++

Features

Read CSV data from files or strings
Select specific columns for processing
Apply multiple filter conditions
Support for various comparison operators:

Greater than (>)
Less than (<)
Equal to (=)
Not equal to (!=)
Greater than or equal to (>=)
Less than or equal to (<=)



Usage
Processing a CSV File
```cpp
  cppCopyprocessCsvFile("data.csv", "column1,column2,column3", "column1>10\ncolumn2!=value");
```
Processing CSV Data from String
```cpp
  cppCopyconst char* csvData = "header1,header2,header3\nvalue1,value2,value3";
  processCsv(csvData, "header1,header3", "header1>value1");
```

Filter Syntax
Filters can be combined using newline characters (\n). Each filter follows the format:
column_name operator value
Supported operators:

Requirements

C++ compiler with C++11 support
Standard Template Library (STL)

Author
Ângelo Mutti

Created
July 6, 2024
