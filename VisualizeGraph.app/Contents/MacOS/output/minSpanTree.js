function visualizeMinSpanTree(graph, d3) {//将图graph上的最短路径可视化
    var nodes = graph.nodes;
    var edges = graph.edges;

    var width = 900;
    var height = 550;


    var svg = d3.select("body")
        .append("svg")
        .attr("width", width)
        .attr("height", height);

    var force = d3.layout.force()
        .nodes(nodes)		//指定节点数组
        .links(edges)		//指定连线数组
        .size([width, height])	//指定范围
        .linkDistance(1)	//指定连线长度
        .charge([-3]);	//相互之间的作用力

    force.start();	//开始作用

    //添加连线
    var svg_edges = svg.selectAll("line")
        .data(edges)
        .enter()
        .append("line")
        .style("stroke", gray)
        .style("stroke-width", 1);

    var color = d3.scale.category20();

    //添加节点
    var svg_nodes = svg.selectAll("circle")
        .data(nodes)
        .enter()
        .append("circle")
        .attr("r", function (d) {
            if (d.degree > 100) return 5;
            else if (d.degree > 10) return 4;
            else return 2;
        })
        .style("fill", function (d) {
            if (d.degree > 100) return orange;
            else if (d.degree > 10) return skyBlue;
            else return lightGreen;
        })
        .sort(function (a, b) {
            return a.degree - b.degree;
        })
        .call(force.drag);	//使得节点能够拖动


    const magin = 10;
    force.on("tick", function () {	//对于每一个时间间隔
        //更新连线坐标
        svg_edges.attr("x1", function (d) {
            if (d.source.x < magin)
                d.source.x = magin;
            else if (d.source.x > width - magin)
                d.source.x = width - magin;
            return d.source.x;
        })
            .attr("y1", function (d) {
                if (d.source.y < magin)
                    d.source.y = magin;
                else if (d.source.y > height - magin)
                    d.source.y = height - magin;
                return d.source.y;
            })
            .attr("x2", function (d) {
                if (d.target.x < magin)
                    d.target.x = magin;
                else if (d.target.x > width - magin)
                    d.target.x = width - magin;
                return d.target.x;
            })
            .attr("y2", function (d) {
                if (d.target.y < magin)
                    d.target.y = magin;
                else if (d.target.y > height - magin)
                    d.target.y = height - magin;
                return d.target.y;
            })

        //更新节点坐标
        svg_nodes.attr("cx", function (d) {
            if (d.x < magin)
                d.x = magin;
            else if (d.x > width - magin)
                d.x = width - magin;
            return d.x;
        })
            .attr("cy", function (d) {
                if (d.y < magin)
                    d.y = magin;
                else if (d.y > height - magin)
                    d.y = height - magin;
                return d.y;
            })
    });
}
