#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_pushButtonFindNext_clicked()
{
    QString text = ui->lineEditFind->text();

    emit findNextRequested(text);
}


void FindDialog::on_pushButtonClose_clicked()
{
    reject();
}
