#include "MyForm.h"
#include <Windows.h>
#include <string>
int Np = 20000; // number of particles
const int WinH = 1000;
const int WinW = 1000; //window size
int radius = WinH / 2; // radius of circle, can be set manually
int mean = 30; //-x.....+x 
double RatioOfIteraction = 10;
double Noise_amp = 0.01;
const int cell_numb = 200; //number of cells in each direction
int shapes_number = 11;
int dist = 0; //0 is for normal distirbution, 1 is for poisson one, 2 is for uniform one
double sttdev = 1; // standart deviation for normal distribution

void Project1::MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {
	Np = int::Parse(Project1::MyForm::textBox1 -> Text); // number of particles
	sttdev = double::Parse(Project1::MyForm::textBox4 -> Text); // standart deviation for normal distribution
	mean = int::Parse(Project1::MyForm::textBox8 -> Text); //-x.....+x 
	RatioOfIteraction = double::Parse(Project1::MyForm::textBox7 -> Text);
	Noise_amp = double::Parse(Project1::MyForm::textBox6 -> Text);
	shapes_number = int::Parse(Project1::MyForm::textBox9->Text) +1; //Number of shapes
	if (Project1::MyForm::radioButton1->Checked) dist = 0; //normal
	if (Project1::MyForm::radioButton2->Checked) dist = 1; //poisson
	if (Project1::MyForm::radioButton3->Checked) dist = 2; //uniform
}