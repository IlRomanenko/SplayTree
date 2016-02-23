#pragma once
#include "base.h"
#include "IShuffler.h"
#include "SplayTree.h"



class Shuffler : public IShuffler
{
    PtrTree root;

public:
    Shuffler()
    {
        root = nullptr;
    }
    
    ~Shuffler() = default;

    int GetSum(int l, int r) override
    {
        PtrTree left, right, middle;
        auto v_split = Split(root, l, r);
        left = v_split[0];
        middle = v_split[1];
        right = v_split[2];

        int result = ::GetSum(middle);

        root = Merge(Merge(left, middle), right);

        return result;
    }

    void Insert(int position, int value) override
    {
        if (!IsExist(root))
        {
            root = Add(root, value);
            return;
        }
        root = Splay(Find(root, position));
        root = Add(root, value);
    }

    void Replace(int position, int value) override
    {
        root = Splay(Find(root, position + 1));
        root->value = value;
        Update(root);
    }

    void AddValue(int l, int r, int value) override
    {
        auto v_split = Split(root, l, r);
        v_split[1]->to_add += value;
        Push(v_split[1]);
        Update(v_split[1]);
        root = Merge(Merge(v_split[0], v_split[1]), v_split[2]);
    }

    void NextPermutation(int l, int r) override
    {
        root = SplayNextPermutation(root, l, r);
    }

    bool CanPermutate() override
    {
        Push(root);
        return !root->is_rev_sorted;
    }

    void Print() override
    {
        PrintTree(root);
    }
};