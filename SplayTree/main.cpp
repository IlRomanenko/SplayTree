#include "base.h"
#include "SplayTree.h"



void TestSplay()
{
    //freopen("output.txt", "w", stdout);
    PtrTree root = nullptr;

    for (int i = 0; i < 4; i++)
    {
        root = Add(root, i + 1);
    }
    root = Add(root, 4);

    for (int i = 0; i < 120; i++)
    {
        PrintTree(root);
        root = NextPermutation(root, 0, 5);
        cout << endl;
    }
    cout << endl << endl;
    
    /*vector<int> v;
    for (int i = 0; i < 4; i++)
        v.push_back(i + 1);
    v.push_back(4);

    do
    {
        for (int i : v)
            cout << i << ' ';
        cout << endl;
    } while (next_permutation(v.begin(), v.end()));*/
}

int main()
{
    TestSplay();
 
    system("pause");
    return 0;
}