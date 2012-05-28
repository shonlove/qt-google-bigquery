#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include "settings_api.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_organizationName = "ICS";
    m_appName = "qt-google-bigquery";

    QSettings settings(m_organizationName, m_appName);
    m_API_key = settings.value("API_KEY").toString();
    m_ClientID = settings.value("ClientID").toString();
    m_RedirectURI = settings.value("RedirectURI").toString();
    ui->leClientID->setText(settings.value("ValueClientID").toString());
    m_managerBigQuery.setAPIKey(m_API_key);

    m_pOAuth2 = new OAuth2(this);
    //m_API_key = m_pOAuth2->getSimpleAPIKey();
    QString str = "https://www.googleapis.com/auth/bigquery";

    m_pOAuth2->setScope(str);
    m_pOAuth2->setClientID(m_ClientID);
    m_pOAuth2->setRedirectURI(m_RedirectURI);
    m_pOAuth2->setCompanyName("Home");
    m_pOAuth2->setAppName("QtPrediction");
    connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(loginDone()));

    connect(&m_managerBigQuery, SIGNAL(replyText(QString)), this, SLOT(addReplyText(QString)));
    connect(&m_managerBigQuery, SIGNAL(recvDatasetsList(QVariantList)), this, SLOT(recvDatasetsList(QVariantList)));
    connect(&m_managerBigQuery, SIGNAL(recvTablesList(QVariantList)), this, SLOT(recvTablesList(QVariantList)));
    connect(&m_managerBigQuery, SIGNAL(recvColumnsList(QVariantList)), this, SLOT(recvColumnsList(QVariantList)));
    connect(&m_managerBigQuery, SIGNAL(recvTableData(QVariantList)), this, SLOT(recvTableData(QVariantList)));

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionLogin, SIGNAL(triggered()), this, SLOT(login()));
    connect(ui->actionAPI_params, SIGNAL(triggered()), this, SLOT(setApiParams()));

    connect(ui->lwDatasetsList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedDatasetChanged()));
    connect(ui->lwTablesList, SIGNAL(itemSelectionChanged()), this, SLOT(selectedTableChanged()));


    startLogin(false);
    m_managerBigQuery.setAccessToken(m_pOAuth2->accessToken());
}

MainWindow::~MainWindow()
{
    QSettings settings(m_organizationName, m_appName);
    settings.setValue("ValueClientID", ui->leClientID->text());
    delete ui;
}

void MainWindow::login()
{
    startLogin(true);
}


void MainWindow::loginDone()
{
    statusBar()->showMessage("Login done.", 2000);
    m_managerBigQuery.setAccessToken(m_pOAuth2->accessToken());
}

void MainWindow::setApiParams()
{
    SettingsAPI formSettingsAPI(m_organizationName, m_appName, this);
    if (formSettingsAPI.exec() == QDialog::Accepted)
    {
        QSettings settings(m_organizationName, m_appName);
        m_API_key = settings.value("API_KEY").toString();
        m_ClientID = settings.value("ClientID").toString();
        m_RedirectURI = settings.value("RedirectURI").toString();

//        m_managerPrediction.setAPIKey(m_API_key);
    }
}

void MainWindow::addReplyText(QString str)
{
    ui->teHTTPReplies->setPlainText(str);
}

void MainWindow::recvDatasetsList(const QVariantList &list)
{
    ui->lwDatasetsList->clear();

    QVariant var, var2;
    QString str;
    for (int i=0; i < list.size(); i++)
    {
        var = list.at(i);
        var2 = var.toMap()["datasetReference"];
        str = var2.toMap()["datasetId"].toString();
        ui->lwDatasetsList->insertItem(0, new QListWidgetItem(str));
    }
}

void MainWindow::recvTablesList(const QVariantList &list)
{
    ui->lwTablesList->clear();

    QVariant var, var2;
    QString str;
    for (int i=0; i < list.size(); i++)
    {
        var = list.at(i);
        var2 = var.toMap()["tableReference"];
        str = var2.toMap()["tableId"].toString();
        ui->lwTablesList->insertItem(0, new QListWidgetItem(str));
    }
}

void MainWindow::recvColumnsList(const QVariantList &list)
{
    ui->lwColumns->clear();
    QVariant var;
    QString str;
    for (int i=0; i < list.size(); i++)
    {
        var = list.at(i);
        str = var.toMap()["name"].toString();
        str += " " + var.toMap()["type"].toString();
        str += " " + var.toMap()["mode"].toString();
        ui->lwColumns->insertItem(0, new QListWidgetItem(str));
    }
}

void MainWindow::recvTableData(const QVariantList &list)
{
    ui->lwData->clear();
    QVariant var;
    QString str;
    for (int i=0; i < list.size(); i++)
    {
        var = list.at(i);

        QVariant var2 = var.toMap()["f"];
        qDebug() << "---------" << var2;
        QVariantList _varList = var2.toList();
        qDebug() << "+++++++" << _varList.size();
        str = "";
        for (int j=0; j < _varList.size(); j++)
        {
            qDebug() << "*********" << _varList.at(j).toString();
            str = str + _varList.at(j).toString() + ";";
        }

        ui->lwData->insertItem(0, new QListWidgetItem(str));
    }
}


void MainWindow::startLogin(bool bForce)
{
    //Now we allow to start logging in when m_oauth2.isAuthorized().
    //User can log in using another Google account.
    if (bForce) {
        m_pOAuth2->startLogin(true);
        return;
    }

    if(!m_pOAuth2->isAuthorized())
    {
        m_pOAuth2->startLogin(bForce); //this is a parent widget for a login dialog.
    }
    else
    {
        loginDone();
    }
}


void MainWindow::on_pbGetDatasets_clicked()
{
    m_managerBigQuery.getDatasetsList(ui->leClientID->text());
}

void MainWindow::selectedDatasetChanged()
{
    m_managerBigQuery.getTablesListForDataset(ui->leClientID->text(), ui->lwDatasetsList->currentItem()->text());
}

void MainWindow::selectedTableChanged()
{
    m_managerBigQuery.getColumnsForTable(ui->leClientID->text(),
        ui->lwDatasetsList->currentItem()->text(), ui->lwTablesList->currentItem()->text());
}


void MainWindow::on_pbGetTableData_clicked()
{
    if (!ui->lwDatasetsList->currentItem()) return;
    if (!ui->lwTablesList->currentItem()) return;
    m_managerBigQuery.geTableData(ui->leClientID->text(),
        ui->lwDatasetsList->currentItem()->text(), ui->lwTablesList->currentItem()->text());
}
