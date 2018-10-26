#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <QString>
#include <string>
#include "preprocess.h"
#include "cpu_instructions.h"
#include "symbolic_character_process.h"
#include "compile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonCompiler_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
