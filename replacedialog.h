#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

signals:
    void replaceRequested(const QString &findText, const QString &replaceText);
    void replaceAllRequested(const QString &findText, const QString &replaceText);

private slots:
    void on_pushButtonReplace_clicked();
    void on_pushButtonReplaceAll_clicked();
    void on_pushButtonClose_clicked();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H
