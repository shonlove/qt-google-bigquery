# Introduction #

Information about use Google BigQuery, testing platforms, Qt versions.

# Details #

### Google API ###

Project **qt-google-bigquery** uses Google BigQuery API v2.

How it works:
Work with Google-API perfomed by send https-request with using QNetworkAccessManager (see files _manager\_big\_query.h_ and _manager\_big\_query.cpp_)

API features used in this project:
|Get datasets list|
|:----------------|
|Get tables list for dataset|
|Get columns for table|
|Get table data   |
|Runs a query     |

File **[HowToRegisterYourAppIicationInGoogle](http://code.google.com/p/qt-google-bigquery/wiki/HowToRegisterYourApplicationInGoogle)** describes how register your own application on Google.

### Tested platforms ###
Project was tested on:
| **OS** | **Qt version** |
|:-------|:---------------|
| Linux 64bit | Qt 4.8.2       |

# Various comments #

This application can be compiled on Linux, MacOS, Windows. For Windows you can use either MingW compiler or MSVC compiler.