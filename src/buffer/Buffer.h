/*
 * Buffer.h
 *
 *  Created on: 28/09/2013
 *      Author: remnanjona
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <vector>
#include <functional>

namespace std {

class BufferBase {
public:
	virtual ~BufferBase() {}
};

template<class T> class Buffer: public BufferBase {
public:
	GLenum type;
	GLuint location;
	T *data;

	/*
	 * require some function returning how many items T uses
	 */
	function<GLsizeiptr()> sizeFunc;

	Buffer(GLenum t, bool init) {
		type = t;
		glGenBuffers(1, &location);

		if (init) {
			data = new T();
			sizeFunc = []() -> GLsizeiptr { return 1; };
		}
	}

	Buffer(GLenum t, T *initial, function<GLsizeiptr()> f) {
		type = t;
		glGenBuffers(1, &location);
		insert(initial, f);
	}

	Buffer(GLenum t, vector<T> array):
			Buffer(t) {
		insert(array);
	}

	virtual ~Buffer() {
		glDeleteBuffers(1, &location);
	}

	void insert(T *initial, function<GLsizeiptr()> f) {
		data = initial;
		sizeFunc = f;

		// initialise buffer data
		glBindBuffer(type, location);
		glBufferData(type, sizeFunc() * sizeof(T), data, GL_STATIC_DRAW);
		glBindBuffer(type, 0);
	}

	void insert(vector<T> array) {
		insert(array.data(), [array]() -> GLsizeiptr { return array.size(); });
	}

	void update() {
		glBindBuffer(type, location);
		glBufferData(type, sizeFunc() * sizeof(T), data, GL_DYNAMIC_DRAW);
		glBindBuffer(type, 0);
	}

	void bind(GLuint bindingPoint) {
		glBindBufferBase(type, bindingPoint, location); //bindBufferRange...
	}
};

} /* namespace std */
#endif /* BUFFER_H_ */
