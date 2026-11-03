#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_pushButtonReplace_clicked()
{
    emit replaceRequested(ui->lineEditFind->text(), ui->lineEditReplace->text());
}

void ReplaceDialog::on_pushButtonReplaceAll_clicked()
{
    emit replaceAllRequested(ui->lineEditFind->text(), ui->lineEditReplace->text());
}

void ReplaceDialog::on_pushButtonClose_clicked()
{
    reject();
}
