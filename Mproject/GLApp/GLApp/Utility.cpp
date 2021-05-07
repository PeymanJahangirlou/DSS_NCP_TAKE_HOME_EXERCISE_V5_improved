/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Utility class -- contains utility functions
 */

#include "Utility.h"




/*@brief displays selected movie title on terminal */
void  Utility::printSelectedMovieInfo(JsonParser::MovieContainer& moviesMap, const std::pair<int, int> selectedMoviePos)
{
	cout << "You selected:" << endl;
	cout << moviesMap[selectedMoviePos.first].categoryName() << endl;
	auto moviesList_ptr = moviesMap[selectedMoviePos.first];
	cout << "\t" << moviesList_ptr.at(selectedMoviePos.second)->title() << endl;
	cout << endl;
}

