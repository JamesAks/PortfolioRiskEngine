#ifndef STOCK_HPP
#define STOCK_HPP

#include "Equity.hpp"

#include <chrono>
#include <string>
#include <vector>
#include <memory>

class LatestPrice;
class HistoricData;

class Stock : public Equity {

    public:

        Stock(std::string, std::shared_ptr<LatestPrice>, std::shared_ptr<HistoricData>);

        void calculateNPV() override;
};

#endif // !STOCK_HPP
