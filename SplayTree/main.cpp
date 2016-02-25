#include "base.h"
#include "Shuffler.h"



void SimpleTestSplay()
{
    freopen("simple.txt", "w", stdout);
    Shuffler root;
    for (int i = 0; i < 4; i++)
    {
        root.Insert(i, i + 1);
    }

    do
    {
        root.Print();
        root.NextPermutation(0, 3);
        cout << endl;
    } while (root.CanPermutate());

    root.Print();
    cout << endl << endl;
    vector<int> v;
    for (int i = 0; i < 4; i++)
        v.push_back(i + 1);

    do
    {
        for_each(v.begin(), v.end(), [](int x) { cout << x << ' '; });
        cout << endl;
    } while (next_permutation(v.begin(), v.end()));
    fclose(stdout);
}

void Error(vector<int> &vect, Shuffler &shuffler)
{
    cout << "Error" << endl;
    for_each(vect.begin(), vect.end(), [](int x) { cout << x << ' '; });
    cout << endl << endl;
    shuffler.Print();
    cout << endl << endl;
}


Shuffler shuffler;
vector<int> vect;
uniform_int_distribution<int> random;
default_random_engine engine;

void TestSplay(int amount_operations)
{
    int oper, l, r, value;

    const int MaxValue = 1000000;

    for (int i = 0; i < amount_operations; i++)
    {
        //Sum(l, r)
        //Insert(pos, value)
        //Replace(pos, value)
        //AddValue(l, r, value)
        //NextPermutation(l, r)
        oper = random(engine) % 5;
        if (vect.size() == 0)
        {
            oper = 1;
            l = 0;
        }
        else
        {
            l = random(engine) % vect.size();
            r = random(engine) % vect.size();
            if (l > r)
                swap(l, r);
        }
        value = random(engine) % MaxValue;

       /* dbg(
        cout << "before " << oper << "   i = " << i << endl << endl;
        shuffler.Print();
        cout << endl << endl;
        for_each(vect.begin(), vect.end(), [](int x) { cout << x << ' '; });
        cout << endl << endl << endl << endl;
        );*/
        switch (oper)
        {
        case 0:
            if (accumulate(vect.begin() + l, vect.begin() + r + 1, 0) != shuffler.GetSum(l, r))
            {
                Error(vect, shuffler);
                return;
            }
            break;
        case 1:
            vect.insert(vect.begin() + l, value);
            shuffler.Insert(l, value);
            break;
        case 2:
            vect[l] = value;
            shuffler.Replace(l, value);
            break;
        case 3:
            //for_each(vect.begin() + l, vect.begin() + r + 1, [value](int &x) mutable { x += value; });
            //shuffler.AddValue(l, r, value);
            break;
        case 4:
            next_permutation(vect.begin() + l, vect.begin() + r + 1);
            shuffler.NextPermutation(l, r);
            break;
        }
        if (shuffler.Size() != vect.size())
        {
            Error(vect, shuffler);
            cout << "Bad size";
            return;
        }
        if (shuffler.GetSum(0, vect.size() - 1) != accumulate(vect.begin(), vect.end(), 0))
        {
            Error(vect, shuffler);
            cout << "Bad sum";
            return;
        }
    }
}

int main()
{
    //freopen("output.txt", "w", stdout);
    //SimpleTestSplay();
    TestSplay(10000);
    
    system("pause");
    return 0;
}