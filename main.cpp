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

    //以下用于求最短路径
    /*
    Graph * graph = new Graph();
    graph->readFromFile(NetworkFile_5);
    int source = 114303, target = 112355;
    QVector<int> path;
    graph->getShortestPath(source, target, path);//才搜索一条最短路径
    graph->writeShortestPath(ShortestPathFile, path);//并写入文件中
    */

    //以下用于求最小生成树
    /*
    Graph * graph = new Graph();
    graph->readFromFile(NetworkFile_5);
    int s = 1;
    graph->getMinSpanTree(114303);//找到以s为树根的最小生成树,Note：这种方式只能找到以s为树根的最小生成树，但是在d3处理中速度和效果较好
    //graph->getMinSpanTree();//找最小生成树，Note：这种方式找到全图所有的最小生成树，但是在d3处理中速度和效果较差
    graph->writeMinSpanTree(MinSpanTreeFile);//并写入文件
    */

    //以下用于求任意阈值的联通分量
    int thread = 35;
    Graph * graph = new Graph();
    graph->adjustThread(thread, NetworkFile, NetworkFile_AnyThread);
    graph->readFromFile(NetworkFile_AnyThread);
    graph->getConnectedComponent();//求全图所有的联通分量
    graph->writeConnectedComponent(ConnetedComponent);//并写入文件
    return a.exec();
}
