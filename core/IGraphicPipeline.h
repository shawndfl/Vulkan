#pragma once

/**
* This is an interface for a graphic pipeline. A graphic pipline 
* encapulates a shader, vertex, and index buffer. It will also render
* what ever needs to be rendered.
*/
class IGraphicPipeline {
private :

protected:
	IGraphicPipeline() { }

public: 
	virtual void update(float ms) = 0;
};

