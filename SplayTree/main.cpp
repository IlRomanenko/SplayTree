#include "base.h"
#include "Shuffler.h"



void TestSplay()
{
    Shuffler root;
    for (int i = 0; i < 4; i++)
    {
        root.Insert(i, i - 1);
    }

    do
    {
        root.Print();
        root.NextPermutation(0, 4);
        cout << endl;
    } while (root.CanPermutate());

    root.Print();
    cout << endl << endl;
    
}

int main()
{
    TestSplay();
 
    system("pause");
    return 0;
}