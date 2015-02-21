#include "sicilyai.h"
#include "ui_sicilyai.h"

SicilyAI::SicilyAI(QWidget *parent) :QMainWindow(parent),ui(new Ui::SicilyAI),sharedMem("ThisisSicily")
{

    if(sharedMem.create(sizeof(sicilyConnect))){
        exit(-1);
        qDebug("shared memory created");
        sharedMem.lock();
        sicilyConnect = (SicilyConnect*)(sharedMem.data());
        sicilyConnect->boxLife = 10;
        //sicilyConnect->text = "Hello Sicily! >.<";
        sharedMem.unlock();
    }
    else if (sharedMem.attach()){
        //exit();
        qDebug("Attach Success");
        sharedMem.lock();
        sicilyConnect = (SicilyConnect*)(sharedMem.data());
        sharedMem.unlock();
    }

    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    //connect(ui->sicily, SIGNAL(click()), this, SLOT(ChangeChatBox()));
    timer->start(25);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(checkTime()));
    timer2->start(1000);
}

void SicilyAI::checkTime(){
    sharedMem.lock();
    sicilyConnect->boxLife = 40*5;
    char *str = sicilyConnect->text;
    sprintf(str,"%d",time(NULL));

    //string str = s;
    //if(str.length()>511)str = str.substr(0,511);
    //memcpy(sicilyConnect->text,str.c_str(),511);
    //sicilyConnect->text[511]='\0';
    sicilyConnect->update = true;
    sharedMem.unlock();
}

void SicilyAI::timerUpDate(){
    sharedMem.lock();
    sicilyConnect->boxLife = 40*5;
    //qDebug("-%d-",sicilyConnect->boxLife);
    //QString str= sicilyConnect->text.c_str();
    //ui->textEdit->setText(str);
    string str = ui->textEdit->toPlainText().toStdString();
    if(str.length()>511)str = str.substr(0,511);
    memcpy(sicilyConnect->text,str.c_str(),511);
    sicilyConnect->text[511]='\0';
    sicilyConnect->update = true;
    sharedMem.unlock();
}

SicilyAI::~SicilyAI(){
    sharedMem.detach();
    delete ui;
}
