#include "dialog.h"
#include <QApplication>
#include "graph.h"
#include <QVector>
#include <QDebug>
#include "parameter.h"

#include <QFile>
#include <QTextStream>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Dialog w;
    w.show();
    Graph * graph = new Graph();
    //graph->adjustThread(50, NetworkFile, NetworkFile_50);
    graph->readFromFile(NetworkFile_50);
    graph->getConnectedComponent();//先对全图求联通分量
    //QVector<int> path;
    //qDebug()<<graph->getShortestPath(47478, 58597, path);
    //graph->writeShortestPath(ShortestPathFile);

    int source = 47478, target = 58597;
    if(graph->group(source) == graph->group(target)){//只有二者在同一联通分量时
        QVector<int> path;
        graph->getShortestPath(source, target, path);//才搜索一条最短路径
        graph->writeShortestPath(ShortestPathFile);//并写入文件中
    }
    //qDebug() << path;
    //graph->prim(117069);
    //graph->reset();
    //graph->debug();

    /*
    Graph* graph = new Graph();//创建一个图
    //graph->readFromFile(":/data/data.txt");//从文件中读取数据到图
    if(graph->readFromFile(NetworkFile)){//读取构建图所需要的节点、边信息
        qDebug()<<"Finishing reading";
        QVector<int> path;
        qDebug()<<graph->getShortestPath(66775, 108976, path);
        qDebug() << path;
        //graph->prim(117069);
        graph->debug();
    }
*/
    return a.exec();
}
