#ifndef PHYSICS2D_GJK_H
#define PHYSICS2D_GJK_H


#include "include/common/common.h"
#include "include/geometry/shape.h"
#include "include/geometry/algorithm/2d.h"

#include "fmt/core.h"
namespace Physics2D
{
	struct Minkowski
	{
		Minkowski() = default;
		Minkowski(const Vector2& point_a, const Vector2& point_b);
		inline bool operator ==(const Minkowski& rhs) const;
		inline bool operator !=(const Minkowski& rhs) const;
		Vector2 pointA;
		Vector2 pointB;
		Vector2 result;
	};

	/// <summary>
	/// Simplex structure for gjk/epa test.
	/// By convention:
	/// 1 points: p0 , construct a single point
	/// 2 points: p0 -> p1, construct a segment
	/// 4 points: p0 -> p1 -> p2 -> p0, construct a triangle
	/// </summary>
	/// <returns></returns>
	struct Simplex
	{
		std::vector<Minkowski> vertices;
		bool isContainOrigin = false;
		bool containOrigin(bool strict = false);
		static bool containOrigin(const Simplex& simplex, bool strict = false);

		void insert(const size_t& pos, const Minkowski& vertex);
		bool contains(const Minkowski& minkowski);
		bool fuzzyContains(const Minkowski& minkowski, const real& epsilon = 0.0001);
		Vector2 lastVertex() const;
	};

	struct PenetrationInfo
	{
		Vector2 normal;
		real penetration = 0;
	};

	struct PenetrationSource
	{
		Vector2 a1;
		Vector2 a2;
		Vector2 b1;
		Vector2 b2;
	};

	struct PointPair
	{
		PointPair() = default;
		Vector2 pointA;
		Vector2 pointB;
		bool isEmpty()const
		{
			return pointA.fuzzyEqual({ 0, 0 }) && pointB.fuzzyEqual({ 0, 0 });
		}
		bool operator==(const PointPair& other)const
		{
			return (other.pointA.fuzzyEqual(this->pointA) && other.pointB.fuzzyEqual(this->pointB))
				|| (other.pointB.fuzzyEqual(this->pointA) && other.pointA.fuzzyEqual(this->pointB));
		}
	};
	/// <summary>
	/// GJK Collision Detection Class
	/// </summary>
	class GJK
	{
	public:
		/// <summary>
		/// Gilbert-Johnson-Keerthi distance algorithm
		/// </summary>
		/// <param name="shape_A"></param>
		/// <param name="shape_B"></param>
		/// <param name="iteration"></param>
		/// <returns>return initial simplex and whether collision exists</returns>
		static std::tuple<bool, Simplex> gjk(const ShapePrimitive& shapeA, const ShapePrimitive& shapeB,
		                                     const size_t& iteration = 20);
		/// <summary>
		/// Expanding Polygon Algorithm
		/// </summary>
		/// <param name="shape_A"></param>
		/// <param name="shape_B"></param>
		/// <param name="src">initial simplex</param>
		/// <param name="iteration">iteration times</param>
		/// <param name="epsilon">epsilon of iterated result</param>
		/// <returns>return expanded simplex</returns>
		static Simplex epa(const ShapePrimitive& shapeA, const ShapePrimitive& shapshapeBe_B, const Simplex& src,
		                   const size_t& iteration = 20, const real& epsilon = Constant::GeometryEpsilon);
		/// <summary>
		/// Dump collision penetration normal and depth
		/// </summary>
		/// <param name="shape_A"></param>
		/// <param name="shape_B"></param>
		/// <param name="simplex"></param>
		/// <returns></returns>
		static PenetrationInfo dumpInfo(const PenetrationSource& source);
		/// <summary>
		/// Support function.
		/// </summary>
		/// <param name="shape_A"></param>
		/// <param name="shape_B"></param>
		/// <param name="direction"></param>
		/// <returns></returns>
		static Minkowski support(const ShapePrimitive& shapeA, const ShapePrimitive& shapeB,
		                         const Vector2& direction);
		/// <summary>
		/// Find two points that can form an edge closest to origin of simplex
		/// </summary>
		/// <param name="simplex"></param>
		/// <returns></returns>
		static std::tuple<size_t, size_t> findEdgeClosestToOrigin(const Simplex& simplex);
		/// <summary>
		/// Find farthest projection point in given direction
		/// </summary>
		/// <param name="shape"></param>
		/// <param name="direction"></param>
		/// <returns></returns>
		static Vector2 findFarthestPoint(const ShapePrimitive& shape, const Vector2& direction);
		/// <summary>
		/// Adjust triangle simplex, remove the point that can not form a triangle that contains origin
		/// </summary>
		/// <param name="simplex"></param>
		/// <param name="closest_1"></param>
		/// <param name="closest_2"></param>
		/// <returns></returns>
		static std::optional<Minkowski> adjustSimplex(Simplex& simplex, const size_t& closest_1,
		                                              const size_t& closest_2);
		/// <summary>
		/// Given two points, calculate the perpendicular vector and the orientation is user-defined.
		/// </summary>
		/// <param name="p1"></param>
		/// <param name="p2"></param>
		/// <param name="pointToOrigin"></param>
		/// <returns></returns>
		static Vector2 calculateDirectionByEdge(const Vector2& p1, const Vector2& p2, bool pointToOrigin = true);
		/// <summary>
		/// Calculate the distance of two shape primitive
		/// </summary>
		/// <param name="shapeA"></param>'
		/// <param name="shapeB"></param>
		/// <returns></returns>
		static PointPair distance(const ShapePrimitive& shapeA, const ShapePrimitive& shapeB,
		                          const real& iteration = 20, const real& epsilon = Constant::GeometryEpsilon);
		/// <summary>
		/// dump collision source point
		/// </summary>
		/// <param name="simplex"></param>
		/// <returns></returns>
		static PenetrationSource dumpSource(const Simplex& simplex);
		/// <summary>
		/// dump contact points
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		static PointPair dumpPoints(const PenetrationSource& source);
	};
}


#endif
