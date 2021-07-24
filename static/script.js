let canvas = document.getElementById('pengis');
let ctx = canvas.getContext('2d');
canvas.style.background = "#222";

// TODO: deploy server
const ws = new WebSocket("ws://127.0.0.1:8080/game");
ws.onerror = function (e) {
    console.error("websocket error, probably nothing to worry about if you don't have the server running locally yet", e);
};

ws.onclose = function (e) {
    console.error("el bad");
    //alert("you've lost connection to the game server");
}

const PLAYER_SPEED = 2;
const PROJECTILE_SPEED = 5;

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
    move() {
        if (keys[87]) {
            player.position.y -= PLAYER_SPEED;
        }
        if (keys[65]) {
            player.position.x -= PLAYER_SPEED;
        }
        if (keys[83]) {
            player.position.y += PLAYER_SPEED;
        }
        if (keys[68]) {
            player.position.x += PLAYER_SPEED;
        }

        if (ws.readyState == 1) {
            // I have no idea if this is blocking, if it is then we may have a problem
            ws.send(JSON.stringify({
                "type": "pos",
                "pos": this.position
            }));
        }
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
let other_players = {};

function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    player.update();

    projectiles.forEach((projectile) => {
        projectile.update();
    });

    Object.values(other_players).forEach((other_player) => {
        other_player.draw();
    });

    requestAnimationFrame(animate);
}

ws.onmessage = function (e) {
    const data = JSON.parse(e.data);

    if (data["type"] = "pos") {
        other_players[data["uuid"]] = new Player(data["pos"], 12, "green");
    }
};

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
