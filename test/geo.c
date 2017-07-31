#include "geo.h"
#include "test.h"

#include <math.h>

int test_rotate_line()
{
    const double pi = 3.14159265358979323846;
    const double epsilon = 0.00001;
    {
        line f, g, expected;
        f.a = -1;
        f.b = 0;
        expected.a = 1;
        expected.b = 0;

        g = rotate_line(pi/2, f);
        assert(fabs(g.a - expected.a) < epsilon);
        assert(fabs(g.b - expected.b) < epsilon);
    }
    {
        line f, g, expected;
        f.a = 0;
        f.b = 0;
        expected.a = 0;
        expected.b = 0;

        g = rotate_line(pi, f);
        assert(fabs(g.a - expected.a) < epsilon);
        assert(fabs(g.b - expected.b) < epsilon);
    }
    {
        line f, g, expected;
        f.a = 0.5;
        f.b = 2;
        expected.a = -0.867295;
        expected.b = -2.367920;

        g = rotate_line(5 * pi / 8, f);
        assert(fabs(g.a - expected.a) < epsilon);
        assert(fabs(g.b - expected.b) < epsilon);
    }

    return 0;
}

int test_intersect()
{
    const double epsilon = 0.00001;
    {
        line f, g;
        point p, expected;
        f.a = 2;
        f.b = 1;
        g.a = 1;
        g.b = 0;
        expected.x = -1;
        expected.y = -1;

        p = intersect(f, g);
        assert(fabs(p.x - expected.x) < epsilon);
        assert(fabs(p.y - expected.y) < epsilon);
    }

    {
        line f, g;
        point p, expected;
        f.a = 100;
        f.b = -100;
        g.a = -0.01;
        g.b = 0.01;
        expected.x = 1;
        expected.y = 0;

        p = intersect(f, g);
        assert(fabs(p.x - expected.x) < epsilon);
        assert(fabs(p.y - expected.y) < epsilon);
    }

    return 0;
}

int main()
{
    int result = test_rotate_line();
    result += test_intersect();
    return result;
}

