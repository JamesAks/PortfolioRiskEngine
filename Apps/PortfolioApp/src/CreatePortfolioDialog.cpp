#include "CreatePortfolioDialog.hpp"
#include "ui_CreatePortfolioDialog.h"

CreatePortfolioDialog::CreatePortfolioDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreatePortfolioDialog)
{
    ui->setupUi(this);
}

CreatePortfolioDialog::~CreatePortfolioDialog()
{
    delete ui;
}

QString CreatePortfolioDialog::portfolioName() const{

    return ui->lineEdit->text();
}
