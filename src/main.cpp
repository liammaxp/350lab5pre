#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int ANIMATION_FRAMES = 120;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }
        if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if(keyPressed->code == sf::Keyboard::Key::Num1) {
                tween = [](float a, float b, float t) {
                    return (1 - t) * a + t * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num2) {
                tween = [](float a, float b, float t) {
                    float p = t * t;
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num3) {
                tween = [](float a, float b, float t) {
                    float p = 1 - (1 - t) * (1 - t);
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num4) {
                tween = [](float a, float b, float t) {
                    float p;
                    if(t < 0.5f) {
                        p = 2 * t * t;
                    }
                    else {
                        p = 1 - 2 * (1 - t) * (1 - t);
                    }
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num5) {
                tween = [](float a, float b, float t) {
                    float p = t * t * t;
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num6) {
                tween = [](float a, float b, float t) {
                    float p = 1 - (1 - t) * (1 - t) * (1 - t);
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num7) {
                tween = [](float a, float b, float t) {
                    float p;
                    if(t < 0.5f) {
                        p = 4 * t * t * t;
                    }
                    else {
                        p = 1 - 4 * (1 - t) * (1 - t) * (1 - t);
                    }
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num8) {
                tween = [](float a, float b, float t) {
                    float p = t * t * t * t;
                    return (1 - p) * a + p * b;
                };
            }
            else if(keyPressed->code == sf::Keyboard::Key::Num9) {
                tween = [](float a, float b, float t) {
                    float p = 1 - (1 - t) * (1 - t) * (1 - t) * (1 - t);
                    return (1 - p) * a + p * b;
                };
            }
        }
        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    static std::uint32_t elapsedFrames = 0;
    float t = static_cast<float>(elapsedFrames % ANIMATION_FRAMES)/ static_cast<float>(ANIMATION_FRAMES - 1);
    const float radius = 20.0f;
    float leftX = radius;
    float rightX = WINDOW_WIDTH - radius;
    float y = WINDOW_HEIGHT / 3.0f;
    float x = tween(leftX, rightX, t);
    sf::CircleShape circle(radius);
    circle.setOrigin({radius, radius});
    circle.setPosition({x, y});
    circle.setFillColor(sf::Color::Cyan);
    window.draw(circle);
    elapsedFrames++;
    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    const float graphLeft = 100.0f;
    const float graphBottom = 700.0f;
    const float graphWidth = 600.0f;
    const float graphHeight = 200.0f;
    sf::RectangleShape xAxis({graphWidth, 2.0f});
    xAxis.setPosition({graphLeft, graphBottom});
    xAxis.setFillColor(sf::Color::White);
    window.draw(xAxis);
    sf::RectangleShape yAxis({2.0f, graphHeight});
    yAxis.setPosition({graphLeft, graphBottom - graphHeight});
    yAxis.setFillColor(sf::Color::White);
    window.draw(yAxis);
    const int samples = 100;
    sf::VertexArray curve(sf::PrimitiveType::LineStrip);
    for(int i = 0; i <= samples; i++) {
        float sampleT = static_cast<float>(i) / samples;
        float value = tween(0.0f, 1.0f, sampleT);
        float graphX = graphLeft + sampleT * graphWidth;
        float graphY = graphBottom - value * graphHeight;
        curve.append(sf::Vertex({graphX, graphY}, sf::Color::Cyan));
    }
    window.draw(curve);
    float currentValue = tween(0.0f, 1.0f, t);
    float currentGraphX = graphLeft + t * graphWidth;
    float currentGraphY = graphBottom - currentValue * graphHeight;
    const float dotRadius = 5.0f;
    sf::CircleShape dot(dotRadius);
    dot.setOrigin({dotRadius, dotRadius});
    dot.setPosition({currentGraphX, currentGraphY});
    dot.setFillColor(sf::Color::Yellow);
    window.draw(dot);
    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
