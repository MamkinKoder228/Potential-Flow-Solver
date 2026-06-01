#include <stdlib.h>
#define BOUNDARY_INIT_CAPACITY 64
#define map(val, m0, M0, m1, M1) (val - m0) / (M0 - m0) * (M1 - m1) + m1

typedef struct {
	double x0, y0, x1, y1;
} BoundaryLineSegment;

typedef struct Boundary {
	size_t SegmentCount;
	size_t Capacity;
	BoundaryLineSegment **Segments;	
} Boundary;

void InitField(double Field[], size_t width, size_t height, double Velocity, double alpha);
void SolveFieldDivergence(double Field[], size_t width, size_t height, double speed);
Boundary *CreateBoundary(void);
void AddSegmentToBoundary(Boundary *B, double x0, double y0, double x1, double y1);
void SolveForNoPenetration(double Field[], size_t width, size_t height, Boundary *B, float speed);
void DrawBoundary(Boundary *B);

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

Boundary *CreateBoundary(void){
	Boundary *B = malloc(sizeof(Boundary)); 
	B->SegmentCount = 0;
	B->Capacity = BOUNDARY_INIT_CAPACITY; 
	B->Segments = malloc(sizeof(BoundaryLineSegment*) * BOUNDARY_INIT_CAPACITY);
	return B;
}

void AddSegmentToBoundary(Boundary *B, double x0, double y0, double x1, double y1){
	if (B->SegmentCount >= B->Capacity){
		B->Segments = realloc(B->Segments, sizeof(BoundaryLineSegment*) * (B->Capacity + BOUNDARY_INIT_CAPACITY));
		B->Capacity += BOUNDARY_INIT_CAPACITY;
	}

	B->Segments[B->SegmentCount] = malloc(sizeof(BoundaryLineSegment));
	B->Segments[B->SegmentCount]->x0 = x0;
	B->Segments[B->SegmentCount]->y0 = y0;
	B->Segments[B->SegmentCount]->x1 = x1;
	B->Segments[B->SegmentCount++]->y1 = y1;
}

void SolveForNoPenetration(double Field[], size_t width, size_t height, Boundary *B, float speed){
	for (int i = 0; i < B->SegmentCount; ++i){
		int x0, y0, x1, y1;
		x0 = B->Segments[i]->x0;
		y0 = B->Segments[i]->y0;
		x1 = B->Segments[i]->x1;
		y1 = B->Segments[i]->y1;

		if (x0 == x1){
			for (int y = fmin(y0, y1); y < fmax(y0, y1); ++y){
				double Avg = 0;
				for (int d = -2; d <= 2; ++d)
					Avg += Field[x0 + d + y * width];
				Avg /= 2 - (-2) + 1; 
				for (int d = -2; d <= 2; ++d)
					Field[x0 + d + y * width] += (Avg - Field[x0 + d + y * width]) * speed;
			}
		}else if (y0 == y1){
			for (int x = fmin(x0, x1); x < fmax(x0, x1); ++x){
				double Avg = 0;
				for (int d = -2; d <= 2; ++d)
					Avg += Field[x + (y0 + d) * width];
				Avg /= 2 - (-2) + 1; 
				for (int d = -2; d <= 2; ++d)
					Field[x + (d + y0) * width] += (Avg - Field[x + (d + y0) * width]) * speed;
			}
		}else{
			double alpha = atan2(x1 - x0, y1 - y0);
			for (int x = fmin(x0, x1); x < fmax(x0, x1); ++x){
				int y = map(x, x0, x1, y0, y1);
				double Avg = 0;

				for (int d = -3; d <= 3; ++d)
					Avg += Field[(int)(x + d * cos(alpha)) + ((int)(y - d * sin(alpha))) * width];
				Avg /= 3 - (-3) + 1;
				for (int d = -3; d <= 3; ++d)
					Field[(int)(x + d * cos(alpha)) + ((int)(y - d * sin(alpha))) * width] += (Avg - Field[(int)(x + d * cos(alpha)) + ((int)(y - d * sin(alpha))) * width]) * speed;
			}
		}
	}
}

void DrawBoundary(Boundary *B){
	for (int i = 0; i < B->SegmentCount; ++i){
		line(B->Segments[i]->x0, B->Segments[i]->y0, B->Segments[i]->x1, B->Segments[i]->y1);
	}
}