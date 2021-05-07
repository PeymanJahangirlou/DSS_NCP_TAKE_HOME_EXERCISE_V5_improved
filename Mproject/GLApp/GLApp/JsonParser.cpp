/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Json Parser class:
 *		-	load json from web then read movies from home url (default mode)
 *		-	load json from web then read movies from erf url (default mode)
 *			-	then populates MovieContainer map.
 */

#include "JsonParser.h"


/* @brief default construct, sets default json url to M_urlLink */
JsonParser::JsonParser()
{
	M_urlLink = M_homeUrl;
	M_mode = Mode::default;
}

/** @brief read json file from url by refId **/
MovieItem JsonParser::readByRefId(const string& refId)
{
	M_mode = Mode::refId;
	M_urlLink = M_refUrl + refId + ".json";
	if (!M_read())
		return {};

	auto subtreeVal = M_root["data"].getMemberNames().front();
	auto movieSetsubtree = M_root["data"][subtreeVal];
	return M_populateMovieItem(movieSetsubtree);
}

 /** @brief  read default json file and appends each refId into refIdSet
  ** @return an unordered_set containing all refIds
  **/
unordered_set<string> JsonParser::getRefIds()
{
	unordered_set<string> refIdSet{};
	if (M_mode == Mode::default &&  M_read()) {
		auto containers = M_root["data"]["StandardCollection"]["containers"];
		auto iter = containers.begin();
		for (; iter != containers.end(); ++iter) {
			auto refId = (*iter)["set"]["refId"];
			if (refId) 
				refIdSet.insert(refId.asString());
		}// end for loop.
	}// end if

	return refIdSet;
}


/** @brief read json file from url by refId
 ** @return movie map containing all movies
*/
JsonParser::MovieContainer JsonParser::readDefault()
{
	M_mode = Mode::default;
	M_urlLink = M_homeUrl;
	if (!M_read()) {
		std::cout << "JSONPARSER::READ::FAILED" << std::endl;
		return {};
	}
	auto movieContainersList = M_root["data"]["StandardCollection"]["containers"];
	MovieContainer moviesMap{};
	for (int i = 0; i < movieContainersList.size(); i++) {
		auto movieSetSubtree = movieContainersList[i]["set"];
		moviesMap[i] = M_populateMovieItem(movieSetSubtree);
	}
	return moviesMap;
}



///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Private Fuction Section /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/** @brief  curl call back function **/
size_t JsonParser::M_curlWriteToString(void* buffer, size_t size, size_t nmemb, void* userp)
{
	if (!userp)
		return 0;

	auto str_ptr = reinterpret_cast<std::string*>(userp);
	str_ptr->append(reinterpret_cast<char*>(buffer), size * nmemb);
	return size * nmemb;
}

/*
 * @brief  read json file from url then M_root has everything.
 * @return true upon successful read, otherwise false.
 */
bool JsonParser::M_read()
{
	auto curl_ptr = curl_easy_init();
	std::string strBuffer{};
	CURLcode errorCode(CURLE_FAILED_INIT);
	if (curl_ptr) {
		if (   CURLE_OK == (curl_easy_setopt(curl_ptr, CURLOPT_URL, M_urlLink.c_str()))
			&& CURLE_OK == (curl_easy_setopt(curl_ptr, CURLOPT_WRITEFUNCTION, M_curlWriteToString))
			&& CURLE_OK == (curl_easy_setopt(curl_ptr, CURLOPT_WRITEDATA, &strBuffer))) {
				 errorCode = curl_easy_perform(curl_ptr);
		}
	}
	curl_easy_cleanup(curl_ptr);
	Json::Reader reader;
	bool successfull{ true };
	if (errorCode != CURLE_OK || (!reader.parse(strBuffer, M_root))) {
		std::cout << "JSON::ERROR Json::Reader::parse() Failed..." << std::endl;
		successfull = false;
	}
	return successfull;
}


/** @brief helper function -> iterates throug Movie Set Subtree and return a MovieItem.
 ** @return MovieItem containing all movies
 **/
MovieItem JsonParser::M_populateMovieItem( const Json::Value & movieSetSubtree)
{
	const string movieSetCategoryName =
		movieSetSubtree["text"]["title"]["full"]["set"]["default"]["content"].asString();
	auto jsonMovieItems = movieSetSubtree["items"];
	MovieItem movieItem(movieSetCategoryName);
	// create movie object
	for (auto item : jsonMovieItems) {
		// create movie object.
		auto imageTileSubtree = item["image"]["tile"];
		auto tileFirstMember = imageTileSubtree.getMemberNames().front();
		imageTileSubtree = imageTileSubtree[tileFirstMember];
		auto tileChildMember = imageTileSubtree.getMemberNames().front();
		imageTileSubtree = imageTileSubtree[tileChildMember];
		auto movieImageUrl = imageTileSubtree["default"]["url"].asString();

		auto fullTitleSubtree = item["text"]["title"]["full"];
		auto membersList = fullTitleSubtree.getMemberNames();
		auto firstMemberStr = membersList.front();

		auto movieTitle = fullTitleSubtree[firstMemberStr]["default"]["content"].asString();
		auto movieLanguage = fullTitleSubtree[firstMemberStr]["default"]["language"].asString();
		auto movieSourceEntity = fullTitleSubtree[firstMemberStr]["default"]["sourceEntity"].asString();

		auto movieImage_ptr = createMovieObject(movieImageUrl);
		auto movie_ptr = std::make_shared<Movie>(movieTitle,
												 movieLanguage,
												 movieSourceEntity,
												 movieImage_ptr);
		movieItem.insertMovie(movie_ptr);
	}// end for loop
	return movieItem;

}


/** @brief download movie image from provided imgUrl to solution directory **/
bool  JsonParser::downloadMovieImage(const string& imgUrl)
{
	// first download image to solution directory
	std::ofstream ofs("movieImage.jpg", std::ostream::binary);
	if (CURLE_OK != setUpCurlToDownloadImage(imgUrl, ofs))
		return false;

	return true;
}

/** @brief load image from directory
 * @return MovieImage pointer
 */
std::shared_ptr<MovieImage> JsonParser::loadImageFromDirectory(const string& imgPath)
{ 
    int width, height, numColCh;
    unsigned char* image_ptr = stbi_load(imgPath.c_str(), &width, &height, &numColCh, 0);
    return std::make_shared<MovieImage>(image_ptr, width, height); 
}



/** @brief sets up curl to download image from provided imgUrl **/
CURLcode JsonParser::setUpCurlToDownloadImage (const string & imgUrl, ostream & os, long timeout)
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURLcode code(CURLE_FAILED_INIT);
	auto curl_ptr = curl_easy_init();

	if (curl_ptr)
	{
		if (   CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_WRITEFUNCTION, &M_curlSaveImgToFile))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_NOPROGRESS, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_FOLLOWLOCATION, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_FILE, &os))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_TIMEOUT, timeout))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_URL, imgUrl.c_str())))
		{
			code = curl_easy_perform(curl_ptr);
		}
		curl_easy_cleanup(curl_ptr);
	}
	curl_global_cleanup();
	return code;
}


/** @brief helper function -> saves loaded image to solution direction **/
size_t JsonParser::M_curlSaveImgToFile(void* buffer, size_t size, size_t nmemb, void* userp)
{
	if (userp) {
		ostream & os = *static_cast<ostream*>(userp);
		if (os.write(static_cast<char*>(buffer), size * nmemb))
			return  size * nmemb;
	}

	return 0;
}

/** @brief helper method to download image from imgUrl then construct MovieImage object
 ** @return MovieImage object
 **/
std::shared_ptr<MovieImage> JsonParser::createMovieObject(const std::string & imgUrl)
{
	// only download movies images once, so you can access them later on without downloading
	if (!downloadMovieImage(imgUrl)) {
		std::cout << "LOASTEXTURE::Utility::downloadMovieImage() Failed\n" << std::endl;
		return nullptr;
	}
   const string imgPath{ "movieImage.jpg" };
   std::shared_ptr<MovieImage> movieImage_ptr = loadImageFromDirectory(imgPath);
   if (!movieImage_ptr) {
		std::cout << "LOASTEXTURE::Utility::loadImageFromDirectory() Failed\n" << std::endl;
		return nullptr;
   }

   return movieImage_ptr;
}
/////////////////////////////////////////// End of JsonParser.cpp /////////////////////////////////////////////
