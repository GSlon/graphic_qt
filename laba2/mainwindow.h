#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QtDebug>
#include <QMenu>
#include <QDir>
#include <QFileSystemModel>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QAction>
#include <QDirModel>
#include <QFlag>
#include <QFlags>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QVariant>
#include <QFileInfo>
#include <QTextEdit>
#include <QState>
#include <QFont>
#include <QColor>
#include <QGroupBox>
#include "qcustomplot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QWidget *widget;
    QWidget *HelpWidget;
    QCustomPlot *painter;
    QVBoxLayout *lay;
    QMenu* aboutMenu;
    QMenu* fileMenu;

    void InitWindow();
    void InitMenu();
    void Draw(const QVector<double>&x, const QVector<double>&y );

private slots:
    void OpenFile();
    void Quit();
    void About();
    void Help();
};

#endif // MAINWINDOW_H
