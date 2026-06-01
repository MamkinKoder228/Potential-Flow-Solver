#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "render.c"
#include "PotentialFlow.c"

#define WIDTH 256
#define HEIGHT 256

int main(void){
	double *Field = malloc(WIDTH * HEIGHT * sizeof(double));

	char string[64];

	InitWindow(WIDTH, HEIGHT, "Potential Flow");
	InitField(Field, WIDTH, HEIGHT, 1000, 0/180.0 * M_PI);


	PointsDA Wall = CreatePointDA();

	// for (int y = -32; y <= 32; ++y)
	// 	AddPointToDA(&Wall, WIDTH/2, HEIGHT/2 + y);
		// for (int x = -32; x <= 32; ++x)


	for (int x = -64; x <= 64; ++x){
		for (int y = -64; y <= 64; ++y){
			if (x*x + y*y <= 64){
				AddPointToDA(&Wall, WIDTH/2 + x, HEIGHT/2 + y);
				// AddPointToDA(&Wall, WIDTH/2 + x + 32, HEIGHT/2 + y);
			}
		}
	}

	float vel;
	while (!(xkbhit() && lastkey() == 'q')){
		SolveFieldDivergence(Field, WIDTH, HEIGHT, 1.4); 
		// SolveFieldDivergence(Field, WIDTH, HEIGHT, 1.4); 
		SolveForNoPenetration(Field, Wall, WIDTH, HEIGHT, 1.5);
		DrawField(Field, WIDTH, HEIGHT, CMAP_NCV);
		DrawIsopotentials(Field, WIDTH, HEIGHT, 32);
		// DrawVelocityMagnitude(Field, WIDTH, HEIGHT, CMAP_BLR);
		// DrawStreamlines(Field, WIDTH, HEIGHT, 30);
		// DrawVelocityField(Field, WIDTH, HEIGHT);
		// DrawDivergence(Field, WIDTH, HEIGHT, CMAP_BLR);
		// DrawBoundary(Circle);
		// if (mouseclick() == SDL_BUTTON_LEFT){
		// 	int x = mousex();
		// 	int y = mousey();
		// 	float dX = (Field[x + 1 + y * WIDTH] - Field[x - 1 + y * WIDTH]) / 2;
		// 	float dY = -(Field[x + (y + 1)  * WIDTH] - Field[x + (y - 1) * WIDTH]) / 2;
		// 	vel = sqrt(dX * dX + dY * dY);
		// }

		// sprintf(string, "%.3f", vel);
		// outtextxy(0, 2, string);
		refresh();
		cleardevice();



	}

	return 0;
}