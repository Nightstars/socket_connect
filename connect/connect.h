#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QLineEdit>
#include <QLineEdit>
#include <QVBoxLayout>
class MainWindow : public QWidget
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void sendMassage();
	void readMassage();
	void displayError(QAbstractSocket::SocketError);
	void connectUpdata();
	void disconnectUpdata();
	void on_sendButton_clicked();
	void on_clearButton_clicked();
	void on_connnectButton_clicked();
	void on_disconnectButton_clicked();
private:
	//QTcpServer *tcpServer;//�����ٽ������������ˣ�ֱ�ӽ���������׽���
	QTcpSocket * tcpSocket;//ֱ�ӽ���TCP�׽�����
	QString tcpIp;//�洢IP��ַ
	QString tcpPort;//�洢�˿ڵ�ַ
	bool flag;
	QLabel *label1;
	QLabel *label2;
	QLineEdit *IPLineEdit;
	QLineEdit *portLineEdit;
	QLineEdit *sendLineEdit;
	QLineEdit *clearLineEdit;
	QPushButton *connnectButton;
	QPushButton *disconnectButton;
	QPushButton *sendButton;
	QPushButton *clearButton;
	QVBoxLayout *mainLayout;
};
#endif // MAINWINDOW_H