#include"MyDraw.h"

void F() { T5 += 2; }
void setColor2(unsigned int coor, sf::Uint8*& pixels, const sf::Color& color)
{
    int newCoor = coor * 4;
    pixels[newCoor] = color.r;
    pixels[newCoor + 1] = color.g;
    pixels[newCoor + 2] = color.b;
    pixels[newCoor + 3] = color.a;
}

void clear(sf::RenderWindow& window, sf::Uint8*& pixels, float* zbuffer, int size)
{
    window.clear();
    std::fill(zbuffer, zbuffer + size, -10000000.f);
    std::fill(pixels, pixels + size * 4, 0);
}

void drawLine(const sf::Vector2i& t0, const sf::Vector2i& t1, sf::Uint8*& pixels, int W, int H, const sf::Color& color)
{
    int x1 = t0.x;
    int y1 = t0.y;

    int x2 = t1.x;
    int y2 = t1.y;
    if (x1 < 0 || x1 >= W || y1 < 0 || y1 >= H) throw "you lox";
    if (x2 < 0 || x2 >= W || y2 < 0 || y2 >= H) throw "you lox";

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    setColor(y2 * W + x2, pixels, color);
    while (x1 != x2 || y1 != y2)
    {
        setColor(y1 * W + x1, pixels, color);
        int error2 = error + error;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}

void drawTriangle(
    const sf::Vector3i& t0,
    const sf::Vector3i& t1,
    const sf::Vector3i& t2,
    const sf::Vector2i& tx0,
    const sf::Vector2i& tx1,
    const sf::Vector2i& tx2,
    const sf::Image& diffmap,
    const sf::Image& normal,
    const sf::Vector3f& light,
    sf::Uint8*& pixels, int W, int H, float* zbuffer)
{

    const sf::Uint8* diffArr = diffmap.getPixelsPtr();
    const sf::Uint8* normArr = normal.getPixelsPtr();
    const int diffW = diffmap.getSize().x;

    sf::Vector3i tt0 = t0;
    sf::Vector3i tt1 = t1;
    sf::Vector3i tt2 = t2;

    sf::Vector2i ttx0 = tx0;
    sf::Vector2i ttx1 = tx1;
    sf::Vector2i ttx2 = tx2;

    //сортируем по убыванию
    if (tt0.y < tt1.y) { std::swap(tt0, tt1); std::swap(ttx0, ttx1); }
    if (tt0.y < tt2.y) { std::swap(tt0, tt2); std::swap(ttx0, ttx2); }
    if (tt1.y < tt2.y) { std::swap(tt1, tt2); std::swap(ttx1, ttx2); }


    sf::Vector3i v1 = tt1 - tt0;
    sf::Vector3i v2 = tt2 - tt0;

    sf::Vector2i uv1 = ttx1 - ttx0;
    sf::Vector2i uv2 = ttx2 - ttx0;

    const int A = v1.x * v2.y - v1.y * v2.x;
    const int txA = uv1.x * uv2.y - uv1.y * uv2.x;
    int nwY = tt2.y;
    int nowX3[2] = { tt2.x, tt2.x };

    const sf::Vector3i v[3] = { tt0, tt1, tt2 };

    for (int j = 0; j < 2; j++) { //рисуем верхнюю часть треугольника(3-2, 3-1), потом нижнию (2-1, 3-1)


        int deltaX[2] = { abs(v[2 - j].x - v[1 - j].x), abs(tt2.x - tt0.x) };
        int deltaY[2] = { abs(v[2 - j].y - v[1 - j].y), abs(tt2.y - tt0.y) };
        int signX[2] = { v[2 - j].x < v[1 - j].x ? 1 : -1, tt2.x < tt0.x ? 1 : -1 };
        int error[2] = { deltaX[0] - deltaY[0], deltaX[1] - deltaY[1] };
        nowX3[0] = v[2 - j].x;

        int h = v[1 - j].y - v[2 - j].y;


        for (int q = 0; q < h; q++) { // идем посторочно в треугольнике
            for (int i = 0; i < 2; i++) {// рассматриваем противоложные стороны в треугольнике
                while (true) {
                    int error2 = error[i] + error[i];
                    if (error2 < deltaX[i]) {
                        error[i] += deltaX[i];
                        nwY += i;//в итоге прибовляем только единицу
                        break;
                    }
                    if (error2 > -deltaY[i]) {
                        error[i] -= deltaY[i];
                        nowX3[i] += signX[i];
                    }
                }
            }

            bool firstIsMin = nowX3[0] < nowX3[1];
            int m = nowX3[!firstIsMin]; // min
            int M = nowX3[firstIsMin]; // max

            if (m < 0) m = 0;
            if (M >= W) M = W - 1;


            if (nwY < 0 || nwY >= H) continue;
            for (int i = m; i <= M; i++) {

                float B = ((i - tt0.x) * v2.y - (nwY - tt0.y) * v2.x);
                float C = -((i - tt0.x) * v1.y - (nwY - tt0.y) * v1.x);

                float z = tt0.z;
                int uvX = ttx0.x;
                int uvY = ttx0.y;
                if (A != 0)
                {
                    z += (B * v1.z + C * v2.z) / A;
                    uvX += (B * uv1.x + C * uv2.x) / A;
                    uvY += (B * uv1.y + C * uv2.y) / A;
                    
                }

                float intensity = 1.f;
                
                int coor = (nwY * W + i);
                if (z > zbuffer[coor])
                {
                    if (uvY < 0 || uvX < 0  || uvY > 2047 || uvX > 2047) continue;
                    zbuffer[coor] = z;
   
                    const sf::Uint8* pixel = diffArr + (uvX + uvY * diffW) * 4;
                    const sf::Uint8* pixel1 = normArr + (uvX + uvY * diffW) * 4;
                    int newCoor = coor * 4;
                    //закрашиваем линию

                    float normX = (pixel1[0] / 255.f - 0.5f) * 2.f;
                    float normY = (pixel1[1] / 255.f - 0.5f) * 2.f;
                    float normZ = (pixel1[2] / 255.f - 0.5f) * 2.f;
                    float len = std::sqrt(normX * normX + normY * normY + normZ * normZ);
                    sf::Vector3f norm(normX / len, normY / len, normZ / len);


                    intensity = norm.x * light.x + norm.y * light.y + norm.z * light.z;
                    intensity = std::max(intensity, 0.1f);
                    // = std::min(intensity, 1.f);
                     
                    pixels[newCoor] = 255 * intensity;//r
                    pixels[newCoor + 1] = 255 * intensity;//g
                    pixels[newCoor + 2] = 255 * intensity;//b
                    pixels[newCoor + 3] = 255;//a

                    pixels[newCoor] = pixel[0] * intensity;//r
                    pixels[newCoor + 1] = pixel[1] * intensity;//g
                    pixels[newCoor + 2] = pixel[2] * intensity;//b
                    pixels[newCoor + 3] = pixel[3];//a


                }
            }

        }


    }


}




void drawZbuffer(sf::Uint8* pixels, int W, int H, const float* zbuffer, int d)
{
    for (int i = 0; i < W * H; i++)
    {
        float deep = 0;
        //std::cout << (zbuffer[i] > 0.f) << "\n";
        if (zbuffer[i] >= -d/ 2.f)
        {
            deep = zbuffer[i] + d/ 2;
            deep = deep / d * 127;
        }
        setColor(i, pixels, sf::Color(deep, deep, deep));

    }
}
