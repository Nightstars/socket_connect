#include "connect.h"
#include <qdebug.h>
#include <QLabel>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	label1 = new QLabel(QStringLiteral("ip ��ַ"));
	label2 = new QLabel(QStringLiteral("�˿�"));
	connnectButton = new QPushButton(QStringLiteral("����"));
	disconnectButton = new QPushButton(QStringLiteral("�Ͽ�"));
	sendButton = new QPushButton(QStringLiteral("����"));
	clearButton = new QPushButton(QStringLiteral("���"));
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
	tcpSocket = NULL;//ʹ��ǰ�����
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
	QByteArray block;//��ʱ�洢������Ҫ���͵�����
	QDataStream out(&block,QIODevice::WriteOnly);//TCP�����������һ��ʹ��
	out.setVersion(QDataStream::Qt_5_7);//�����������İ汾���������������汾һ����ͬ��
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
	if (tcpSocket) delete tcpSocket;//�����ָ�������ռ�ֱ��ɾ��
	tcpSocket = new QTcpSocket(this);//����ѿռ���TCP���ͺͽ��ܲ���
	tcpIp = IPLineEdit->text();
	tcpPort = portLineEdit->text();
	if (tcpIp == NULL || tcpPort == NULL)//�ж�IP��PORT�Ƿ�Ϊ��
	{
		QMessageBox msgBox;
		msgBox.setText("IP or PORT is Empty");
		msgBox.exec();
		return;
	}
	tcpSocket->connectToHost(tcpIp, tcpPort.toInt());//��������
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
		SLOT(displayError(QAbstractSocket::SocketError)));//��������
	connect(tcpSocket, SIGNAL(connected()), this,
		SLOT(connectUpdata()));//��������֮��ť��ʹ��
	connect(tcpSocket, SIGNAL(readyRead()), this,
		SLOT(readMassage()));//��ȡ��Ϣ������
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