#ifndef ADDPOSITIONDIALOG_HPP
#define ADDPOSITIONDIALOG_HPP

#include <QDialog>
#include <memory>

namespace Ui {
class AddPositionDialog;
}

class Asset;
class MarketDataStore;
class PortfolioManager;
class Position;

class AddPositionDialog : public QDialog
{
    Q_OBJECT

    public:

        explicit AddPositionDialog(PortfolioManager*, MarketDataStore*, QWidget *parent = nullptr);
        ~AddPositionDialog();

        std::shared_ptr<Position> getPosition() const;

    private:

        Ui::AddPositionDialog *ui;
        PortfolioManager* portfolio_manager;
        MarketDataStore* market_data_store;

        std::shared_ptr<Asset> asset;
        std::shared_ptr<Position> position;

    private slots:

        void onAssetTypeSelected();
        void onOkClicked();
        void onCancel();

};

#endif // ADDPOSITIONDIALOG_HPP
