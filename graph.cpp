#include "graph.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <queue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextCodec>
using namespace std;
Graph::Graph(QObject *parent) : QObject(parent)
{
}

Graph::~Graph()
{
}

bool Graph::readFromFile(QString filename)//从txt文件filename中读取边和点的数据
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can not open the graph file";
        return false;
    }
    QTextStream textInput(&file);

    int n;//节点数
    textInput >> n;

    for(int i=0; i<n; i++)
    {
        //QString name = textInput.readLine();
        QString name = " ";
        shared_ptr<Vertex> node = make_shared<Vertex>(name);
        node->id = i;
        m_vertex.push_back(node);
    }
    int i=0;
    while(!textInput.atEnd()){
        int source, target; double weight;
        textInput >> source >> target >> weight;
        if(textInput.atEnd())break;
        qDebug()<<source<<target<<weight;
        shared_ptr<Edge> edge=make_shared<Edge>(source,target,weight);
        edge->id = i++;
        m_edges.push_back(edge);
        m_vertex[source]->NbrEdges.push_back(edge);
        m_vertex[target]->NbrEdges.push_back(edge);
    }
    return true;
}

bool Graph::adjustThread(double thread, QString inFile, QString outFile){
    QFile inputFile(inFile);
    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can not open the input file";
        return false;
    }
    QTextStream in(&inputFile);

    QFile outputFile(outFile);
    if(!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Can not open the output file";
        return false;
    }
    QTextStream out(&outputFile);

    int n;//节点数和边数
    in >> n; out << n <<endl;

    while(!in.atEnd()){
        int source, target; double weight;
        in >> source >> target >> weight;
        if(weight >= thread){//只有不小于阈值的边才保留
            out << source << " " << target << " " << weight << endl;
        }
    }
    return true;
}

void Graph::reset()//所有顶点、边的信息复位
{
    foreach (auto node, m_vertex) {//所有顶点的
        node->status = UNDISCOVERED;//状态
        node->parent = -1;//（在遍历树中的）父节点
        node->priority = PRIORITY_MAX;//优先级数
        node->vType = OUT_PATH;//节点类型
        node->pEdge = -1;//和父节点共同关联边的编号
        node->group = -1;//节点所属的联通分量
    }
    foreach (auto edge, m_edges) {//所有边的
        edge->type = UNDETERMINED;//类型
    }
}

template<typename PU> void Graph::pfs(int s, PU prioUpdater)//优先级搜索（全图）
{
    reset(); int v = s;//初始化
    do//逐一检查所有顶点
        if(UNDISCOVERED == status(v))//一旦遇到尚未发现的顶点
            PFS(v,prioUpdater);//即从该顶点出发启动一次PFS
    while( s != (v = (++v % m_vertex.size())));//按序号检查
}

template<typename PU> void Graph::PFS(int s, PU prioUpdater)//优先级搜索（单个连通域）
{
    priority_queue<Vertex> PQ;//优先级队列
    priority(s) = 0.0; PQ.push(*m_vertex[s]);//将起点加入优先级队列
    while(!PQ.empty()){
        Vertex v = PQ.top(); s = v.id; PQ.pop();//从优先级队列中弹出队首元素s
        if(m_vertex[s]->status == VISITED) continue;//如果发现此前已经访问过该元素，则直接跳过此次循环
        for(int i=0; i<degree(s); i++){//枚举当前节点的所有邻居
            if(prioUpdater(this, s, i)){//如果发现该邻居更新了优先级数
                int v = NthNbr(s, i);//则取得该邻居的编号
                PQ.push(*m_vertex[v]);//然后将该邻居的一个拷贝加入优先级队列
            }
        }
        status(s) = VISITED;//至此节点s访问完毕
    }
}

struct PrimPU{//针对Prim算法的顶点优先级更新器
    bool operator()(Graph* g, const int uk, const int i){
        int v = g->NthNbr(uk, i);//v是uk的第i个邻接节点
        if(UNDISCOVERED == g->status(v))//对于uk的每一个尚未被发现的邻接节点v
        {
            if(g->priority(v) > g->weight(uk,i)){//如果v的优先级低于边(uk,v)的权重，Note：此处weight2(uk,i)指的是uk的第i条邻边，即边(uk,v)
                g->priority(v) = g->weight(uk, i);//则更新优先级数
                g->parent(v) = uk;//更新父节点
                return true;
            }
        }
        return false;
    }
};

void Graph::getMinSpanTree(int s)//求最小生成树的prim算法
{
    PrimPU primPU;
    pfs(s, primPU);//调用优先级搜索(全图）
}

struct DijkstraPU{//针对Dijkstra算法的节点优先级更新器
    bool operator()(Graph* g, int uk, int i){
        int v = g->NthNbr(uk, i);//v是uk的第i个邻接节点
        if(UNDISCOVERED == g->status(v))//对于uk每一个尚未被发现的邻接节点v，按Dijkstra策略
        {
            if( g->priority(v) > g->priority(uk) + g->weight(uk, i) ){//做松弛
                g->priority(v) = g->priority(uk) + g->weight(uk, i);//更新优先级数
                g->parent(v) = uk;//并同时更新父节点
                g->pEdge(v) = g->NthEdge(uk, i)->id;//更新节点v和父节点所共同关联的边
                return true;
            }
        }
        return false;
    }
};

double Graph::getShortestPath(int source, int target, QVector<int>& path){//求从source到target的最短路径,将路径打印到path中，并返回路径总长度
    reset(); DijkstraPU dijkstraPU;
    PFS(source, dijkstraPU);//调用优先级搜索,只需要在source所在连通域寻找

    if(priority(target) < PRIORITY_MAX)//如果从source到target直接有路，则
    {
        printPath(source, target, path);//将路径打印到S中
    }
    for(int i=1; i<path.size(); i++){//路径上的点和边
        vType(path[i]) = IN_PATH;//标记为在路径上
        m_edges[pEdge(path[i])]->type = SHORTEST_PATH;
    }
    vType(source) = SOURCE; vType(target) = TARGET;
    return priority(target);//返回路径的总长度
}

void Graph::getConnectedComponent(int root){
    for(int j=0; j<degree(root); j++){//遍历root的每个
        int u = NthNbr(root, j);//邻点
        if(VStatus(u) == UNDISCOVERED){//如果当前节点u尚未被发现
            VStatus(u) = DISCOVERED;//则标记u已经被发现了
            parent(u) = root;//u是被root发现的
            group(u) = group(root);//u的集合编号保持和root的集合编号相同
        }
    }
}

void Graph::getConnectedComponent(){
    reset();
    for(int v = 0; v < n(); v++){//遍历所有节点
        if(VStatus(v) == UNDISCOVERED){//如果发现尚未访问当前节点
            group(v) = v;//那么当前节点所在集合的编号不妨和v相同
            getConnectedComponent(v);//求v所在连通域的所有节点
        }
    }
}

int Graph::writeShortestPath(QString filename){
    //-----将生成的最短路径写入本地文件------
    QJsonArray nodes;
    for(int i=0; i<n(); i++){
        QJsonObject node;
        node.insert("name", name(i));
        switch (vType(i)) {
        case SOURCE:
            node.insert("type", "source");
            break;
        case TARGET:
            node.insert("type", "target");
            break;
        case IN_PATH:
            node.insert("type", "inPath");
            break;
        default:
            node.insert("type","outPath");
            break;
        }
        nodes.insert(i, node);
    }
    QJsonArray edges;
    for(int i=0; i<e(); i++){
        QJsonObject edge;
        edge.insert("source", m_edges[i]->source);
        edge.insert("target", m_edges[i]->target);
        switch (m_edges[i]->type) {
        case SHORTEST_PATH:
            edge.insert("type", "shortestPath");
            break;
        default:
            edge.insert("type", "outsidePath");
            break;
        }
        edges.insert(i, edge);
    }
    QJsonObject graph;
    graph.insert("nodes",nodes);
    graph.insert("edges", edges);
    //将Json对象转换为字符串
    QJsonDocument document;
    document.setObject(graph);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString jsonString(byteArray);

    //打开文件
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<<"Failed to open txt";
        return -1;
    }
    QTextStream out(&file);
    out<<jsonString;//将字符串写入文件
    file.close();

    //-----将生成的最短路径写入本地文件------
    return 0;
}

int Graph::writeMinSpanTree(QString filename){

    return 0;
}

int Graph::writeConnectedComponent(QString filename){

    return 0;
}

void Graph::printParent()//打印所有的父节点，以调试
{
    qDebug()<<"Print parent of nodes:";
    qDebug()<<"node parent";
    foreach (auto node, m_vertex) {
        qDebug() << node->name << " "<< (node->parent==-1?"-1":m_vertex[node->parent]->name);
    }
}
void Graph::debug(){
    qDebug()<<"Debugging";
    qDebug()<<"node parent priority";
    foreach (auto node, m_vertex) {
        if(node->parent == -1) continue;
        qDebug() <<"<"<< node->name << ", " << node->id << "> <"<< (node->parent==-1?" ":m_vertex[node->parent]->name) <<","
                <<(node->parent==-1?-1:m_vertex[node->parent]->id)<<"> " <<  node->priority;
    }
}

void Graph::printPath(int source, int target){//打印路径到qDebug(),Note：前提是source和target之间有路径
    QVector<int> path;
    printPath(source, target, path);//先将路径打印到栈S中
    qDebug()<<"The path from "<<source << " to "<<target;
    for(int i=0; i<path.size(); i++){
        qDebug()<<path[i];
    }
}

void Graph::printPath(int source, int target, QVector<int>& path){//打印路径到path中，Note：前提是source和target之间有路径
    QStack<int> S;//辅助栈
    int p = target;
    S.push(p);
    while( p != source){//S自顶向下存储source到target的路径
        p = parent(p);
        S.push(p);
    }
    //将栈中元素转移到path中
    while(!S.empty()){
        path.push_back(S.top());
        S.pop();
    }
}
