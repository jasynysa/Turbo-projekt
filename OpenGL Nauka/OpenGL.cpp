// OpenGL Nauka.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "include\SDL.h"
#include "include\glew.h"
#include "include\glm\glm.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include "Transformacja.h"
#include "include/glm/gtc/type_ptr.hpp"
#include "Kamera.h"


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* okno = SDL_CreateWindow("borys", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);//ustawienia dla kontekstu OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GLContext kontekst = SDL_GL_CreateContext(okno);//tworzenie kontekstu (cokolwiek to znaczy) OpenGL 

	glewExperimental = GL_TRUE;
	glewInit();


	//----------------------------------------------------
	///talice z danymi 
	//----------------------------------------------------
	float tablicaWierzcholkow[] = {
	-0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
	 0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.5f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.5f,
	-0.5f,  0.5f, -0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f, 0.5f, 0.5f,

	-0.5f, -0.5f,  0.5f,  1.0f, 0.5f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.5f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.5f,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.5f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.5f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.5f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.4f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.1f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.6f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.4f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.7f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.5f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.7f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.7f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.3f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.4f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.3f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.2f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.6f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.5f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.3f,
	-0.5f,  0.5f,  0.5f,  0.2f, 0.6f,
	-0.5f,  0.5f, -0.5f,  0.4f, 1.0f
	};



	///tworzenie vertex array object
	GLuint VAO[2];	
	glGenVertexArrays(2, VAO);
	


	///tworzenie buforu z wierzcholkoami (vertex buffor object VBO)
	GLuint VBO;
	glGenBuffers(1, &VBO);//tworzy tylko numer(nazwe) dla buforu, jeszcze nie tworzy żadnego bufora nazwa bez sesu? 
	
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//wiazanie z aktualny kontekstem
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 3*2 *6, tablicaWierzcholkow, GL_STATIC_DRAW);
	

	//----------------------------------------------------	
	///tworzenie vertexShader i fragmentShader(albo i nie), oraz programu
	//----------------------------------------------------
	//tworzenie programu
	GLuint idProgram = glCreateProgram();
	//kod zrodlowy programu jakis ciekawy sposob na c stringa c++11
	const char* vsSource = R"glsl(
    #version 150 core

    in vec3 wspolrzedne;
	in vec2 kolor;
	
	out vec2 Kolor;

	uniform mat4 macierz;

    void main()
    {
		Kolor = kolor;
        gl_Position = macierz * vec4(wspolrzedne, 1.0);
    }
)glsl";

	const char* fsSource = R"glsl(
    #version 150 core

	in vec2 Kolor;

    out vec4 outColor;

	uniform vec3 kolorObiektu;
	uniform vec3 kolorSwiatla;

    void main()
    {
       outColor = vec4(Kolor,1.0, 1.0);
    }
)glsl";

	//tworzenie shaderow
	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//wczytywanie kodo źrodlowego
	glShaderSource(vShader, 1, &vsSource, NULL);
	glShaderSource(fShader, 1, &fsSource, NULL);
	//kompilacja
	glCompileShader(vShader);
	glCompileShader(fShader);
	//dodawanie shaderow do programu i linkowanie
	glAttachShader(idProgram, vShader);
	glAttachShader(idProgram, fShader);
	glLinkProgram(idProgram);
	char informacja[512];
	glGetShaderInfoLog(fShader, 512, NULL, informacja);
	
	glUseProgram(idProgram);
	int status;
	glGetProgramiv(idProgram, GL_LINK_STATUS, &status);

	std::cout << std::endl << "SHADER::INFO:: " << informacja<<"PROGRAM::STATUS:: "<<status;


	//----------------------------------------------------
	///informowanie (właściwie to co ? Shader? Program?) o tym w jaki sposob odczytywac strumien danych z bufora
	//----------------------------------------------------
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	GLuint IDAtrybutuWspolrzedne = glGetAttribLocation(idProgram, "wspolrzedne");
	glVertexAttribPointer(IDAtrybutuWspolrzedne, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
	//aktywowanie atrybutu(cokolwiek to zanczy)
	glEnableVertexAttribArray(IDAtrybutuWspolrzedne);

	GLuint IDAtrybutuKolor = glGetAttribLocation(idProgram, "kolor");
	glVertexAttribPointer(IDAtrybutuKolor, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));
	//aktywowanie atrybutu(cokolwiek to zanczy)
	glEnableVertexAttribArray(IDAtrybutuKolor);


	//----------------------------------------------------
	///dodatkowe ustawienia
	//----------------------------------------------------
	//tworzenie macierzy transformacji
	glm::mat4 macierzTransformacji(1);
	//macierzTransformacji = glm::mat4(1)* tran::obrotY(0.7) *tran::widok(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	float kolorObiektu[] = { 1,0.5,0 };
	float kolorSwiatla[] = { 1,1,1 };
	//uzyskiwanie uchwytów/id uniformów
	GLuint idUniMacierz,
		idUniKolorObiektu,
		idUniKolorSwiatla;
	idUniMacierz = glGetUniformLocation(idProgram, "macierz");
	idUniKolorObiektu = glGetUniformLocation(idProgram, "kolorObiektu");
	idUniKolorSwiatla = glGetUniformLocation(idProgram, "kolorSwiatla");

	//nadawanie wartości uniformą
	glUniformMatrix4fv(idUniMacierz, 1, GL_FALSE, glm::value_ptr(macierzTransformacji));

	glUniform3fv(idUniKolorObiektu, 1, kolorObiektu);
	glUniform3fv(idUniKolorSwiatla, 1, kolorSwiatla);

	Kamera kamera(glm::fvec3(0,0,4.4),glm::fvec3(0,1,0));

	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
	glLineWidth(10);
	bool czyOtwarte=true;
	while(czyOtwarte)//glowna petla
	{
		static float i;
		static glm::mat4 przesuniecie(1);
		przesuniecie[3] = glm::fvec4(0,0,0.51,1);// 
		macierzTransformacji = tran::perspektywaUkosna(glm::fvec3(cos(i/100), sin(i/100), 2),100)* przesuniecie;
		glUniformMatrix4fv(idUniMacierz, 1, GL_FALSE, glm::value_ptr(macierzTransformacji));

		glBindVertexArray(VAO[0]);
		SDL_Event wydarzenie;
		SDL_PollEvent(&wydarzenie);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		SDL_GL_SwapWindow(okno);
		czyOtwarte = (wydarzenie.type == SDL_QUIT) ? false : true;
		i++;
	}

	SDL_GL_DeleteContext(kontekst);//zamykanie kontekstu
	SDL_Quit();
    return 0;
}

void funkcja(int &borys)
{

}