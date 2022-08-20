/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "Maths.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"

namespace NCL {
	namespace Maths {
		void ScreenBoxOfTri(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector2& topLeft, Vector2& bottomRight) {
			topLeft.x = std::min(v0.x, std::min(v1.x, v2.x));
			topLeft.y = std::min(v0.y, std::min(v1.y, v2.y));

			bottomRight.x = std::max(v0.x, std::max(v1.x, v2.x));
			bottomRight.y = std::max(v0.y, std::max(v1.y, v2.y));
		}

		int ScreenAreaOfTri(const Vector3 &a, const Vector3 &b, const Vector3 & c) {
			int area =(int) (((a.x * b.y) + (b.x * c.y) + (c.x * a.y)) -
				((b.x * a.y) + (c.x * b.y) + (a.x * c.y)));
			return (area >> 1);
		}

		float FloatAreaOfTri(const Vector3 &a, const Vector3 &b, const Vector3 & c) {
			float area = ((a.x * b.y) + (b.x * c.y) + (c.x * a.y)) -
				((b.x * a.y) + (c.x * b.y) + (a.x * c.y));
			return (area * 0.5f);
		}

		float CrossAreaOfTri(const Vector3 &a, const Vector3 &b, const Vector3 & c) {
			Vector3 area = Vector3::Cross(a - b, a - c);
			return area.Length() * 0.5f;
		}
	

		Vector3 Clamp(const Vector3& a, const Vector3&mins, const Vector3& maxs) {
			return Vector3(
				Clamp(a.x, mins.x, maxs.x),
				Clamp(a.y, mins.y, maxs.y),
				Clamp(a.z, mins.z, maxs.z)
			);
		}

		float LineToLineDistance(const Vector3& a1, const Vector3& a2, const Vector3& b1, const Vector3& b2)
		{
			float distance = FLT_MAX;
			Vector3 d1 = a2 - a1;
			Vector3 d2 = b2 - b1;

			Vector3 e = b1 - a1;

			Vector3 cross_e_d2 = Vector3::Cross(e, d2);
			Vector3 cross_e_d1 = Vector3::Cross(e, d1);
			Vector3 cross_d1_d2 = Vector3::Cross(d1, d2); //common perpendicular

			float t1 = Vector3::Dot(cross_e_d2, cross_d1_d2);
			float t2 = Vector3::Dot(cross_e_d1, cross_d1_d2);

			float dd = cross_d1_d2.Length();

			if (fabs(dd) < 1e-6) {
				distance = PointToLineDistance(a1, a2, b1);
				return distance;
			}

			t1 /= dd * dd;
			t2 /= dd * dd;

			Vector3 a = a1 + (a2 - a1) * t1;
			Vector3 b = b1 + (b2 - b1) * t2;
			distance = (b - a).Length();

			return distance;
		}

		float LineToLineDistance(const Vector3& a1, const Vector3& a2, const Vector3& b1, const Vector3& b2, Vector3& a, Vector3& b)
		{
			float distance = FLT_MAX;
			Vector3 d1 = a2 - a1; //dir vector
			Vector3 d2 = b2 - b1;

			Vector3 e = b1 - a1; //needed by calculation
			Vector3 cross_e_d2 = Vector3::Cross(e, d2);
			Vector3 cross_e_d1 = Vector3::Cross(e, d1);

			Vector3 cross_d1_d2 = Vector3::Cross(d1, d2); //common perpendicular

			float t1 = Vector3::Dot(cross_e_d2, cross_d1_d2);
			float t2 = Vector3::Dot(cross_e_d1, cross_d1_d2);

			float dd = cross_d1_d2.Length();

			if (fabs(dd) < 1e-6) { //Two lines are parallel
				distance = PointToLineDistance(a1, a2, b1); //Distance is equal to Distance of any point in one line to the other line in this condition
				return distance;
			}

			t1 /= dd * dd;
			t2 /= dd * dd;

			//a = a1 + a2 * t1; //Foot point of line a1a2
			//b = b1 + b2 * t2;

			a = a1 + (a2 - a1) * t1; //Foot point of line a1a2
			b = b1 + (b2 - b1) * t2; //Foot point of line b1b2
			distance = (b - a).Length();

			return distance;
		}

		float PointToLineDistance(const Vector3& p1, const Vector3& p2, const Vector3& p)
		{
			Vector3 n = (p2 - p1).Normalised();
			float k = Vector3::Dot((p - p1), n);
			Vector3 p3 = p1 + n * k;
			float distance = (p - p3).Length();
			return distance;
		}
	}
}