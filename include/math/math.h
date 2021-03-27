#pragma once

#include "include/common/common.h"

namespace Physics2D
{
	//trigonometric function
	static inline number sinx(const number& x)
	{
		return sin(x);
	}
	static inline number cosx(const number& x)
	{
		return cos(x);
	}
	static inline number tanx(const number& x)
	{
		return tan(x);
	}
	static inline number arcsinx(const number& x)
	{
		return asin(x);
	}
	static inline number arccosx(const number& x)
	{
		return acos(x);
	}
	static inline number arctanx(const number& x)
	{
		return atan(x);
	}
	//other trick
	static inline number fastInverseSqrt(number x)
	{
		assert(x != 0, "Divisor cannot be zero.");
		number xhalf = 0.5f * x;
		int i = *(int*)&x; // get bits for floating VALUE
		i = 0x5f375a86 - (i >> 1); // gives initial guess y0
		x = *(number*)&i; // convert bits BACK to float
		x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
		x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
		x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
		return x;
	}
	//basic number utility
	static inline void numberSwap(number& lhs, number& rhs)
	{
		const number temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
	static inline bool numberEqual(const number& lhs, const number& rhs)
	{
		return abs(lhs - rhs) < EPSILON;
	}
	//linear algebra
	struct Vector2
	{
		Vector2(const number& _x = 0.0f, const number& _y = 0.0f)
		{
			x = _x;
			y = _y;
		}
		Vector2(const Vector2& _copy)
		{
			x = _copy.x;
			y = _copy.y;
		}
		Vector2& operator=(const Vector2& _copy)
		{
			x = _copy.x;
			y = _copy.y;
			return *this;
		}
		Vector2(Vector2&& other) = default;

		Vector2 operator+(const Vector2& rhs)const
		{
			return Vector2(x + rhs.x, y + rhs.y);
		}

		Vector2 operator-(const Vector2& rhs)const
		{
			return Vector2(x - rhs.x, y - rhs.y);
		}

		Vector2 operator*(const int& factor)const
		{
			return Vector2(x * factor, y * factor);
		}

		Vector2 operator*(const number& factor)const
		{
			return Vector2(x * factor, y * factor);
		}

		Vector2 operator/(const number& factor)const
		{
			assert(factor != 0, "Divisor cannot be zero.");
			return Vector2(x / factor, y / factor);
		}

		Vector2 operator/(const int& factor)const
		{
			assert(factor != 0, "Divisor cannot be zero.");
			return Vector2(x / factor, y / factor);
		}

		Vector2& operator+=(const Vector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2& operator*=(const number& factor)
		{
			x *= factor;
			y *= factor;
			return *this;
		}

		Vector2& operator*=(const int& factor)
		{
			x *= factor;
			y *= factor;
			return *this;
		}

		Vector2& operator/=(const number& factor)
		{
			assert(factor != 0, "Divisor cannot be zero.");
			x /= factor;
			y /= factor;
			return *this;
		}

		Vector2& operator/=(const int& factor)
		{
			assert(factor != 0, "Divisor cannot be zero.");
			x /= factor;
			y /= factor;
			return *this;
		}

		bool operator==(const Vector2& rhs)
		{
			return x == rhs.x && y == rhs.y;
		}

		bool operator!=(const Vector2& rhs)
		{
			return x != rhs.x || y != rhs.y;
		}

		number lengthSquare()const
		{
			return x * x + y * y;
		}

		number length()const
		{
			return sqrt(lengthSquare());
		}

		Vector2& set(const number& _x, const number& _y)
		{
			x = _x;
			y = _y;
			return *this;
		}

		Vector2& set(const Vector2& _copy)
		{
			x = _copy.x;
			y = _copy.y;
			return *this;
		}

		Vector2& clear()
		{
			x = 0.0f;
			y = 0.0f;
			return *this;
		}

		Vector2& negate()
		{
			x *= -1;
			y *= -1;
			return *this;
		}

		Vector2& swap(Vector2& _other) noexcept
		{
			numberSwap(x, _other.x);
			numberSwap(y, _other.y);
			return *this;
		}

		Vector2& normalize()
		{
			number length_inv = fastInverseSqrt(lengthSquare());
			x *= length_inv;
			y *= length_inv;
			return *this;
		}

		Vector2 normal()const
		{
			return Vector2(*this).normalize();
		}

		bool equal(const Vector2& rhs)const
		{
			return numberEqual(x, rhs.x) && numberEqual(y, rhs.y);
		}

		number dot(const Vector2& rhs)const
		{
			return x * rhs.x + y * rhs.y;
		}

		number cross(const Vector2& rhs)
		{
			return x * rhs.y - y * rhs.x;
		}
		static number dotProduct(const Vector2& lhs, const Vector2& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}
		static number crossProduct(const Vector2& lhs, const Vector2& rhs)
		{
			return lhs.x * rhs.y - lhs.y * rhs.x;
		}
		static number crossProduct(const number& x1, const number& y1, const number& x2, const number& y2)
		{
			return x1 * y2 - x2 * y1;
		}
		number x;
		number y;
	};
	struct Vector3
	{
		Vector3(const number& _x = 0.0f, const number& _y = 0.0f, const number& _z = 0.0f)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		Vector3(const Vector3& _copy)
		{
			x = _copy.x;
			y = _copy.y;
			z = _copy.z;
		}
		Vector3& operator=(const Vector3& _copy)
		{
			x = _copy.x;
			y = _copy.y;
			z = _copy.z;
			return *this;
		}
		Vector3(Vector3&& other) = default;

		Vector3 operator+(const Vector3& rhs)const
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}

		Vector3 operator-(const Vector3& _other)const
		{
			return Vector3(x - _other.x, y - _other.y, z - _other.z);
		}

		Vector3 operator*(const number& factor)const
		{
			return Vector3(x * factor, y * factor, z * factor);
		}

		Vector3 operator*(const int& factor)const
		{
			return Vector3(x * factor, y * factor, z * factor);
		}

		Vector3 operator/(const number& factor)const
		{
			assert(factor == 0, "Divisor cannot be zero.");
			return Vector3(x / factor, y / factor, z / factor);
		}

		Vector3 operator/(const int& factor)const
		{
			assert(factor == 0, "Divisor cannot be zero.");
			return Vector3(x / factor, y / factor, z / factor);
		}

		Vector3& operator+=(const Vector3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3& operator*=(const number& factor)
		{
			x *= factor;
			y *= factor;
			z *= factor;
			return *this;
		}

		Vector3& operator*=(const int& factor)
		{
			x *= factor;
			y *= factor;
			z *= factor;
			return *this;
		}

		Vector3& operator/=(const number& factor)
		{
			assert(factor != 0, "Divisor cannot be zero.");
			x /= factor;
			y /= factor;
			z /= factor;
			return *this;
		}

		Vector3& operator/=(const int& factor)
		{
			assert(factor != 0, "Divisor cannot be zero.");
			x /= factor;
			y /= factor;
			z /= factor;
			return *this;
		}

		Vector3& set(const number& _x, const number& _y, const number& _z)
		{
			x = _x;
			y = _y;
			z = _z;
			return *this;
		}

		Vector3& set(const Vector3& _other)
		{
			x = _other.x;
			y = _other.y;
			z = _other.z;
			return *this;
		}

		Vector3& clear()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			return *this;
		}

		Vector3& negate()
		{
			x *= -1;
			y *= -1;
			z *= -1;
			return *this;
		}

		number lengthSquare()const
		{
			return x * x + y * y + z * z;
		}

		number length()const
		{
			return sqrt(lengthSquare());
		}

		Vector3& normalize()
		{
			number length_inv = fastInverseSqrt(lengthSquare());
			x *= length_inv;
			y *= length_inv;
			z *= length_inv;
			return *this;
		}

		Vector3 normal()const
		{
			return Vector3(*this).normalize();
		}

		bool equal(const Vector3& rhs)const
		{
			return numberEqual(x, rhs.x) && numberEqual(y, rhs.y) && numberEqual(z, rhs.z);
		}

		Vector3& swap(Vector3& _other)
		{
			numberSwap(x, _other.x);
			numberSwap(y, _other.y);
			numberSwap(z, _other.z);
			return *this;
		}

		number dot(const Vector3& rhs)const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		Vector3& cross(const Vector3& rhs)
		{
			x = y * rhs.z - rhs.y * z;
			y = rhs.x * z - x * rhs.z;
			z = x * rhs.y - y * rhs.x;
			return *this;
		}
		static number dotProduct(const Vector3& lhs, const Vector3& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		static Vector3 crossProduct(const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3(lhs.y * rhs.z - rhs.y * lhs.z, rhs.x * lhs.z - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
		}
		number x;
		number y;
		number z;
	};
	struct Matrix2x2
	{
		Matrix2x2() = default;
		
		Matrix2x2(const Vector2& _col1, const Vector2& _col2)
		{
			column1 = _col1;
			column2 = _col2;
		}
		Matrix2x2(const number& col1_x, const number& col1_y, const number& col2_x, const number& col2_y)
		{
			column1.set(col1_x, col1_y);
			column2.set(col2_x, col2_y);
		}
		Matrix2x2(Matrix2x2&& other) = default;
		Matrix2x2& operator=(const Matrix2x2& rhs)
		{
			column1 = rhs.column1;
			column2 = rhs.column2;
			return *this;
		}

		Matrix2x2& operator+=(const Matrix2x2& rhs)
		{
			column1 += rhs.column1;
			column2 += rhs.column2;
			return *this;
		}

		Matrix2x2& operator-=(const Matrix2x2& rhs)
		{
			column1 -= rhs.column1;
			column2 -= rhs.column2;
			return *this;
		}

		Matrix2x2& operator*=(const number& factor)
		{
			column1 *= factor;
			column2 *= factor;
			return *this;
		}

		Matrix2x2& operator/=(const number& factor)
		{
			assert(factor != 0, "Divisor cannot be zero.");
			column1 /= factor;
			column2 /= factor;
			return *this;
		}

		Matrix2x2 operator+(const Matrix2x2& rhs)
		{
			return Matrix2x2(column1 + rhs.column1, column2 + rhs.column2);
		}

		Matrix2x2 operator-(const Matrix2x2& rhs)
		{
			return Matrix2x2(column1 - rhs.column1, column2 - rhs.column2);
		}

		number determinant()const
		{
			return Matrix2x2::determinant(*this);
		}

		Matrix2x2& transpose()
		{
			numberSwap(column1.y, column2.x);
			return *this;
		}

		Matrix2x2& invert()
		{
			Matrix2x2::invert(*this);
			return *this;
		}

		Matrix2x2& multiply(const Matrix2x2& rhs)
		{
			*this = Matrix2x2::multiply(*this, rhs);
			return *this;
		}

		Vector2 multiply(const Vector2& rhs)const
		{
			return Matrix2x2::multiply(*this, rhs);
		}

		Matrix2x2& clear()
		{
			column1.clear();
			column2.clear();
			return *this;
		}

		Matrix2x2& set(const number& col1_x, const number& col1_y, const number& col2_x, const number& col2_y)
		{
			column1.set(col1_x, col1_y);
			column2.set(col2_x, col2_y);
		}

		Matrix2x2& set(const Vector2& _col1, const Vector2& _col2)
		{
			column1 = _col1;
			column2 = _col2;
			return *this;
		}

		Matrix2x2& set(const Matrix2x2& _other)
		{
			column1 = _other.column1;
			column2 = _other.column2;
			return *this;
		}
		static Matrix2x2 identityMatrix()
		{
			return Matrix2x2(1, 0, 0, 1);
		}
		static Vector2 multiply(const Matrix2x2& lhs, const Vector2& rhs)
		{
			return Vector2(lhs.column1.x * rhs.x + lhs.column2.x * rhs.y, lhs.column1.y * rhs.x + lhs.column2.y * rhs.y);
		}
		static Matrix2x2 multiply(const Matrix2x2& lhs, const Matrix2x2& rhs)
		{
			return Matrix2x2(lhs.column1.x * rhs.column1.x + lhs.column2.x * rhs.column1.y,
				lhs.column1.y * rhs.column1.x + lhs.column2.y * rhs.column1.y,
				lhs.column1.x * rhs.column2.x + lhs.column2.x * rhs.column2.y,
				lhs.column1.y * rhs.column2.x + lhs.column2.y * rhs.column2.y);
		}
		static number determinant(const Matrix2x2& mat)
		{
			return mat.column1.x * mat.column2.y - mat.column2.x * mat.column1.y;
		}
		static void invert(Matrix2x2& mat)
		{
			const number det = mat.determinant();
			
			if (numberEqual(det, 0.0f))
				return;
			
			numberSwap(mat.column1.x, mat.column2.y);
			mat.column1.y *= -1;
			mat.column2.x *= -1;
			mat /= det;
		}
		Vector2 column1;
		Vector2 column2;
	};
	struct Matrix3x3
	{
		Matrix3x3(const Vector3& _col1, const Vector3& _col2, const Vector3& _col3)
		{
			column1 = _col1;
			column2 = _col2;
			column3 = _col3;
		}
		Matrix3x3(const number& col1_x, const number& col1_y, const number& col1_z,
			const number& col2_x, const number& col2_y, const number& col2_z, 
			const number& col3_x, const number& col3_y, const number& col3_z)
		{
			column1.set(col1_x, col1_y, col1_z);
			column2.set(col2_x, col2_y, col2_z);
			column3.set(col3_x, col3_y, col3_z);
		}
		Matrix3x3(Matrix3x3&& other) = default;
		Matrix3x3& operator=(const Matrix3x3& rhs)
		{
			column1 = rhs.column1;
			column2 = rhs.column2;
			column3 = rhs.column3;
			return *this;
		}

		Matrix3x3& operator+=(const Matrix3x3& rhs)
		{
			column1 += rhs.column1;
			column2 += rhs.column2;
			column3 += rhs.column3;
			return *this;
		}

		Matrix3x3& operator-=(const Matrix3x3& rhs)
		{
			column1 -= rhs.column1;
			column2 -= rhs.column2;
			column3 -= rhs.column3;
			return *this;
		}

		Matrix3x3& operator*=(const number& factor)
		{
			column1 *= factor;
			column2 *= factor;
			column3 *= factor;
			return *this;
		}

		Matrix3x3& operator/=(const number& factor)
		{
			assert(factor != 0, "Divisor cannot be zero.");
			column1 /= factor;
			column2 /= factor;
			column3 /= factor;
			return *this;
		}

		number determinant()
		{
			return Matrix3x3::determinant(*this);
		}

		Matrix3x3& transpose()
		{
			numberSwap(column1.y, column2.x);
			numberSwap(column1.z, column3.x);
			numberSwap(column2.z, column3.y);
			return *this;
		}

		Matrix3x3& invert()
		{
			Matrix3x3::invert(*this);
			return *this;
		}

		Matrix3x3& clear()
		{
			column1.clear();
			column2.clear();
			column3.clear();
			return *this;
		}

		Matrix3x3& set(const number& col1_x, const number& col1_y, const number& col1_z,
		               const number& col2_x, const number& col2_y, const number& col2_z,
		               const number& col3_x, const number& col3_y, const number& col3_z)
		{
			column1.set(col1_x, col1_y, col1_z);
			column2.set(col2_x, col2_y, col2_z);
			column3.set(col3_x, col3_y, col3_z);
			return *this;
		}

		Matrix3x3& set(const Vector3& _col1, const Vector3 _col2, const Vector3 _col3)
		{
			column1 = _col1;
			column2 = _col2;
			column3 = _col3;
			return *this;
		}

		Matrix3x3& set(const Matrix3x3& _other)
		{
			column1 = _other.column1;
			column2 = _other.column2;
			column3 = _other.column3;
			return *this;
		}

		Vector3 multiply(const Vector3& rhs)
		{
			return Matrix3x3::multiply(*this, rhs);
		}

		Matrix3x3& multiply(const Matrix3x3& rhs)
		{
			*this = Matrix3x3::multiply(*this, rhs);
			return *this;
		}
		static Matrix3x3 identityMatrix()
		{
			return Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
		}
		static Matrix3x3 multiply(const Matrix3x3& lhs, const Matrix3x3& rhs)
		{
			return Matrix3x3(Matrix3x3::multiply(lhs, rhs.column1),
				Matrix3x3::multiply(lhs, rhs.column2),
				Matrix3x3::multiply(lhs, rhs.column3));
		}
		static Vector3 multiply(const Matrix3x3& lhs, const Vector3& rhs)
		{
			return Vector3(lhs.column1.x * rhs.x + lhs.column2.x * rhs.y + lhs.column3.x * rhs.z,
				lhs.column1.y * rhs.x + lhs.column2.y * rhs.y + lhs.column3.y * rhs.z,
				lhs.column1.z * rhs.x + lhs.column2.z * rhs.y + lhs.column3.z * rhs.z);
		}
		static number determinant(const Matrix3x3& mat)
		{
			return mat.column1.x * Vector2::crossProduct(mat.column2.y, mat.column2.z, mat.column3.y, mat.column3.z) +
				mat.column2.x * Vector2::crossProduct(mat.column3.y, mat.column3.z, mat.column1.y, mat.column1.z) +
				mat.column3.x * Vector2::crossProduct(mat.column1.y, mat.column1.z, mat.column2.y, mat.column2.z);
		}
		static void invert(Matrix3x3& mat)
		{
			const number det = mat.determinant();
			if (numberEqual(det, 0.0f))
				return;

			const number det11 = Vector2::crossProduct(mat.column2.y, mat.column2.z, mat.column3.y, mat.column3.z);
			const number det12 = Vector2::crossProduct(mat.column2.x, mat.column2.z, mat.column3.x, mat.column3.z) * -1;
			const number det13 = Vector2::crossProduct(mat.column2.x, mat.column2.y, mat.column3.x, mat.column3.y);
			
			const number det21 = Vector2::crossProduct(mat.column1.y, mat.column1.z, mat.column3.y, mat.column3.z) * -1;
			const number det22 = Vector2::crossProduct(mat.column1.x, mat.column1.z, mat.column3.x, mat.column3.z);
			const number det23 = Vector2::crossProduct(mat.column1.x, mat.column1.y, mat.column3.x, mat.column3.y) * -1;
			
			const number det31 = Vector2::crossProduct(mat.column1.y, mat.column1.z, mat.column2.y, mat.column2.z);
			const number det32 = Vector2::crossProduct(mat.column1.x, mat.column1.z, mat.column2.x, mat.column2.z) * -1;
			const number det33 = Vector2::crossProduct(mat.column1.x, mat.column1.y, mat.column2.x, mat.column2.y);

			mat.set(det11, det12, det13, det21, det22, det23, det31, det32, det33);
			mat.transpose();
			mat /= det;
		}
		Vector3 column1;
		Vector3 column2;
		Vector3 column3;
	};
	struct Rotation2
	{
	public:
		Rotation2(const number& angle = 0.0f)
		{
			setAngle(angle);
		}

		void setAngle(const number& angle)
		{
			m_angle = angle;
			m_rotationMatrix.column1.set(cosx(angle), sinx(angle));
			m_rotationMatrix.column1.set(sinx(angle) * -1, cosx(angle));
		}

		number angle()const
		{
			return m_angle;
		}

		Vector2 multiply(const Vector2& rhs)
		{
			return m_rotationMatrix.multiply(rhs);
		}

		Vector2 operator*(const Vector2& rhs)
		{
			return m_rotationMatrix.multiply(rhs);
		}
	private:
		number m_angle;
		Matrix2x2 m_rotationMatrix;
	};
	struct Rotation3
	{

	};
}
