#include "Asset.hpp"

#include <chrono>
#include <memory>
#include <vector>

class HistoricData;
class TimeSeries;
class LatestPrice;

class Stock: public Asset {


	public:

        Stock(std::string, std::shared_ptr<LatestPrice>, std::shared_ptr<HistoricData>);

        void calculateNPV() override;
        const std::string& symbol() const;
};
