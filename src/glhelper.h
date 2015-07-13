#ifndef GLHELPER_H
#define GLHELPER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include <string>

void setupOrtho(int width, int height);
void setupPerspective(int width, int height, double fovy, double aspect, double zNear, double zFar);

void setupTexture(GLuint& texture);
void setupTexture(GLuint& texture, SDL_Surface *s);
void deleteTexture(GLuint& texture);

void renderCircle(float x, float y, float rad, int deg, GLfloat r, GLfloat g, GLfloat b);

void renderSquare(float x0, float y0, float x1, float y1, GLfloat r, GLfloat g, GLfloat b);

void renderText(const TTF_Font *font, const GLubyte& r, const GLubyte& g, const GLubyte& b, const double& x,  const double& y, const double& z,  const std::string& text);

void updateTexture(GLuint texture, SDL_Surface* surface);

#endif
