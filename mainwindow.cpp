#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
symbolic_character_process g_sc_process_o;
compile g_compile_o;
cpu_instructions g_instruction_o;

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
    std::string code;
    std::string tmp;
    std::size_t pos;
    std::size_t end;
    QFile *tempFile = new QFile;
    QDir dir;
    ui->listWidgetFile->addItem("main.c");
    ui->listWidgetFile->setCurrentRow(0);
    if(!dir.exists("src"))
    {
        dir.mkdir("src");
    }
    qDebug()<<dir.currentPath();
    tempFile->setFileName("src/main.c");
    if(!tempFile->exists("src/main.c"))
    {
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

    if(!tempFile->open(QIODevice::ReadWrite | QIODevice::Text)) {
         qDebug()<<"Can't open the file!"<<endl;
         return;
    }

    code = tempFile->readAll().toStdString();
    ui->textEditCode->clear();
    ui->textEditCode->setPlainText(QString::fromStdString(code.c_str()));
    tempFile->close();
    //find header files and add to our GUI
    while(1)
    {
     pos=code.find("#include<");
     if (pos!=std::string::npos)
     {
        qDebug()<< "found at: " << pos <<endl;
        end=code.find(">");
        if (end!=std::string::npos)
        {
            qDebug()<< "end at: "<<end<<endl;
            tmp=code.substr(pos+9,end-(pos+9));
            if(tempFile->exists(QString::fromStdString(("src/"+tmp))))
            {
                ui->listWidgetFile->addItem(QString::fromStdString(tmp));
            }
            else
            {
                qDebug()<<"header file not exists"<<endl;
            }
            qDebug()<< "header file"<<tmp.c_str()<<endl;
            code.erase (pos,end+1);
            //code.replace(pos,end-pos,"");
        }
        else
        {
            break;//have #include<, but does not have >
        }
     }
     else
     {
         break;//does not have include< at all
     }
    }
     //need to find the #include files , and put into ui->listWidgetFile->addItem("*");

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
    std::string status="";
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
    ui->textEditMessage->append("->symbolic_header_preprocess before");
    g_sc_process_o.symbolic_header_preprocess(g_compile_o.m_code,g_compile_o.m_code_count);
    display = QString::fromStdString(g_compile_o.m_code);
    ui->textEditMessage->append(display);
    ui->textEditMessage->append("->symbolic_header_preprocess end");
    // 2 preprocess include and define code first}


    // 3 preprocess replace define code{
    ui->textEditMessage->append("->symbolic_replace_define before");
    g_compile_o.m_code_count = 0;
    g_sc_process_o.symbolic_replace_define(g_compile_o.m_code,g_compile_o.m_code_count);
    display = QString::fromStdString(g_compile_o.m_code);
    ui->textEditMessage->append(display);
    ui->textEditMessage->append("->symbolic_replace_define end");
    // 3 preprocess replace define code}




    // 4 real compile
    ui->textEditMessage->append("->COMPILE STEPS STARTS");
    ui->textEditMessage->append("->compile step 4a - init");
    //ROM page
    for(int i=0;i<MAX_PAGE;i++)
    {
        g_compile_o.m_ROM_PAGE[i].clear();
        g_compile_o.m_ROM_PAGE_byte_use_c[i] = 0;
    }
    g_compile_o.m_ROM_PAGE_use = 0;
    //ROM tmp cache
    g_compile_o.m_ROM_Cache.clear();
    g_compile_o.m_ROM_Cahce_c = 0;
    //interruptramaddress status
    g_compile_o.m_RAM_allocate_address_interrupt_status=0;
    //global constants?????
    //global valuables
    g_compile_o.m_total_global_valuables=0;
    //RAM page switch control
    g_compile_o.m_ROM_PAGE_init_f = 1;
    g_compile_o.m_RAM_allocate_address = 25;//0x00000 - 0x80000 - 512KBytes

    //need to setup ROM buff to fill out instructions , excited !!!!!
    g_instruction_o.set_buff_count(&g_compile_o.m_ROM_Cache,&g_compile_o.m_ROM_Cahce_c);

    //init the rom bin with couple bytes
    g_instruction_o.I_A_set(0x01);//for fun
    g_instruction_o.I_B_set(0x02);//for fun
    g_instruction_o.I_A0_set(0x01);//for fun
    g_instruction_o.I_A1_set(0x01);//for fun
    g_instruction_o.I_A2_set(0x00);//for fun
    g_instruction_o.I_NOP();

    //step 4b scan global valueable
    ui->textEditMessage->append("->compile step 4b - scan global valuables");
    g_compile_o.m_code_count = 0;
    status = g_compile_o.scan_valuable();


    //main step here !!!!!
    ui->textEditMessage->append("->compile step 4c - real compiling");
    status = g_compile_o.compiling_all(status);//in this function, we will do a lot of stuff man !!!!!
    ui->textEditMessage->append(QString::fromStdString(status));
    ui->textEditMessage->append("->COMPILE STEPS ENDS");
    //main step finished here !!!!!



    //some debug informaiton prints
    display = QString::fromStdString(g_compile_o.m_code);
    display.append("\nmin current position->");
    display.append(QString::number(g_compile_o.m_code_count));

    display.append("\ntotal global valuables->");
    display.append(QString::number(g_compile_o.m_total_global_valuables));

    for(int i =0;i<g_compile_o.m_total_global_valuables;i++)
    {
        display.append("\nglobal valuable type ->");
        display.append(QString::fromStdString(g_compile_o.m_local_valuable_type[i]));
        display.append("\nglobal valuable name ->");
        display.append(QString::fromStdString(g_compile_o.m_global_valuable_name[i]));
        display.append("\nglobal valuable address ->");
        display.append(QString::number(g_compile_o.m_global_valuable_address[i]));
    }
    ui->textEditMessage->append(display);
    ui->textEditMessage->append("->compile step 4 end");

}

void MainWindow::on_listWidgetFile_itemClicked(QListWidgetItem *item)
{
    QListWidgetItem *curItem = ui->listWidgetFile->currentItem();
    QFile *tempFile = new QFile;
    if( curItem != NULL )
    {
        tempFile->setFileName("src/"+curItem->text());
        if(tempFile->exists("src/"+curItem->text()))
        {
            if(!tempFile->open(QIODevice::ReadWrite | QIODevice::Text))
            {
                qDebug()<<"file open failed"<<endl;
            }
            ui->textEditCode->clear();
            ui->textEditCode->setPlainText(QString::fromStdString(tempFile->readAll().toStdString()));
            tempFile->close();
        }
        else
        {
                qDebug()<<"file not exists"<<endl;
        }
    }
}
