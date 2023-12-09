/*
12-08-2023 Edit
//This is web scraping program written in c++
//By VM (me :)

Prerequisites
Before diving into coding, you need to:

1. Have a C++ compiler
2. Set up the vcpkg C++ package manager
3. Install CMake

web scraping libraries we need,
1. CPR: A modern C++ HTTP client library inspired by the Python Requests project. 
It is a wrapper of libcurl that provides an easy-to-understand interface, 
built-in authentication capabilities, and support for asynchronous calls.

2. libxml2: A powerful and full-featured library for parsing XML and HTML documents originally developed for Gnome. 
It supports DOM manipulation via XPath selectors.



Side notes:
1) CMakeLists.txt and launch.json

1.1) Project name in CMakeLists.txt needs to be the same as the .exe name in launch.json in order to run and debug.
CMake introductory and edit CMakeLists.txt can be read at
 https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html
and https://cmake.org/cmake/help/book/mastering-cmake/chapter/Writing%20CMakeLists%20Files.html

1.2) Change C++ Settings to use C++20 standard,
     Gear -> Command Palette (Ctrl+Shift+P) -> Type "C/C++:Edit Configurations (UI)"
                                            -> Under "C++ Standard" -> Choose "C++20" -> Save

2) CMakeLists.txt and CMakeCache.txt

Changing anything in CMakeLists.txt will automatically change CMakeCache.txt.  CMakeCache.txt is in "build" folder.

3) Check configuration and resolve configuration errors

Run >CMake: Configure, in CMakeCache.txt the default CMAKE_GENERATOR:INTERNAL is "Visual Studio 16 2019".  If other version of Visual Studio 
other than version 16 2019 is installed, such as Visual Studio 17 2022, there will be a configure error says:
CMake Error at CMakeLists.txt:11(project):
Generator

Visual Studio 16 2019

could not find any instance of Visual Studio.
--Configuring incomplete, errors occurred!

Solution to this is to 
Step 1) Change CMAKE_GENERATOR:INTERNAL value to Visual Studio 17 2022.
Legit CMAKE_GENERATOR can be found at https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#id9

Step 2) Change CMake Settings in Visual Studio Code,
        Gear -> Command Palette (Ctrl+Shift+P) -> Type "Open CMake Tools Extension Settings"
                                                -> Under "User" -> Find "CMake:Generator" -> Enter "Visual Studio 17 2022"
        Reference to change CMake Settings in Visual Studio can be read here,
        https://learn.microsoft.com/en-us/cpp/build/cmakesettings-reference?view=msvc-170#configurations

Step 3) Close project and re-open project to load changes of settings from above steps.

4) To run program, type and select the following steps,

1) >CMake:Configure
On Windows, opt for “Visual Studio Build Tools 2019 Release – x86_amd64”:

2) >CMake:Build

3) Run the compiled program from the “Run & Debug” section or by pressing F5

End 12-08-2023 Edit
*/

// scraper.cpp

#include <iostream>
#include "cpr/cpr.h"
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"
#include <vector>

// define a struct where to store the scraped data
struct IndustryCard
{
    std::string image;
    std::string url;
    std::string name;
};

int main()
{
    // define the user agent for the GET request
    cpr::Header headers = {{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36"}};
    // make an HTTP GET request to retrieve the target page
    cpr::Response response = cpr::Get(cpr::Url{"https://brightdata.com/"}, headers);
    
    // parse the HTML document returned by the server
    htmlDocPtr doc = htmlReadMemory(response.text.c_str(), response.text.length(), nullptr, nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
    
    // define an array to store all retrieved data
    std::vector<IndustryCard> industry_cards;
    // set the libxml2 context to the current document
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    // select all industry card HTML elements
    // with an XPath selector
    xmlXPathObjectPtr industry_card_html_elements = xmlXPathEvalExpression((xmlChar *)"//div[contains(@class, 'section_cases_row_col_item')]", context);

    // iterate over the list of industry card elements
    for (int i = 0; i < industry_card_html_elements->nodesetval->nodeNr; ++i)
    {
        // get the current element of the loop
        xmlNodePtr industry_card_html_element = industry_card_html_elements->nodesetval->nodeTab[i];
        // set the libxml2 context to the current element
        // to limit the XPath selectors to its children
        xmlXPathSetContextNode(industry_card_html_element, context);

        xmlNodePtr image_html_element = xmlXPathEvalExpression((xmlChar *)".//figure/a/img", context)->nodesetval->nodeTab[0];
        std::string image = std::string(reinterpret_cast<char *>(xmlGetProp(image_html_element, (xmlChar *)"data-lazy-src")));

        xmlNodePtr url_html_element = xmlXPathEvalExpression((xmlChar *)".//figure/a", context)->nodesetval->nodeTab[0];
        std::string url = std::string(reinterpret_cast<char *>(xmlGetProp(url_html_element, (xmlChar *)"href")));

        xmlNodePtr name_html_element = xmlXPathEvalExpression((xmlChar *)".//div[contains(@class, 'elementor-image-box-title')]/a", context)->nodesetval->nodeTab[0];
        std::string name = std::string(reinterpret_cast<char *>(xmlNodeGetContent(name_html_element)));

        // instantiate an IndustryCard struct with the collected data
        IndustryCard industry_card = {image, url, name};
        // add the object with the scraped data to the vector
        industry_cards.push_back(industry_card);
    }

    // free up the resource allocated by libxml2
    xmlXPathFreeObject(industry_card_html_elements);
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);

    // initialize the CSV output file
    std::ofstream csv_file("output.csv");
    // write the CSV header
    csv_file << "url,image,name" << std::endl;

    // poupulate the CSV output file
    for (IndustryCard industry_card : industry_cards)
    {
        // transfrom each industry card record to a CSV record
        csv_file << industry_card.url << "," << industry_card.image << "," << industry_card.name << std::endl;
    }

    // free up the file resources
    csv_file.close();

    return 0;
}