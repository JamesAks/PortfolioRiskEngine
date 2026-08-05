#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "MarketData.hpp"

#include "AddPositionDialog.hpp"
#include "CreatePortfolioDialog.hpp"
#include "PortfolioManager.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"
#include "RemoveFromListDialog.hpp"

#include <qstring.h>

Q_DECLARE_METATYPE(TimeFrame)

// ----- MainWindow -----

// --- Private Members ---

void MainWindow::clearAnalyticsPage() {

    ui->portfolio_risk_output->clear();
    ui->sharpe_ratio_output->clear();
    ui->total_return_output->clear();

    ui->timeframe_box->setCurrentIndex(-1);
    ui->analyse_button->setEnabled(false);

    ui->parametric_95->clear();
    ui->parametric_99->clear();
    ui->parametric_995->clear();
    ui->parametric_999->clear();

    ui->historical_95->clear();
    ui->historical_99->clear();
    ui->historical_995->clear();
    ui->historical_999->clear();
}

// --- Public Members ---
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(
        ui->add_portfolio_button,
        &QPushButton::clicked,
        this,
        &MainWindow::onAddPortfolioClicked
    );

    connect(
        ui->remove_portfolio_button,
        &QPushButton::clicked,
        this,
        &MainWindow::onRemovePortfolioClicked
    );

    connect(
        ui->portfolio_list,
        &QListWidget::itemClicked,
        this,
        &MainWindow::onPortfolioClicked
    );

    connect(
        ui->analyse_button,
        &QPushButton::clicked,
        this,
        &MainWindow::onAnalyseClicked
    );

    connect(
        ui->add_position_button,
        &QPushButton::clicked,
        this,
        &MainWindow::onAddPositionClicked
    );

    connect(
        ui->remove_position_button,
        &QPushButton::clicked,
        this,
        &MainWindow::onRemovePositionClicked
    );

    connect(
        ui->timeframe_box,
        &QComboBox::activated,
        this,
        &MainWindow::onTimeFrameSelected
    );



    ui->remove_portfolio_button->setEnabled(false);
    ui->add_position_button->setEnabled(false);
    ui->remove_position_button->setEnabled(false);
    ui->analyse_button->setEnabled(false);

    // Set up Time Frame combo box.

    ui->timeframe_box->addItem("Daily", QVariant::fromValue(TimeFrame::DAILY));
    ui->timeframe_box->addItem("Weekly", QVariant::fromValue(TimeFrame::WEEKLY));
    ui->timeframe_box->addItem("Monthly", QVariant::fromValue(TimeFrame::MONTHLY));
    ui->timeframe_box->setCurrentIndex(-1);
    ui->timeframe_box->setEnabled(false);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::registerPortfolioManager(std::shared_ptr<PortfolioManager> port) {

    if (port != nullptr) {

        portfolio_manager = port;
    }
}


void MainWindow::registerMarketDataStore(std::shared_ptr<MarketDataStore> mds) {

    if (mds != nullptr) {

        market_data_store = mds;
    }
}


void MainWindow::onAddPortfolioClicked() {

    CreatePortfolioDialog dialog;
    QString name;

    if (dialog.exec() == QDialog::Accepted) {

        name = dialog.portfolioName();
        

        if (!name.isEmpty()) {

            // Add portfolio to list of portfolios.
            portfolio_manager->addPortfolio(name.toStdString());

            QListWidgetItem* item = new QListWidgetItem(name);
            item->setData(Qt::UserRole, name);

            ui->portfolio_list->addItem(item);

            if (ui->remove_portfolio_button->isEnabled() == false) { ui->remove_portfolio_button->setEnabled(true); }
        }
    }
}


void MainWindow::onRemovePortfolioClicked(){

    RemoveFromListDialog dialog{portfolio_manager->viewPortfolioIDs()};
    dialog.setDialogText("Select Portfolios to delete.");

    if(dialog.exec() == QDialog::Accepted){
    
        if (dialog.selectedItems().size() != 0) {

            for (auto& portfolio_ID : dialog.selectedItems()) {

                portfolio_manager->removePortfolio(portfolio_ID);
            }

            ui->portfolio_list->clear();

            for (auto& portfolio_ID : portfolio_manager->viewPortfolioIDs()) {

                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(portfolio_ID));
                item->setData(Qt::UserRole, QString::fromStdString(portfolio_ID));

                ui->portfolio_list->addItem(item);
            }

            if (portfolio_manager->numbPortfolio() == 0) {

                ui->remove_portfolio_button->setEnabled(false);
                ui->add_position_button->setEnabled(false);
                ui->remove_position_button->setEnabled(false);
                ui->timeframe_box->setEnabled(false);
            }
        }
    }
}


void MainWindow::onPortfolioClicked(QListWidgetItem* item) {

    auto portfolio_id = item->data(Qt::UserRole).toString();

    ui->portfolio_frame->setText((portfolio_id));
    portfolio_manager->setCurrentPortfolio(portfolio_id);

    // Set up new page for current portfolio.
    clearAnalyticsPage();
    ui->position_list->clear();

    // TO:DO retrieve cached results if available.

    if (portfolio_manager->currentPortfolio()->size() == 0) {

        ui->remove_position_button->setEnabled(false);

    }else{

        for (const auto& position_ID : portfolio_manager->currentPortfolio()->viewPositionIDs()) {

            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(position_ID));
            item->setData(Qt::UserRole, QString::fromStdString(position_ID));
            ui->position_list->addItem(item);
        }

        ui->remove_position_button->setEnabled(true);
    }

    ui->number_of_assets->setText("Number of Positions: " + QString::number(portfolio_manager->currentPortfolio()->size()));
    ui->timeframe_box->setEnabled(true);

    if (!ui->add_position_button->isEnabled()){ ui->add_position_button->setEnabled(true); }
}


void MainWindow::onAnalyseClicked() {

    PortfolioReport result = portfolio_manager->analysePortfolio(ui->timeframe_box->currentData().value<TimeFrame>());

    // Show results on main window.

    ui->total_return_output->setText(QString::number(result.total_return));
    ui->portfolio_risk_output->setText(QString::number(result.volatitilty));
    ui->sharpe_ratio_output->setText(QString::number(result.sharpe_ratio));

    ui->parametric_95->setText(QString::number(result.parametric_VaR_95));
    ui->parametric_99->setText(QString::number(result.parametric_VaR_99));
    ui->parametric_995->setText(QString::number(result.parametric_VaR_995));
    ui->parametric_999->setText(QString::number(result.parametric_VaR_999));

    ui->historical_95->setText(QString::number(result.historical_VaR_95));
    ui->historical_99->setText(QString::number(result.historical_VaR_99));
    ui->historical_995->setText(QString::number(result.historical_VaR_995));
    ui->historical_999->setText(QString::number(result.historical_VaR_999));
}


void MainWindow::onAddPositionClicked() {

    if (portfolio_manager->currentPortfolio() == nullptr) { return; }

    AddPositionDialog dialog{ portfolio_manager.get(), market_data_store.get()};
    if (dialog.exec() == QDialog::Accepted) {

        portfolio_manager->currentPortfolio()->addPosition(dialog.getPosition());

        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(dialog.getPosition()->viewID()));
        ui->position_list->addItem(item);

        if (!ui->remove_position_button->isEnabled()) { ui->remove_position_button->setEnabled(true); }
        ui->number_of_assets->setText("Number of Positions: " + QString::number(portfolio_manager->currentPortfolio()->size()));
    }
}


void MainWindow::onRemovePositionClicked() {

    if (portfolio_manager->currentPortfolio()->size() == 0) { return; }
    
    RemoveFromListDialog dialog{ portfolio_manager->currentPortfolio()->viewPositionIDs()};
    dialog.setDialogText("Deleting positions from portfolio: " + QString::fromStdString(portfolio_manager->currentPortfolio()->viewID()));

    if (dialog.exec() == QDialog::Accepted) {

        for (auto& position_ID : dialog.selectedItems()) {

            portfolio_manager->currentPortfolio()->removePosition(position_ID.toStdString());
        }

        ui->position_list->clear();

        for (auto& position_ID : portfolio_manager->currentPortfolio()->viewPositionIDs()) {

            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(position_ID));
            item->setData(Qt::UserRole, QString::fromStdString(position_ID));

            ui->position_list->addItem(item);
        }

        if (portfolio_manager->currentPortfolio()->size() == 0) {

            ui->remove_position_button->setEnabled(false);
        }
    }
}


void MainWindow::onTimeFrameSelected() {

    portfolio_manager->setTimeFrame(ui->timeframe_box->currentData().value<TimeFrame>());

    if (!ui->analyse_button->isEnabled()) {

        ui->analyse_button->setEnabled(true);
    }
}