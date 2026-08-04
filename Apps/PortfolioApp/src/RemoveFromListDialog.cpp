#include "RemoveFromListDialog.hpp"
#include "ui_RemoveFromListDialog.h"
#include "Portfolio.hpp"

#include <qmessagebox.h>
#include <qpushbutton.h>



RemoveFromListDialog::RemoveFromListDialog(const std::vector<std::string>& names, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::RemoveFromListDialog)
{
    ui->setupUi(this);

    for (const auto& name : names) {

        QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(name) };
        item->setData(Qt::UserRole, QString::fromStdString(name));
        ui->list_of_items->addItem(item);
    }

    auto cancel_button = ui->buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
    auto ok_button = ui->buttonBox->addButton("Remove Position", QDialogButtonBox::ActionRole);

    connect(

        cancel_button,
        &QPushButton::clicked,
        this,
        &RemoveFromListDialog::onCancelClicked
    );

    connect(

        ok_button,
        &QPushButton::clicked,
        this,
        &RemoveFromListDialog::onOkClicked
    ); 
}

RemoveFromListDialog::~RemoveFromListDialog()
{
    delete ui;
}


void RemoveFromListDialog::setDialogText(QString text) { ui->textBrowser->setText(text); }


const std::vector<QString>& RemoveFromListDialog::selectedItems() const { return selected_items; }


void RemoveFromListDialog::onCancelClicked() { reject(); }


void RemoveFromListDialog::onOkClicked() {

    QMessageBox dialog;
    dialog.setInformativeText("Are you sure you want to delete these items?");
    dialog.setText("This action cannot be undone.");
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if (dialog.exec() == QMessageBox::Yes) {

        selected_items.reserve(ui->list_of_items->selectedItems().size());

        for (auto item : ui->list_of_items->selectedItems()) {

            selected_items.push_back(item->data(Qt::UserRole).toString());
        }

        accept();
    }
}