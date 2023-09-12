#include <bits/stdc++.h>
using namespace std;
class F
{
public:
    F adds(F const &other)
    {
        F f = *this;
        for (auto const &i : other.data)
            f.add(i);
        return move(f);
    }
    void add(pair<int, int> const &other)
    {
        if (other.second == 0)
            return;
        if (this->data.find(other.first) == this->data.end())
            this->data[other.first] = other.second;
        else
            this->data[other.first] = this->data[other.first] + other.second;
    }
    F dels(F const &other)
    {
        F f = *this;
        for (auto const &i : other.data)
            f.del(i);
        return move(f);
    }
    void del(pair<int, int> const &other)
    {
        if (this->data.find(other.first) == this->data.end())
            this->data[other.first] = -other.second;
        else
            this->data[other.first] = this->data[other.first] - other.second;
    }
    F Mut(F const &other)
    {
        F f;
        for (auto const &i : other.data)
            for (auto const &ii : this->data)
                f.add(make_pair(i.first + ii.first, ii.second * i.second));
        return move(f);
    }
    F dao()
    {
        F f;
        for (auto const &i : data)
            f.add(make_pair(i.first - 1, i.first * i.second));
        return move(f);
    }
    void print()
    {
        bool Isfirst = true;
        for (auto const &i : data)
        {
            if (i.second == 0)
                continue;
            printf("+%d" + (Isfirst || (i.second < 0)), i.second);
            if (i.first != 0)
            {
                if (i.first == 1)
                    printf("X");
                else
                    printf("X^%d", i.first);
            }
            Isfirst = false;
        }
        puts("");
    }
    void Get()
    {
        string input;
        getline(std::cin, input);
        int count = stoi(input);
        getline(std::cin, input);
        stringstream ss(input);
        for (int i = 0; i < count; i++)
        {
            char openParenthesis, comma, closeParenthesis;
            int key, value;
            ss >> openParenthesis >> key >> comma >> value >> closeParenthesis;
            this->add(make_pair(value, key));
        }
    }
    long long Do(int i)
    {
        long long result = 0;
        for (auto &ii : this->data)
        {
            result += ii.second * pow(i, ii.first);
        }
        return result;
    }
    map<int, int> data;
};

int main()
{
    F f, ff;
    f.Get();
    int i;
    cin >> i;
    cout << f.Do(i);
}