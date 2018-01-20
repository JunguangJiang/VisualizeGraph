#include "dialog.h"
#include "ui_dialog.h"
#include "parameter.h"
#include <QUrl>
#include <QWebEngineView>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include "graph.h"

#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    QDir::setCurrent(QApplication::applicationDirPath());
    ui->setupUi(this);
    view = new QWebEngineView(ui->graphicsView);
    view->show();
    dataSet = DOUBAN;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_minSpanTreeButton_clicked()
{
    Graph graph;
    switch (dataSet) {
    case DOUBAN:
        graph.readFromFile(NetworkFile_5_08);
        break;
    case ZHIHU:
        graph.readFromFile(ZhihuNetworkFile);
        break;
    }
    int groupNumber = graph.getMinSpanTree();//找最小生成树，Note：这种方式找到全图所有的最小生成树，但是在d3处理中速度和效果较差
    QString minSpanTreeString = QString("全图由") + QString::number(groupNumber) + QString("个最小生成树组成，图中只显示非孤立点");
    ui->resultEdit->setText(minSpanTreeString);

    //并进行可视化
    graph.writeMinSpanTree(MinSpanTreeFile, true);//并写入文件,选择删除孤立点
    view->resize(ui->graphicsView->size());
    QString minSpanTreeHtmlAbsolutePath = "file:///"+QFileInfo(MinSpanTreeHtml).absoluteFilePath();//最小生成树的html的绝对路径
    view->show();
    view->load(minSpanTreeHtmlAbsolutePath);//加载最小生成树
}

void Dialog::on_shortestPathButton_clicked()
{
    int source = ui->sourceEdit->text().toInt();//得到源点
    int target = ui->targetEdit->text().toInt();//和终点
    Graph graph;
    switch (dataSet) {
    case DOUBAN:
        graph.readFromFile(NetworkFile_5_08);
        break;
    case ZHIHU:
        graph.readFromFile(ZhihuNetworkFile);
        break;
    }
    if( graph.isValidVertex(source) && graph.isValidVertex(target) ){//如果源点和终点输入都合法，则
        QVector<int> path;
        double pathLength = graph.getShortestPath(source, target, path);//才搜索一条最短路径

        if(pathLength == PRIORITY_MAX){//如果二者之间不存在路径，则输出提示信息
            QString pathString = QString("节点")+QString::number(source)+QString("和节点")+QString::number(target)+QString("之间不存在路径");
            ui->resultEdit->setText(pathString);
            view->hide();
        }else{//否则输出二者之间的路径
            QString pathString = QString("节点")+QString::number(source)+"和节点"+QString::number(target)+"之间的路径总长为"+QString::number(pathLength)+" 路径为：";
            for(int i = 0; i < path.size(); i++){
                pathString.append(QString::number(path[i]));
                if(i<path.size()-1) pathString.append("->");
            }
            ui->resultEdit->setText(pathString);

            //并进行可视化
            graph.writeShortestPath(ShortestPathFile, path);//并写入文件中
            view->resize(ui->graphicsView->size());
            QString shortestPathHtmlAbsolutePath = "file:///"+QFileInfo(ShortestPathHtml).absoluteFilePath();//最短路径的html的绝对路径
            view->show();
            view->load(shortestPathHtmlAbsolutePath);//加载最短路径
        }
    }else if(!graph.isValidVertex(source)){
        QMessageBox::information(this,tr("错误输入"),"错误的源点输入,源点在[0,"+QString::number(graph.n())+"范围内", QMessageBox::Ok);
    }else{
        QMessageBox::information(this,tr("错误输入"),"错误的终点输入,终点在[0,"+QString::number(graph.n())+")范围内", QMessageBox::Ok);
    }
}

void Dialog::on_connectedComponentButton_clicked()
{
    double thread = ui->threadEdit->text().toDouble();//得到总阈值
    double similarityThread = ui->similarityThreadEdit->text().toDouble();//和相似度阈值
    switch (dataSet) {
    case DOUBAN:
        if(thread > 2 && similarityThread <= 0.9 && similarityThread >= 0.1){
            Graph graph;
            graph.adjustThread(thread, NetworkFile, NetworkFile_AnyThread, similarityThread);
            graph.readFromFile(NetworkFile_AnyThread);
            int groupNumber = graph.getConnectedComponent();//求全图所有的联通分量
            QString connectedComponentString = "当前阈值下共有连通域"+QString::number(groupNumber)+"个，图中只显示非孤立节点";
            ui->resultEdit->setText(connectedComponentString);

            //并进行可视化
            graph.writeConnectedComponent(ConnectedComponentFile, true);//并写入文件(删除孤立点）
            view->resize(ui->graphicsView->size());
            QString connectedComponentHtmlAbsolutePath = "file:///"+QFileInfo(ConnectedComponentHtml).absoluteFilePath();//最短路径的html的绝对路径
            view->show();
            view->load(connectedComponentHtmlAbsolutePath);//加载最短路径
        }else if(thread <= 2){
            QMessageBox::information(this,tr("错误输入"),"错误的总阈值输入，总阈值需要大于2", QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("错误输入"),"错误的相似度阈值输入，相似度阈值在[0.1,0.9]范围内", QMessageBox::Ok);
        }
        break;
    case ZHIHU:
        if(thread > 2){
            Graph graph;
            graph.adjustThread(thread, ZhihuNetworkFile, NetworkFile_AnyThread);
            graph.readFromFile(NetworkFile_AnyThread);
            int groupNumber = graph.getConnectedComponent();//求全图所有的联通分量
            QString connectedComponentString = "当前阈值下共有连通域"+QString::number(groupNumber)+"个，图中只显示非孤立节点";
            ui->resultEdit->setText(connectedComponentString);

            //并进行可视化
            graph.writeConnectedComponent(ConnectedComponentFile, true);//并写入文件(删除孤立点）
            view->resize(ui->graphicsView->size());
            QString connectedComponentHtmlAbsolutePath = "file:///"+QFileInfo(ConnectedComponentHtml).absoluteFilePath();//最短路径的html的绝对路径
            view->show();
            view->load(connectedComponentHtmlAbsolutePath);//加载最短路径
        }else if(thread <= 2){
            QMessageBox::information(this,tr("错误输入"),"错误的总阈值输入，总阈值需要大于2", QMessageBox::Ok);
        }
        break;
    }
}

void Dialog::getGroupNumberWithDifferentThread(QString filename){
    double thread[7] = {3, 8, 15, 25, 35, 55, 75};
    double similarityThread[6] = {0.2, 0.5, 0.7, 0.85, 0.89, 0.899};
    QJsonArray datas;//多次测试的结果
    int count1 = 0;
    for(int i=0; i<7; i++){
        QJsonArray sameThread;// 一组测试的结果
        int count2 = 0;
        for(int j=0; j<6; j++){
            Graph graph;
            graph.adjustThread(thread[i], NetworkFile, NetworkFile_AnyThread, similarityThread[j]);
            graph.readFromFile(NetworkFile_AnyThread);
            int groupNumber = graph.getConnectedComponent();
            QJsonObject data;//一次测试的结果
            data.insert("thread", thread[i]);
            data.insert("similarityThread",similarityThread[j]);
            data.insert("groupNumber", groupNumber);//记录连通域个数
            sameThread.insert(count2++, data);
        }
        datas.insert(count1++, sameThread);
    }
    //将Json对象转换为字符串
    QJsonDocument document;
    document.setArray(datas);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString jsonString(byteArray);

    //打开文件
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"Failed to open txt";
        return;
    }
    QTextStream out(&file);
    out<<jsonString;//将字符串写入文件
    file.close();
}

void Dialog::on_ConnectedComponentQuickButton_clicked()
{
    //getGroupNumberWithDifferentThread(GroupNumberInDifferentThreadsFile);
    ui->resultEdit->setText("请在下方选择总阈值");
    view->resize(ui->graphicsView->size());
    QString getGroupNumberWithDifferentThreadHtmlAbsolutePath = "file:///"+QFileInfo(GroupNumberInDifferentThreadsHtml).absoluteFilePath();//最短路径的html的绝对路径
    view->show();
    view->load(getGroupNumberWithDifferentThreadHtmlAbsolutePath);//可视化不同阈值下的联通分量
}

void Dialog::on_betweennessCentralityButton_clicked()
{
    Graph graph;
    switch (dataSet) {
    case DOUBAN:
        graph.readFromFile(NetworkFile_20_088);
        break;
    case ZHIHU:
        graph.readFromFile(ZhihuNetworkFile_3);
        break;
    }
    graph.getBetweennessCentrality();//在发布版本中为提高速度，可以将该句注释掉
    QString string = QString("图中只显示非孤立点");
    ui->resultEdit->setText(string);

    //并进行可视化
    graph.writeBetweennessCentrality(BetweenessCentralityFile,true);//并写入文件,选择删除孤立点
    view->resize(ui->graphicsView->size());
    QString betweenessCentralityHtmlAbsolutePath = "file:///"+QFileInfo(BetweenessCentralityHtml).absoluteFilePath();//介数中心度html的绝对路径
    view->show();
    view->load(betweenessCentralityHtmlAbsolutePath);
}

void Dialog::on_closenessCentralityButton_clicked()
{
    Graph graph;
    switch (dataSet) {
    case DOUBAN:
        graph.readFromFile(NetworkFile_20_088);
        break;
    case ZHIHU:
        graph.readFromFile(ZhihuNetworkFile_3);
        break;
    }
    graph.getClosenessCentrality();//在发布版本中为提高速度，可以将该句注释掉
    QString string = QString("图中只显示非孤立点");
    ui->resultEdit->setText(string);

    //并进行可视化
    graph.writeClosenessCentrality(ClosenessCentralityFile,true);//并写入文件,选择删除孤立点
    view->resize(ui->graphicsView->size());
    QString closenessCentralityHtmlAbsolutePath = "file:///"+QFileInfo(ClosenessCentralityHtml).absoluteFilePath();//介数中心度html的绝对路径
    view->show();
    view->load(closenessCentralityHtmlAbsolutePath);
}

void Dialog::on_dataSelect_currentTextChanged(const QString &arg1)
{
    if(arg1 == "豆瓣影评数据集"){
        dataSet = DOUBAN;
        ui->similarityThreadEdit->setHidden(false);
        ui->ConnectedComponentQuickButton->setHidden(false);
    }else{
        dataSet = ZHIHU;
        ui->similarityThreadEdit->setHidden(true);
        ui->ConnectedComponentQuickButton->setHidden(true);
    }
}
