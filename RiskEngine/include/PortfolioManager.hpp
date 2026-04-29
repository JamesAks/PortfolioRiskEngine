#include "../include/Portfolio.hpp"
#include "../include/Adapter.hpp"

class PortfolioManager {

private:

    std::vector<Portfolio> portfolios = {};

public:

    PortfolioManager();
    PortfolioManager(std::vector<Portfolio>);

};