#include"MyDraw.h"


void clear(sf::Uint8*& pixels, float* zbuffer, int size)
{
    std::fill(zbuffer, zbuffer + size, -1000000.f);
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
    const sf::Vector2f& tx0,
    const sf::Vector2f& tx1,
    const sf::Vector2f& tx2,
    const sf::Image& diffmap,
    float intensity0,
    float intensity1,
    float intensity2,
    sf::Uint8*& pixels, int W, int H, float* zbuffer)
{

    const sf::Uint8* diffArr = diffmap.getPixelsPtr();
    const int diffW = diffmap.getSize().x;
    const int diffH = diffmap.getSize().y;

    sf::Vector3i tt0 = t0;
    sf::Vector3i tt1 = t1;
    sf::Vector3i tt2 = t2;

    sf::Vector2i ttx0(tx0.x * diffW, tx0.y * diffH);
    sf::Vector2i ttx1(tx1.x * diffW, tx1.y * diffH);
    sf::Vector2i ttx2(tx2.x * diffW, tx2.y * diffH);

    //сортируем по убыванию
    if (tt0.y < tt1.y) { std::swap(tt0, tt1); std::swap(ttx0, ttx1); std::swap(intensity0, intensity1); }
    if (tt0.y < tt2.y) { std::swap(tt0, tt2); std::swap(ttx0, ttx2); std::swap(intensity0, intensity2); }
    if (tt1.y < tt2.y) { std::swap(tt1, tt2); std::swap(ttx1, ttx2); std::swap(intensity1, intensity2); }

    const sf::Vector3i v1 = tt1 - tt0;
    const sf::Vector3i v2 = tt2 - tt0;
    const int A = v1.x * v2.y - v1.y * v2.x;

    const sf::Vector2i uv1 = ttx1 - ttx0;
    const sf::Vector2i uv2 = ttx2 - ttx0;

    const float dInten1 = intensity1 - intensity0;
    const float dInten2 = intensity2 - intensity0;

    
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


            if (nwY < 0 || nwY >= H) continue;//нет смысла обрабатывать пиксели за пределами экрана
            for (int i = m; i <= M; i++) {

                //интерполяция интенсивности освещения, z координат и текстурных координат
                float B = ((i - tt0.x) * v2.y - (nwY - tt0.y) * v2.x);
                float C = -((i - tt0.x) * v1.y - (nwY - tt0.y) * v1.x);

                float z = tt0.z;
                int uvX = ttx0.x;
                int uvY = ttx0.y;
                float intensity = intensity0;
                if (A != 0)
                {
                    z += (B * v1.z + C * v2.z) / A;
                    uvX += (B * uv1.x + C * uv2.x) / A;
                    uvY += (B * uv1.y + C * uv2.y) / A;
                    intensity += (B * dInten1 + C * dInten2) / A;
                }
                intensity = std::max(intensity, .1f);

                if(intensity > 0.99999f) continue;
                if (uvY < 0 || uvX < 0  || uvY >=  diffH || uvX >= diffW) continue;
                
                int coor = (nwY * W + i);

                if (z > zbuffer[coor])//если текущий пиксель ближе к камере
                {
                    zbuffer[coor] = z;
   
                    const sf::Uint8* pixel = diffArr + (uvX + uvY * diffW) * 4;
                    int newCoor = coor * 4;
                    //закрашиваем линию

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
        if (zbuffer[i] >= 0)
        {
            deep = (zbuffer[i]) / d;
            deep *= 300.f;
        }
        setColor(i, pixels, sf::Color(deep, deep, deep));

    }
}
