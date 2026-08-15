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

    private:

        void calculateNPV() override;

    public:

        Stock(std::string, std::shared_ptr<LatestPrice>, std::shared_ptr<HistoricData>);        
};

#endif // !STOCK_HPP
