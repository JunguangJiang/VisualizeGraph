#include "dialog.h"
#include "ui_dialog.h"
#include "parameter.h"
#include <QUrl>
#include <QWebEngineView>
#include <QDebug>
#include <QFileInfo>
#include "graph.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    view = new QWebEngineView(ui->graphicsView);
    view->show();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_minSpanTreeButton_clicked()
{
    Graph graph;
    graph.readFromFile(NetworkFile_5);
    int s = 114303;
    //graph->getMinSpanTree(s);//找到以s为树根的最小生成树,Note：这种方式只能找到以s为树根的最小生成树，但是在d3处理中速度和效果较好
    graph.getMinSpanTree();//找最小生成树，Note：这种方式找到全图所有的最小生成树，但是在d3处理中速度和效果较差
    graph.writeMinSpanTree(MinSpanTreeFile, true);//并写入文件,选择删除孤立点

    view->resize(ui->graphicsView->size());
    QString minSpanTreeHtmlAbsolutePath = "file:///"+QFileInfo(MinSpanTreeHtml).absoluteFilePath();//最小生成树的html的绝对路径
    view->load(minSpanTreeHtmlAbsolutePath);//加载最小生成树
}

void Dialog::on_shortestPathButton_clicked()
{
    int source = ui->sourceEdit->text().toInt();//得到源点
    int target = ui->targetEdit->text().toInt();//和终点
    Graph graph;
    graph.readFromFile(NetworkFile_5);
    if( graph.isValidVertex(source) && graph.isValidVertex(target) ){//如果源点和终点输入都合法，则
        QVector<int> path;
        graph.getShortestPath(source, target, path);//才搜索一条最短路径
        graph.writeShortestPath(ShortestPathFile, path);//并写入文件中

        view->resize(ui->graphicsView->size());
        QString shortestPathHtmlAbsolutePath = "file:///"+QFileInfo(ShortestPathHtml).absoluteFilePath();//最短路径的html的绝对路径
        view->load(shortestPathHtmlAbsolutePath);//加载最短路径
    }else{
        qDebug()<<"error input";
    }
}

void Dialog::on_connectedComponentButton_clicked()
{

}
