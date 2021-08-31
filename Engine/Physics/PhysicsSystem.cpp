#include "PhysicsSystem.h"
#include "Object/Actor.h"
#include "Math/MathUtils.h"

const float pbls::PhysicsSystem::pixelsPerUnit = 48.0f;

namespace pbls
{
    void PhysicsSystem::Startup()
    {
        b2Vec2 gravity{ 0, 10 };
        world = std::make_unique<b2World>(gravity);
        contactListener = std::make_unique<ContactListener>();
        world->SetContactListener(contactListener.get());
    }

    void PhysicsSystem::Shutdown()
    {
        //
    }

    void PhysicsSystem::Update(float dt)
    {
        float timeStep = 1.0f / 60.0f;
        world->Step(timeStep, 8, 3);
    }

    b2Body* PhysicsSystem::CreateBody(const Vector2& position, float angle, const RigidBodyData& data, Actor* actor)
    {
        Vector2 worldPosition = ScreenToWorld(position);

        b2BodyDef bodyDef;
        bodyDef.type = (data.isDynamic) ? b2_dynamicBody : b2_staticBody;
        bodyDef.position = worldPosition;
        bodyDef.angle = pbls::DegToRad(angle);
        bodyDef.fixedRotation = data.lockAngle;
        b2Body* body = world->CreateBody(&bodyDef);

        b2PolygonShape shape;
        Vector2 worldSize = PhysicsSystem::ScreenToWorld(data.size * 0.5f);
        shape.SetAsBox(worldSize.x, worldSize.y);

        b2FixtureDef fixtureDef;
        fixtureDef.density = data.density;
        fixtureDef.friction = data.friction;
        fixtureDef.restitution = data.restitution;
        fixtureDef.isSensor = data.isSensor;
        fixtureDef.shape = &shape;
        fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);

        body->CreateFixture(&fixtureDef);

        return body;
    }

    void PhysicsSystem::DestroyBody(b2Body* body)
    {
        world->DestroyBody(body);
    }
}
