#include <stdlib.h>
#include <stdio.h>
#include "render.c"
#include "PotentialFlow.c"

#define WIDTH 128
#define HEIGHT 128

int main(void){
	double *Field = malloc(WIDTH * HEIGHT * sizeof(double));

	char string[64];

	InitWindow(WIDTH, HEIGHT, "Potential Flow");
	InitField(Field, WIDTH, HEIGHT, 100, 0/180.0 * M_PI);


	Boundary B = CreateBoundary();
	// BoundaryAddSegment(&B, WIDTH/2 - 15, HEIGHT/2 + 15, WIDTH/2 + 15, HEIGHT/2 - 15);
	float R = 16;
	float dA = 0.2;
	for (float alpha = 0; alpha < 2 * M_PI; alpha += dA){
		BoundaryAddSegment(&B, WIDTH/2 + R * cos(alpha), HEIGHT/2 + R * sin(alpha), WIDTH/2 + R * cos(alpha + dA), HEIGHT/2 + R * sin(alpha + dA));
		// BoundaryAddSegment(&B, WIDTH/2 + R * cos(alpha) - 1, HEIGHT/2 + R * sin(alpha) - 1, WIDTH/2 + R * cos(alpha) - 1, HEIGHT/2 + R * sin(alpha) + 1);
		// BoundaryAddSegment(&B, WIDTH/2 + R * cos(alpha) - 1, HEIGHT/2 + R * sin(alpha) + 1, WIDTH/2 + R * cos(alpha) + 1, HEIGHT/2 + R * sin(alpha) + 1);
		// BoundaryAddSegment(&B, WIDTH/2 + R * cos(alpha) + 1, HEIGHT/2 + R * sin(alpha) + 1, WIDTH/2 + R * cos(alpha) + 1, HEIGHT/2 + R * sin(alpha) - 1);
		// BoundaryAddSegment(&B, WIDTH/2 + R * cos(alpha) + 1, HEIGHT/2 + R * sin(alpha) - 1, WIDTH/2 + R * cos(alpha) - 1, HEIGHT/2 + R * sin(alpha) - 1);
	}

	float vel;
	while (!(xkbhit() && lastkey() == 'q')){
		SolveFieldDivergence(Field, WIDTH, HEIGHT, 1); 
		SolveForNoPenetration(Field, &B, WIDTH, HEIGHT, 1e-1);
		SolveForNoPenetration(Field, &B, WIDTH, HEIGHT, 1e-1);
		DrawVelocityMagnitude(Field, WIDTH, HEIGHT, CMAP_BLR);
		// DrawField(Field, WIDTH, HEIGHT, CMAP_NCV);
		DrawBoundary(&B);
		// DrawIsopotentials(Field, WIDTH, HEIGHT, 32);
		// DrawStreamlines(Field, WIDTH, HEIGHT, 30);
		// DrawVelocityField(Field, WIDTH, HEIGHT);
		// DrawDivergence(Field, WIDTH, HEIGHT, CMAP_BLR);
		// DrawBoundary(Circle);
		if (mouseclick() == SDL_BUTTON_LEFT){
			int x = mousex();
			int y = mousey();
			float dX = (Field[x + 1 + y * WIDTH] - Field[x - 1 + y * WIDTH]) / 2;
			float dY = -(Field[x + (y + 1)  * WIDTH] - Field[x + (y - 1) * WIDTH]) / 2;
			vel = sqrt(dX * dX + dY * dY);
		}

		sprintf(string, "%.3f", vel);
		outtextxy(0, 2, string);
		// delay(50);
		refresh();
		cleardevice();



	}

	return 0;
}