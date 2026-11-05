#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_actionFont_triggered();
    void on_actionTextColor_triggered();
    void on_actionBackgroundColor_triggered();
    void on_actionWordWrap_toggled(bool checked);
    void on_actionAbout_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionShowStatusbar_triggered(bool checked);
    void on_actionShowToolbar_triggered(bool checked);

private:
    void closeEvent(QCloseEvent *event) override;
    bool maybeSave();
    bool saveFile();
    bool saveFileAs();
    bool writeFile(const QString &path);
    bool loadFile(const QString &path);
    void newFile();
    void openFile();
    void setCurrentFile(const QString &path);
    QString currentFile;
    void updateStatusBar();

    Ui::MainWindow *ui;
    QLabel *lengthLabel;
    QLabel *linesLabel;
    QLabel *lnLabel;
    QLabel *colLabel;
    QLabel *userLabel;
};
#endif // MAINWINDOW_H
