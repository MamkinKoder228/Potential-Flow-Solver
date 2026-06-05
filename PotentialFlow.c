#include <stdlib.h>
#define map(val, m0, M0, m1, M1) (val - m0) / (M0 - m0) * (M1 - m1) + m1
#define BOUNDARY_INIT_CAPACITY 64

typedef struct {
	double x0, y0;
	double x1, y1;
} BoundarySegment;

typedef struct {
	BoundarySegment *Segments;
	size_t SegmentCount;
	size_t Capacity;
} Boundary;


Boundary CreateBoundary();
void BoundaryAddSegment(Boundary *B, double x0, double y0, double x1, double y1);
void DrawBoundary(Boundary *B);
void InitField(double Field[], size_t width, size_t height, double Velocity, double alpha);
void SolveFieldDivergence(double Field[], size_t width, size_t height, double speed);
void SolveForNoPenetration(double Field[], Boundary *B, size_t width, size_t height, float speed);

Boundary CreateBoundary(){
	Boundary B = {malloc(sizeof(BoundarySegment) * BOUNDARY_INIT_CAPACITY), 0, BOUNDARY_INIT_CAPACITY};
	return B;
}

void BoundaryAddSegment(Boundary *B, double x0, double y0, double x1, double y1){
	if (B->SegmentCount >= B->Capacity){
		B->Capacity += BOUNDARY_INIT_CAPACITY;
		B->Segments = realloc(B->Segments, B->Capacity * sizeof(BoundarySegment));
	}
	B->Segments[B->SegmentCount].x0 = x0;
	B->Segments[B->SegmentCount].y0 = y0;
	B->Segments[B->SegmentCount].x1 = x1;
	B->Segments[B->SegmentCount++].y1 = y1;
}

void DrawBoundary(Boundary *B){
	setcolor(GREEN);
	for (int i = 0; i < B->SegmentCount; ++i){
		line(B->Segments[i].x0, B->Segments[i].y0, B->Segments[i].x1, B->Segments[i].y1);
	}
}

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


void SolveForNoPenetration(double Field[], Boundary *B, size_t width, size_t height, float speed){
	double AvgCrossFlow = 0;
	for (int i = 0; i < B->SegmentCount; ++i){
		int x0 = B->Segments[i].x0;
		int y0 = B->Segments[i].y0;
		int x1 = B->Segments[i].x1;
		int y1 = B->Segments[i].y1;

		if (B->Segments[i].x0 == B->Segments[i].x1){
			for (size_t y = fmin(y0, y1); y < fmax(y0, y1); ++y){
				float a = ((Field[x0 + y * width] + Field[x0 - 1 + y * width])/2 - Field[x0 + 1 + y * width]) * speed;
				float b = ((Field[x0 + 1 + y * width] + Field[x0 + y * width])/2 - Field[x0 - 1 + y * width]) * speed;
				Field[x0 + 1 + y * width] += a;
				// Field[x0 + y * width] += ((Field[x0 + 1 + y * width] + Field[x0 - 1 + y * width])/2 - Field[x0 + y * width]) * speed;
				Field[x0 - 1 + y * width] += b;
			}
		}
		else if (B->Segments[i].y0 == B->Segments[i].y1){
			for (size_t x = fmin(x0, x1); x < fmax(x0, x1); ++x){
				float a = ((Field[x + y0 * width] + Field[x + (y0 - 1) * width])/2 - Field[x + (y0 + 1) * width]) * speed;
				float b = ((Field[x + (y0 + 1) * width] + Field[x + y0 * width])/2 - Field[x + (y0 - 1) * width]) * speed;
				Field[x + (y0 + 1) * width] += a;
				// Field[x + y0 * width] += ((Field[x + (y0 + 1) * width] + Field[x + (y0 - 1) * width])/2 - Field[x + y0 * width]) * speed;
				Field[x + (y0 - 1) * width] += b;
			}
		}else{
			int dX = abs(x1 - x0);
			int dY = -abs(y1 - y0);
			int sX = (x1 - x0) < 0? -1:1;
			int sY = (y1 - y0) < 0? -1:1;
			int err = dX + dY;
			int x = x0, y = y0;
			double Nx = -(B->Segments[i].y1 - B->Segments[i].y0);
			double Ny = (B->Segments[i].x1 - B->Segments[i].x0);
			while (x != x1 && y != y1){					
				double dPx = (Field[x + 1 + y * width] - Field[x - 1 + y * width]);
				double dPy = (Field[x + (y - 1) * width] - Field[x + (y + 1) * width]);
				double CrossFlow = dPx * Nx + dPy * Ny;
				double a, b, c, d;
				AvgCrossFlow += fabs(CrossFlow);
				// a = (-(CrossFlow - Field[x + 1 + y * width] * Nx) / Nx - Field[x + 1 + y * width]);
				// b = ((CrossFlow + Field[x - 1 + y * width] * Nx) / Nx - Field[x - 1 + y * width]);
				// c = (-(CrossFlow - Field[x + (y - 1) * width] * Ny) / Ny - Field[x + (y - 1) * width]);
				// d = ((CrossFlow + Field[x + (y - 1) * width] * Ny) / Ny - Field[x + (y + 1) * width]);

				a = -CrossFlow * Nx;
				b = -CrossFlow * Nx;
				c = -CrossFlow * Ny;
				d = -CrossFlow * Ny;

				Field[x + 1 + y * width] += a * speed;
				Field[x - 1 + y * width] += b * speed;
				Field[x + (y - 1) * width] += c * speed;
				Field[x + (y + 1) * width] += d * speed;
				
				if (2 * err >= dY){ 
					err += dY;
					x += sX;
				}
				if (2 * err <= dX){
					err += dX;
					y += sY;
				}
			}
		}
	}
	printf("%.3f\n", AvgCrossFlow);
}