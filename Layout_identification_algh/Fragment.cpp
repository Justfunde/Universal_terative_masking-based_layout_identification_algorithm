#include "Layout_comparator.h"
Fragment::~Fragment()
{
	for (size_t i = 0; i < bitmap_size; i++)
	{
		delete[] bitmap[i];
		bitmap[i] = nullptr;
	}
	delete[] bitmap;
	bitmap = nullptr;
}
