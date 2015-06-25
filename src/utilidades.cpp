#include "utilidades.h"


ofPixels_<unsigned char> utilidades::dither(const ofPixels_<unsigned char>& pixels,
                                            float threshold,
                                            float quantWeight)
{

    // Special thanks to @julapy / ofxDither

    ofPixels_<unsigned char> pixelsIn = pixels;

    // ensure the image is grayscale
    if(OF_IMAGE_GRAYSCALE != pixelsIn.getImageType())
    {
        pixelsIn = toGrayscale(pixels);
    }

    // make a copy
    ofPixels_<unsigned char> pixelsOut =  pixelsIn;

    // set up the quantization error
    int width  = pixelsOut.getWidth();
    int height = pixelsOut.getHeight();

    std::size_t numPixels = width * height; // 1 byte / pixel

    float qErrors[numPixels];
    std::fill(qErrors, qErrors + numPixels, 0.0);

    //unsigned char* inPix  = pixelsIn.getPixels();
    unsigned char* outPix = pixelsOut.getPixels();

    float limit = ofColor_<unsigned char>::limit();

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            int p = pixelsIn.getPixelIndex(x, y);

            int oldPx = outPix[p] + qErrors[p]; // add error
            int newPx = (oldPx < (threshold * limit)) ? 0 : limit;  // threshold

            outPix[p] = newPx;

            int qError = oldPx - newPx;

            accumulateDitherError(x+1,y  ,pixelsOut,qError,qErrors,quantWeight); // check east
            accumulateDitherError(x+2,y  ,pixelsOut,qError,qErrors,quantWeight); // check east east
            accumulateDitherError(x-1,y+1,pixelsOut,qError,qErrors,quantWeight); // check southwest
            accumulateDitherError(x  ,y+1,pixelsOut,qError,qErrors,quantWeight); // check south
            accumulateDitherError(x+1,y+1,pixelsOut,qError,qErrors,quantWeight); // check southeast
            accumulateDitherError(x  ,y+2,pixelsOut,qError,qErrors,quantWeight); // check south south
        }
    }

    return pixelsOut;
}

ofPixels_<unsigned char> utilidades::toGrayscale(const ofPixels_<unsigned char>& pixels){

    ofPixels pix;

    pix.allocate(pixels.getWidth(), pixels.getHeight(), OF_IMAGE_GRAYSCALE);

    for(std::size_t x = 0; x < pixels.getWidth(); ++x)
    {
        for(std::size_t y = 0; y < pixels.getHeight(); ++y)
        {
            ofColor_<unsigned char> c = pixels.getColor(x, y);
            pix.setColor(x, y, 0.21 * c.r + 0.71 * c.g + 0.07 * c.b);
        }
    }

    return pix;
    
}

void utilidades::accumulateDitherError(int x,
                                      int y,
                                      ofPixels_<unsigned char>& pixels,
                                      int qError,
                                      float* qErrors,
                                      float quantWeight)
    {
        if (x >= 0 && x < pixels.getWidth() && y >= 0 && y < pixels.getHeight())
        {
            qErrors[pixels.getPixelIndex(x, y)] += quantWeight * qError;
        }
    }