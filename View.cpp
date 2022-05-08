#include "View.h"
#include "MyDraw.h"



View::View( sf::Uint8* _pixels,  const sf::Vector3i& _size,  float* _zbuffer) : size{ _size }
{
    pixels = _pixels;
    zbuffer = _zbuffer;
}

void View::render(const std::vector<objl::Loader>& arrLoaders,
    const std::vector<sf::Image>& arrDiffmap, 
    const sf::Vector3f& light, 
    const Matrix& transform )
{
    float k = size.z;
    int index = 0;
    for (objl::Loader loader : arrLoaders)
    {
        for (objl::Mesh mesh : loader.LoadedMeshes)
        {
            for (int i = 0; i < mesh.Indices.size(); i += 3)
            {
                sf::Vector3i sc[3];
                sf::Vector2f tx[3];
                for (int j = 0; j < 3; j++)
                {
                    auto t = mesh.Vertices[mesh.Indices[i + j]];
                    tx[j] = sf::Vector2f(t.TextureCoordinate.X, 1.f - t.TextureCoordinate.Y);
                    sf::Vector3f temp(t.Position.X, t.Position.Y, t.Position.Z);
                    temp = (transform * Matrix(temp)).getVector();
                    sc[j] = sf::Vector3i(temp.x, temp.y, temp.z);
                }
                auto norm = mesh.Vertices[mesh.Indices[i]].Normal;
                sf::Vector3f temp = sf::Vector3f(norm.X, norm.Y, norm.Z);
                normalize(temp);
                float intensity = std::max(0.1f, dot(temp, light));
                //float intensity = 1.f;

                if (sc[0].z <= 2 * k && sc[0].z > 0.f &&
                    sc[1].z <= 2 * k && sc[1].z > 0.f &&
                    sc[2].z <= 2 * k && sc[2].z > 0.f)
                {
                    drawTriangle(
                        sc[0], sc[1], sc[2],
                        tx[0], tx[1], tx[2],
                        arrDiffmap[index], intensity, pixels, size.x, size.y, zbuffer);
                }
            }


        }
        index++;
    }
    


}
