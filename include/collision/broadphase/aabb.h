#ifndef PHYSICS2D_BROADPHASE_AABB_H
#define PHYSICS2D_BROADPHASE_AABB_H

#include "include/math/linear/linear.h"
#include "include/geometry/shape.h"

namespace Physics2D
{
	class Body;

	struct AABB
	{
		AABB() = default;
		real width = 0;
		real height = 0;
		Vector2 position;
		bool collide(const AABB& other) const;
		void expand(const real& factor);
		void clear();
		AABB& unite(const AABB& other);
		real surfaceArea()const;
		real volume()const;
		bool isSubset(const AABB& other)const;
		bool isEmpty()const;
		bool operator==(const AABB& other)const;
		std::optional<Vector2> raycast(const Vector2& start, const Vector2& direction)const;
		/// <summary>
		/// Create AABB from shape.
		/// </summary>
		/// <param name="shape">shape source</param>
		/// <param name="factor">AABB scale factor. Default factor 1 means making tight AABB</param>
		/// <returns></returns>
		static AABB fromShape(const ShapePrimitive& shape, const real& factor = 0);
		static AABB fromBody(Body* body, const real& factor = 0);
		/// <summary>
		/// Check if two aabbs are overlapping
		/// </summary>
		/// <param name="src"></param>
		/// <param name="target"></param>
		/// <returns></returns>
		static bool collide(const AABB& src, const AABB& target);
		/// <summary>
		/// Return two aabb union result
		/// </summary>
		/// <param name="src"></param>
		/// <param name="target"></param>
		/// <returns></returns>
		static AABB unite(const AABB& src, const AABB& target, const real& factor = 0);
		/// <summary>
		/// Check if b is subset of a
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns></returns>
		static bool isSubset(const AABB& a, const AABB& b);

		static void expand(AABB& aabb, const real& factor = 0.0);

		static std::optional<Vector2> raycast(const AABB& aabb, const Vector2& start, const Vector2& direction);
		
	};

	struct Pair
	{
		Pair() {}
		Pair(const AABB& aabb, Body* source = nullptr) : body(source), aabb(aabb) {}
		Body* body = nullptr;
		AABB aabb;
		void clear();
	};
	inline bool AABB::isEmpty() const
	{
		return realEqual(width, 0) && realEqual(height, 0) && position.fuzzyEqual({ 0, 0 });
	}
	inline std::optional<Vector2> AABB::raycast(const Vector2& start, const Vector2& direction) const
	{
		return raycast(*this, start, direction);
	}
}
#endif