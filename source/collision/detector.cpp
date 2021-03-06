#include "include/collision/detector.h"
namespace Physics2D
{
	
	bool Detector::collide(Body* bodyA, Body* bodyB)
	{
		assert(bodyA != nullptr && bodyB != nullptr);

		ShapePrimitive shapeA, shapeB;
		shapeA.shape = bodyA->shape();
		shapeA.rotation = bodyA->rotation();
		shapeA.transform = bodyA->position();

		shapeB.shape = bodyB->shape();
		shapeB.rotation = bodyB->rotation();
		shapeB.transform = bodyB->position();

		AABB a = AABB::fromShape(shapeA);
		AABB b = AABB::fromShape(shapeB);
		if (!a.collide(b))
			return false;


		auto [isColliding, simplex] = GJK::gjk(shapeA, shapeB);

		if (shapeA.transform.fuzzyEqual(shapeB.transform) && !isColliding)
			isColliding = simplex.containOrigin(true);

		return isColliding;
	}
	Collision Detector::detect(Body* bodyA, Body* bodyB)
	{
		Collision result;

		if (bodyA == nullptr || bodyB == nullptr)
			return result;

		if (bodyA == bodyB)
			return result;

		result.bodyA = bodyA;
		result.bodyB = bodyB;

		ShapePrimitive shapeA, shapeB;
		shapeA.shape = bodyA->shape();
		shapeA.rotation = bodyA->rotation();
		shapeA.transform = bodyA->position();

		shapeB.shape = bodyB->shape();
		shapeB.rotation = bodyB->rotation();
		shapeB.transform = bodyB->position();

		AABB a = AABB::fromShape(shapeA);
		AABB b = AABB::fromShape(shapeB);
		if (!a.collide(b))
			return result;

		auto [isColliding, simplex] = GJK::gjk(shapeA, shapeB);

		if (shapeA.transform.fuzzyEqual(shapeB.transform) && !isColliding)
			isColliding = simplex.containOrigin(true);


		result.isColliding = isColliding;

		
		if (isColliding)
		{
			auto oldSimplex = simplex;
			simplex = GJK::epa(shapeA, shapeB, simplex);
			PenetrationSource source = GJK::dumpSource(simplex);

			
			const auto info = GJK::dumpInfo(source);
			result.normal = info.normal;
			result.penetration = info.penetration;
			result.contactList.emplace_back(GJK::dumpPoints(source));
		}
		assert(result.contactList.size() != 3);
		return result;
	}

	std::optional<PointPair> Detector::distance(Body* bodyA, Body* bodyB)
	{
		if (bodyA == nullptr || bodyB == nullptr)
			return std::nullopt;

		if (bodyA == bodyB)
			return std::nullopt;

		ShapePrimitive shapeA, shapeB;
		shapeA.shape = bodyA->shape();
		shapeA.rotation = bodyA->rotation();
		shapeA.transform = bodyA->position();

		shapeB.shape = bodyB->shape();
		shapeB.rotation = bodyB->rotation();
		shapeB.transform = bodyB->position();

		return std::optional<PointPair>(GJK::distance(shapeA, shapeB));
	}
}