// Use after MQTT In and a JSON node. Wire three outputs to three Debug nodes or charts.
const keys = ["temperature", "humidity", "pressure"];
if (msg.payload?.status !== "success" ||
    !keys.every(key => Number.isFinite(msg.payload[key]))) return null;
return keys.map(key => ({ topic: key, payload: msg.payload[key] }));
