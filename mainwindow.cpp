#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file_operation.h"
QStandardItemModel *model = new QStandardItemModel;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer();
    timer->setInterval(10);
    connect(timer,SIGNAL(timeout()),this,SLOT(runtime_show()));
    timer->start();
}
std::vector<char> temp;
int status=0;
long long int total_length;
long long int current_length;
bool inited = false;
FILE_INFO info;
read_file a;
write_hex b;
write_raw c;
bool start_ok = false;
MainWindow::~MainWindow()
{
    a.stop();
    temp.clear();
    delete ui;
}

void MainWindow::runtime_show()
{
    if(status==0)
    {
        ui->label_14->setText(QString::number(total_length));
        return;
    }
    if(status==1)
    {
        //读取
        ui->label_14->setText(QString::number(total_length));
        info=a.get_status();
        total_length = info.total_length;
        current_length = info.current_length;
        //qDebug()<<total_length<<"\\"<<current_length<<Qt::endl;
        ui->label_6->setText(QString::number(total_length));
        ui->label_8->setText(QString::number(current_length));
        temp = a.get_bin_data();
        ui->progressBar->setMaximum(total_length);
        ui->progressBar->setValue(current_length);
        if(total_length==current_length)
        {
            //qDebug()<<"inited"<<Qt::endl;
            int page = total_length%(16*8)==0? total_length/(16*8):total_length/(16*8)+1;
            ui->spinBox_4->setMaximum(page);
            status = 0;
            start_ok = true;
            temp = a.get_bin_data();
            qDebug()<<temp.size()<<Qt::endl;
            ui->label_14->setText(QString::number(total_length));
            on_spinBox_4_valueChanged(1);
            status = 0;
        }
        //qDebug()<<"OK"<<Qt::endl;
        return;
    }
    if(status==2)
    {
        //写hex数据
        ui->label_14->setText(QString::number(total_length));
        info = b.get_status();
        total_length = info.total_length;
        current_length = info.current_length;
        ui->label_6->setText(QString::number(total_length));
        ui->label_8->setText(QString::number(current_length));
        ui->progressBar->setMaximum(total_length);
        ui->progressBar->setValue(current_length);
        if(total_length==current_length)
        {
            QMessageBox::information(this,tr("提示"),tr("文件保存成功"));
            ui->label_14->setText(QString::number(total_length));
            status = 0;
        }
        return;
    }
    if(status==3)
    {
        //写raw数据
        ui->label_14->setText(QString::number(total_length));
        info = c.get_status();
        total_length = info.total_length;
        current_length = info.current_length;
        ui->label_6->setText(QString::number(total_length));
        ui->label_8->setText(QString::number(current_length));
        ui->progressBar->setMaximum(total_length);
        ui->progressBar->setValue(current_length);
        if(total_length==current_length)
        {
            QMessageBox::information(this,tr("提示"),tr("文件保存成功"));
            ui->label_14->setText(QString::number(total_length));
            status = 0;
        }
        return;
    }
}


void MainWindow::on_pushButton_clicked()
{
    //读取数据
    if(temp.size())
    {
        temp.clear();
        current_length = 0;
        total_length = 0;
        a.stop();
    }
    start_ok = false;
    QString filename;
    filename = QFileDialog::getOpenFileName(this,tr("选择一个文件"),QCoreApplication::applicationDirPath(),tr("所有文件(*.*)"));
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        const char* ch = nullptr;
        std::string temp_str = filename.toStdString();
        //QByteArray ch_temp = filename.toLatin1();
        ch = temp_str.c_str();
        qDebug()<<ch<<Qt::endl;
        //下面可以执行load data操作
        a.attach_run((char*)ch);
        a.start();
        status = 1;
        //qDebug()<<"finished"<<Qt::endl;
        on_spinBox_4_valueChanged(1);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    exit(0);
}


void MainWindow::on_pushButton_2_clicked()
{
    //保存hex数据
    QString filename;
    filename = QFileDialog::getSaveFileName(this,tr("选择要保存的位置"),QCoreApplication::applicationDirPath(),tr("所有文件(*.*)"));
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        const char* ch = nullptr;
        std::string temp_str = filename.toStdString();
        ch = temp_str.c_str();
        FILE* f1 = fopen(ch,"rb");
        if(f1)
        {
            //box = QMessageBox::information(this,tr("提示"),filename+tr("文件已经存在，是否替换"),QMessageBox::No|QMessageBox::Yes);
            fclose(f1);
            remove(ch);
        }
        //开始执行写文件操作
        b.attach_run((char*)ch,temp);
        b.start();
        status = 2;
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    //保存raw数据
    QString filename;
    filename = QFileDialog::getSaveFileName(this,tr("选择要保存的位置"),QCoreApplication::applicationDirPath(),tr("所有文件(*.*)"));
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        const char* ch = nullptr;
        std::string temp_str = filename.toStdString();
        ch = temp_str.c_str();
        FILE* f1 = fopen(ch,"rb");
        if(f1)
        {
            //box = QMessageBox::information(this,tr("提示"),filename+tr("文件已经存在，是否替换"),QMessageBox::No|QMessageBox::Yes);
            fclose(f1);
            remove(ch);
        }
        c.attach_run((char*)ch,temp);
        c.start();
        status = 3;
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    //提交数据更改
    long long int position = ui->spinBox->value();
    int pos_add = ui->spinBox_3->value();
    int chr_info = ui->spinBox_2->value();
    long long int real_position = position/8+pos_add+1;
    qDebug()<<"real_position="<<real_position<<Qt::endl;
    if(real_position>temp.size())
    {
        for(auto i = temp.size();i<real_position;i++)
        {
            temp.push_back(char(0));
        }
    }
    assert(real_position<=temp.size());
    temp[real_position-1]=char(chr_info);
    qDebug()<<"fix result="<<int(temp[real_position-1]);
    QMessageBox::information(this,tr("提示"),tr("数据修改完成"));
    on_spinBox_4_valueChanged(ui->spinBox_4->value());
}


void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    //pos_page = arg1;
    qDebug()<<"is_ok="<<start_ok<<Qt::endl;
    qDebug()<<"arg1="<<arg1<<Qt::endl;
    //ui->spinBox_4->setEnabled(false);
    int arg;
    arg = ui->spinBox_4->value();
    qDebug()<<"arg="<<arg<<Qt::endl;
    //此处加载tableView界面
    ui->tableWidget->setColumnCount(17);
    ui->tableWidget->setColumnWidth(0,80);
    int aaaa = 0;
    for(aaaa=0;aaaa<16;aaaa++)
    {
        ui->tableWidget->setColumnWidth(1+aaaa,31);
    }
    ui->tableWidget->setRowCount(16);
    QStringList labels = {"position","0","1","2","3","4","5","6","7","c0","c1","c2","c3","c4","c5","c6","c7"};
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    //ui->tableWidget->setItem(0,0,new QTableWidgetItem("test"));
    long long int start_pos = 0;
    start_pos = 16*8*(arg-1);
    qDebug()<<start_pos<<"/"<<temp.size();
    //int max_page = 0;
    //max_page = start_pos%(16*8)==0 ? start_pos/(16*8):start_pos/(16*8)+1;
    //ui->spinBox_4->setMaximum(max_page);
    //ui->spinBox_4->setMinimum(1);
    int rowcount = 0;
    int last = 0;
    last = temp.size() - start_pos-1;
    rowcount = (temp.size()-start_pos) > 16*8 ? 16 : (temp.size()-start_pos)/8+1;
    ui->tableWidget->setRowCount(rowcount);
    //写入数据
    int i,j;
    //bool stop_pos = false;
    for(i = 0;i<rowcount;i++)
    {
        for(j=0;j<8;j++)
        {
            char buffer_2[11] = {'\000','\000','\000','\000','\000','\000','\000','\000','\000','\000','\000'};
            sprintf(buffer_2,"0x%08X",start_pos*8+i*8*8);
            std::string str2;
            str2 = buffer_2;
            //qDebug()<<buffer_2<<Qt::endl;
            //qDebug()<<str2<<Qt::endl;
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::fromStdString(str2)));
            //ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString("0x")+QString::number(start_pos+i*8,16)));
            //if(stop_pos==false)
            {
                if(i*8+j>last)
                {
                    ui->tableWidget->setItem(i,j+1,new QTableWidgetItem(QString("")));
                    ui->tableWidget->setItem(i,j+9,new QTableWidgetItem(QString("")));
                }
                else if(i*8+j==last)
                {
                //stop_pos = true;
                qDebug()<<"i="<<i<<"j="<<j<<Qt::endl;
                std::string str;
                char buffer[3] = {'\000','\000','\000'};
                int tt = temp[start_pos+i*8+j]>=0? temp[start_pos+i*8+j]:256+temp[start_pos+i*8+j];
                //qDebug()<<"tt="<<tt<<Qt::endl;
                sprintf(buffer,"%02X",tt);
                str = buffer;
                char aa[2] = {'\000','\000'};
                aa[0] = temp[start_pos+i*8+j];
                //assert(1==2);
                //ui->tableWidget->setItem(i,j+1,new QTableWidgetItem(QString::number(tt,16)));
                ui->tableWidget->setItem(i,j+1,new QTableWidgetItem(QString::fromStdString(str)));
                char buffer_1[3] = {'\000','\000','\000'};
                std::string str1;
                sprintf(buffer_1,"%c.",temp[start_pos+i*8+j]);
                str1 = buffer_1;
                //qDebug()<<str1<<Qt::endl;
                //ui->tableWidget->setItem(i,j+9,new QTableWidgetItem(QString(QLatin1String(aa))));
                ui->tableWidget->setItem(i,j+9,new QTableWidgetItem(QString::fromStdString(str1)));
                }
                else
                {
                //if(start_ok==true&&stop_pos==false){
                std::string str;
                char buffer[3] = {'\000','\000','\000'};
                int tt = temp[start_pos+i*8+j]>=0? temp[start_pos+i*8+j]:256+temp[start_pos+i*8+j];
                //qDebug()<<"tt="<<tt<<Qt::endl;
                sprintf(buffer,"%02X",tt);
                str = buffer;
                char aa[2] = {'\000','\000'};
                aa[0] = temp[start_pos+i*8+j];
                //assert(1==2);
                //ui->tableWidget->setItem(i,j+1,new QTableWidgetItem(QString::number(tt,16)));
                ui->tableWidget->setItem(i,j+1,new QTableWidgetItem(QString::fromStdString(str)));
                char buffer_1[3] = {'\000','\000','\000'};
                std::string str1;
                sprintf(buffer_1,"%c.",temp[start_pos+i*8+j]);
                str1 = buffer_1;
                //qDebug()<<str1<<Qt::endl;
                //ui->tableWidget->setItem(i,j+9,new QTableWidgetItem(QString(QLatin1String(aa))));
                ui->tableWidget->setItem(i,j+9,new QTableWidgetItem(QString::fromStdString(str1)));
                //}
                }
            }
        }
    }
    //ui->spinBox_4->setEnabled(true);
}




void MainWindow::on_spinBox_valueChanged(int arg1)
{

}

