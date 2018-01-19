function betweenessCentrality(graph,d3) {//将图graph上的最短路径可视化
  var nodes = graph.nodes;
  var edges = graph.edges;

  var width = 1000;
  var height = 550;


  var svg = d3.select("body")
      .append("svg")
      .attr("width",width)
      .attr("height",height);

  var force = d3.layout.force()
      .nodes(nodes)		//指定节点数组
      .links(edges)		//指定连线数组
      .size([width,height])	//指定范围
      .linkDistance(100)	//指定连线长度
      .charge([-80]);	//相互之间的作用力

  force.start();	//开始作用

  //添加连线
  var svg_edges = svg.selectAll("line")
      .data(edges)
      .enter()
      .append("line")
      .style("stroke", gray)
      .style("stroke-width", 1);

  var min = nodes[0].bCentrality;
  var max = nodes[0].bCentrality;
  nodes.forEach(element => {
      if (element.bCentrality > max) max = element.bCentrality;
      if (element.bCentrality < min) min = element.bCentrality;
  });
  var scale = d3.scale.linear()
  .domain([min, max])
  .range([0, 1]);
  var color = d3.scale.category10();

  //添加节点
  var svg_nodes = svg.selectAll("circle")
      .data(nodes)
      .enter()
      .append("circle")
      .attr("r", "2")
      .style("fill",function(d){
          return color(scale(d.bCentrality)*10);
      })
      .sort(function(a, b) {
          return b.bCentrality - a.bCentrality;
      })
      .call(force.drag);	//使得节点能够拖动


  force.on("tick", function(){	//对于每一个时间间隔
      //更新连线坐标
      svg_edges.attr("x1",function(d){ 
              if (d.source.x < 0)
                  d.source.x = 0;
              else if (d.source.x > width)
                  d.source.x = width;
              return d.source.x; 
          })
          .attr("y1",function(d){ 
              if (d.source.y < 0)
                  d.source.y = 0;
              else if (d.source.y > height)
                  d.source.y = height;
              return d.source.y; 
          })
          .attr("x2",function(d){ 
              if (d.target.x < 0)
                  d.target.x = 0;
              else if (d.target.x > width)
                  d.target.x = width;
              return d.target.x; 
          })
          .attr("y2",function(d){ 
              if (d.target.y < 0)
                  d.target.y = 0;
              else if (d.target.y > height)
                  d.target.y = height;
              return d.target.y; 
          })

      //更新节点坐标
      svg_nodes.attr("cx",function(d){ 
              if (d.x < 0)
                  d.x = 0;
              else if (d.x > width)
                  d.x = width;
              return d.x; 
          })
          .attr("cy",function(d){ 
              if (d.y < 0)
                  d.y = 0;
              else if (d.y > height)
                  d.y = height;
              return d.y; 
          })
  });
}
