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
        auto v_split = Split3(root, l + 1, r + 1);
        left = v_split[0];
        middle = v_split[1];
        right = v_split[2];

        int result = ::GetSum(middle);

        root = Merge(Merge(left, middle), right);

        return result;
    }

    void Insert(int position, int value) override
    {
        PtrTree left, right;
        Split(root, left, right, position);
        root = Merge(Merge(left, PtrTree(new SplayTree(value))), right);
    }

    void Replace(int position, int value) override
    {
        root = Splay(Find(root, position + 1));
        root->value = value;
        Lock(root);
    }

    void AddValue(int l, int r, int value) override
    {
        auto v_split = Split3(root, l + 1, r + 1);
        v_split[1]->to_add += value;
        Lock(v_split[1]);
        root = Merge(Merge(v_split[0], v_split[1]), v_split[2]);
    }

    void NextPermutation(int l, int r) override
    {
        root = SplayNextPermutation(root, l + 1, r + 1);
    }

    bool CanPermutate() override
    {
        Lock(root);
        return !root->is_rev_sorted;
    }

    void Print() override
    {
        PrintTree(root);
    }

    int Size()
    {
        return GetSize(root);
    }
};