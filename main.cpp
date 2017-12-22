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
    //graph->getMinSpanTree(114303);//找到以s为树根的最小生成树,Note：这种方式只能找到以s为树根的最小生成树，但是在d3处理中速度和效果较好
    graph->getMinSpanTree();//找最小生成树，Note：这种方式找到全图所有的最小生成树，但是在d3处理中速度和效果较差
    graph->writeMinSpanTree(MinSpanTreeFile, true);//并写入文件,选择删除孤立点
*/

    //以下用于求任意阈值的联通分量
/*
    int thread = 30; double similarityThread = 0.88;//看过电影数的阈值和相似度的阈值
    Graph * graph = new Graph();
    graph->adjustThread(thread, NetworkFile, NetworkFile_AnyThread, similarityThread);
    graph->readFromFile(NetworkFile_AnyThread);
    graph->getConnectedComponent();//求全图所有的联通分量
    graph->writeConnectedComponent(ConnetedComponentFile, true);//并写入文件(删除孤立点）
*/
    return a.exec();
}
