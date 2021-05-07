/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Utility class -- contains utility functions
 */

#ifndef Utility_H
#define Utility_H

#define CURL_STATICLIB
#include <curl\curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"advapi32.lib")

#include "sdb_image.h"
#include "MovieImage.hpp"
#include "MovieItem.h"
#include "JsonParser.h"
#include "Movie.h"

using std::string;
using std::ostream;
using std::cout;
using std::endl;

class Utility
{
public:



	/*@brief displays selected movie title on terminal */
	static void printSelectedMovieInfo(JsonParser::MovieContainer& moviesMap, const std::pair<int, int> selectedMoviePos);

	/*@brief clears terminal screen*/
	inline static void clearTerminalScreen() { std::cout << string(200, '\n'); }

	/* @brief display loading message while load from json file **/
	inline static void displayMessage(const string & msg) { std::cout << msg << std::endl; }
};

#endif // !Utility_H
