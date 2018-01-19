#ifndef PARAMETER_H
#define PARAMETER_H
#include <limits>

#define NetworkFile "output/network.csv"//网络构建后得到的文件
#define NetworkBuildingThread 2 //网络构建时的阈值

#define NetworkFile_5_08  "output/network_5_08.csv"//阈值为5和0.8的文件
#define NetworkFile_20_088 "output/network_20_088.csv"//阈值为20和0.88的文件
#define NetworkFile_AnyThread "output/network_anythread.csv"//求任意阈值的联通分量文件

#define ShortestPathFile "output/shortestPath.json"//生成的最短路径图信息
#define MinSpanTreeFile "output/minSpanTree.json"//生成的最小生成树图信息
#define ConnectedComponentFile "output/connectedComponent.json"//生成的连通图信息
#define GroupNumberInDifferentThreadsFile "output/groupNumberInDifferentThreads.json"//在不同阈值下的连通图个数
#define BetweenessCentralityFile "output/betweenessCentrality.json"//介数中心度
#define ClosenessCentralityFile "output/closenessCentrality.json"//紧密中心度

#define ShortestPathHtml "output/shortestPath.html"//生成的最短路径html
#define MinSpanTreeHtml "output/minSpanTree.html"//生成的最小生成树html
#define ConnectedComponentHtml "output/ConnectedComponent.html"//生成的连通图html
#define GroupNumberInDifferentThreadsHtml "output/GroupNumberInDifferentThreads.html"//生成的连通图html
#define BetweenessCentralityHtml "output/betweenessCentrality.html"//介数中心度
#define ClosenessCentralityHtml "output/closenessCentrality.html"

#define PRIORITY_MAX std::numeric_limits<double>::max()/2.0//节点权重的最大可能取值

#define ZhihuNetworkFile "output/zhihuNetwork.csv"//知乎网络构建文件
#define ZhihuNetworkFile_3 "output/zhihuNetwork_3.csv"//阈值为3的文件

#endif // PARAMETER_H
