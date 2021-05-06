/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Simple Movie Class:
 */

#ifndef Movie_H
#define Movie_H

#include <string>

/* @brief simple movie class */
class Movie
{
public:
	/*@brief constructs a movie */
	Movie(const std::string& title,
		  const std::string& language,
		  const std::string& sourceEntity,
		  const std::string& imgUrl);

	/* @brief copy constructor */
	Movie(const Movie& other);
		
	/* inline getters */
	inline const std::string& title() const { return M_title; }
	inline const std::string& language() const { return M_language; }
	inline const std::string& sourceEntity() const { return M_sourceEntity; }
	inline const std::string& imgUrl() const { return M_imgUrl; }
	inline bool isLastItemOnViewPort() const { return M_isLastItemOnViewPort; }
	inline bool isFirstItemOnViewPort() const { return M_isFirstItemOnViewPort;; }

	/* inline setters */
	inline void settitle(const std::string& title) { M_title = title; }
	inline void setLanguage(const std::string& language) { M_language = language; }
	inline void setSourceEntity(const std::string& sourceEntity) { M_sourceEntity = sourceEntity; }
	inline void setImgUrl(const std::string& imgUrl) { M_imgUrl = imgUrl; }
	inline void setIsLastItemOnViewPort(bool on) { M_isLastItemOnViewPort = on; }
	inline void setIsFirstItemOnViewPort(bool on) { M_isFirstItemOnViewPort = on; }
private:
	std::string		M_title;
	std::string		M_language;
	std::string		M_sourceEntity;
	std::string		M_imgUrl;
	bool	M_isLastItemOnViewPort;
	bool	M_isFirstItemOnViewPort;


};


#endif
