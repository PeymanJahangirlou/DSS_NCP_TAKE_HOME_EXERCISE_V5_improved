

/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Json Parser class:
 *		-	load json from web then read movies from home url (default mode)
 *		-	load json from web then read movies from erf url (default mode)
 *			-	then populates MovieContainer map.
 */

#ifndef JsonParser_h
#define JsonParser_h

#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <fstream>

#include <json\json.h>

#define CURL_STATICLIB
#include <curl\curl.h>

#pragma comment(lib,"Normaliz.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib, "Urlmon.lib")

#include "sdb_image.h"
#include "Movie.h"
#include "MovieItem.h"

using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::string;
using std::ostream;

/*
 *  Simple Json parser class which reads from json file
*/

class JsonParser
{
public:

	typedef unordered_map<int, MovieItem> MovieContainer;

	/* @brief default construct, sets default json url to M_urlLink */
	JsonParser();

	/** @brief read json file from url by refId **/
	MovieItem readByRefId(const string& refId);

	/** @brief read json file from home url **/
	MovieContainer readDefault();

	/** @brief  read default json file and appends refId to refIdSet **/
	unordered_set<string>  getRefIds();


private:

	enum class  Mode {default, refId};
	string	    M_urlLink;
	Json::Value M_root;
	Mode		M_mode{ Mode::default };

	const string M_homeUrl{ "https://cd-static.bamgrid.com/dp-117731241344/home.json" };
	const string M_refUrl{ "https://cd-static.bamgrid.com/dp-117731241344/sets/" };


	/** @brief  curl call back function **/
	static size_t M_curlWriteToString(void* buffer, size_t size, size_t nmemb, void* userp);

	/*
	 * @brief  read json file from url then M_root has everything.
	 * @return true upon successful read, otherwise false.
	 */
	bool M_read();

	/** @brief helper function -> iterates throug MovieItems and append each to MovieMap **/
	MovieItem M_populateMovieItem(const Json::Value& jsonMovieItems);


	/////////////////////////////// Utility Helper Functions /////////////////////////////////////

	/** @brief sets up curl to download image from provided imgUrl **/
	CURLcode setUpCurlToDownloadImage(const string& imgUrl, ostream& os);

	/** @brief download movie image from provided imgUrl to solution directory **/
	bool  downloadMovieImage(const string& imgUrl);

	/** @brief load image from directory
     ** @return MovieImage pointer
     **/
	std::shared_ptr<MovieImage> loadImageFromDirectory(const string& imgPath);

	/** @brief helper function -> saves loaded image to solution direction **/
	static size_t M_curlSaveImgToFile(void* buffer, size_t size, size_t nmemb, void* userp);


	/** @brief helper method to download image from imgUrl then construct MovieImage object
	 ** @return MovieImage object
	 **/
	std::shared_ptr<MovieImage> createMovieObject(const std::string& imgUrl);

};



#endif