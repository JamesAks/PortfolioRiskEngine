#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <qlistwidget.h>
#include <Memory>



namespace Ui {
class MainWindow;
}

class PortfolioManager;
class MarketDataStore;
class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:

        Ui::MainWindow* ui;
        std::shared_ptr<PortfolioManager> portfolio_manager;
        std::shared_ptr<MarketDataStore> market_data_store;

        void clearAnalyticsPage();

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        void registerPortfolioManager(std::shared_ptr<PortfolioManager>);
        void registerMarketDataStore(std::shared_ptr<MarketDataStore>);

        


    private slots:

        void onAddPortfolioClicked();
        void onRemovePortfolioClicked();
        void onPortfolioClicked(QListWidgetItem*);
        void onAnalyseClicked();
        
        void onAddPositionClicked();
        void onRemovePositionClicked();

        void onTimeFrameSelected();
        
};




#endif // MAINWINDOW_HPP
 