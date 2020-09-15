#include "connect.h"
#include <qdebug.h>
#include <QLabel>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	label1 = new QLabel(QStringLiteral("ip 地址"));
	label2 = new QLabel(QStringLiteral("端口"));
	connnectButton = new QPushButton(QStringLiteral("连接"));
	disconnectButton = new QPushButton(QStringLiteral("断开"));
	sendButton = new QPushButton(QStringLiteral("发送"));
	clearButton = new QPushButton(QStringLiteral("清空"));
	IPLineEdit = new QLineEdit();
	portLineEdit = new QLineEdit();
	sendLineEdit = new QLineEdit();
	clearLineEdit = new QLineEdit();
	mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(label1);
	mainLayout->addWidget(IPLineEdit);
	mainLayout->addWidget(connnectButton);
	mainLayout->addWidget(label2);
	mainLayout->addWidget(portLineEdit);
	mainLayout->addWidget(disconnectButton);
	mainLayout->addWidget(sendLineEdit);
	mainLayout->addWidget(sendButton);
	mainLayout->addWidget(clearLineEdit);
	mainLayout->addWidget(clearButton);
	sendButton->setEnabled(false);
	disconnectButton->setEnabled(false);
	IPLineEdit->setText("127.0.0.1");
	portLineEdit->setText("8000");
	tcpSocket = NULL;//使用前先清空
	connect(connnectButton, SIGNAL(clicked()), this,SLOT(on_connnectButton_clicked()));
	connect(sendButton, SIGNAL(clicked()), this,SLOT(on_sendButton_clicked()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(on_clearButton_clicked()));
	connect(disconnectButton, SIGNAL(clicked()), this, SLOT(on_disconnectButton_clicked()));
}
MainWindow::~MainWindow()
{
	delete tcpSocket;
}
void MainWindow::sendMassage() {}
void MainWindow::readMassage()
{
	QByteArray data = tcpSocket->readAll();
	clearLineEdit->setText(QString(data));
}
void MainWindow::displayError(QAbstractSocket::SocketError)
{
	QMessageBox::warning(this, tr("Warnning"), tcpSocket->errorString());
	tcpSocket->close();
	connnectButton->setEnabled(true);
	disconnectButton->setEnabled(false);
	sendButton->setEnabled(false);
}
void MainWindow::on_sendButton_clicked()
{
	QString sendmessage;
	sendmessage =sendLineEdit->text();
	/* if(sendmessage == NULL) return;
	QByteArray block;//暂时存储我们需要发送的数据
	QDataStream out(&block,QIODevice::WriteOnly);//TCP必须和数据流一起使用
	out.setVersion(QDataStream::Qt_5_7);//设置数据流的版本（服务器和主机版本一定相同）
	out << sendmessage;
	tcpSocket->write(block);*/
	QByteArray data;
	data.append(sendmessage);
	tcpSocket->write(data);
}
void MainWindow::on_clearButton_clicked()
{
	clearLineEdit->clear();
}
void MainWindow::on_connnectButton_clicked()
{
	flag = false;
	if (tcpSocket) delete tcpSocket;//如果有指向其他空间直接删除
	tcpSocket = new QTcpSocket(this);//申请堆空间有TCP发送和接受操作
	tcpIp = IPLineEdit->text();
	tcpPort = portLineEdit->text();
	if (tcpIp == NULL || tcpPort == NULL)//判断IP和PORT是否为空
	{
		QMessageBox msgBox;
		msgBox.setText("IP or PORT is Empty");
		msgBox.exec();
		return;
	}
	tcpSocket->connectToHost(tcpIp, tcpPort.toInt());//连接主机
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
		SLOT(displayError(QAbstractSocket::SocketError)));//错误连接
	connect(tcpSocket, SIGNAL(connected()), this,
		SLOT(connectUpdata()));//更新连接之后按钮的使能
	connect(tcpSocket, SIGNAL(readyRead()), this,
		SLOT(readMassage()));//读取信息的连接
	connnectButton->setEnabled(false);
	disconnectButton->setEnabled(true);
}
void MainWindow::on_disconnectButton_clicked()
{
	tcpSocket->abort();
	delete tcpSocket;
	tcpSocket = NULL;
	disconnectUpdata();
}
void MainWindow::connectUpdata()
{
	if (!flag)
	{
		QMessageBox msgBox;
		msgBox.setText("TCP connect successful");
		msgBox.exec();
		connnectButton->setEnabled(false);
		sendButton->setEnabled(true);
		disconnectButton->setEnabled(true);
		IPLineEdit->setEnabled(false);
		portLineEdit->setEnabled(false);
	}
	flag = true;
}
void MainWindow::disconnectUpdata()
{
	connnectButton->setEnabled(true);
	sendButton->setEnabled(false);
	disconnectButton->setEnabled(false);
	IPLineEdit->setEnabled(true);
	portLineEdit->setEnabled(true);
}