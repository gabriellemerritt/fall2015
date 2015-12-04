#include <raytracing/samplers/uniformpixelsampler.h>

UniformPixelSampler::UniformPixelSampler():PixelSampler()
{

}

UniformPixelSampler::UniformPixelSampler(int samples):PixelSampler(samples)
{}

QList<glm::vec2> UniformPixelSampler::GetSamples(int x, int y)
{
    QList<glm::vec2> result;

    for (int i = 0 ; i < this->samples_sqrt; i++)
    {
        for (int j = 0; j <this->samples_sqrt; j++)
        {
           result.append(glm::vec2 ((float)i*(x /samples_sqrt + x),
                                    float(j)*(y/samples_sqrt) + y));
        }
    }
    return result;
}
