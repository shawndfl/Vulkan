#pragma once

/**
* Dispose things
*/
class IDisposable {
public:
	virtual void dispose() = 0;

protected:
	IDisposable() {}
};