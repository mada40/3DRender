#include "View.h"
#include "MyDraw.h"
#include <iostream>


View::View( sf::Uint8* _pixels,  const sf::Vector3i& _size,  float* _zbuffer) : size{ _size }
{
    pixels = _pixels;
    zbuffer = _zbuffer;
}

void View::render(const std::vector<objl::Loader>& arrLoaders,
    const std::vector<Matrix>& arrMatrix,
    const std::vector<sf::Image>& arrDiffmap, 
    const sf::Vector3f& light, 
    const Matrix& transform)
{


    float k = size.z;

    int index = 0;

    for (objl::Loader loader : arrLoaders)
    {
        Matrix mT = Matrix::inverseTransposeMatrix4x4(arrMatrix[index]);
        Matrix transAndModel = transform * arrMatrix[index];
        for (objl::Mesh mesh : loader.LoadedMeshes)
        {
            for (int i = 0; i < mesh.Indices.size(); i += 3)
            {
                sf::Vector3i sc[3];
                sf::Vector2f tx[3];
                for (int j = 0; j < 3; j++)
                {
                    auto t = mesh.Vertices[mesh.Indices[i + j]];
                    tx[j] = sf::Vector2f(1.f - t.TextureCoordinate.X, 1.f - t.TextureCoordinate.Y);
                    sf::Vector3f temp(t.Position.X, t.Position.Y, t.Position.Z);
                    temp = (transAndModel * Matrix(temp)).getVector();
                    sc[j] = sf::Vector3i(temp.x, temp.y, temp.z);
                }
                auto n1 = mesh.Vertices[mesh.Indices[i + 0]].Normal; 
                auto n2 = mesh.Vertices[mesh.Indices[i + 1]].Normal;
                auto n3 = mesh.Vertices[mesh.Indices[i + 2]].Normal;
                sf::Vector3f t1 = mT * sf::Vector3f(n1.X, n1.Y, n1.Z);
                sf::Vector3f t2 = mT * sf::Vector3f(n2.X, n2.Y, n2.Z);
                sf::Vector3f t3 = mT * sf::Vector3f(n3.X, n3.Y, n3.Z);
                float i0 = dot(normalize(t1), light);
                float i1 = dot(normalize(t2), light);
                float i2 = dot(normalize(t3), light);


                //float intensity = 1.f;

                if (sc[0].z <= 10 * k && sc[0].z > 0.f &&
                    sc[1].z <= 10 * k && sc[1].z > 0.f &&
                    sc[2].z <= 10 * k && sc[2].z > 0.f)
                {
                    drawTriangle(
                        sc[0], sc[1], sc[2],
                        tx[0], tx[1], tx[2],
                        arrDiffmap[index], i0, i1, i2, pixels, size.x, size.y, zbuffer);
                }
            }


        }
        index++;
    }
    


}
