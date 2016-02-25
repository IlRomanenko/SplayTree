#include "base.h"
#include "Shuffler.h"



void SimpleTestSplay()
{
    Shuffler root;
    for (int i = 0; i < 4; i++)
    {
        root.Insert(i, i - 1);
    }

    do
    {
        root.Print();
        root.NextPermutation(0, 3);
        cout << endl;
    } while (root.CanPermutate());

    root.Print();
    cout << endl << endl;
    cout << root.GetSum(3, 3);
    cout << endl << endl;
    cout << root.GetSum(2, 2);
    cout << endl << endl;
    cout << root.GetSum(1, 1);
    cout << endl << endl;
    cout << root.GetSum(2, 3);
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
        value = random(engine);

        /*dbg(
        cout << "before " << oper << endl << endl;
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
            //vect[l] = value;
            //shuffler.Replace(l, value);
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
            assert(false, "Bad size");
        }
    }
}

int main()
{
    //freopen("output.txt", "w", stdout);
    //SimpleTestSplay();
    try
    {
        TestSplay(10000);
    }
    catch (exception e)
    {
        Error(vect, shuffler);
    }
    system("pause");
    return 0;
}