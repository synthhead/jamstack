class SoundFilterZdfDiodeLadder
{
public:
  SoundFilterZdfDiodeLadder() {}
  ~SoundFilterZdfDiodeLadder() {}

  void  Init() {}
  float set_k(const float val) { k = val; }
  float set_A(const float val) { A = val; }

  void update()
  {
    float y0, y1, y2, y3, y4;

    float wc = PI * (in2 * 1. / samplerate);

    float wc2 = wc * wc;
    float wc3 = wc2 * wc;
    float wc4 = wc3 * wc;
    float b   = 1 / (1 + 8 * wc + 20 * wc2 + 16 * wc3 + 2 * wc4);
    float g   = 2 * wc4 * b;

    float s
        = (z0 * wc3 + z1 * (wc2 + 2 * wc3) + z2 * (wc + 4 * wc2 + 2 * wc3) + z3 * (1 + 6 * wc + 9 * wc2 + 2 * wc3)) * b;

    y4 = (g * in1 + s) / (1 + g * k);

    y0 = tanh(in1 - k * y4);

    y1 = (y0 * (2 * wc + 12 * wc2 + 20 * wc3 + 8 * wc4) + z0 * (1 + 6 * wc + 10 * wc2 + 4 * wc3)
          + z1 * (2 * wc + 8 * wc2 + 6 * wc3) + z2 * (2 * wc2 + 4 * wc3) + z3 * 2 * wc3)
         * b;
    y2 = (y0 * (2 * wc2 + 8 * wc3 + 6 * wc4) + z0 * (wc + 4 * wc2 + 3 * wc3) + z1 * (1 + 6 * wc + 11 * wc2 + 6 * wc3)
          + z2 * (wc + 4 * wc2 + 4 * wc3) + z3 * (wc2 + 2 * wc3))
         * b;
    y3 = (y0 * (2 * wc3 + 4 * wc4) + z0 * (wc2 + 2 * wc3) + z1 * (wc + 4 * wc2 + 4 * wc3)
          + z2 * (1 + 6 * wc + 10 * wc2 + 4 * wc3) + z3 * (wc + 4 * wc2 + 2 * wc3))
         * b;
    y4 = g * y0 + s;

    z0 += 4 * wc * (y0 - y1 + y2);
    z1 += 2 * wc * (y1 - 2 * y2 + y3);
    z2 += 2 * wc * (y2 - 2 * y3 + y4);
    z3 += 2 * wc * (y3 - 2 * y4);

    out1 = A * y4;
  }

private:
  float k = 0.f, A = 0.f;
  float z0 = 0.f, z1 = 0.f, z2 = 0.f, z3 = 0.f;
};
