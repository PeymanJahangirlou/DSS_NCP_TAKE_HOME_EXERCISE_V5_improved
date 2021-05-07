
/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Movie Item class: a container for Movies
 */

#include "MovieItem.h"



//////////////////////////////////////////////////////////////////////
//////////////////// Public Functions Section ////////////////////////
//////////////////////////////////////////////////////////////////////


/* @brief constructor */
MovieItem::MovieItem(const string& catergoryName, const MovieList& movieList)
	:M_CategoryName(catergoryName)
{
	if (!movieList.empty()) 
		std::copy(movieList.begin(), movieList.end(), std::back_inserter(M_movieList));
}

MovieItem::~MovieItem()
{
	for (auto& movie : M_movieList) {
		movie.reset();
	}
}

/** @insert a new movie into movie list **/
void MovieItem::insertMovie(std::shared_ptr<Movie> movie_ptr)
{
	M_movieList.push_back(movie_ptr);
}



/** @brief validate M_movieList index **/
bool MovieItem::isIndexValid(const int index) const
{
	return (index >= 0 && index < M_movieList.size());
}


//////////////////////////////////////////////////////////////////////
//////////////////// Operator Overloading Functions Section ////////////////////////
//////////////////////////////////////////////////////////////////////

/** @brief subscript operator overload **/
std::shared_ptr<Movie> MovieItem::operator[](const int index)
{
	if (M_movieList.empty())
		return nullptr;

	if (index >= 0 && index < M_movieList.size())
		return M_movieList[index];

	return nullptr;
}
