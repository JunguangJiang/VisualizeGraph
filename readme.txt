1. d3.js学习网址
	if(英文好)
		直接看官方文档
	else
		http://d3.decembercafe.org/index.html
	
4. 第4-5周的任务	
	1）main.html中加入clearSVG();//清空画布svg，目前没有找到有效的解决方案
	2）main.html中加入showNodeName();//当鼠标移动到节点上时显示节点的名字
			showNodeId();//鼠标移动到节点上时显示节点的编号
			showEdgeWeight();//鼠标移动到边上时显示边的权重
	3）main.html中写示例函数visualize1(),visualize2()…//实现nodes，edges的多种形式的输出，可参考大作业说明PPT

5. 未完成的任务+待改进的方面
	1）graph.cpp, pfs优先级搜索算法O(n^2)部分可用优先级队列优化到O(nlogn)
	2)main.html, 改进isShortestPath函数，从而在可视化界面中显示带箭头的最短路径
	3)改善qt主界面的美观程度，比如多个切换的界面？
	4）改善可视化图形的美观程度，尤其是最短路径的美观程度
	5）假如有多条最短路径，此时的算法改进

6. 目前的进度：
	已经实现前两个算法及其可视化
	完成从数据集到网络的构建
	（可在main.cpp中调用networkbuilding模块生成处理后的图，由于生成文件过大，因此无法上传到Github）

8. 目前的一些bug：
	1） 不能连续生成最小生成树，但是按照先最小生成树，再最短路径，在最小生成树的次序就没有bug
	2） 生成新的图形，画布需要手动改变
	
	
	