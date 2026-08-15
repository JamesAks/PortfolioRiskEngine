#include "PositionDialog.hpp"
#include "ui_PositionDialog.h"

#include "Asset.hpp"
#include "DateValueGraph.hpp"
#include "MarketData.hpp"
#include "PortfolioManager.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"

#include <qstring.h>
#include <QtQuickWidgets/qquickwidget.h>
#include <QtGraphs/qlineseries.h>


Q_DECLARE_METATYPE(TimeFrame)

void PositionDialog::updateDialogFields(TimeFrame tf) {

    auto position_report = RiskEngine::analysePosition(position, tf);
  
    auto asset_report = RiskEngine::analyseAsset(*position.viewAsset(), tf);

    ui->latest_price_output->setText(QString::number(asset_report.net_present_value));
    ui->volatility_output->setText(QString::number(asset_report.volatility));
    ui->exp_return_output->setText(QString::number(asset_report.expected_return));

    auto& returns = position.viewAsset()->historicData()->periodicReturns(tf);

    date_return_graph->updateGraph(

        position.viewAsset()->historicData()->periodicPrices(tf).dates(),
        position.viewAsset()->historicData()->periodicReturns(tf)
    );
}

PositionDialog::PositionDialog(const Position& pos, QWidget *parent)
    : QDialog(parent), position{ pos }
    , ui(new Ui::PositionDialog)
{
    ui->setupUi(this);

    // Set up interactions.
    connect(

        ui->analyse_asset_button,
        &QPushButton::clicked,
        this,
        &PositionDialog::onAnalyseAssetClicked
    );

    connect(

        ui->timeframe_box,
        &QComboBox::activated,
        this,
        &PositionDialog::onTimeFrameSelected
    );

    // Set up UI.
    ui->position_analysis_title->setText(QString::fromStdString(position.viewID() + " Analysis"));

    ui->timeframe_box->addItem("Daily", QVariant::fromValue(TimeFrame::DAILY));
    ui->timeframe_box->addItem("Weekly", QVariant::fromValue(TimeFrame::WEEKLY));
    ui->timeframe_box->addItem("Monthly", QVariant::fromValue(TimeFrame::MONTHLY));

    // Set up graph
    date_return_graph = std::make_shared<DateValueGraph>();
    ui->returns_graph_layout->addWidget(date_return_graph->graph());

    // Set static position/asset details

    switch (position.viewPositionType())
    {
    case PositionType::NA:
        ui->position_type_output->setText("N/A");
        break;

    case PositionType::SHORT:
        ui->position_type_output->setText("SHORT");
        break;

    case PositionType::LONG:
        ui->position_type_output->setText("LONG");
        break;

    default:
        break;
    }

    ui->buyin_output->setText(QString::number(position.viewPriceBoughtAt()));
    ui->quantity_output->setText(QString::number(position.viewQuantity()));
    ui->initial_investment_output->setText(QString::number(position.initialInvestment()));
    ui->market_value_output->setText(QString::number(position.marketValue()));
    ui->unrealised_gain_output->setText(QString::number(position.unrealizedGains()));

    ui->asset_ID_output->setText(QString::fromStdString(position.viewAsset()->symbol()));

    QDate latest_valuation{ position.viewAsset()->latestValuationDate() };
    ui->valuation_date_output->setText(QString::number(latest_valuation.day()) + "/" + QString::number( latest_valuation.month()) + "/" + QString::number(latest_valuation.year()));

    switch (position.viewAsset()->assetType())
    {
    case AssetType::NUMBER_OF_TYPES:
    case AssetType::NA:
        ui->asset_type_output->setText("N/A");
        break;

    case AssetType::STOCK:
        ui->asset_type_output->setText("Stock");
        break;

    default:
        break;
    }

    //Initial dialog shows DAILY info.
    updateDialogFields(TimeFrame::DAILY);
       
}

PositionDialog::~PositionDialog()
{
    delete ui;
}

void PositionDialog::onAnalyseAssetClicked() {

}

void PositionDialog::onTimeFrameSelected() {

    updateDialogFields(ui->timeframe_box->currentData().value<TimeFrame>());
}