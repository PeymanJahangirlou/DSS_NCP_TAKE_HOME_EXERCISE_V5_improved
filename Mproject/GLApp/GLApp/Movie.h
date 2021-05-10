/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Simple Movie Class:
 */

#ifndef Movie_H
#define Movie_H

#include <string>
#include <memory>

#include "MovieImage.hpp"

/* @brief simple movie class */
class Movie
{
public:
	/*@brief constructs a movie */
	Movie(  const std::string& title,
			const std::string& language,
			const std::string& sourceEntity,
			std::shared_ptr <MovieImage> imagePtr  = nullptr);

	/* @brief copy constructor */
	Movie(const Movie& other);
		
	/* inline getters */
	inline const std::string& title() const { return M_title; }
	inline const std::string& language() const { return M_language; }
	inline const std::string& sourceEntity() const { return M_sourceEntity; }
	inline const auto image() const { return M_imagePtr; }

	/* inline setters */
	inline void settitle(const std::string& title) { M_title = title; }
	inline void setLanguage(const std::string& language) { M_language = language; }
	inline void setSourceEntity(const std::string& sourceEntity) { M_sourceEntity = sourceEntity; }
	inline void setImage(std::shared_ptr<MovieImage> imagePtr) { M_imagePtr = imagePtr; }

private:
	std::string		M_title;
	std::string		M_language;
	std::string		M_sourceEntity;

	std::shared_ptr<MovieImage> M_imagePtr;


};


#endif
