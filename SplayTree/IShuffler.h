#pragma once


class IShuffler
{
public:
    virtual int GetSum(int l, int r) = 0;
    virtual void Insert(int position, int value) = 0;
    virtual void Replace(int position, int value) = 0;
    virtual void AddValue(int l, int r, int value) = 0;
    virtual void NextPermutation(int l, int r) = 0;
    virtual bool CanPermutate() = 0;
    virtual void Print() = 0;
    virtual int Size() = 0;
};