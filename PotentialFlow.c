#include <stdlib.h>
#define map(val, m0, M0, m1, M1) (val - m0) / (M0 - m0) * (M1 - m1) + m1
#define POINTS_INIT_CAPACITY 64
#define NO_PENETRATION_DEPTH 1

typedef struct {
	int x, y;
} Point;

typedef struct {
	Point *Points;
	size_t Capacity;
	size_t Count;
} PointsDA;

void InitField(double Field[], size_t width, size_t height, double Velocity, double alpha);
void SolveFieldDivergence(double Field[], size_t width, size_t height, double speed);
PointsDA CreatePointDA(void);
void AddPointToDA(PointsDA *Points, int x, int y);
void SolveForNoPenetration(double Field[], PointsDA Points, size_t width, size_t height, float speed);

void InitField(double Field[], size_t width, size_t height, double Velocity, double alpha){
	for (size_t y = 0; y < height; ++y){
		for (size_t x = 0; x < width; ++x){
			if (x == 0 || y == 0 || y == height - 1 || x == width - 1)
				Field[x + y * width] = Velocity * (x * cos(alpha) - y * sin(alpha));
			else
				Field[x + y * width] = 0;
		}
	}
}

void SolveFieldDivergence(double Field[], size_t width, size_t height, double speed){
	for (size_t y = 1; y < height - 1; ++y){
		for (size_t x = 1; x < width - 1; ++x){
			if (!(x == 0 || y == 0 || y == height - 1 || x == width - 1))
				Field[x + y * width] += ((Field[x + 1 + y * width] + Field[x - 1 + y * width] + Field[x + (y + 1) * width] + Field[x + (y - 1) * width]) / 4 - Field[x + y * width]) * speed;
		}
	}
}

PointsDA CreatePointDA(void){
	PointsDA Points = {malloc(sizeof(Point) * POINTS_INIT_CAPACITY), POINTS_INIT_CAPACITY, 0};
	return Points;
}

void AddPointToDA(PointsDA *Points, int x, int y){
	Point P = {x, y};
	if (Points->Count >= Points->Capacity){
		Points->Capacity += POINTS_INIT_CAPACITY;
		Points->Points = realloc(Points->Points, Points->Capacity * sizeof(Point));
	}
	Points->Points[Points->Count++] = P;
}

void SolveForNoPenetration(double Field[], PointsDA Points, size_t width, size_t height, float speed){
	for (int i = 0; i < Points.Count; ++i){
		double Avg = 0;
		int x = Points.Points[i].x;
		int y = Points.Points[i].y;


		// X-axis
		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Avg += Field[x + d + y * width];
		}

		Avg /= NO_PENETRATION_DEPTH * 2 + 1;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Field[x + d + y * width] += (Avg - Field[x + d + y * width]) * speed;
		}

		// Y-axis
		Avg = 0;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Avg += Field[x + (d + y) * width];
		}

		Avg /= NO_PENETRATION_DEPTH * 2 + 1;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Field[x + (d + y) * width] += (Avg - Field[x + (d + y) * width]) * speed;
		}


		// Main diagonal axis
		Avg = 0;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Avg += Field[x + d + (d + y) * width];
		}

		Avg /= NO_PENETRATION_DEPTH * 2 + 1;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Field[x + d + (d + y) * width] += (Avg - Field[x + d + (d + y) * width]) * speed;
		}

		// Secondary diagonal axis
		Avg = 0;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Avg += Field[x + d + (y - d) * width];
		}

		Avg /= NO_PENETRATION_DEPTH * 2 + 1;

		for (int d = -NO_PENETRATION_DEPTH; d <= NO_PENETRATION_DEPTH; ++d){
			Field[x + d + (y - d) * width] += (Avg - Field[x + d + (y - d) * width]) * speed;
		}
	}
}