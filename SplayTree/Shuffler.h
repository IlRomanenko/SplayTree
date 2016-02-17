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
        if (root == nullptr)
            return 0;
    }
    void Insert(int position, int value) override
    {
    }
    void Replace(int position, int value) override
    {
    }
    void AddValue(int l, int r, int value) override
    {
    }
    void NextPermutation(int l, int r) override
    {
    }
    bool CanPermutate() override
    {
    }
};