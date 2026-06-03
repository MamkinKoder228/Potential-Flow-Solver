#include <BGI/graphics.c>
#include <stdlib.h>
#include "colormaps.c"

void InitWindow(int width, int height, char *title);
void DrawField(double Field[], size_t width, size_t height, short colormap);
void DrawVelocityMagnitude(double Field[], size_t width, size_t height, short colormap);
void DrawVelocityField(double Field[], size_t width, size_t height);
void DrawLine(int x, int y, float alpha, float l);
void DrawIsopotentials(double Field[], size_t width, size_t height, int count);
void DrawDivergence(double Field[], size_t width, size_t height, short colormap);

void InitWindow(int width, int height, char *title){
	initwindow(width, height, title);
	sdlbgifast();
}

void DrawField(double Field[], size_t width, size_t height, short colormap){
	double ValMin = 1e99, ValMax = -1e99;

	for (size_t y = 0; y < height; ++y){
		for (size_t x = 0; x < width; ++x){
			ValMin = fmin(Field[x + y * width], ValMin);
			ValMax = fmax(Field[x + y * width], ValMax);
		}
	}

	for (size_t y = 0; y < height; ++y){
		for (size_t x = 0; x < width; ++x){
			double norm = (Field[x + y * width] - ValMin) / (ValMax - ValMin);
			putpixel(x, y, ValToColor(norm, colormap));
		}
	}
}

void DrawVelocityMagnitude(double Field[], size_t width, size_t height, short colormap){
	float vel;
	double ValMin = 1e99, ValMax = -1e99;

	for (size_t y = 1; y < height - 1; ++y){
		for (size_t x = 1; x < width - 1; ++x){
			float dX = (Field[x + 1 + y * width] - Field[x - 1 + y * width]) / 2;
			float dY = -(Field[x + (y + 1)  * width] - Field[x + (y - 1) * width]) / 2;
			vel = sqrt(dX * dX + dY * dY);
			ValMin = fmin(vel, ValMin);
			ValMax = fmax(vel, ValMax);
		}
	}

	for (size_t y = 1; y < height - 1; ++y){
		for (size_t x = 1; x < width - 1; ++x){
			float dX = (Field[x + 1 + y * width] - Field[x - 1 + y * width]) / 2;
			float dY = -(Field[x + (y + 1)  * width] - Field[x + (y - 1) * width]) / 2;
			vel = sqrt(dX * dX + dY * dY);
			putpixel(x, y, ValToColor((vel - ValMin) / (ValMax - ValMin), colormap));
		}
	}
}

void DrawVelocityField(double Field[], size_t width, size_t height){
	setcolor(WHITE);
	for (size_t y = 1; y < height - 1; y += 10){
		for (size_t x = 1; x < width - 1; x += 10){
			float dX = (Field[x + 1 + y * width] - Field[x - 1 + y * width]);
			float dY = -(Field[x + (y + 1)  * width] - Field[x + (y - 1) * width]);
			DrawLine(x, y, atan2f(dY, dX), 10);
		}
	}
}

void DrawLine(int x, int y, float alpha, float l){
	int X0 = x - cos(alpha) * l/2;
	int Y0 = y + sin(alpha) * l/2;

	int X1 = x + cos(alpha) * l/2;
	int Y1 = y - sin(alpha) * l/2;

	line(X0, Y0, X1, Y1);
	putpixel(X1, Y1, GREEN);
}

void DrawIsopotentials(double Field[], size_t width, size_t height, int count){
	double ValMin = 1e99, ValMax = -1e99;

	for (size_t y = 0; y < height; ++y){
		for (size_t x = 0; x < width; ++x){
			ValMin = fmin(Field[x + y * width], ValMin);
			ValMax = fmax(Field[x + y * width], ValMax);
		}
	}

	for (double	C = ValMin; C < ValMax; C += (ValMax - ValMin) / (double)count){
		for (size_t y = 0; y < height; y += 1){
			for (size_t x = 0; x < width; x += 1){
				if (fabs(Field[x + y * width]/C - 1) < 0.01)
					putpixel(x, y, WHITE);
			}
		}
	}
}	

void DrawDivergence(double Field[], size_t width, size_t height, short colormap){
	double ValMin = 1e99, ValMax = -1e99;
	double divergence;

	for (size_t y = 1; y < height - 1; ++y){
		for (size_t x = 1; x < width - 1; ++x){
			divergence = (Field[x - 1 + y * width] + Field[x + 1 + y * width] + Field[x + (1 + y) * width] + Field[x + (y - 1) * width] - 4 * Field[x + y * width]);
			ValMin = fmin(divergence, ValMin);
			ValMax = fmax(divergence, ValMax);
		}
	}
	
	for (size_t y = 1; y < height - 1; ++y){
		for (size_t x = 1; x < width - 1; ++x){
			divergence = (Field[x - 1 + y * width] + Field[x + 1 + y * width] + Field[x + (1 + y) * width] + Field[x + (y - 1) * width] - 4 * Field[x + y * width]);
			putpixel(x, y, ValToColor((divergence - ValMin) / (ValMax - ValMin), colormap));
		}
	}
}