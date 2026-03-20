#include <vector>
#include<chrono>

using namespace std;
using namespace chrono;

class TimeSeries{

    private:
        
        std::vector<year_month_day> dates;
        std::vector<double> prices;


    public:

        TimeSeries(vector<year_month_day> d, vector<double> p);
        pair<vector<year_month_day>,vector<double>> view();


};