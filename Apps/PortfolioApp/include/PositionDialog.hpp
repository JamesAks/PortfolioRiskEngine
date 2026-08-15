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
        const Position& position;

        std::shared_ptr<DateValueGraph> date_return_graph;

        void updateDialogFields(TimeFrame);
        
    public:
        explicit PositionDialog(const Position&, QWidget *parent = nullptr);
        ~PositionDialog();

    private slots:

        void onAnalyseAssetClicked();
        void onTimeFrameSelected();

};

#endif // POSITIONDIALOG_HPP
