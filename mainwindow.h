#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oauth2.h"
#include "manager_bigquery.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startLogin(bool bForce);
    
private:
    Ui::MainWindow *ui;

    QString m_organizationName;
    QString m_appName;
    QString m_API_key;
    QString m_ClientID;
    QString m_RedirectURI;
    OAuth2* m_pOAuth2;

    ManagerBigQuery m_managerBigQuery;

    //if login succefully done
    void loginDone();

private slots:
    //try login
    void login();
    //show form for set API params
    void setApiParams();
    void on_pbGetDatasets_clicked();    
    void selectedDatasetChanged();
    void selectedTableChanged();

    //slots for API request
    void addReplyText(QString str);
    void recvDatasetsList(const QVariantList& list);
    void recvTablesList(const QVariantList& list);
    void recvColumnsList(const QVariantList& list);
    void recvTableData(const QVariantList& list);

    void on_pbGetTableData_clicked();
    void on_pbExecQuery_clicked();
};

#endif // MAINWINDOW_H
