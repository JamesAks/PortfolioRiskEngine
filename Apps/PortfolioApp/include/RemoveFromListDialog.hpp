#ifndef REMOVEFROMLISTDIALOG_HPP
#define REMOVEFROMLISTDIALOG_HPP

#include <QDialog>


namespace Ui {
class RemoveFromListDialog;
}
class Portfolio;

class RemoveFromListDialog : public QDialog
{
    Q_OBJECT

    public:

        explicit RemoveFromListDialog(const std::vector<std::string>&, QWidget* parent = nullptr);
        ~RemoveFromListDialog();

        void setDialogText(QString);
        const std::vector<QString>& selectedItems() const;

        
    private:

        Ui::RemoveFromListDialog *ui;
        std::vector<QString> selected_items;

    private slots:

        void onCancelClicked();
        void onOkClicked();
};

#endif // REMOVEFROMLISTDIALOG_HPP
