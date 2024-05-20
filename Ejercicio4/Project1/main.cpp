#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>}
using namespace sf; 

const float SCALE = 30.0f;

int main()
{
    RenderWindow window(VideoMode(800, 600), "SFML + Box2D");

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.0f / SCALE, 600.0f / SCALE);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(800.0f / SCALE, 10.0f / SCALE);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef leftWallDef;
    leftWallDef.position.Set(0.0f, 0.0f);
    b2Body* leftWallBody = world.CreateBody(&leftWallDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(10.0f / SCALE, 600.0f / SCALE);
    leftWallBody->CreateFixture(&leftWallBox, 0.0f);

    b2BodyDef rightWallDef;
    rightWallDef.position.Set(800.0f / SCALE, 0.0f);
    b2Body* rightWallBody = world.CreateBody(&rightWallDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(10.0f / SCALE, 600.0f / SCALE);
    rightWallBody->CreateFixture(&rightWallBox, 0.0f);

    b2BodyDef topWallDef;
    topWallDef.position.Set(0.0f, 0.0f);
    b2Body* topWallBody = world.CreateBody(&topWallDef);
    b2PolygonShape topWallBox;
    topWallBox.SetAsBox(800.0f / SCALE, 10.0f / SCALE);
    topWallBody->CreateFixture(&topWallBox, 0.0f);

    b2BodyDef bottomWallDef;
    bottomWallDef.position.Set(0.0f, 600.0f / SCALE);
    b2Body* bottomWallBody = world.CreateBody(&bottomWallDef);
    b2PolygonShape bottomWallBox;
    bottomWallBox.SetAsBox(800.0f / SCALE, 10.0f / SCALE);
    bottomWallBody->CreateFixture(&bottomWallBox, 0.0f);

    b2BodyDef squareBodyDef;
    squareBodyDef.type = b2_dynamicBody;
    squareBodyDef.position.Set(400.0f / SCALE, 300.0f / SCALE);
    b2Body* squareBody = world.CreateBody(&squareBodyDef);
    b2PolygonShape squareShape;
    squareShape.SetAsBox(30.0f / SCALE, 30.0f / SCALE); 
    b2FixtureDef squareFixtureDef;
    squareFixtureDef.shape = &squareShape;
    squareFixtureDef.density = 1.0f;
    squareFixtureDef.friction = 0.3f;
    squareBody->CreateFixture(&squareFixtureDef);

    RectangleShape square(Vector2f(60.0f, 60.0f));
    square.setFillColor(Color::Red);
    square.setOrigin(30.0f, 30.0f);
    square.setPosition(400.0f, 300.0f);

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
            default:
                break;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            squareBody->ApplyForceToCenter(b2Vec2(-50.0f, 0.0f), true);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            squareBody->ApplyForceToCenter(b2Vec2(50.0f, 0.0f), true);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            squareBody->ApplyForceToCenter(b2Vec2(0.0f, -50.0f), true);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            squareBody->ApplyForceToCenter(b2Vec2(0.0f, 50.0f), true);
        }

        world.Step(1.0f / 60.0f, 8, 3);

        b2Vec2 squarePos = squareBody->GetPosition();
        square.setPosition(squarePos.x * SCALE, squarePos.y * SCALE);
        float squareAngle = squareBody->GetAngle() * 180 / b2_pi;
        square.setRotation(squareAngle);

        window.clear(Color::Black);

        window.draw(square);

        window.draw(square);

        window.display();
    }

    return 0;
}