var globalinfo;

function groupNumberInDifferentThreads(info, d3) {
    globalinfo = info;

}

function draw(info) {
    var width = 900;
    var height = 550;

    var svg = d3.select("#graph")
        .append("svg")
        .attr("width", width)
        .attr("height", height);
    //.style("background", skyBlue);

    var padding = { left: 100, right: 50, top: 20, bottom: 40 };// 留白

    //定义一个数组
    var dataset = [0.2, 0.5, 0.7, 0.85, 0.89, 0.899];

    //x轴的比例尺
    var xScale = d3.scale.ordinal()
        .domain(dataset)
        .rangeRoundBands([0, width - padding.left - padding.right]);

    //找合适的y轴
    const minheight = 1000;
    var min = info[0].groupNumber;
    var max = info[0].groupNumber;
    info.forEach(element => {
        if (element.groupNumber > max) max = element.groupNumber;
        if (element.groupNumber < min) min = element.groupNumber;
    });

    //y轴的比例尺
    var yScale = d3.scale.linear()
        .domain([min - minheight, max + minheight])
        .range([height - padding.top - padding.bottom, 0]);

    //定义x轴
    var xAxis = d3.svg.axis()
        .scale(xScale)
        .orient("bottom");

    //定义y轴
    var yAxis = d3.svg.axis()
        .scale(yScale)
        .orient("left");

    //矩形之间的空白
    var rectPadding = 20;

    //添加矩形元素
    var rects = svg.selectAll(".MyRect")
        .data(info)
        .enter()
        .append("rect")
        .attr("class", "MyRect")
        .attr("transform", "translate(" + padding.left + "," + padding.top + ")")
        .attr("x", function (d, i) {
            return xScale(d.similarityThread) + rectPadding / 2;
        })
        .attr("y", function (d) {
            return yScale(d.groupNumber);
        })
        .attr("width", xScale.rangeBand() - rectPadding)
        .attr("height", function (d) {
            return height - padding.top - padding.bottom - yScale(d.groupNumber);
        })
        .attr("fill", "skyblue");

    //添加文字元素
    var texts = svg.selectAll(".MyText")
        .data(info)
        .enter()
        .append("text")
        .attr("class", "MyText")
        .attr("transform", "translate(" + padding.left + "," + padding.top + ")")
        .attr("x", function (d, i) {
            return xScale(d.similarityThread) + rectPadding / 2;
        })
        .attr("y", function (d) {
            return yScale(d.groupNumber);
        })
        .attr("dx", function () {
            return (xScale.rangeBand() - rectPadding) / 2;
        })
        .attr("dy", function (d) {
            return -10;
        })
        .text(function (d) {
            return d.groupNumber;
        })
        .attr("fill", "steelblue");

    //添加x轴
    svg.append("g")
        .attr("class", "axis")
        .attr("transform", "translate(" + padding.left + "," + (height - padding.bottom) + ")")
        .call(xAxis)
        .attr("fill", "steelblue");

    //添加y轴
    svg.append("g")
        .attr("class", "axis")
        .attr("transform", "translate(" + padding.left + "," + padding.top + ")")
        .call(yAxis)
        .attr("fill", "steelblue");

    //坐标轴名称
    svg.append("g")
        .append("text")
        .text("连通分量个数")
        .attr("transform", "rotate(-90)")//text旋转-90°
        .attr("text-anchor", "end")//字体尾部对齐
        .attr("dy", "20")
        .attr("dx", "-20")
        .attr("fill", "steelblue");
    svg.append("g")
        .append("text")
        .text("相似度阈值")
        .attr("text-anchor", "end")//字体尾部对齐
        .attr("y", height - 10)
        .attr("x", width)
        .attr("fill", "steelblue");
}

function addBtn() {
    const threads = [3, 8, 15, 25, 35, 55, 75];
    var MyDiv = document.getElementById("buttons");
    threads.forEach(function (element, i) {
        var bt = document.createElement("button");           //createElement生成button对象
        bt.innerHTML = element;
        bt.onclick = function () {                          //绑定点击事件
            document.getElementById("graph").innerHTML = "";
            draw(globalinfo[i]);
        };
        MyDiv.appendChild(bt);
    }, this);
}
