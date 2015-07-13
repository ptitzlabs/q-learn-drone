#include "glhelper.h"

void setupOrtho(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setupPerspective(int width, int height, double fovy, double aspect, double zNear, double zFar) {
	double xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	//glViewport(-width/2, -height/2, width, height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setupTexture(GLuint& texture) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setupTexture(GLuint& texture, SDL_Surface *s) {
	setupTexture(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, s->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, s->pixels);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, s->w, s->h, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
}

void deleteTexture(GLuint& texture) {
	glDeleteTextures(1, &texture);
}

void renderCircle(float x, float y, float rad, int deg, GLfloat r, GLfloat g, GLfloat b) {
	glDisable(GL_TEXTURE_2D);
	glColor4f(r, g, b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i+= deg) glVertex3f(x+cos(i/180.0*M_PI)*rad, y+sin(i/180.0*M_PI)*rad, 0);
	glEnd();
}

void renderSquare(float x0, float y0, float x1, float y1, GLfloat r, GLfloat g, GLfloat b) {
	glDisable(GL_TEXTURE_2D);
	glColor4f(r, g, b, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(x0, y0, 0);
	glVertex3f(x0, y1, 0);
	glVertex3f(x1, y1, 0);
	glVertex3f(x1, y0, 0);
	glEnd();
}

void renderText(const TTF_Font *font, const GLubyte& r, const GLubyte& g, const GLubyte& b, const double& x,  const double& y, const double& z,  const std::string& text) {
	SDL_Color color = {r, g, b};
	//SDL_Surface *Message = TTF_RenderText_Solid(const_cast<TTF_Font*>(font), text.c_str(), color);
	//SDL_Surface *Message = TTF_RenderText_Shaded(const_cast<TTF_Font*>(font), text.c_str(), color);
	SDL_Surface *message = TTF_RenderText_Blended(const_cast<TTF_Font*>(font), text.c_str(), color);
	GLuint texture;
	setupTexture(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, message->w, message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, message->pixels);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(x,              y,              z);
	glTexCoord2f(1, 0); glVertex3f(x + message->w, y,              z);
	glTexCoord2f(1, 1); glVertex3f(x + message->w, y + message->h, z);
	glTexCoord2f(0, 1); glVertex3f(x,              y + message->h, z);
	glEnd();
	glDisable(GL_BLEND);
	deleteTexture(texture);
	SDL_FreeSurface(message);
}

void updateTexture(GLuint texture, SDL_Surface* surface) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
}