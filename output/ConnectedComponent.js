function visualizeConnectedComponent(graph,d3) {//将图graph上的最短路径可视化
    var nodes = graph.nodes;
    var edges = graph.edges;

    var width = 1000;
    var height = 1000;


    var svg = d3.select("body")
        .append("svg")
        .attr("width",width)
        .attr("height",height);

    var force = d3.layout.force()
        .nodes(nodes)		//指定节点数组
        .links(edges)		//指定连线数组
        .size([width,height])	//指定范围
        .linkDistance(100)	//指定连线长度
        .charge([-60]);	//相互之间的作用力

    force.start();	//开始作用

    //添加连线
    var svg_edges = svg.selectAll("line")
        .data(edges)
        .enter()
        .append("line")
        .style("stroke", gray)
        .style("stroke-width", 2);

    var color = d3.scale.category20();

    //添加节点
    var svg_nodes = svg.selectAll("circle")
        .data(nodes)
        .enter()
        .append("circle")
        .attr("r", 3)
        .style("fill",function(d){
            return color((d.group%20));
        })
        .call(force.drag);	//使得节点能够拖动


    force.on("tick", function(){	//对于每一个时间间隔
        //更新连线坐标
        svg_edges.attr("x1",function(d){ return d.source.x; })
            .attr("y1",function(d){ return d.source.y; })
            .attr("x2",function(d){ return d.target.x; })
            .attr("y2",function(d){ return d.target.y; });

        //更新节点坐标
        svg_nodes.attr("cx",function(d){ return d.x; })
            .attr("cy",function(d){ return d.y; });
    });
}