#include "randompixelsampler.h"

#include <raytracing/samplers/stratifiedpixelsampler.h>
#include <iostream>
#include <functional>
#include <time.h>       /* time */

RandomPixelSampler::RandomPixelSampler():PixelSampler(){}

RandomPixelSampler::RandomPixelSampler(int samples) : PixelSampler(samples), mersenne_generator(), unif_distribution(0,1){}

QList<glm::vec2> RandomPixelSampler::GetSamples(int x, int y)
{
    QList<glm::vec2> result;
    for ( int i = 0; i < samples_sqrt*samples_sqrt; i++)
    {
        srand (time(NULL));
        float ran = (float) (rand() %samples_sqrt) ;
        result.append(glm::vec2 ((float)ran*(x /samples_sqrt + x),
                                 float(ran)*(y/samples_sqrt) + y));

    }

    return result;
}
