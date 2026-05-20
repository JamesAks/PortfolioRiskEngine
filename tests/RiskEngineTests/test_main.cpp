#include "PortfolioManager.hpp"

void main() {
	 
	PortfolioManager port_manager;
	port_manager.createPortfolio("TestPort1");
	port_manager.addPosition("Test1", 15, "APPL", 150, PositionType::LONG, "TestPort1");
	port_manager.addPosition("Test2", 5, "IBM", 100, PositionType::SHORT, "TestPort1");

	port_manager.analysePortfolio("TestPort1",TimeFrame::DAILY);

	port_manager.removePosition("Test1", "TestPort1");
	std::cout << "Number of positions in \"TestPort1\": " << port_manager.portfolioSize("TestPort1") << "\n";
	port_manager.removePortfolio("TestPort1");
	std::cout << "Number of Portfolios: " << port_manager.numbPortfolio() << "\n";
	
}