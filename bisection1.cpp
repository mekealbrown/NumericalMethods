double find_root(double(*fp)(double), double(*df)(double), double a, double b)
{
    double c{(a + b) * 0.5};
    int iterations{0};
    while (iterations++ < 100)
    {
        double fc{fp(c)};
        if (fc == 0.0) //found the root, nice
            break;
        fc * fp(a) > 0 ? a = c : b = c; //check sign, assign accordingly
        c = (a + b) * 0.5;
    }
    return c;
}

