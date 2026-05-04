const messages = document.getElementById("messages");
const input = document.getElementById("msgInput");
const sendBtn = document.getElementById("sendBtn");

function addMessage(name, text, color = "#6ea8ff") {
    const msg = document.createElement("div");
    msg.className = "msg";

    msg.innerHTML = `
        <span class="name" style="color:${color}">${name}</span>
        <span class="text">${text}</span>
    `;

    messages.appendChild(msg);
    messages.scrollTop = messages.scrollHeight;
}

function fakeAI(text) {
    const t = text.toLowerCase();

    if (t.includes("hello")) return "Hello. I'm CordAI.";
    if (t.includes("c++")) return "C++ gives you power and control.";
    if (t.includes("recursion")) return "Recursion = function calling itself with a base case.";
    return "Interesting question. We'll connect real AI soon.";
}

function sendMessage() {
    const text = input.value.trim();
    if (!text) return;

    addMessage("You", text, "#66d9a3");

    if (text.startsWith("/ai")) {
        const question = text.slice(3).trim();
        setTimeout(() => {
            addMessage("AI Bot", fakeAI(question), "#ffb86c");
        }, 400);
    }

    input.value = "";
}

sendBtn.addEventListener("click", sendMessage);

input.addEventListener("keydown", e => {
    if (e.key === "Enter") sendMessage();
});