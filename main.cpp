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

    int source = 47478, target = 58597;
    if(graph->group(source) == graph->group(target)){//只有二者在同一联通分量时
        QVector<int> path;
        graph->getShortestPath(source, target, path);//才搜索一条最短路径
        graph->writeShortestPath(ShortestPathFile, path);//并写入文件中
    }
    return a.exec();
}
