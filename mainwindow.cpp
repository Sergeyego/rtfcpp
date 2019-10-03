#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QPixmap(":/icon.png"));
    trayIcon->show();
    setWindowIcon(trayIcon->icon());

    restoreAction = new QAction(QString::fromUtf8("Настройки"), this);
    quitAction = new QAction(QString::fromUtf8("Выход"), this);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 5555)){
        QMessageBox::critical(0,"Server Error","Unable to start the server:" + tcpServer->errorString());
        tcpServer->close();
        qApp->quit();
    }

    connect(tcpServer, SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->cmdSave,SIGNAL(clicked(bool)),this,SLOT(saveSettings()));

}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::createDoc(TableModel &heater, TableModel &content, QString filename)
{
    if (heater.rowCount()<1 || content.columnCount()<3){
        return;
    };
    QString from=heater.data(heater.index(0,4),Qt::EditRole).toString();
    QString to=heater.data(heater.index(0,5),Qt::EditRole).toString();
    RtfWriter rtfWriter;
    RTF_DOCUMENT_FORMAT *df = rtfWriter.get_documentformat();
    RTF_SECTION_FORMAT *sf = rtfWriter.get_sectionformat();
    RTF_PARAGRAPH_FORMAT *ph = rtfWriter.get_paragraphformat();

    RTF_TABLECELL_FORMAT noborderFormat = *rtfWriter.get_tablecellformat();
    RTF_TABLECELL_FORMAT borderFormat = *rtfWriter.get_tablecellformat();
    borderFormat.borderBottom.border=true;
    borderFormat.borderTop.border=true;
    borderFormat.borderLeft.border=true;
    borderFormat.borderRight.border=true;

    df->paperHeight=16838;
    df->paperWidth=11907;
    df->landscape=true;

    sf->pageWidth=16838;
    sf->pageHeight=11907;
    sf->landscape=true;
    sf->pageMarginBottom=850;
    sf->pageMarginTop=850;
    sf->pageMarginLeft=850;
    sf->pageMarginRight=850;

    rtfWriter.start_doc();

    ph->paragraphAligment=2;
    ph->spaceAfter=100;
    ph->CHARACTER.fontSize=16;

    rtfWriter.start_paragraph(QString::fromUtf8("Типовая межотраслевая форма № М-4"),false);
    rtfWriter.start_paragraph(QString::fromUtf8("Утверждена Постановлением Госкомстата России"),true);
    rtfWriter.start_paragraph(QString::fromUtf8("от 30.10.1997 № 71а"),true);
    ph->CHARACTER.fontSize=20;
    ph->CHARACTER.boldCharacter=true;
    ph->paragraphAligment=1;
    rtfWriter.start_paragraph(heater.data(heater.index(0,3),Qt::EditRole).toString().toUpper()+QString::fromUtf8(" № ")+heater.data(heater.index(0,0),Qt::EditRole).toString(),true);
    ph->CHARACTER.boldCharacter=false;
    rtfWriter.start_paragraph("",true);

    rtfWriter.set_tablecellformat(noborderFormat);
    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(7650);
    ph->paragraphAligment=0;
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9350);
    ph->paragraphAligment=2;
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.set_tablecellformat(borderFormat);
    rtfWriter.start_tablecell(11050);
    ph->paragraphAligment=1;
    rtfWriter.start_paragraph(QString::fromUtf8("Коды"),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();
    rtfWriter.start_tablerow();
    rtfWriter.set_tablecellformat(noborderFormat);
    rtfWriter.start_tablecell(7650);
    ph->paragraphAligment=0;
    rtfWriter.start_paragraph(QString::fromUtf8("Организация ООО Судиславский завод сварочных материалов"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9350);
    ph->paragraphAligment=2;
    rtfWriter.start_paragraph(QString::fromUtf8("Форма по ОКУД"),false);
    rtfWriter.end_tablecell();
    rtfWriter.set_tablecellformat(borderFormat);
    rtfWriter.start_tablecell(11050);
    ph->paragraphAligment=1;
    rtfWriter.start_paragraph(QString::fromUtf8("0315003"),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();
    rtfWriter.start_tablerow();
    rtfWriter.set_tablecellformat(noborderFormat);
    rtfWriter.start_tablecell(7650);
    ph->paragraphAligment=0;
    rtfWriter.start_paragraph(QString::fromUtf8("Структурное подразделение"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9350);
    ph->paragraphAligment=2;
    rtfWriter.start_paragraph(QString::fromUtf8("по ОКПО"),false);
    rtfWriter.end_tablecell();
    rtfWriter.set_tablecellformat(borderFormat);
    rtfWriter.start_tablecell(11050);
    ph->paragraphAligment=1;
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    ph->CHARACTER.fontSize=16;
    rtfWriter.start_paragraph("",true);
    rtfWriter.set_tablecellformat(borderFormat);
    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(1670,false,false,true,false);
    ph->paragraphAligment=1;
    rtfWriter.start_paragraph(QString::fromUtf8("Дата составления"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(2350,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Код вида операции"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(3030,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Склад"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8133,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Поставщик"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8983,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10117,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Страховая компания"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10967,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Корреспондирующий счет"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12101,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13235,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Номер документа"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14367,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(1670,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(2350,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(3030,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8133);
    rtfWriter.start_paragraph(QString::fromUtf8("Наименование"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8983);
    rtfWriter.start_paragraph(QString::fromUtf8("Код"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10117,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10967);
    rtfWriter.start_paragraph(QString::fromUtf8("счет, субсчет"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12101);
    rtfWriter.start_paragraph(QString::fromUtf8("код аналитического учета"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13235);
    rtfWriter.start_paragraph(QString::fromUtf8("сопроводительного"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14367);
    rtfWriter.start_paragraph(QString::fromUtf8("платежного"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    QDate date=heater.data(heater.index(0,1),Qt::EditRole).toDate();

    ph->CHARACTER.fontSize=20;
    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(1670);
    rtfWriter.start_paragraph(QLocale().toString(date,("dd MMMM yyyy"))+QString::fromUtf8(" г"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(2350);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(3030);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8133);
    rtfWriter.start_paragraph(heater.data(heater.index(0,2),Qt::EditRole).toString().toUpper(),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8983);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10117);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10967);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12101);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13235);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14367);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    rtfWriter.start_paragraph("",true);
    ph->CHARACTER.fontSize=16;
    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(5103,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Материальные ценности"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(5953,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(6823,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Единица измерения"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(7847,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8863,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Количество"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9883,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10743,false,false,true,false);
    if(content.columnCount()>=4){
        rtfWriter.start_paragraph(QString::fromUtf8("в т.ч. БРАК"),false);
    } else {
        rtfWriter.start_paragraph(QString::fromUtf8("Цена, руб. коп."),false);
    }
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(11603,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Сумма без учета НДС, руб. коп."),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12465,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Сумма НДС, руб. коп."),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13323,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Всего с учетом НДС, руб. коп."),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14183,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Номер паспорта"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047,false,false,true,false);
    rtfWriter.start_paragraph(QString::fromUtf8("Порядковый номер по складской картотеке"),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(5103);
    rtfWriter.start_paragraph(QString::fromUtf8("наименование, сорт, размер, марка"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(5953);
    rtfWriter.start_paragraph(QString::fromUtf8("номенклатурный номер"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(6823);
    rtfWriter.start_paragraph(QString::fromUtf8("код"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(7847);
    rtfWriter.start_paragraph(QString::fromUtf8("наименование"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8863);
    rtfWriter.start_paragraph(QString::fromUtf8("по документу"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9883);
    rtfWriter.start_paragraph(QString::fromUtf8("принято"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10743,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(11603,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12465,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13323,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14183,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047,false,false,false,true);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(5103);
    rtfWriter.start_paragraph(QString::fromUtf8("1"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(5953);
    rtfWriter.start_paragraph(QString::fromUtf8("2"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(6823);
    rtfWriter.start_paragraph(QString::fromUtf8("3"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(7847);
    rtfWriter.start_paragraph(QString::fromUtf8("4"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8863);
    rtfWriter.start_paragraph(QString::fromUtf8("5"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9883);
    rtfWriter.start_paragraph(QString::fromUtf8("6"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(10743);
    rtfWriter.start_paragraph(QString::fromUtf8("7"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(11603);
    rtfWriter.start_paragraph(QString::fromUtf8("8"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12465);
    rtfWriter.start_paragraph(QString::fromUtf8("9"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13323);
    rtfWriter.start_paragraph(QString::fromUtf8("10"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14183);
    rtfWriter.start_paragraph(QString::fromUtf8("11"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047);
    rtfWriter.start_paragraph(QString::fromUtf8("12"),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    ph->CHARACTER.fontSize=20;
    double sum=0.0, sumbr=0.0;

    for (int i=0; i<content.rowCount(); i++){
        rtfWriter.start_tablerow();
        rtfWriter.start_tablecell(5103);
        ph->paragraphAligment=0;
        rtfWriter.start_paragraph(QString::number(i+1)+QString(". ")+content.data(content.index(i,0),Qt::EditRole).toString(),false);
        ph->paragraphAligment=1;
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(5953);     
        rtfWriter.start_paragraph(content.data(content.index(i,1),Qt::EditRole).toString(),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(6823);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(7847);
        rtfWriter.start_paragraph(QString::fromUtf8("кг"),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(8863);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(9883);
        double val=content.data(content.index(i,2),Qt::EditRole).toDouble();
        sum+=val;

        QString strbr;
        if (content.columnCount()>=4){
            double valbr=content.data(content.index(i,3),Qt::EditRole).toDouble();
            sumbr+=valbr;
            if (valbr>0) {
                strbr=QLocale().toString(valbr,'f',2);
            }
        }
        ph->paragraphAligment=2;
        rtfWriter.start_paragraph(QLocale().toString(val,'f',2),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(10743);
        rtfWriter.start_paragraph(strbr,false);
        rtfWriter.end_tablecell();
        ph->paragraphAligment=1;
        rtfWriter.start_tablecell(11603);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(12465);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(13323);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(14183);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.start_tablecell(15047);
        rtfWriter.start_paragraph(QString::fromUtf8(""),false);
        rtfWriter.end_tablecell();
        rtfWriter.end_tablerow();
    }

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(5103);
    ph->paragraphAligment=0;
    rtfWriter.start_paragraph(QString::fromUtf8("ИТОГО"),false);
    ph->paragraphAligment=1;
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(5953);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(6823);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(7847);
    rtfWriter.start_paragraph(QString::fromUtf8("кг"),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(8863);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(9883);
    ph->paragraphAligment=2;
    rtfWriter.start_paragraph(QLocale().toString(sum,'f',2),false);
    rtfWriter.end_tablecell();
    QString brit;
    if (sumbr>0){
        brit=QLocale().toString(sumbr,'f',2);
    }
    rtfWriter.start_tablecell(10743);
    rtfWriter.start_paragraph(brit,false);
    rtfWriter.end_tablecell();
    ph->paragraphAligment=1;
    rtfWriter.start_tablecell(11603);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(12465);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(13323);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(14183);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.start_tablecell(15047);
    rtfWriter.start_paragraph(QString::fromUtf8(""),false);
    rtfWriter.end_tablecell();
    rtfWriter.end_tablerow();

    ph->paragraphAligment=0;
    rtfWriter.start_paragraph(QString::fromUtf8(""),true);
    QString txt;
    if (!to.isEmpty()){
        txt+=QString::fromUtf8("ПРИНЯЛ _________________ ")+to;
    }
    if (!txt.isEmpty() && !from.isEmpty()){
        txt+=QString::fromUtf8("                               ");
    }
    if (!from.isEmpty()){
        txt+=QString::fromUtf8("СДАЛ _________________")+from;
    }

    rtfWriter.start_paragraph(txt,true);

    rtfWriter.end_doc();
    rtfWriter.saveDoc(filename);
    QString cmd=ui->lineEditOffice->text()+" "+filename+" &";

    system(cmd.toLocal8Bit().data());
    //qDebug()<<filename;
}

void MainWindow::slotNewConnection()
{
    QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()),this,SLOT(slotReadClient()));
    //qDebug()<<"new connection";
}

void MainWindow::slotReadClient()
{
    //qDebug()<<"ready read";
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QString mes=QString::fromLocal8Bit(pClientSocket->readAll().data());
    QStringList list=mes.split(':');
    ui->lineEditMess->setText(mes);
    bool ok=false;
    QString naklVid, naklTip;
    if (list.size()==4){
        TableModel heater, content;
        if (list.at(0).toInt()==0){
            //Электроды
            naklVid=QString::fromUtf8("Электроды");
            if (list.at(1).toInt()==0){
                //Цех
                naklTip=QString::fromUtf8("Цех");
                QString query="select n.num, n.dat, t.nam, d.nam, ef.nam, et.nam from parti_nakl as n "
                              "inner join parti_nakl_tip as t on t.id=n.tip "
                              "inner join nakl_doc as d on t.id_doc=d.id "
                              "inner join nakl_emp as ef on t.id_from=ef.id "
                              "inner join nakl_emp as et on t.id_to=et.id "
                              "where n.id = ?";
                sqlParams param;
                param.push_back(list.at(2).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);
                if (ok){
                    QString queryCont;
                    if (list.at(3).toInt()==1){
                        queryCont="select e.marka||' '||"+QString::fromUtf8("'ф'")+"||p.diam, p.n_s, w.kvo from parti_pack as w "
                                                                                   "inner join parti as p on p.id=w.id_part "
                                                                                   "inner join elrtr as e on e.id=p.id_el "
                                                                                   "where w.id_nakl = ? order by w.id";
                    } else if (list.at(3).toInt()==2){
                        queryCont="select e.marka||' '||"+QString::fromUtf8("'ф'")+"||p.diam, p.n_s, w.kvo from parti_break as w "
                                                                                   "inner join parti as p on p.id=w.id_part "
                                                                                   "inner join elrtr as e on e.id=p.id_el "
                                                                                   "where w.id_nakl = ? order by w.id";
                    }
                    sqlParams paramCont;
                    paramCont.push_back(list.at(2).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }

            } else if (list.at(1).toInt()==1){
                //Склад
                naklTip=QString::fromUtf8("Склад");
                QString query="select n.num, n.dat, t.fnam, d.nam, ef.nam, et.nam from prod_nakl as n "
                              "inner join prod_nakl_tip as t on t.id=n.id_ist "
                              "inner join nakl_doc as d on t.id_doc=d.id "
                              "inner join nakl_emp as ef on t.id_from=ef.id "
                              "inner join nakl_emp as et on t.id_to=et.id "
                              "where n.id = ?";
                sqlParams param;
                param.push_back(list.at(2).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);
                if (ok){
                    QString queryCont;
                    queryCont="select e.marka||' '||"+QString::fromUtf8("'ф'")+"||p.diam, p.n_s, w.kvo from prod as w "
                                                                               "inner join parti as p on p.id=w.id_part "
                                                                               "inner join elrtr as e on e.id=p.id_el "
                                                                               "where w.id_nakl = ? order by w.id";
                    sqlParams paramCont;
                    paramCont.push_back(list.at(2).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }
            } else if (list.at(1).toInt()==2){
                //собственное потребление
                naklTip=QString::fromUtf8("Собственное_потребление");
                QString query="select n.num, n.dat, n.kto, (select d.nam from nakl_doc as d where d.id=2), (select f.nam from nakl_emp as f where f.id=1), NULL "
                              "from prod_self as n "
                              "where n.id = ?";
                sqlParams param;
                param.push_back(list.at(2).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);
                if (ok){
                    QString queryCont;
                    queryCont="select e.marka||' '||"+QString::fromUtf8("'ф'")+"||p.diam, p.n_s, w.kvo from prod_self_items as w "
                                                                               "inner join parti as p on p.id=w.id_part "
                                                                               "inner join elrtr as e on e.id=p.id_el "
                                                                               "where w.id_self = ? ";
                    sqlParams paramCont;
                    paramCont.push_back(list.at(2).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }
            } else if (list.at(1).toInt()==3){
                //переупаковка
                naklTip=QString::fromUtf8("Переупаковка");
                QString query="select n.num, n.dat, t.nam, d.nam, ef.nam, et.nam from parti_nakl as n "
                              "inner join parti_nakl_tip as t on t.id=n.tip "
                              "inner join nakl_doc as d on t.id_doc=d.id "
                              "inner join nakl_emp as ef on t.id_from=ef.id "
                              "inner join nakl_emp as et on t.id_to=et.id "
                              "where n.id = ?";
                sqlParams param;
                param.push_back(list.at(2).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);
                if (ok){
                    QString queryCont;
                    queryCont=QString::fromUtf8("select 'из '||e.marka||' '||'ф'||p.diam || CASE WHEN (np.id=0) THEN '' ELSE '\nв '||ne.marka||' '||'ф'|| np.diam ||'' END, "
                                                "'из ' || p.n_s || CASE WHEN (np.id=0) THEN '' ELSE '\nв '||np.n_s END, w.kvo, w.kvo_break from parti_perepack as w "
                                                "inner join parti as p on p.id=w.id_part "
                                                "inner join elrtr as e on e.id=p.id_el "
                                                "inner join parti as np on np.id=w.id_new_part "
                                                "inner join elrtr as ne on ne.id=np.id_el "
                                                "where w.id_nakl = ? ");
                    sqlParams paramCont;
                    paramCont.push_back(list.at(2).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }
            }
        } else {
            //Проволока
            naklVid=QString::fromUtf8("Проволока");
            if (list.at(1).toInt()==0){
                //Цех
                naklTip=QString::fromUtf8("Цех");
                QDate dat=QDate::fromString(list.at(2),"dd.MM.yyyy");
                QString query="select date_part('doy', ? ::date), ? ::date, t.nam, d.nam, ef.nam, et.nam from wire_in_cex_type as t "
                              "inner join nakl_doc as d on t.id_doc=d.id "
                              "inner join nakl_emp as ef on t.id_from=ef.id "
                              "inner join nakl_emp as et on t.id_to=et.id "
                              "where t.id = ?";
                sqlParams param;
                param.push_back(dat.toString("yyyy-MM-dd"));
                param.push_back(dat.toString("yyyy-MM-dd"));
                param.push_back(list.at(3).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);
                if (ok){
                    QString queryCont="select "+QString::fromUtf8("'проволока '")+"||pr.nam||' '||"+QString::fromUtf8("'ф'")+"||d.sdim||' '||k.short, m.n_s, w.m_netto from wire_in_cex_data as w "
                                                                                                                             "inner join wire_parti as p on p.id=w.id_wparti "
                                                                                                                             "inner join wire_parti_m as m on m.id=p.id_m "
                                                                                                                             "inner join provol as pr on pr.id=m.id_provol "
                                                                                                                             "inner join diam as d on d.id=m.id_diam "
                                                                                                                             "inner join wire_pack_kind as k on k.id=p.id_pack "
                                                                                                                             "where w.dat = ? and w.id_type = ? order by w.id";
                    sqlParams paramCont;
                    paramCont.push_back(dat);
                    paramCont.push_back(list.at(3).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }

            } else if (list.at(1).toInt()==1) {
                //Склад
                naklTip=QString::fromUtf8("Склад");
                QString query="select w.num, w.dat, t.fnam, d.nam, ef.nam, et.nam from wire_whs_waybill as w "
                              "inner join wire_way_bill_type as t on t.id=w.id_type "
                              "inner join nakl_doc as d on t.id_doc=d.id "
                              "inner join nakl_emp as ef on t.id_from=ef.id "
                              "inner join nakl_emp as et on t.id_to=et.id "
                              "where w.id = ?";
                sqlParams param;
                param.push_back(list.at(2).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);

                if (ok){
                    QString queryCont="select "+QString::fromUtf8("'проволока '")+"||pr.nam||' '||"+QString::fromUtf8("'ф'")+"||d.sdim||' '||k.short, m.n_s, w.m_netto from wire_warehouse as w "
                                                                                                                             "inner join wire_parti as p on p.id=w.id_wparti "
                                                                                                                             "inner join wire_parti_m as m on m.id=p.id_m "
                                                                                                                             "inner join provol as pr on pr.id=m.id_provol "
                                                                                                                             "inner join diam as d on d.id=m.id_diam "
                                                                                                                             "inner join wire_pack_kind as k on k.id=p.id_pack "
                                                                                                                             "where w.id_waybill = ? order by w.id";
                    sqlParams paramCont;
                    paramCont.push_back(list.at(2).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }

            } else if (list.at(1).toInt()==2) {
                //переупаковка
                naklTip=QString::fromUtf8("Переупаковка");
                QString query="select w.num, w.dat, '"+QString::fromUtf8("Переупаковка")+"', (select d.nam from nakl_doc as d where d.id=1), (select f.nam from nakl_emp as f where f.id=1), NULL "
                              "from wire_perepack_nakl as w "
                              "where w.id = ?";
                sqlParams param;
                param.push_back(list.at(2).toInt());
                ok=SqlEngine::executeQuery(query,param,&heater);
                if (ok){
                    QString queryCont="select '"+QString::fromUtf8("из ")+"'||pr.nam||' '||'"+QString::fromUtf8("ф")+"'||d.sdim||' '||k.short ||'"+QString::fromUtf8("\n в ")+"' ||rpr.nam||' '|| "
                            "'"+QString::fromUtf8("ф")+"'||rd.sdim||' '||rk.short as str, NULL, sum(w.m_netto) "
                            "from wire_perepack as w "
                            "inner join wire_parti as p on p.id=w.id_wpartisrc "
                            "inner join wire_parti_m as m on m.id=p.id_m "
                            "inner join provol as pr on pr.id=m.id_provol "
                            "inner join diam as d on d.id=m.id_diam "
                            "inner join wire_pack_kind as k on k.id=p.id_pack "

                            "inner join wire_parti as rp on rp.id=w.id_wpartires "
                            "inner join wire_parti_m as rm on rm.id=rp.id_m "
                            "inner join provol as rpr on rpr.id=rm.id_provol "
                            "inner join diam as rd on rd.id=rm.id_diam "
                            "inner join wire_pack_kind as rk on rk.id=rp.id_pack "
                            "where w.id_nakl = ? "
                            "group by str";
                    sqlParams paramCont;
                    paramCont.push_back(list.at(2).toInt());
                    ok=SqlEngine::executeQuery(queryCont,paramCont,&content);
                }

            }
        }
        if (ok) {
            QDir dir(QDir::homePath()+QString::fromUtf8("/Накладные"));
            if (!dir.exists()) dir.mkdir(dir.path());
            dir.setPath(dir.path()+"/"+naklVid);
            if (!dir.exists()) dir.mkdir(dir.path());
            dir.setPath(dir.path()+"/"+naklTip);
            if (!dir.exists()) dir.mkdir(dir.path());
            QString year=QString::number(heater.data(heater.index(0,1),Qt::EditRole).toDate().year());
            dir.setPath(dir.path()+"/"+year);
            if (!dir.exists()) dir.mkdir(dir.path());
            QString filename=heater.data(heater.index(0,2),Qt::EditRole).toString().toUpper()+"-"+heater.data(heater.index(0,0),Qt::EditRole).toString();
            filename=filename.replace(QRegExp("[^\\w]"), "_");
            QString totalName=dir.path()+"/"+filename+".rtf";
            createDoc(heater,content,totalName);
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *pe)
{
    this->hide();
    pe->ignore();
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "rtfcpp");
    ui->lineEditOffice->setText(settings.value("office",QString::fromUtf8("libreoffice")).toString());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "rtfcpp");
    settings.setValue("office",ui->lineEditOffice->text());
}


