#pragma once

class Attribute {

public:	
	Attribute();
	Attribute(int attrNumber, int dataType, int componentCount);
	Attribute(int attrNumber, int dataType, int componentCount, bool normalized);

	void enable(bool enable);
	void link(int offset, int stride);

protected:
	int mAttributeNumber;
	int mDataType;
	bool mNormalized;
	int mComponentCount;
	int mBytesPerVertex;
	

private:
	int calcBytesPerVertex();
};