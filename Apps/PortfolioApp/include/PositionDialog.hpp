#ifndef POSITIONDIALOG_HPP
#define POSITIONDIALOG_HPP

#include <QDialog>
#include <QWidget>

#include <memory>

namespace Ui {
class PositionDialog;
}

class DateValueGraph;
class PortfolioManager;
class Position;
enum class TimeFrame;

class PositionDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::PositionDialog* ui;
  
        const PortfolioManager& portfolio_manager;
        const QString& position_ID;
        std::shared_ptr<DateValueGraph> date_return_graph;

        void updateDialogFields(TimeFrame);
        
    public:
        explicit PositionDialog(const PortfolioManager&, const QString&, QWidget *parent = nullptr);
        ~PositionDialog();

    private slots:

        void onAnalyseAssetClicked();
        void onTimeFrameSelected();

};

#endif // POSITIONDIALOG_HPP
