#ifndef MovieItem_H
#define MovieItem_H

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
	auto at(const int col) const { return  M_movieList.at(col); }
	int lastSelected() const { return M_lastSelected; }

	// setters
	auto  categoryName() const { return M_CategoryName; }
	void setCategoryName(const string& categoryName) { M_CategoryName = categoryName; }
	void setLastSelected(const int col) { M_lastSelected = col; }

	/** @insert a new movie into movie list **/
	void insertMovie(std::shared_ptr<Movie> movie_ptr);

	/** @brief validate M_movieList index **/
	bool isIndexValid(const int index) const;

	/** operator overloading **/
	std::shared_ptr<Movie> operator[] (const int index);

private:
	MovieList			M_movieList;
	string				M_CategoryName;
	int					M_lastSelected{ -1 };
	
};

#endif
