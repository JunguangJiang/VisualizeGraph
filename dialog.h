#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWebEngineView>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_minSpanTreeButton_clicked();

    void on_shortestPathButton_clicked();

    void on_connectedComponentButton_clicked();


    void on_ConnectedComponentQuickButton_clicked();

private:
    Ui::Dialog *ui;
    QWebEngineView *view;

    void getGroupNumberWithDifferentThread(QString filename);//计算不同阈值下的连通域个数
};

#endif // DIALOG_H
