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

private:
    Ui::Dialog *ui;
    QWebEngineView *view;

};

#endif // DIALOG_H
