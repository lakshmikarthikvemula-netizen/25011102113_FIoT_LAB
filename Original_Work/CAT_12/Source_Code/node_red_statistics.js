// Paste into a Node-RED Function node after an HTTP Request and JSON parsing.
if (msg.statusCode !== 200 || msg.payload?.status !== "success") return null;

const keys = ["temperature", "humidity", "pressure"];
if (!keys.every(key => Number.isFinite(msg.payload[key]))) return null;

const stats = context.get("stats") || {};
const output = {};
for (const key of keys) {
    const value = msg.payload[key];
    const previous = stats[key] || { minimum: value, maximum: value, total: 0, count: 0 };
    previous.minimum = Math.min(previous.minimum, value);
    previous.maximum = Math.max(previous.maximum, value);
    previous.total += value;
    previous.count++;
    stats[key] = previous;
    output[key] = {
        minimum: previous.minimum,
        maximum: previous.maximum,
        average: previous.total / previous.count
    };
}
context.set("stats", stats);
msg.payload = output;
return msg;
