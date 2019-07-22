#include "MyForm.h"
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "configs.h"
#include <fstream>
#include <random>
#include <algorithm>
using namespace Project1;
using namespace std;
#define PI 3.14159265359

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm);
	return 0;
}



void update1(int * lscl, int * head, double *coords, double *speed, const int & Np, const double & dt, const int L, const int L2, const double r, const float x0, const float y0, double * OrtsVX, double * OrtsVY, int radius)// works
{
	//Building linked list and turn update the locations according to Vichek's model
	double sx = 0.;
	double sy = 0.;
	double Vx = 0.;
	double Vy = 0.;
	double mod = 0.;
	float noise_action = 0.;
	double distance = 0.;

	for (int i = 0; i < Np; i++) {
		OrtsVX[i] = speed[2 * i + 1] / hypot(speed[2 * i + 1], speed[2 * i]);
		OrtsVY[i] = speed[2 * i] / hypot(speed[2 * i + 1], speed[2 * i]);
	}

	for (int i = 0; i < L2; i++) head[i] = -1;

	for (int i = 0; i < Np; i++) {
		int x1 = (int)((coords[2 * i + 1] + x0) / r);
		int y1 = (int)((coords[2 * i] + y0) / r);
		int c = x1 * L + y1;
		lscl[i] = head[c];
		head[c] = i;
	}


	for (int xc = 0; xc < L; xc++) {
		for (int yc = 0; yc < L; yc++) {
			int c = xc * L + yc;
			for (int xc1 = xc - 1; xc1 <= xc + 1; xc1++) {
				for (int yc1 = yc - 1; yc1 <= yc + 1; yc1++) {
					int c1 = ((xc1 + L) % L) * L + ((yc1 + L) % L);
					int i = (int)head[c];
					while (i != -1) {
						int j = (int)head[c1];
						while (j != -1) {

							distance = sqrt(pow(coords[2 * i + 1] - coords[2 * j + 1], 2) + pow(coords[2 * i] - coords[2 * j], 2));

							if ((distance <= RatioOfIteraction) && (i != j)) {
								sx += OrtsVX[j];
								sy += OrtsVY[j];
							}


							j = (int)lscl[j];
						}
						noise_action = Noise_amp * PI * (1 - 2 * ((double)rand() / (RAND_MAX)));

						if (abs(sx) < 1e-5 && abs(sy) < 1e-5) {
							Vx = OrtsVX[i];
							Vy = OrtsVY[i];
							OrtsVX[i] = Vx * cos(noise_action) - Vy * sin(noise_action);
							OrtsVY[i] = Vx * sin(noise_action) + Vy * cos(noise_action);
						}
						else {
							OrtsVX[i] = (sx * cos(noise_action) - sy * sin(noise_action)) / hypot(sx, sy);
							OrtsVY[i] = (sx * sin(noise_action) + sy * cos(noise_action)) / hypot(sx, sy);
						}
						sx = 0.;
						sy = 0.;
						mod = hypot(speed[2 * i + 1], speed[2 * i]);

						speed[2 * i + 1] = OrtsVX[i] * mod;
						speed[2 * i] = OrtsVY[i] * mod;

						double x1 = coords[2 * i + 1] + speed[2 * i + 1] * dt;
						double y1 = coords[2 * i] + speed[2 * i] * dt;
						

						if (sqrt(pow(x1,2) + pow(y1,2))> (double)(radius)) {
							double SQRT = pow(((coords[2 * i + 1] * speed[2 * i + 1] + coords[2 * i] * speed[2 * i]) / (speed[2 * i + 1] * speed[2 * i + 1] + speed[2 * i] * speed[2 * i])), 2) + (((double)(radius*radius) - coords[2 * i] * coords[2 * i] - coords[2 * i + 1] * coords[2 * i + 1]) / (speed[2 * i + 1] * speed[2 * i + 1] + speed[2 * i] * speed[2 * i]));
							double dt1 = fabs(sqrt(fabs(SQRT)) - ((coords[2 * i + 1] * speed[2 * i + 1] + coords[2 * i] * speed[2 * i]) / (speed[2 * i + 1] * speed[2 * i + 1] + speed[2 * i] * speed[2 * i])));

							double x2 = coords[2 * i + 1] + speed[2 * i + 1] * dt1;
							double y2 = coords[2 * i] + speed[2 * i] * dt1;

							Vx = speed[2 * i + 1];
							Vy = speed[2 * i];

							
							
							double nx = -x2 / radius;
							double ny = -y2 / radius;

							double vn = Vx * nx + Vy * ny;
							double vx1 = -2 * vn * nx + Vx;
							double vy1 = Vy - 2 * ny*vn;

							

							speed[2 * i + 1] = vx1;
							speed[2 * i] = vy1;

							coords[2 * i + 1] = x2 + speed[2 * i + 1] * (dt - dt1);
							coords[2 * i] = y2 + speed[2 * i] * (dt - dt1);

							if (sqrt(pow(coords[2 * i + 1], 2) + pow(coords[2 * i], 2)) > (double)radius) {
								coords[2 * i + 1] = x2;
								coords[2 * i] = y2;
							}

						}
						else {
							coords[2 * i + 1] = x1;
							coords[2 * i] = y1;
						}
						
						i = (int)lscl[i];
					}
				}
			}
		}
	}

}


void Initialization(double* coords, double * speed, int radius, const int mean, double *mod, unsigned char *colors) {
	default_random_engine generator;
	switch(dist)
	{
	case 0:
		{
		normal_distribution<double> nor_distribution(mean, sttdev);
		for (int i = 0; i < Np; i++) { //normal distribution for speed
			mod[i] = nor_distribution(generator);
			}
		break;
		}
	case 1:
		{
		poisson_distribution<int> pois_distribution(mean);
		for (int i = 0; i < Np; i++) { //poisson distribution for speed
			mod[i] = pois_distribution(generator);
			}
		break;
		}
	case 2:
		{
		uniform_real_distribution<double> uni_distribution(0, 2*mean);
		for (int i = 0; i < Np; i++) { //uniform distribution for speed
			mod[i] = uni_distribution(generator);
			}
		}
	}
	double max, min;
	max = -1000;
	min = 1000;
	for (int i = 0; i < Np; i++) {
		if (min > mod[i]) min = mod[i];//values for coloring the particles
		if (max < mod[i]) max = mod[i];
	}
	for (int i = 0; i < Np; i++) {

		double phi = 2. * PI * ((double)(rand()) / RAND_MAX);
		speed[2 * i + 1] = mod[i] * sin(phi);
		speed[2 * i] = mod[i] * cos(phi);

		double t = 2.0 * PI * ((double)rand() / (RAND_MAX));
		double r = radius * sqrt((double)rand() / (RAND_MAX));

		coords[2 * i + 1] = (double)(r * sin(t)); /// x
		coords[2 * i] = (double)(r * cos(t)); /// y

		colors[3 * i] = 255 * ((mod[i]-min)/(max-min)); //the faster particle becomes more red
		colors[3 * i + 1] = 30;
		colors[3 * i + 2] = 255 - colors[3 * i]; // the slower particle becomes more blue

	}
}
void Project1::MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) 
{
	
		ofstream file;
		sf::Image Screenshot;
		sf::ContextSettings set;
		set.antialiasingLevel = 8;
		srand((unsigned int)time(0));
		sf::RenderWindow window(sf::VideoMode(WinW, WinH), "Animation", sf::Style::Default, set);
		//////////////////////////Settings
		sf::CircleShape circle;
		circle.setFillColor(sf::Color::Transparent);
		circle.setPointCount(100);
		circle.setOutlineThickness(2);
		circle.setRadius(radius);
		const float x0 = float(WinW / 2);
		const float y0 = float(WinH / 2);

		/////////////////////////////////circle shapes

		sf::CircleShape *counts = new sf::CircleShape[shapes_number];
		for (int i = 0; i < shapes_number; i++) {
			counts[i].setFillColor(sf::Color::Transparent);
			counts[i].setPointCount(100);
			counts[i].setOutlineThickness(2);
			counts[i].setRadius((radius / shapes_number) * i);
			counts[i].setOrigin((radius / shapes_number) * i, (radius / shapes_number) * i);
			counts[i].setPosition(x0, y0);
		}
		sf::CircleShape kostil;
		kostil.setFillColor(sf::Color::Transparent);
		kostil.setOutlineThickness(0);
		kostil.setOrigin(x0, y0);
		kostil.setPosition(x0, y0);

		/////////////////////////////////Setting for recusrion
		

		/////////////////////////////////

		/////////////////////////////////Enviroment

		double *coords = new double[Np * 2]; //cooridnates of particles
		double *speed = new double[Np * 2]; //speed projection of particles
		double *mod = new double[Np]; //speed module of particles
		double *coordsrend = new double[Np * 2];//coordinates to render
		unsigned char *colors = new unsigned char[Np * 3];//color of particles in RGB

		//settings for Linked List
		double r = (2 * radius) / cell_numb; //size of each cell
		int L = (int)(floor((2 * radius) / r) + 1); //number of cells in one direction
		int L2 = L * L; //number of cells in general
		double *OrtsVX = new double[Np];
		double *OrtsVY = new double[Np];
		int *head = new int[L2];
		int *lscl = new int[Np];

		Initialization(coords, speed, radius, mean, mod, colors);

		

		  //////////////////////////////Configs
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPointSize(3); // size of particles
		glOrtho(0, WinW, WinH, 0, 1, -1);
		////////////////////////////////////////////OpenGl settings
		//sf::Clock clock1;
		double frame_time = double(0.05);
		sf::Event event;
		while (window.isOpen())
		{
			clock_t end = clock();
			while (window.pollEvent(event))
			{

				if (event.type == sf::Event::Closed)
				{
					string text;
					text = "paricles.txt";
					file.open(text);
					for (int i = 1; i <= shapes_number; i++) //Count the number of particles in rings and write in a file
						file << i << " ";
					file << endl;
					int sum, total = 0;
					for (int i = 1; i < shapes_number; i++) {
						sum = 0;
						for (int j = 0; j < Np; j++)
							if ((sqrt(pow(coords[2 * j + 1], 2) + pow(coords[2 * j], 2)) <= ((radius / shapes_number) * i)) && (((sqrt(pow(coords[2 * j + 1], 2) + pow(coords[2 * j], 2)) >= ((radius / shapes_number) * (i - 1))))))
								sum++;
						total += sum;
						file << sum << " ";
					}
					file << Np - total;
					file << endl;
					//Screenshot = window.capture();
					//string picture;
					//picture = "noise = " + to_string(Noise_amp) + ".png";
					//Screenshot.saveToFile(picture);
					file.close();
					window.close();

				}
			}

			glClear(GL_COLOR_BUFFER_BIT);

			update1(lscl, head, coords, speed, Np, frame_time, L, L2, r, x0, y0, OrtsVX, OrtsVY, radius);
			for (int i = 0; i < Np; i++) {
				coordsrend[2 * i + 1] = coords[2 * i + 1] + x0;
				coordsrend[2 * i] = coords[2 * i] + y0;
			}

			glPushMatrix();
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(2, GL_DOUBLE, 0, coordsrend);
			glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);
			glDrawArrays(GL_POINTS, 0, Np);

			//glDisableClientState(GL_VERTEX_ARRAY);
			glPopMatrix();
			glFlush();

			window.draw(circle);
			for (int i = 0; i < shapes_number; i++) {
				window.draw(counts[i]);
			}

			window.draw(kostil);
			window.display();
			
		}
		delete[] coords;
		delete[] coordsrend;
		delete[] speed;
		delete[] head;
		delete[] lscl;
		delete[] colors;
		delete[] OrtsVX;
		delete[] OrtsVY;
		delete[] counts;
		delete[] mod;
		//counter++;
		//if (counter < 1) goto start; //end with iterations
}
	
