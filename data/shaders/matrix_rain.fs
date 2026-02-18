#version 330

out vec4 finalColor;

uniform float uTime;
uniform vec2 uResolution; // Must be updated every frame!

float random(float x) {
    return fract(sin(x) * 43758.5453123);
}

void main() {
    vec2 screenPos = gl_FragCoord.xy;

    // make y-coordinates start from the top
    float flippedY = uResolution.y - screenPos.y;

    float colWidth = 20.0;
    float xID = floor(screenPos.x / colWidth);

    float speed = random(xID) * 0.5 + 0.2;

    // use flippedY for the head position logic
    float headPos = mod(uTime * speed * 500.0 + random(xID) * uResolution.y, uResolution.y);

    float rowHeight = 20.0;
    float cellY = floor(flippedY / rowHeight) * rowHeight;
    // distance now calculates based on the falling top-down value
    float dist = headPos - cellY;
    if (dist < 0.0) dist += uResolution.y;

    float brightness = pow(max(1.0 - (dist / 500.0), 0.0), 5.0);

    vec3 green = vec3(0.0, 1.0, 0.2);
    if (brightness > 0.98) green = vec3(0.8, 1.0, 0.9);

    finalColor = vec4(green * brightness, 1.0);
}