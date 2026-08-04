#include "AddPositionDialog.hpp"
#include "ui_AddPositionDialog.h"
#include "Logger.hpp"

#include "Asset.hpp"
#include "GenericDataStore.hpp"
#include "MarketDataStore.hpp"
#include "PortfolioManager.hpp"
#include "Position.hpp"
#include "Equities/Stock.hpp"

#include <qstring.h>
#include <qmessagebox.h>
#include <qpushbutton.h>



Q_DECLARE_METATYPE(PositionType)

AddPositionDialog::AddPositionDialog(PortfolioManager* port, MarketDataStore* store, QWidget *parent )
    : market_data_store{ store }, portfolio_manager{ port }, asset{ nullptr }, position{nullptr}, QDialog(parent)
    , ui(new Ui::AddPositionDialog)
{
    ui->setupUi(this);

    QPushButton* cancel_button = ui->buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
    QPushButton* create_position_button = ui->buttonBox->addButton("Create Position", QDialogButtonBox::ActionRole);

    connect(
        
        ui->asset_type_box,
        &QComboBox::activated,
        this,
        &AddPositionDialog::onAssetTypeSelected
    );

    connect(

        cancel_button,
        &QPushButton::clicked,
        this,
        &AddPositionDialog::onCancel
    );

    connect(
        create_position_button,
        &QPushButton::clicked,
        this,
        &AddPositionDialog::onOkClicked
    );


    std::array<std::string, 1> asset_types = { "Stock"};
    
    for (const auto& type : asset_types) {

        ui->asset_type_box->addItem(QString::fromStdString(type), QString::fromStdString(type));    
    }

    ui->asset_type_box->setCurrentIndex(-1);
    Logger::logInfo("ComboBox index: " + std::to_string(ui->asset_type_box->currentIndex()));

    ui->asset_pages->setCurrentWidget(ui->default_page);

    ui->position_type_box->addItem("LONG", QVariant::fromValue(PositionType::LONG));
    ui->position_type_box->addItem("SHORT", QVariant::fromValue(PositionType::SHORT));
    ui->position_type_box->addItem("NA", QVariant::fromValue(PositionType::NA));

    create_position_button->setEnabled(false);
}


AddPositionDialog::~AddPositionDialog()
{
    delete ui;
}

std::shared_ptr<Position> AddPositionDialog::getPosition() const { return position; }

void AddPositionDialog::onAssetTypeSelected(){

    ui->asset_pages->setCurrentWidget(ui->asset_pages->widget(ui->asset_type_box->currentIndex()));
    for (auto& button : ui->buttonBox->buttons()) {

        button->setEnabled(true);
    }
}


void AddPositionDialog::onOkClicked() {

    if (ui->asset_type_box->currentData() == "Stock") {

        if (ui->stock_ticker_input->text().isEmpty()) {

            QMessageBox dialog;
            dialog.setInformativeText("Requires Stock ticker to create asset.");
            dialog.setText("Please provide ticker/ID for stock asset.");
            dialog.setStandardButtons(QMessageBox::Ok);
            dialog.exec();

            return;
        }

        auto data = market_data_store->addMarketData(ui->stock_ticker_input->text().toStdString());

        if (data) {

            asset = std::make_shared<Stock>(

                ui->stock_ticker_input->text().toStdString(),
                market_data_store->getLatestPrice(ui->stock_ticker_input->text().toStdString()),
                market_data_store->getHistoricData(ui->stock_ticker_input->text().toStdString())
            );

            position = std::make_shared<Position>(

                ui->position_id_input->text().toStdString(),
                ui->quantity_input->value(),
                asset,
                ui->buy_in_input->value(),
                ui->position_type_box->currentData(Qt::UserRole).value<PositionType>()
            );

            accept();
        }
        else {

            QMessageBox dialog;
            dialog.setInformativeText("Could not find market data in the data store or fetch data from data provider. Check spelling of ticker.");
            dialog.setText("Error when creating stock.");
            dialog.setStandardButtons(QMessageBox::Ok);
            dialog.exec();
        }
    }
}


void AddPositionDialog::onCancel() { reject(); }