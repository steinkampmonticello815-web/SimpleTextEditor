#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QTextCursor>
#include <QTextDocument>
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>

namespace {
// Search from the current cursor, wrapping once without losing the selection on failure.
bool findNext(QPlainTextEdit *editor, const QString &text)
{
    QTextCursor found = editor->document()->find(text, editor->textCursor());
    if (found.isNull())
        found = editor->document()->find(text, 0);
    if (found.isNull())
        return false;
    editor->setTextCursor(found);
    editor->ensureCursorVisible();
    return true;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化时让编辑器换行模式与现有菜单勾选状态保持一致。
    on_actionWordWrap_toggled(ui->actionWordWrap->isChecked());

    // 菜单项和工具栏共用 QAction，两处会同步更新。
    connect(ui->plainTextEdit, &QPlainTextEdit::copyAvailable,
            ui->actionCut, &QAction::setEnabled);
    connect(ui->plainTextEdit, &QPlainTextEdit::copyAvailable,
            ui->actionCopy, &QAction::setEnabled);
    ui->actionCut->setEnabled(ui->plainTextEdit->textCursor().hasSelection());
    ui->actionCopy->setEnabled(ui->plainTextEdit->textCursor().hasSelection());

    const auto updatePasteAction = [this]() {
        ui->actionPaste->setEnabled(ui->plainTextEdit->canPaste());
    };
    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, updatePasteAction);
    updatePasteAction();

    connect(ui->actionCut, &QAction::triggered,
            ui->plainTextEdit, &QPlainTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered,
            ui->plainTextEdit, &QPlainTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered,
            ui->plainTextEdit, &QPlainTextEdit::paste);

    lengthLabel = new QLabel("length: 0", this);
    linesLabel = new QLabel("lines: 1", this);
    lnLabel = new QLabel("Ln: 1", this);
    colLabel = new QLabel("Col: 1", this);
    userLabel = new QLabel("golden luo", this);

    ui->statusbar->addPermanentWidget(lengthLabel);
    ui->statusbar->addPermanentWidget(linesLabel);
    ui->statusbar->addPermanentWidget(lnLabel);
    ui->statusbar->addPermanentWidget(colLabel);
    ui->statusbar->addPermanentWidget(userLabel);

    // 文本内容变化时更新长度和行数。
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged,
            this, &MainWindow::updateStatusBar);
    // 光标移动时更新当前行、列。
    connect(ui->plainTextEdit, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::updateStatusBar);
    updateStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatusBar()
{
    const QString text = ui->plainTextEdit->toPlainText();
    const QTextCursor cursor = ui->plainTextEdit->textCursor();

    lengthLabel->setText(QString("length: %1").arg(text.length()));
    linesLabel->setText(QString("lines: %1").arg(ui->plainTextEdit->document()->blockCount()));
    lnLabel->setText(QString("Ln: %1").arg(cursor.blockNumber() + 1));
    colLabel->setText(QString("Col: %1").arg(cursor.positionInBlock() + 1));
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionFind_triggered()
{
    FindDialog dialog(this);
    connect(&dialog, &FindDialog::findNextRequested, this,
            [this, &dialog](const QString &text) {
        if (text.isEmpty()) {
            QMessageBox::information(&dialog, tr("提示"), tr("请输入查找内容"));
            return;
        }
        if (!findNext(ui->plainTextEdit, text))
            QMessageBox::information(&dialog, tr("提示"), tr("没有找到该内容"));
    });
    dialog.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dialog(this);
    connect(&dialog, &ReplaceDialog::replaceRequested, this,
            [this, &dialog](const QString &findText, const QString &replaceText) {
        if (findText.isEmpty()) {
            QMessageBox::information(&dialog, tr("提示"), tr("请输入查找内容"));
            return;
        }
        QTextCursor cursor = ui->plainTextEdit->textCursor();
        const QString selected = QString(cursor.selectedText()).replace(QChar::ParagraphSeparator, QChar('\n'));
        if (!cursor.hasSelection() || selected.compare(findText, Qt::CaseInsensitive) != 0) {
            if (!findNext(ui->plainTextEdit, findText)) {
                QMessageBox::information(&dialog, tr("提示"), tr("没有找到该内容"));
                return;
            }
            cursor = ui->plainTextEdit->textCursor();
        }
        cursor.insertText(replaceText);
        ui->plainTextEdit->setTextCursor(cursor);
        ui->plainTextEdit->ensureCursorVisible();
    });
    connect(&dialog, &ReplaceDialog::replaceAllRequested, this,
            [this, &dialog](const QString &findText, const QString &replaceText) {
        if (findText.isEmpty()) {
            QMessageBox::information(&dialog, tr("提示"), tr("请输入查找内容"));
            return;
        }
        QTextDocument *doc = ui->plainTextEdit->document();
        QTextCursor editBlock(doc);
        editBlock.beginEditBlock();
        QTextCursor cursor(doc);
        int count = 0;
        while (true) {
            QTextCursor match = doc->find(findText, cursor);
            if (match.isNull())
                break;
            match.insertText(replaceText);
            cursor = match;
            ++count;
        }
        editBlock.endEditBlock();
        QMessageBox::information(&dialog, tr("替换完成"), tr("共替换 %1 处").arg(count));
    });
    dialog.exec();
}

void MainWindow::on_actionShowStatusbar_triggered(bool checked)
{
    ui->statusbar->setVisible(checked);
}


void MainWindow::on_actionShowToolbar_triggered(bool checked)
{
    ui->mainToolBar->setVisible(checked);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    const QFont font = QFontDialog::getFont(
        &ok, ui->plainTextEdit->font(), this, tr("设置字体"));
    if (ok)
        ui->plainTextEdit->setFont(font);
}

void MainWindow::on_actionTextColor_triggered()
{
    const QColor color = QColorDialog::getColor(
        ui->plainTextEdit->palette().color(QPalette::Text),
        this, tr("设置字体颜色"));
    if (!color.isValid())
        return;
    QPalette palette = ui->plainTextEdit->palette();
    palette.setColor(QPalette::Text, color);
    ui->plainTextEdit->setPalette(palette);
}

void MainWindow::on_actionBackgroundColor_triggered()
{
    const QColor color = QColorDialog::getColor(
        ui->plainTextEdit->palette().color(QPalette::Base),
        this, tr("设置编辑器背景颜色"));
    if (!color.isValid())
        return;
    QPalette palette = ui->plainTextEdit->palette();
    palette.setColor(QPalette::Base, color);
    ui->plainTextEdit->setPalette(palette);
}

void MainWindow::on_actionWordWrap_toggled(bool checked)
{
    ui->plainTextEdit->setLineWrapMode(
        checked ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
}
