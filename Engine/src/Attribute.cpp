#include <GL/glew.h>
#include "Attribute.h"
#include "GLErrorHandling.h"

Attribute::Attribute() 
{
}

Attribute::Attribute(int attrNumber, int dataType, int componentCount) {
	mAttributeNumber = attrNumber;
	mDataType = dataType;
	mComponentCount = componentCount;
	mNormalized = false;
	mBytesPerVertex = calcBytesPerVertex();
}

Attribute::Attribute(int attrNumber, int dataType, int componentCount, bool normalized) {
	mAttributeNumber = attrNumber;
	mDataType = dataType;
	mComponentCount = componentCount;
	mNormalized = normalized;
	mBytesPerVertex = calcBytesPerVertex();
}

void Attribute::enable(bool enable) {
	if (enable) {
		GLCall(glEnableVertexAttribArray(mAttributeNumber));
	}
	else {
		GLCall(glDisableVertexAttribArray(mAttributeNumber));
	}
}

void Attribute::link(int offset, int stride) {
	GLCall(glVertexAttribPointer(mAttributeNumber, mComponentCount, mDataType, mNormalized, stride, (void*)offset));
}

int Attribute::calcBytesPerVertex() {
	if (mDataType == GL_FLOAT || mDataType == GL_UNSIGNED_INT || mDataType == GL_INT) {
		return 4 * mComponentCount;
	}
	else if (mDataType == GL_SHORT || mDataType == GL_UNSIGNED_SHORT) {
		return 2 * mComponentCount;
	}
	else if (mDataType == GL_BYTE || mDataType == GL_UNSIGNED_BYTE) {
		return 1 * mComponentCount;
	}
	else if (mDataType == GL_UNSIGNED_INT_2_10_10_10_REV) {
		return 4;
	}

	return 0;
}