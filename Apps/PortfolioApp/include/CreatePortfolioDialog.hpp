#ifndef CREATEPORTFOLIODIALOG_HPP
#define CREATEPORTFOLIODIALOG_HPP

#include <QDialog>
#include <qstring.h>

namespace Ui {
class CreatePortfolioDialog;
}

class CreatePortfolioDialog : public QDialog
{
    Q_OBJECT

    private:

        Ui::CreatePortfolioDialog *ui;

    public:

        explicit CreatePortfolioDialog(QWidget* parent = nullptr);
        ~CreatePortfolioDialog();

        QString portfolioName() const;
};

#endif // CREATEPORTFOLIODIALOG_HPP
