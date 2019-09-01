#ifndef RECT3I_H
#define RECT3I_H

#include "vector3i.h"
#include <core/variant.h>

class Rect3i {

public:
	Vector3i pos;
	Vector3i size;

	Rect3i() {}

	Rect3i(Vector3i p_pos, Vector3i p_size) :
			pos(p_pos),
			size(p_size) {}

	Rect3i(int ox, int oy, int oz, int sx, int sy, int sz) :
			pos(ox, oy, oz),
			size(sz, sy, sz) {}

	Rect3i(const Rect3i &other) :
			pos(other.pos),
			size(other.size) {}

	static inline Rect3i from_center_extents(Vector3i center, Vector3i extents) {
		return Rect3i(center - extents, 2 * extents);
	}

	static inline Rect3i get_bounding_box(Rect3i a, Rect3i b) {

		Rect3i box;

		box.pos.x = MIN(a.pos.x, b.pos.x);
		box.pos.y = MIN(a.pos.y, b.pos.y);
		box.pos.z = MIN(a.pos.z, b.pos.z);

		Vector3i max_a = a.pos + a.size;
		Vector3i max_b = b.pos + b.size;

		box.size.x = MAX(max_a.x, max_b.x) - box.pos.x;
		box.size.y = MAX(max_a.y, max_b.y) - box.pos.y;
		box.size.z = MAX(max_a.z, max_b.z) - box.pos.z;

		return box;
	}

	bool inline contains(Vector3i p_pos) const {
		Vector3i end = pos + size;
		return p_pos.x >= pos.x &&
			   p_pos.y >= pos.y &&
			   p_pos.z >= pos.z &&
			   p_pos.x < end.x &&
			   p_pos.y < end.y &&
			   p_pos.z < end.z;
	}

	String to_string() const {
		return String("(o:{0}, s:{1})").format(varray(pos.to_vec3(), size.to_vec3()));
	}

	bool intersects(Rect3i other) const {
		if (pos.x > other.pos.x + other.size.x) {
			return false;
		}
		if (pos.y > other.pos.y + other.size.y) {
			return false;
		}
		if (pos.z > other.pos.z + other.size.z) {
			return false;
		}
		if (other.pos.x > pos.x + size.x) {
			return false;
		}
		if (other.pos.y > pos.y + size.y) {
			return false;
		}
		if (other.pos.z > pos.z + size.z) {
			return false;
		}
		return true;
	}

	struct NoAction {
		inline void operator()(const Vector3i pos) {}
	};

	template <typename A>
	inline void for_each_cell(A a) const {
		Vector3i max = pos + size;
		Vector3i p;
		for (p.z = pos.z; p.z < max.z; ++p.z) {
			for (p.y = pos.y; p.y < max.y; ++p.y) {
				for (p.x = pos.x; p.x < max.x; ++p.x) {
					a(p);
				}
			}
		}
	}

	template <typename A>
	static void difference(Rect3i a, Rect3i b, A action) {

		if (!a.intersects(b)) {
			action(a);
			return;
		}

		Vector3i a_min = a.pos;
		Vector3i b_min = b.pos;
		Vector3i a_max = a.pos + a.size;
		Vector3i b_max = b.pos + b.size;

		if (a_min.x < b_min.x) {
			Vector3i a_rect_size(b_min.x - a_min.x, a.size.y, a.size.z);
			action(Rect3i(a_min, a_rect_size));
			a_min.x = b_min.x;
			a.pos.x = b.pos.x;
			a.size.x = a_max.x - a_min.x;
		}
		if (a_min.y < b_min.y) {
			Vector3i a_rect_size(a.size.x, b_min.y - a_min.y, a.size.z);
			action(Rect3i(a_min, a_rect_size));
			a_min.y = b_min.y;
			a.pos.y = b.pos.y;
			a.size.y = a_max.y - a_min.y;
		}
		if (a_min.z < b_min.z) {
			Vector3i a_rect_size(a.size.x, a.size.y, b_min.z - a_min.z);
			action(Rect3i(a_min, a_rect_size));
			a_min.z = b_min.z;
			a.pos.z = b.pos.z;
			a.size.z = a_max.z - a_min.z;
		}

		if (a_max.x > b_max.x) {
			Vector3i a_rect_pos(b_max.x, a_min.y, a_min.z);
			Vector3i a_rect_size(a_max.x - b_max.x, a.size.y, a.size.z);
			action(Rect3i(a_rect_pos, a_rect_size));
			a_max.x = b_max.x;
			a.size.x = a_max.x - a_min.x;
		}
		if (a_max.y > b_max.y) {
			Vector3i a_rect_pos(a_min.x, b_max.y, a_min.z);
			Vector3i a_rect_size(a.size.x, a_max.y - b_max.y, a.size.z);
			action(Rect3i(a_rect_pos, a_rect_size));
			a_max.y = b_max.y;
			a.size.y = a_max.y - a_min.y;
		}
		if (a_max.z > b_max.z) {
			Vector3i a_rect_pos(a_min.x, a_min.y, b_max.z);
			Vector3i a_rect_size(a.size.x, a.size.y, a_max.z - b_max.z);
			action(Rect3i(a_rect_pos, a_rect_size));
		}
	}

	template <typename A, typename B>
	static inline void check_cell_enters_and_exits(const Rect3i &old_box, const Rect3i &new_box, A exit_action, B enter_action) {

		// For all cells of the old box that are not in new box
		difference(old_box, new_box, [=](const Rect3i rect) {
			rect.for_each_cell(exit_action);
		});

		// For all cells of the new box that are not in old box
		difference(new_box, old_box, [=](const Rect3i rect) {
			rect.for_each_cell(enter_action);
		});
	}
};

inline bool operator!=(const Rect3i &a, const Rect3i &b) {
	return a.pos != b.pos || a.size != b.size;
}

#endif // RECT3I_H
