#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_set>

struct Vec3 {
	int64_t x, y, z;
};

Vec3 operator-(const Vec3& a, const Vec3& b)
{
	return Vec3{ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3{ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 operator*(const Vec3& a, const Vec3& b)
{
	return Vec3{ a.x*b.x, a.y*b.y, a.z*b.z };
}

Vec3 operator/(const Vec3& a, const Vec3& b)
{
	return Vec3{ a.x / b.x, a.y / b.y, a.z / b.z };
}


struct Particle {
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
};

int64_t one_norm(const Vec3& x)
{
	return abs(x.x) + abs(x.y) + abs(x.z);
}

std::vector<Particle> load_input(const char* filename)
{
	std::vector<Particle> result;
	std::ifstream fs(filename);
	while (fs.good()) {
		Particle p;

		fs.ignore(std::numeric_limits<std::streamsize>::max(), '<');

		fs >> p.pos.x; fs.ignore(1);
		fs >> p.pos.y; fs.ignore(1);
		fs >> p.pos.z;

		fs.ignore(std::numeric_limits<std::streamsize>::max(), '<');

		fs >> p.vel.x; fs.ignore(1);
		fs >> p.vel.y; fs.ignore(1);
		fs >> p.vel.z;

		fs.ignore(std::numeric_limits<std::streamsize>::max(), '<');

		fs >> p.acc.x; fs.ignore(1);
		fs >> p.acc.y; fs.ignore(1);
		fs >> p.acc.z; fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		result.emplace_back(std::move(p));
	}

	return result;
}

size_t part_1(const std::vector<Particle>& particles)
{
	auto min = std::min_element(particles.cbegin(), particles.cend(), [](const Particle& a, const Particle& b) -> bool {
		return one_norm(a.acc) < one_norm(b.acc);
	});

	return std::distance(particles.cbegin(), min);
}

bool quadratic_solve(int64_t a, int64_t b, int64_t c, int64_t* t0, int64_t* t1)
{
	int64_t discriminant = b*b - 4*a*c;
	if (discriminant < 0) { return false; }
	int64_t sqrt_disc = round(sqrt(discriminant));
	if (sqrt_disc*sqrt_disc != discriminant) {
		return false;
	}
	*t0 = (sqrt_disc - b)/(2*a);
	*t1 = (-b - sqrt_disc)/(2*a);
	return true;
}

bool linear_solve(int64_t a, int64_t b, int64_t* t)
{
	if (a != 0) {
		*t = b / a;
		return true;
	}
	return false;
}

size_t part_2(std::vector<Particle> particles)
{
	struct Collision {
		size_t pa, pb;
		int64_t time;
	};

	std::vector<Collision> collisions;
	for (size_t i = 0; i < particles.size(); ++i) {
		for (size_t j = i + 1; j < particles.size(); ++j) {
			Vec3 dA = particles[i].acc - particles[j].acc;
			Vec3 dV = particles[i].vel - particles[j].vel;
			Vec3 dP = particles[i].pos - particles[j].pos;

			Vec3 t0, t1;
			if (dA.x != 0) {
				if (!quadratic_solve(dA.x, dV.x, dP.x, &t0.x, &t1.x)) {
					continue;
				}
			}
			else {
				if (!linear_solve(dV.x, dP.x, &t0.x)) {
					continue;
				}
				t1.x = t0.x;
			}

			if (dA.y != 0) {
				if (!quadratic_solve(dA.y, dV.y, dP.y, &t0.y, &t1.y)) {
					continue;
				}
			}
			else {
				if (!linear_solve(dV.y, dP.y, &t0.y)) {
					continue;
				}
				t1.y = t0.y;
			}

			if (dA.z != 0) {
				if (!quadratic_solve(dA.z, dV.z, dP.z, &t0.z, &t1.z)) {
					continue;
				}
			}
			else {
				if (!linear_solve(dV.z, dP.z, &t0.z)) {
					continue;
				}
				t1.z = t0.z;
			}

			if (t0.x == t0.y && t0.x == t0.z && t0.x > 0) {
				collisions.emplace_back(Collision{i, j, t0.x});
			}
			if (t1.x == t1.y && t1.x == t1.z && t1.x > 0) {
				collisions.emplace_back(Collision{ i, j, t1.x });
			}
		}
	}

	//Now we have all the collisions. Sort them by time.
	std::sort(collisions.begin(), collisions.end(), [](const Collision& a, const Collision& b) { return a.time < b.time; });
	//Need to remove "duplicate" collisions. How to best do this? An element is removed if it involves a collision with a particle which has another collision


	return collisions.size();
}

int main(int argc, char** argv)
{
	auto particles = load_input("day20.txt");
	std::cout << part_1(particles) << std::endl;
	std::cout <<part_2(particles) << std::endl;
	std::getchar();
}