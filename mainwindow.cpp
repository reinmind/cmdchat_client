#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectedToHost = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    if(!connectedToHost)
    {
        socket = new QTcpSocket();

        connect(socket,SIGNAL(connected()),this,
                SLOT(socketConnected()));
        connect(socket,SIGNAL(disconnect()),this,
                SLOT(socketDisconnected()));
        connect(socket,SIGNAL(readyRead()),this,
                SLOT(socketReadyRead()));

        socket->connectToHost("127.0.0.1",8001);
    }
    else
    {
        QString name = ui->nameInput->text();
        socket->write("<font color=\"orange\">"+name.toUtf8()+" has left the chat room.</font>");

        socket->disconnectFromHost();
    }
}

void MainWindow::socketConnected()
{
    qDebug() << "Connected to server.";

    printMessage("<font color=\"green\">Connected from server.</font>");

    QString name = ui->nameInput->text();
    socket->write("<font color=\"purple\">" + name.toUtf8() + " has joined the chat room.</font>");

    ui->connectButton->setText("Disconnect");
    connectedToHost = true;

}

void MainWindow::socketDisconnected()
{
    qDebug() << "Disconnected from server.";

    printMessage("<font color=\"red\">Disconnected from server.</font>");

    ui->connectButton->setText("Connect");
    connectedToHost = false;
}

void MainWindow::socketReadyRead()
{
   ui->chatDisplay->append(socket->readAll());

}

void MainWindow::printMessage(QString message)
{
    ui->chatDisplay->append(message);
}

void MainWindow::on_sendButton_clicked()
{
    QString name = ui->nameInput->text();
    QString message = ui->messageInput->text();
    socket->write("<font color=\"blue\">"+name.toUtf8()+":</font>"+message.toUtf8());

    ui->messageInput->clear();
}


void MainWindow::on_messageInput_returnPressed()
{
    on_sendButton_clicked();
}
