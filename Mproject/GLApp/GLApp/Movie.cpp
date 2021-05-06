/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Simple Movie Class:
 */

#include "Movie.h"

/*@brief constructs a movie */
Movie::Movie(   const std::string& title,
			    const std::string& language,
				const std::string& sourceEntity,
				const std::string& imgUrl)
{
	M_title = title;
	M_language = language;
	M_sourceEntity = sourceEntity;
	M_imgUrl = imgUrl;
	M_isFirstItemOnViewPort = false;
	M_isLastItemOnViewPort  = false;
}

/* @brief copy constructor */
Movie::Movie(const Movie& other) 
{
	M_title = other.M_title;
	M_language = other.M_language;
	M_sourceEntity = other.M_sourceEntity;
	M_imgUrl = other.M_imgUrl;
	M_isFirstItemOnViewPort = other.M_isFirstItemOnViewPort;
	M_isLastItemOnViewPort  = other.M_isLastItemOnViewPort;
}
