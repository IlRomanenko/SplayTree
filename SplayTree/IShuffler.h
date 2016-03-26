#pragma once


class IShuffler
{
public:
    virtual long long GetSum(int l, int r) = 0;
    virtual void Insert(int position, long long value) = 0;
    virtual void Replace(int position, long long value) = 0;
    virtual void AddValue(int l, int r, long long value) = 0;
    virtual void NextPermutation(int l, int r) = 0;
    virtual bool CanPermutate() const = 0;
    virtual void Print() = 0;
    virtual size_t Size() const = 0;
    virtual ~IShuffler() { }
};