//图形Graph类，采用十字链表实现
//用于数据的输入、核心算法的实现
#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QVector>
#include <QString>
#include <memory>
#include <QStack>
#include "parameter.h"
typedef enum {UNDISCOVERED, DISCOVERED, VISITED} VStatus;//顶点状态,未发现，已发现，已访问完毕
//typedef enum {UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD, PATH} EType;//边在遍历树中所属的类型
typedef enum {UNDETERMINED, SHORTEST_PATH, MIN_SPAN_TREE} EType;//边类型：尚未决定的边、最短路径的边、最小生成树的树边
typedef enum {SOURCE, TARGET, IN_PATH, OUT_PATH }VType;//节点类型:源点、终点、在最短路径时，不在最短路径上
struct Edge{//边对象，不封装
    double weight; EType type;//权重、边类型
    int id;//边的编号
    int source, target;//源点和终点
    Edge(int source=-1, int target=-1, double weight=-1):
        source(source), target(target), weight(weight), type(UNDETERMINED){}
};

struct Vertex {//顶点对象，不封装
    QString name; //名字
    int id;//编号
    QVector<std::shared_ptr<Edge>> NbrEdges;//邻边集合
    VStatus status;//状态
    int parent; double priority;//在遍历树中的父节点、优先级数
    int pEdge;//和遍历树中父节点先关联的边的编号
    VType vType;//节点类型
    int group;//所属的联通分量集合编号

    Vertex(QString name)://构造新顶点
        name(name), id(-1), status(UNDISCOVERED),
        parent(NULL), priority(PRIORITY_MAX),pEdge(-1), vType(OUT_PATH){}
    int getNbrVertex(int i)//求当前节点的第i个邻接节点的编号
        {return ( NbrEdges[i]->source==id ) ? NbrEdges[i]->target : NbrEdges[i]->source; }
    Vertex(const Vertex& rhs)://拷贝构造
        name(rhs.name), id(rhs.id), status(rhs.status), parent(rhs.parent),priority(rhs.priority),pEdge(rhs.pEdge),vType(rhs.vType){}
    friend bool operator<(const Vertex& a, const Vertex& b){return a.priority > b.priority;}//用于优先级队列中的比较
};


class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);
    bool readFromFile(QString filename);//从文件filename中读取边和点的数据
    bool adjustThread(double thread, QString inFile,  QString outFile);//改变阈值，从inFile读入，生成一张新的图存到outFile中
    ~Graph();

//读取图中的节点属性
    int n(){return m_vertex.size();}//返回节点数目
    bool isValidVertex(int i){return (0 <= i) && (i < n());}//判断是否为合法的节点编号
    QString name(int i){return m_vertex[i]->name;}//返回节点i的名字
    int degree(int i){return m_vertex[i]->NbrEdges.size();}//返回节点i的度
    int NthNbr(int i, int j){return m_vertex[i]->getNbrVertex(j);}//返回节点i的第j个邻接节点
    VStatus& status(int i){return m_vertex[i]->status;}//节点状态
    int& parent(int i){return m_vertex[i]->parent;}//在遍历树中的父亲
    int& pEdge(int i){return m_vertex[i]->pEdge;}//和遍历树中的父节点所共同关联的边的编号
    double& priority(int i){return m_vertex[i]->priority;}//在遍历树中的优先级数
    VType& vType(int i){return m_vertex[i]->vType;}//返回节点i的节点类型
    int& group(int i){return m_vertex[i]->group;}//返回节点i的联通分量集合的编号

//读取图中的边属性
    int e(){return m_edges.size();}//返回边的数目
    std::shared_ptr<Edge> NthEdge(int i, int j){return m_vertex[i]->NbrEdges[j];}//返回节点i的第j条边
    double& weight(int i, int j){return m_vertex[i]->NbrEdges[j]->weight;}//节点i的第j条边的权重，效率较高

//对图的动态操作
    void reset();//所有顶点、边的辅助信息复位

//算法
    template<typename PU> void pfs(int s, PU prioUpdater);//优先级搜索（全图）
    template<typename PU> void PFS(int s, PU prioUpdater);//优先级搜索（单个连通域）
    void getMinSpanTree(int s);//求最小生成树的prim算法
    double getShortestPath(int source, int target, QVector<int>& path);//求从source到target的最短路径,返回最短路径的长度
    void getConnectedComponent(int root);//求解以root为根的一个联通分量
    void getConnectedComponent();//求解所有的联通分量

//将算法处理后的图写入文件的函数
    int writeShortestPath(QString filename);
    int writeMinSpanTree(QString filename);
    int writeConnectedComponent(QString filename);


    //path存储从source到target路径上的所有节点
    void printPath(int source, int target, QVector<int>& path);//从源点到终点打印路径,打印到path中

//和数据的可视化（与js的交互）相关的函数
    void conveyGraphData();//传送图的节点和边信息

//和调试相关的函数
    void debug();
    void printParent();//打印所有节点的父节点
    void printPath(int source, int target);//从源点到终点打印路径,打印到qDebug()中

signals:
//和数据的可视化（与js的交互）相关的信号
    void visualizeMinSpanTree();//可视化最小生成树
    void visualizeShortestPath();//可视化最短路径

public slots:

private:
    QVector< std::shared_ptr<Edge> > m_edges;//所有的边集合
    QVector< std::shared_ptr<Vertex> > m_vertex;//所有的顶点集合
};



#endif // GRAPH_H