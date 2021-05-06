/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Movie Item class: a container for Movies
 */



#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <iterator>

#include "Movie.h"

using std::string;
using std::vector;
using std::pair;
using std::optional;

class MovieItem
{
public:

	typedef vector<std::shared_ptr<Movie>> MovieList;
	typedef MovieList::iterator			   MovieListIterator;

	/* @brief constructor */
	MovieItem(const string& catergoryName = {}, const MovieList& movieList = {});
	~MovieItem();


	// returns movieCounts;
	int movieCounts() const { return M_movieList.size(); }

	// getters.
	auto row() const    { return M_row; }
	auto column() const { return M_column; }
	auto lastMovieOnScreen() { return M_lastMovieIter; }
	auto firstMovieOnScreen() { return M_lastMovieIter; }
	auto at(const int col) const { return  M_movieList.at(col); }

	// setters
	void setRow(const int row)    { M_row = row; }
	void setColumn(const int col) { M_column = col; }
	auto  categoryName() const { return M_CategoryName; }
	void setCategoryName(const string& categoryName) { M_CategoryName = categoryName; }

	// next & previous Movie 
	auto nextMovie(const int col)		{ return M_nextMovie(col); }
	auto previousMovie(const int col)   { return M_previousMovie(col); }

	const auto nextMovie(const int col) const
	{ return (const_cast<MovieItem*>(this))->M_nextMovie(col); }
	const auto previousMovie(const int col) const
	{ return (const_cast<MovieItem*>(this))->M_previousMovie(col); }

	// movie list iterators.
	auto nextMovie(MovieListIterator pos)		{ return M_nextMovie(pos); }
	auto previousMovie(MovieListIterator pos)   { return M_previousMovie(pos); }

	auto end() { return M_movieList.end(); }
	auto begin() { return M_movieList.begin(); }


	/** @insert a new movie into movie list **/
	void insertMovie(std::shared_ptr<Movie> movie_ptr);

	/** operator overloading **/
	std::shared_ptr<Movie> operator[] (const int index);

	bool isIndexValid(const int index) const;





private:

	string M_CategoryName;
	MovieListIterator	M_firstMovieIter; // points to the first element on the screen.
	MovieList::reverse_iterator M_lastMovieIter;  // points to the last element on the screen.
	MovieList			M_movieList;
	optional<int> M_row;
	optional<int> M_column;

	std::shared_ptr<Movie> M_nextMovie(int col);
	std::shared_ptr<Movie> M_previousMovie(int col);
	MovieListIterator	   M_nextMovie(MovieListIterator pos);
	MovieListIterator	   M_previousMovie(MovieListIterator pos);

	
	
};
