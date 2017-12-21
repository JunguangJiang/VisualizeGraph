#ifndef PARAMETER_H
#define PARAMETER_H
#include <limits>
#define MovieFile "input/movie.csv"//读取电影的文件
#define UserFile "input/user.csv"//读取用户的文件
#define NetworkFile "output/network.csv"//网络构建后得到的文件
#define NetworkFile_50 "output/network_50.csv"//阈值为50的文件
#define NetworkFile_20 "output/network_20.csv"//阈值为20的文件
#define NetworkFile_5 "output/network_5.csv"//阈值为5的文件
#define ShortestPathFile "output/shortestPath.json"//生成的最短路径图信息
#define MinSpanTreeFile "output/minSpanTree.json"//生成的最小生成树图信息
#define ConnetedComponent "output/connectedComponent.json"//生成的连通图信息

#define NetworkBuildingThread 2 //网络构建时的阈值
#define PRIORITY_MAX std::numeric_limits<double>::max()//节点权重的最大可能取值
#endif // PARAMETER_H
