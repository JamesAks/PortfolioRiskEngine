#include <QApplication>

#include "MainWindow.hpp"

#include "AlphaVantageProvider.hpp"
#include "CSVDataProvider.hpp"
#include "GenericDataStore.hpp"
#include "MarketDataStore.hpp"
#include "PortfolioManager.hpp"

#include <thread>
#include <Memory>

const char* API_KEY = getenv("ALPHA_VANTAGE_API_KEY");

void updateStore(MarketDataStore& market_data_store) { 
	
	while (true) {

		std::this_thread::sleep_for(std::chrono::minutes(3));
		market_data_store.update();
	} 
}

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);

	auto market_data_provider = std::make_shared<AlphaVantageProvider>(API_KEY);
	//auto market_data_provider = std::make_shared<CSVDataProvider>();
	auto market_data_store = std::make_shared<GenericDataStore>(market_data_provider);

	// Asynchronous and automatic updating of the market data store.
	std::jthread updating_thread(updateStore, std::ref(*market_data_store));

	auto portfolio_manager = std::make_shared<PortfolioManager>();
	MainWindow* window = new MainWindow();

	portfolio_manager->registerMarketDataStore(market_data_store.get());

	window->registerPortfolioManager(portfolio_manager);
	window->registerMarketDataStore(market_data_store);

	window->setWindowTitle("PortfolioApp");
	window->show();

	return app.exec();
}

