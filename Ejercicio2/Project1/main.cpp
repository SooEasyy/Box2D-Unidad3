#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

b2Body* CreateBall(b2World& world, float radius, float x, float y, CircleShape& shape, float density = 1.0f) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / 30.0f, y / 30.0f); 
    b2Body* body = world.CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = radius / 30.0f; 

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = density;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.6f;

    body->CreateFixture(&fixtureDef);

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(x, y);
    shape.setFillColor(Color::Green);

    return body;
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Box2D with SFML - Spring Example");

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    float ballRadius = 15.0f;

    CircleShape ballShape1;
    CircleShape ballShape2;
    b2Body* ballBody1 = CreateBall(world, ballRadius, 300.0f, 300.0f, ballShape1);
    b2Body* ballBody2 = CreateBall(world, ballRadius, 500.0f, 300.0f, ballShape2);

    b2Vec2 delta = ballBody2->GetPosition() - ballBody1->GetPosition();
    float naturalLength = delta.Length();

    b2DistanceJointDef springDef;
    springDef.bodyA = ballBody1;
    springDef.bodyB = ballBody2;
    springDef.localAnchorA.Set(0, 0);
    springDef.localAnchorB.Set(0, 0);
    springDef.length = naturalLength * 0.9f; 
    b2DistanceJoint* spring = (b2DistanceJoint*)world.CreateJoint(&springDef);

    bool dragging = false;
    b2Body* draggedBody = nullptr;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    b2Vec2 mousePos(event.mouseButton.x, event.mouseButton.y);
                    mousePos.x /= 30.0f;
                    mousePos.y /= 30.0f;
                    for (b2Body* b : { ballBody1, ballBody2 }) {
                        b2Vec2 bodyPos = b->GetPosition();
                        float dx = mousePos.x - bodyPos.x;
                        float dy = mousePos.y - bodyPos.y;
                        float distanceSquared = dx * dx + dy * dy;
                        if (distanceSquared <= ballRadius * ballRadius) {
                            dragging = true;
                            draggedBody = b;
                            break;
                        }
                    }
                }
            }
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    dragging = false;
                    draggedBody = nullptr;
                }
            }
            if (event.type == Event::MouseMoved) {
                if (dragging && draggedBody) {
                    b2Vec2 mousePos(event.mouseMove.x, event.mouseMove.y);
                    mousePos.x /= 30.0f;
                    mousePos.y /= 30.0f;
                    draggedBody->SetTransform(mousePos, 0);
                }
            }
        }

        b2Vec2 force = spring->GetAnchorB() - spring->GetAnchorA();
        spring->GetBodyA()->ApplyForceToCenter(0.1f * force, true);
        spring->GetBodyB()->ApplyForceToCenter(-0.1f * force, true);

        world.Step(1 / 60.0f, 6, 2);

        b2Vec2 position1 = ballBody1->GetPosition();
        ballShape1.setPosition(position1.x * 30.0f, position1.y * 30.0f); 

        b2Vec2 position2 = ballBody2->GetPosition();
        ballShape2.setPosition(position2.x * 30.0f, position2.y * 30.0f); 

        window.clear();
        window.draw(ballShape1);
        window.draw(ballShape2);
        window.display();
    }

    return 0;
}