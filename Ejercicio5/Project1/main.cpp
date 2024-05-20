#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
using namespace sf;

const float SCALE = 30.0f;

int main() {
    RenderWindow window(VideoMode(800, 600), "SFML + Box2D");

    b2Vec2 gravity(0.0f, 0.1f);
    b2World world(gravity);

    // Suelo
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(800.0f / SCALE, 10.0f / SCALE);
    groundBody->CreateFixture(&groundBox, 0.0f); 

    // Torso
    b2BodyDef torsoBodyDef;
    torsoBodyDef.type = b2_dynamicBody; 
    torsoBodyDef.position.Set(400.0f / SCALE, 350.0f / SCALE);
    b2Body* torso = world.CreateBody(&torsoBodyDef);
    b2PolygonShape torsoShape;
    torsoShape.SetAsBox(20.0f / SCALE, 40.0f / SCALE);
    b2FixtureDef torsoFixtureDef;
    torsoFixtureDef.shape = &torsoShape;
    torsoFixtureDef.density = 1.0f; 
    torso->CreateFixture(&torsoFixtureDef);

    // Cabeza
    b2BodyDef headBodyDef;
    headBodyDef.type = b2_dynamicBody;
    headBodyDef.position.Set(400.0f / SCALE, 280.0f / SCALE);
    b2Body* head = world.CreateBody(&headBodyDef);
    b2CircleShape headShape;
    headShape.m_radius = 15.0f / SCALE;
    b2FixtureDef headFixtureDef;
    headFixtureDef.shape = &headShape;
    headFixtureDef.density = 0.5f;
    head->CreateFixture(&headFixtureDef);

    // Brazos
    b2BodyDef armBodyDef;
    armBodyDef.type = b2_dynamicBody;
    armBodyDef.position.Set(370.0f / SCALE, 350.0f / SCALE);
    b2Body* leftArm = world.CreateBody(&armBodyDef);
    b2Body* rightArm = world.CreateBody(&armBodyDef);
    b2PolygonShape armShape;
    armShape.SetAsBox(20.0f / SCALE, 10.0f / SCALE);
    b2FixtureDef armFixtureDef;
    armFixtureDef.shape = &armShape;
    armFixtureDef.density = 0.5f; 
    leftArm->CreateFixture(&armFixtureDef);
    rightArm->CreateFixture(&armFixtureDef);

    // Piernas
    b2BodyDef legBodyDef;
    legBodyDef.type = b2_dynamicBody;
    legBodyDef.position.Set(390.0f / SCALE, 480.0f / SCALE);
    b2Body* leftLeg = world.CreateBody(&legBodyDef);
    b2Body* rightLeg = world.CreateBody(&legBodyDef);
    b2PolygonShape legShape;
    legShape.SetAsBox(12.0f / SCALE, 25.0f / SCALE);
    b2FixtureDef legFixtureDef;
    legFixtureDef.shape = &legShape;
    legFixtureDef.density = 0.5f; 
    leftLeg->CreateFixture(&legFixtureDef);
    rightLeg->CreateFixture(&legFixtureDef);

    // Unimos el torso con la cabeza
    b2RevoluteJointDef jointHeadDef;
    jointHeadDef.bodyA = torso;
    jointHeadDef.bodyB = head;
    jointHeadDef.localAnchorA.Set(0.0f, -40.0f / SCALE);
    jointHeadDef.localAnchorB.Set(0.0f, 15.0f / SCALE);
    jointHeadDef.enableLimit = true;
    jointHeadDef.lowerAngle = -b2_pi / 4;
    jointHeadDef.upperAngle = b2_pi / 4;
    world.CreateJoint(&jointHeadDef);

    // Unimos los brazos con el torso
    b2RevoluteJointDef jointLeftArmDef;
    jointLeftArmDef.bodyA = torso;
    jointLeftArmDef.bodyB = leftArm;
    jointLeftArmDef.localAnchorA.Set(-20.0f / SCALE, 0.0f);
    jointLeftArmDef.localAnchorB.Set(0.0f, 0.0f);
    jointLeftArmDef.enableLimit = true;
    jointLeftArmDef.lowerAngle = -b2_pi / 2;
    jointLeftArmDef.upperAngle = b2_pi / 2;
    world.CreateJoint(&jointLeftArmDef);

    b2RevoluteJointDef jointRightArmDef;
    jointRightArmDef.bodyA = torso;
    jointRightArmDef.bodyB = rightArm;
    jointRightArmDef.localAnchorA.Set(20.0f / SCALE, 0.0f);
    jointRightArmDef.localAnchorB.Set(0.0f, 0.0f);
    jointRightArmDef.enableLimit = true;
    jointRightArmDef.lowerAngle = -b2_pi / 2;
    jointRightArmDef.upperAngle = b2_pi / 2;
    world.CreateJoint(&jointRightArmDef);

    // Unimos las piernas con el torso
    b2RevoluteJointDef jointLeftLegDef;
    jointLeftLegDef.bodyA = torso;
    jointLeftLegDef.bodyB = leftLeg;
    jointLeftLegDef.localAnchorA.Set(-10.0f / SCALE, 40.0f / SCALE);
    jointLeftLegDef.localAnchorB.Set(0.0f, -25.0f / SCALE);
    jointLeftLegDef.enableLimit = true;
    jointLeftLegDef.lowerAngle = -b2_pi / 4;
    jointLeftLegDef.upperAngle = b2_pi / 4;
    world.CreateJoint(&jointLeftLegDef);

    b2RevoluteJointDef jointRightLegDef;
    jointRightLegDef.bodyA = torso;
    jointRightLegDef.bodyB = rightLeg;
    jointRightLegDef.localAnchorA.Set(10.0f / SCALE, 40.0f / SCALE);
    jointRightLegDef.localAnchorB.Set(0.0f, -25.0f / SCALE);
    jointRightLegDef.enableLimit = true;
    jointRightLegDef.lowerAngle = -b2_pi / 4;
    jointRightLegDef.upperAngle = b2_pi / 4;
    world.CreateJoint(&jointRightLegDef);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::Black);

        world.Step(1 / 60.0f, 8, 3);

        RectangleShape ground(Vector2f(800.0f, 20.0f));
        ground.setFillColor(Color(139, 69, 19));
        ground.setOrigin(400.0f, 10.0f);
        ground.setPosition(400.0f, 590.0f);
        window.draw(ground);

        // Dibujo del Torso
        RectangleShape torsoShape(Vector2f(40.0f, 80.0f));
        torsoShape.setFillColor(Color::Red);
        torsoShape.setOrigin(20.0f, 40.0f);
        torsoShape.setPosition(torso->GetPosition().x * SCALE, torso->GetPosition().y * SCALE);
        torsoShape.setRotation(torso->GetAngle() * 180 / b2_pi);
        window.draw(torsoShape);

        // Dibujo de la cabeza
        CircleShape headShape(15.0f);
        headShape.setFillColor(Color::Yellow);
        headShape.setOrigin(10.0f, 15.0f);
        headShape.setPosition(head->GetPosition().x * SCALE, head->GetPosition().y * SCALE);
        headShape.setRotation(head->GetAngle() * 180 / b2_pi);
        window.draw(headShape);

        // Dibujo de los brazos
        RectangleShape leftArmShape(Vector2f(40.0f, 20.0f));
        leftArmShape.setFillColor(Color::Blue);
        leftArmShape.setOrigin(40.0f, 30.0f);
        leftArmShape.setPosition(leftArm->GetPosition().x * SCALE, leftArm->GetPosition().y * SCALE);
        leftArmShape.setRotation(leftArm->GetAngle() * 180 / b2_pi);
        window.draw(leftArmShape);

        RectangleShape rightArmShape(Vector2f(40.0f, 20.0f));
        rightArmShape.setFillColor(Color::Blue);
        rightArmShape.setOrigin(0.0f, 30.0f);
        rightArmShape.setPosition(rightArm->GetPosition().x * SCALE, rightArm->GetPosition().y * SCALE);
        rightArmShape.setRotation(rightArm->GetAngle() * 180 / b2_pi);
        window.draw(rightArmShape);

        //Dibujo de las piernas
        RectangleShape leftLegShape(Vector2f(24.0f, 50.0f));
        leftLegShape.setFillColor(Color::Green);
        leftLegShape.setOrigin(12.0f, 25.0f);
        leftLegShape.setPosition(leftLeg->GetPosition().x * SCALE, leftLeg->GetPosition().y * SCALE);
        leftLegShape.setRotation(leftLeg->GetAngle() * 180 / b2_pi);
        window.draw(leftLegShape);

        RectangleShape rightLegShape(Vector2f(24.0f, 50.0f));
        rightLegShape.setFillColor(Color::Green);
        rightLegShape.setOrigin(12.0f, 25.0f);
        rightLegShape.setPosition(rightLeg->GetPosition().x * SCALE, rightLeg->GetPosition().y * SCALE);
        rightLegShape.setRotation(rightLeg->GetAngle() * 180 / b2_pi);
        window.draw(rightLegShape);

        window.display();
    }

    return 0;
}

