#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <qlistwidget.h>
#include <Memory>

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>




namespace Ui {
class MainWindow;
}

class EfficientFrontierGraph;
class PortfolioManager;
class MarketDataStore;

class QQuickWidget;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:

        Ui::MainWindow* ui;
        std::shared_ptr<PortfolioManager> portfolio_manager;
        std::shared_ptr<MarketDataStore> market_data_store;

        std::shared_ptr<EfficientFrontierGraph> efficient_frontier_graph;

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
        void onPositionClicked(QListWidgetItem*);

        void onTimeFrameSelected();
        
};

#endif // MAINWINDOW_HPP