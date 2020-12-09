#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rtf/rtfcpp.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSettings>
#include <QMenu>
#include <QDir>
#include <QDesktopServices>
#include "sqlengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    void createDoc(TableModel &heater, TableModel &content, QString filename);
    QSystemTrayIcon *trayIcon;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;

private slots:
    void slotNewConnection();
    void slotReadClient();
    void closeEvent(QCloseEvent *pe);
    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
