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


    public:

        Stock(std::string);        
};

#endif // !STOCK_HPP
