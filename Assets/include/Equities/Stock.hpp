#ifndef STOCK_HPP
#define STOCK_HPP

#include "Asset.hpp"
#include "Equity.hpp"

#include <chrono>
#include <memory>
#include <vector>



class HistoricData;
class TimeSeries;
class LatestPrice;

class Stock : public Equity {


public:

    Stock(std::string, std::shared_ptr<LatestPrice>, std::shared_ptr<HistoricData>);

    void calculateNPV() override;
};

#endif // !STOCK_HPP


