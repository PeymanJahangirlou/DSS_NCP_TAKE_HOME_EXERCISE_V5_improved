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
				std::shared_ptr <MovieImage> imagePtr)
{
	M_title = title;
	M_language = language;
	M_sourceEntity = sourceEntity;
	M_imagePtr = imagePtr;

}

/* @brief copy constructor */
Movie::Movie(const Movie& other) 
{
	M_title = other.M_title;
	M_language = other.M_language;
	M_sourceEntity = other.M_sourceEntity;
	M_imagePtr = other.M_imagePtr;
}
