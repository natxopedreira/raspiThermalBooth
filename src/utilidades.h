#include "ofMain.h"

class utilidades
{
public:
	ofPixels_<unsigned char> dither(const ofPixels_<unsigned char>& pixels,
                                           float threshold = 0.5, // will template
											float quantWeight = 0.125);

	ofPixels_<unsigned char> toGrayscale(const ofPixels_<unsigned char>& pixels);

	void accumulateDitherError(int x,
                                      int y,
                                      ofPixels_<unsigned char>& pixels,
                                      int qError,
                                      float* qErrors,
                                      float quantWeight);
};

