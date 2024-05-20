#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

const float SCALE = 30.0f;
const float DEGTORAD = 0.017;
const float RESTITUTION = 0.5f; 

int main()
{
    RenderWindow window(VideoMode(800, 600), "Pelota en un punto fijo");

    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.0f / SCALE, 500.0f / SCALE);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(800.0f / SCALE, 10.0f / SCALE);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef anchorDef;
    anchorDef.position.Set(400.0f / SCALE, 300.0f / SCALE);
    b2Body* anchorBody = world.CreateBody(&anchorDef);

    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(400.0f / SCALE, 100.0f / SCALE);
    b2Body* ballBody = world.CreateBody(&ballBodyDef);
    b2CircleShape ballShape;
    ballShape.m_radius = 20.0f / SCALE;
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &ballShape;
    ballFixtureDef.density = 1.0f;
    ballFixtureDef.friction = 0.3f;
    ballFixtureDef.restitution = RESTITUTION; 
    ballBody->CreateFixture(&ballFixtureDef);

    b2DistanceJointDef jointDef;
    jointDef.Initialize(anchorBody, ballBody, anchorBody->GetWorldCenter(), ballBody->GetWorldCenter());
    jointDef.collideConnected = true;
    world.CreateJoint(&jointDef);

    CircleShape ball(20.0f);
    ball.setFillColor(Color::Red);
    ball.setOrigin(20.0f, 20.0f);
    ball.setPosition(400.0f, 100.0f);

    bool isDragging = false;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePosition = Mouse::getPosition(window);
                    Vector2f ballPosition = ball.getPosition();
                    float dx = mousePosition.x - ballPosition.x;
                    float dy = mousePosition.y - ballPosition.y;
                    float distance = std::sqrt(dx * dx + dy * dy);
                    if (distance <= ball.getRadius())
                    {
                        isDragging = true;
                    }
                }
                break;
            case Event::MouseButtonReleased:
                if (event.mouseButton.button == Mouse::Left)
                {
                    isDragging = false;
                    ballBody->SetTransform(b2Vec2(400.0f / SCALE, 100.0f / SCALE), 0.0f);
                    ballBody->SetLinearVelocity(b2Vec2_zero);
                }
                break;
            default:
                break;
            }
        }

        if (isDragging)
        {
            Vector2i mousePosition = Mouse::getPosition(window);
            b2Vec2 ballPosition(mousePosition.x / SCALE, mousePosition.y / SCALE);
            ballBody->SetTransform(ballPosition, 0.0f);
        }

        world.Step(1.0f / 60.0f, 8, 3);

        b2Vec2 ballPos = ballBody->GetPosition();
        ball.setPosition(ballPos.x * SCALE, ballPos.y * SCALE);

        window.clear(Color::Black);

        window.draw(ball);

        window.display();
    }

    return 0;
}