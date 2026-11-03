#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

signals:
    void findNextRequested(const QString &text);

private slots:
    void on_pushButtonFindNext_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::FindDialog *ui;
};
#endif // FINDDIALOG_H
