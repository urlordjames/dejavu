let canvas = document.getElementById('pengis');
let ctx = canvas.getContext('2d');
canvas.style.background = "#222";

class Player {
    constructor(position, radius, color) {
        this.position = position;
        this.radius = radius;
        this.color = color;
    }
    draw() {
        ctx.fillStyle = this.color;
        ctx.beginPath();
        ctx.arc(this.position.x, this.position.y, this.radius, 0, 2*Math.PI);
        ctx.fill();
    }
}

class Projectile {
    constructor(position, radius, color, velocity) {
        this.position = position;
        this.radius = radius;
        this.color = color;
        this.velocity = velocity;
    }
    draw() {
        ctx.fillStyle = this.color;
        ctx.beginPath();
        ctx.arc(this.position.x, this.position.y, this.radius, 0, 2*Math.PI);
        ctx.fill();
    }
    update() {
        this.position.x += this.velocity.x;
        this.position.y += this.velocity.y;
        this.draw();
    }
}

const player = new Player({x: canvas.width/2, y: canvas.height/2}, 12, "red");
const projectiles = [];
function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    player.draw();
    projectiles.forEach((projectile) => {
        projectile.update();
    })
    requestAnimationFrame(animate);
}

canvas.addEventListener("click", (event) => {
    const angle = Math.atan2(event.clientY - player.position.y, event.clientX - player.position.x);
    const velocity = {x: Math.cos(angle), y: Math.sin(angle)};
    projectiles.push(new Projectile({x: player.position.x, y: player.position.y}, 6, "blue", velocity));
});

animate();
