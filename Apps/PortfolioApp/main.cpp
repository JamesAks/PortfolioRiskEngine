#include <QApplication>

#include "MainWindow.hpp"

#include "AlphaVantageProvider.hpp"
#include "CSVDataProvider.hpp"
#include "GenericDataStore.hpp"


#include "include/PortfolioManager.hpp"

#include <Memory>

const char* API_KEY = getenv("ALPHA_VANTAGE_API_KEY");

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);

	auto market_data_provider = std::make_shared<AlphaVantageProvider>(API_KEY);
	auto market_data_store = std::make_shared<GenericDataStore>(market_data_provider);
	auto portfolio_manager = std::make_shared<PortfolioManager>()
		;
	MainWindow* window = new MainWindow();

	window->registerPortfolioManager(portfolio_manager);
	window->registerMarketDataStore(market_data_store);

	window->setWindowTitle("PortfolioApp");
	window->show();

	return app.exec();
}

