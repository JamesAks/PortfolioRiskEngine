#include "../includes/TimeSeries.hpp"

TimeSeries::TimeSeries(vector<chrono::year_month_day> d, vector<double> p): dates{d}, prices{p}{};

pair<vector<chrono::year_month_day>,vector<double>> TimeSeries::view(){return pair{dates,prices};};

