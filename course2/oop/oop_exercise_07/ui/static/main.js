let field = document.getElementById('field');
let ctx = field.getContext('2d');

ctx.beginPath();
ctx.strokeStyle = "rgb(255,165,0)";
let createLine = (x1, y1, x2, y2) => {
	ctx.moveTo(x1, y1);
	ctx.lineTo(x2, y2);
	ctx.stroke();
}

let draw = (coors) => {
	const n = coors.length
	console.log(n);
	for (let i = 0; i < n; ++i)
		createLine(coors[i][0], coors[i][1], coors[(i + 1) % n][0], coors[(i + 1) % n][1]);
};

setInterval(() => {
	fetch('http://localhost:5000/test.json')
	.then((res) => {
		return res.json();
	}).then((shapes) => {
		shapes.forEach(shape => {
			draw(shape.coors);
		});
	});
}, 200)
