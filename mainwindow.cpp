#include "mainwindow.h"
#include "ui_mainwindow.h"

symbolic_character_process g_sc_process_o;
compile g_compile_o;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpen_clicked()
{
    ui->listWidgetFile->addItem("main.c");
    ui->listWidgetFile->setCurrentRow(0);
    QDir dir;
    if(!dir.exists("src"))
    {
        dir.mkdir("src");
    }
    QFile *tempFile = new QFile;
    qDebug()<<dir.currentPath();
    if(!tempFile->exists("src/main.c"))
    {

        tempFile->setFileName("src/main.c");
        if(!tempFile->open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug()<<"file open failed"<<endl;
        }
        tempFile->close();
    }
    else
    {
            qDebug()<<"file exists"<<endl;
    }

     QFile file("src/main.c");
     if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
         qDebug()<<"Can't open the file!"<<endl;
         return;
     }
     QTextStream stream(&file);
     ui->textEditCode->clear();
     ui->textEditCode->setPlainText(stream.readAll());
     file.close();
}

void MainWindow::on_pushButtonSave_clicked()
{
    QString filename ="src/" + ui->listWidgetFile->currentItem()->text();
    QFile file(filename);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        qDebug()<<"file write can't open"<<endl;
        return;
    }
    QTextStream in(&file);
    in<<ui->textEditCode->toPlainText();
    file.flush();
    file.close();
}

void MainWindow::on_pushButtonCompiler_clicked()
{
    QString display;
    //check if you selected file to compile
    if(ui->listWidgetFile->currentRow() < 0)
    {

        qDebug()<<"please select a file to compile"<<endl;
        return;

    }
    //save the contect to source code file
    QString filename ="src/" + ui->listWidgetFile->currentItem()->text();
    QFile file(filename);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        qDebug()<<"file write can't open"<<endl;
        return;
    }
    QTextStream in(&file);
    in<<ui->textEditCode->toPlainText();
    file.flush();
    file.close();

    //compiling
    // 1 loading main.c to code string{
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        return;
    }
    QTextStream out(&file);
    g_compile_o.m_code = out.readAll().toStdString();
    file.close();
    ui->textEditMessage->clear();
    ui->textEditMessage->append("Loading .......... src/main.c");
    // append 16 ' '
    g_compile_o.m_code.append("                ");
    // 1 loading main.c to code string}




    // 2 preprocess include and define code first{
    g_sc_process_o.symbolic_header_preprocess(g_compile_o.m_code,g_compile_o.m_code_count);
    display = QString::fromStdString(g_compile_o.m_code);
    ui->textEditMessage->append(display);
    // 2 preprocess include and define code first}


    // 3 preprocess replace define code{
    g_sc_process_o.symbolic_replace_define(g_compile_o.m_code,g_compile_o.m_code_count);
    display = QString::fromStdString(g_compile_o.m_code);
    ui->textEditMessage->append(display);
    // 3 preprocess replace define code}






    // 4 scan global valueable






    // 5 scan function







    // 6 scan local valueable

}
