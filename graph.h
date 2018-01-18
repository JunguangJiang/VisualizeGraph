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
typedef enum {UNDETERMINED, SHORTEST_PATH, MIN_SPAN_TREE} EType;//边类型：尚未决定的边、最短路径的边、最小生成树的树边
typedef enum {SOURCE, TARGET, IN_PATH, OUT_PATH} VType;//节点类型:源点、终点、在最短路径时，不在最短路径上

struct Edge{//边对象，不封装
    double relation; EType type;//用户关系的大小、边类型
    int id;//边的编号
    int source, target;//源点和终点
    Edge(int source=-1, int target=-1, double relation=0):
        source(source), target(target), relation(relation), type(UNDETERMINED){}
};

struct Vertex {//顶点对象，不封装
    int id;//编号，用于图算法中
    int name;//名字，用于写入文件，和编号id不同
    QVector<std::shared_ptr<Edge>> NbrEdges;//邻边集合
    VStatus status;//状态
    int parent; double priority;//在遍历树中的父节点、优先级数
    int pEdge;//和遍历树中父节点先关联的边的编号
    VType vType;//节点类型
    int group;//所属的联通分量集合编号
    int minTreeDegree;//在最小生成树中的度数
    int betweennessCentrality;//介数中心度
    double closenessCentrality;//紧密中心度

    Vertex()://构造新顶点
        name(-1), id(-1), status(UNDISCOVERED),
        parent(-1), priority(PRIORITY_MAX),pEdge(-1), vType(OUT_PATH),group(-1),minTreeDegree(0),
    betweennessCentrality(0),closenessCentrality(0){}
    int getNbrVertex(int i)//求当前节点的第i个邻接节点的编号
        {return ( NbrEdges[i]->source==id ) ? NbrEdges[i]->target : NbrEdges[i]->source; }
    Vertex(const Vertex& rhs)://拷贝构造
        name(rhs.name), id(rhs.id), status(rhs.status), parent(rhs.parent),priority(rhs.priority),pEdge(rhs.pEdge),vType(rhs.vType), group(rhs.group),minTreeDegree(rhs.minTreeDegree){}
    friend bool operator<(const Vertex& a, const Vertex& b){return a.priority > b.priority;}//用于优先级队列中的比较
};


class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);
    bool readFromFile(QString filename);//从文件filename中读取边和点的数据
    bool adjustThread(double thread, QString inFile,  QString outFile, double similarityThread = 0.1);//改变阈值，从inFile读入，生成一张新的图存到outFile中 Note:thread是对总阈值的要求，similarityThread是对相似度的要求
    ~Graph();

//读取图中的节点属性
    int n(){return m_vertex.size();}//返回节点数目
    bool isValidVertex(int i){return (0 <= i) && (i < n());}//判断是否为合法的节点编号
    //QString name(int i){return m_vertex[i]->name;}//返回节点i的名字
    inline int& name(int i){return m_vertex[i]->name;}//返回节点i的名字
    inline int degree(int i){return m_vertex[i]->NbrEdges.size();}//返回节点i的度
    inline int NthNbr(int i, int j){return m_vertex[i]->getNbrVertex(j);}//返回节点i的第j个邻接节点
    inline VStatus& status(int i){return m_vertex[i]->status;}//节点状态
    inline int& parent(int i){return m_vertex[i]->parent;}//在遍历树中的父亲
    inline int& pEdge(int i){return m_vertex[i]->pEdge;}//和遍历树中的父节点所共同关联的边的编号
    inline double& priority(int i){return m_vertex[i]->priority;}//在遍历树中的优先级数
    inline VType& vType(int i){return m_vertex[i]->vType;}//返回节点i的节点类型
    inline int& group(int i){return m_vertex[i]->group;}//返回节点i的联通分量集合的编号
    inline int& minTreeDegree(int i){return m_vertex[i]->minTreeDegree;}//返回节点i在最小生成树中的度数
    inline int& betweennessCentrality(int i){return m_vertex[i]->betweennessCentrality;}//返回节点i的介数中心度
    inline double& closenessCentrality(int i){return m_vertex[i]->closenessCentrality;}//返回节点i的紧密中心度
//读取图中的边属性
    int e(){return m_edges.size();}//返回边的数目
    std::shared_ptr<Edge> NthEdge(int i, int j){return m_vertex[i]->NbrEdges[j];}//返回节点i的第j条边
    double weight(int i, int j){return m_vertex[i]->NbrEdges[j]->relation == 0 ? PRIORITY_MAX : (MaxRelation + 1.0 - m_vertex[i]->NbrEdges[j]->relation);}//节点i的第j条边的权重，效率较高

//对图的动态操作
    void reset();//所有顶点、边的辅助信息复位 Note:不会清空节点的连通域集合编号

//算法
    template<typename PU> int pfs(int s, PU prioUpdater);//优先级搜索（全图）
    template<typename PU> void PFS(int s, PU prioUpdater);//优先级搜索（单个连通域)
    int getMinSpanTree();//求最小生成树的prim算法
    //void getMinSpanTree(int s);//求以s为树根的最小生成树
    double getShortestPath(int source, int target, QVector<int>& path);//求从source到target的最短路径,返回最短路径的长度
    void getConnectedComponent(int root);//求解以root为根的一个联通分量
    int getConnectedComponent();//求解所有的联通分量,返回连通域的数量
    void getBetweennessCentrality();//求解节点的介数中心度
    void getClosenessCentrality();//求解节点的紧密中心度

//将算法处理后的图写入文件的函数
    int writeShortestPath(QString filename, const QVector<int>& path);//将最短路径所在的联通分量的信息写入文件，需要知道最短路径
    int writeMinSpanTree(QString filename, bool removeIsolatedPoint = true);//将最小生成树写入文件,如果removeIsolatedPoint为true，则删除孤立点
    int writeConnectedComponent(QString filename, bool removeIsolatedPoint=true);//将联通分量写入文件，如果removeIsolatedPoint为true，则删除孤立点
    int writeBetweennessCentrality(QString filename, bool removeIsolatedPoint = true);//将介数中心度写入文件，如果removeIsolatedPoint为true，则删除孤立点
    int writeClosenessCentrality(QString filename, bool removeIsolatedPoint = true);//将紧密中心度写入文件，如果removeIsolatedPoint为true，则删除孤立点

    //path存储从source到target路径上的所有节点
    void printPath(int source, int target, QVector<int>& path);//从源点到终点打印路径,打印到path中

//和调试相关的函数
    void debug();
    void printPath(int source, int target);//从源点到终点打印路径,打印到qDebug()中

private:
    QVector< std::shared_ptr<Edge> > m_edges;//所有的边集合
    QVector< std::shared_ptr<Vertex> > m_vertex;//所有的顶点集合
    int MaxRelation;//读入的所有边（用户之间的关系）中最大值
    void getMinSpanTreeDegrees();//得到所有节点在最小生成树中的度数
};



#endif // GRAPH_H
