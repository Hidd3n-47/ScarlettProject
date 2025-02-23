#pragma once

struct ComponentA
{
    int value = 0;
};
struct ComponentB
{
    float value = 0.0f;
};
struct ComponentC
{
    bool value = false;
};
struct ComponentMatrix
{
    float value[2][2] = { { 1.0f, 0.0f }, { 0.0f, 1.0f } };

    [[nodiscard]] float A() const { return value[0][0]; }
    [[nodiscard]] float B() const { return value[0][1]; }
    [[nodiscard]] float C() const { return value[1][0]; }
    [[nodiscard]] float D() const { return value[1][1]; }
    inline ComponentMatrix& operator*=(const ComponentMatrix& rhs)
    {
        const float a = value[0][0];
        const float b = value[0][1];
        const float c = value[1][0];
        const float d = value[1][1];

        const float a_ = rhs.A();
        const float b_ = rhs.B();
        const float c_ = rhs.C();
        const float d_ = rhs.D();

        value[0][0] = a * a_ + b * c_;
        value[0][1] = a * b_ + b * d_;
        value[1][0] = c * a_ + d * c_;
        value[1][1] = c * b_ + d * d_;

        return *this;
    }

    [[nodiscard]] inline bool static IsCloseTo(const float a, const float b)
    {
        return fabs(a - b) < 0.0001f;
    }

    inline bool operator==(const ComponentMatrix& rhs) const
    {
        bool equal = true;

        equal &= IsCloseTo(A(),rhs.A());
        equal &= IsCloseTo(B(),rhs.B());
        equal &= IsCloseTo(C(),rhs.C());
        equal &= IsCloseTo(D(),rhs.D());

        return equal;
    }
};
