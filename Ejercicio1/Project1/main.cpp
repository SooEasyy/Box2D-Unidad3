#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

void CreateBoundary(b2World& world, float x, float y, float width, float height, RectangleShape& shape, float restitution) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(width / 2, height / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.restitution = restitution;
    fixtureDef.friction = 0.0f;
    body->CreateFixture(&fixtureDef);

    shape.setSize(Vector2f(width, height));
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(x, y);
    shape.setFillColor(Color::White);
}

b2Body* CreateBall(b2World& world, float radius, float x, float y, CircleShape& shape, float restitution) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    b2Body* body = world.CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f; 
    fixtureDef.restitution = restitution; 

    body->CreateFixture(&fixtureDef);

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(Color::Green);

    return body;
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Box2D with SFML");

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float borderThickness = 10.0f;
    float restitution = 0.8f; 

    RectangleShape topBorder;
    RectangleShape bottomBorder;
    RectangleShape leftBorder;
    RectangleShape rightBorder;

    CreateBoundary(world, windowWidth / 2, borderThickness / 2, windowWidth, borderThickness, topBorder, restitution);
    CreateBoundary(world, windowWidth / 2, windowHeight - borderThickness / 2, windowWidth, borderThickness, bottomBorder, restitution);
    CreateBoundary(world, borderThickness / 2, windowHeight / 2, borderThickness, windowHeight, leftBorder, restitution);
    CreateBoundary(world, windowWidth - borderThickness / 2, windowHeight / 2, borderThickness, windowHeight, rightBorder, restitution);

    float ballRadius = 15.0f;
    float ballRestitution = 0.9f;
    CircleShape ballShape;
    b2Body* ballBody = CreateBall(world, ballRadius, windowWidth / 2, windowHeight / 2, ballShape, ballRestitution);

    ballBody->SetLinearVelocity(b2Vec2(50.0f, -50.0f));

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        world.Step(1 / 60.0f, 6, 2);

        b2Vec2 ballPosition = ballBody->GetPosition();
        ballShape.setPosition(ballPosition.x, ballPosition.y);

        window.clear();

        window.draw(topBorder);
        window.draw(bottomBorder);
        window.draw(leftBorder);
        window.draw(rightBorder);

        window.draw(ballShape);

        window.display();
    }

    return 0;
}
