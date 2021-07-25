let canvas = document.getElementById('pengis');
let ctx = canvas.getContext('2d');
canvas.style.background = "#222";

const PLAYER_SPEED = 2;
const PROJECTILE_SPEED = 5;

class Player {
    constructor(position, radius, color) {
        this.position = position;
        this.radius = radius;
        this.color = color;
        this.velocity = {x: 0, y: 0};
    }
    draw() {
        ctx.fillStyle = this.color;
        ctx.beginPath();
        ctx.arc(this.position.x, this.position.y, this.radius, 0, 2*Math.PI);
        ctx.fill();
    }
    move() {
        this.velocity.x = 0;
        this.velocity.y = 0;
        if (keys[87]) {
            this.velocity.y -= 1;
        }
        if (keys[65]) {
            this.velocity.x -= 1;
        }
        if (keys[83]) {
            this.velocity.y += 1;
        }
        if (keys[68]) {
            this.velocity.x += 1;
        }
        let magnitude = Math.sqrt(this.velocity.x**2 + this.velocity.y**2);
        if (magnitude != 0) {
            this.velocity.x = this.velocity.x / magnitude * PLAYER_SPEED;
            this.velocity.y = this.velocity.y / magnitude * PLAYER_SPEED;
        }
        this.position.x += this.velocity.x;
        this.position.y += this.velocity.y;
    }
    update() {
        this.move();
        this.draw();
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

const keys = [];
const player = new Player({x: canvas.width/2, y: canvas.height/2}, 12, "red");
const projectiles = [];
function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    player.update();
    projectiles.forEach((projectile) => {
        projectile.update();
    })
    requestAnimationFrame(animate);
}

canvas.addEventListener("click", (event) => {
    const angle = Math.atan2(event.clientY - player.position.y, event.clientX - player.position.x);
    const velocity = {x: Math.cos(angle) * 5, y: Math.sin(angle) * 5};
    projectiles.push(new Projectile({x: player.position.x, y: player.position.y}, 6, "blue", velocity));
});

window.addEventListener("keydown", (event) => {
    keys[event.keyCode] = true;
})
window.addEventListener("keyup", (event) => {
    delete keys[event.keyCode];
})

animate();
