#include "MovieItem.h"




//////////////////////////////////////////////////////////////////////
//////////////////// Public Functions Section ////////////////////////
//////////////////////////////////////////////////////////////////////


	/* @brief constructor */
MovieItem::MovieItem(const string& catergoryName, const MovieList& movieList)
	:M_CategoryName(catergoryName)
{
	if (!movieList.empty()) {
		std::copy(movieList.begin(), movieList.end(), std::back_inserter(M_movieList));
		M_firstMovieIter = M_movieList.begin();
	}


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
	M_firstMovieIter = M_movieList.begin();
	M_lastMovieIter = M_movieList.rbegin();
}


bool MovieItem::isIndexValid(const int index) const
{
	return (index >= 0 && index < M_movieList.size());
}



//////////////////////////////////////////////////////////////////////
//////////////////// Private Functions Section ////////////////////////
//////////////////////////////////////////////////////////////////////

 std::shared_ptr<Movie> MovieItem::M_nextMovie(int col)
{
	 if ( col < 0 || col >= M_movieList.size() || col + 1 >= M_movieList.size())
		 return nullptr;

	 return M_movieList[col+1];
}

 std::shared_ptr<Movie> MovieItem::M_previousMovie(int col)
{
	 if ( col < 0 || col >= M_movieList.size() || col - 1 < 0)
		 return nullptr;

	 return M_movieList[col - 1];
}


MovieItem::MovieListIterator MovieItem::M_nextMovie(MovieItem::MovieListIterator pos)
{
	if (pos == M_movieList.end() || std::next(pos) == M_movieList.end())
		return pos;

	return std::next(pos);

}
MovieItem::MovieListIterator MovieItem::M_previousMovie(MovieItem::MovieListIterator pos)
{
	if (pos == M_movieList.end() || std::prev(pos) == M_movieList.end())
		return pos;

	return std::prev(pos);
}

//////////////////////////////////////////////////////////////////////
//////////////////// Operator Overloading Functions Section ////////////////////////
//////////////////////////////////////////////////////////////////////

std::shared_ptr<Movie> MovieItem::operator[](const int index)
{
	if (M_movieList.empty())
		return nullptr;

	if (index >= 0 && index < M_movieList.size())
		return M_movieList[index];

	return nullptr;
}
