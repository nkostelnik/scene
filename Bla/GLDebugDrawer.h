#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

#include "LinearMath/btIDebugDraw.h"

#include "shader.h"

class Camera;
class Batch;

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:

	GLDebugDrawer();


	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);

	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

	virtual void	drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
	virtual void	drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha);

	virtual void	drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
	
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

	virtual void	reportErrorWarning(const char* warningString);

	virtual void	draw3dText(const btVector3& location,const char* textString);

	virtual void	setDebugMode(int debugMode);

	virtual int		getDebugMode() const { return m_debugMode;}
  
  void init();
  
  inline void set_camera(Camera* camera) { camera_ = camera; };
  
private:
  
  Camera* camera_;
  Batch* batch;
  Shader shader;

};

#endif//GL_DEBUG_DRAWER_H
