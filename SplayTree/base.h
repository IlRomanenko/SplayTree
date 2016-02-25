#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <random>
#include <numeric>
using namespace std;

#ifdef _DEBUG
#define dbg(x) x
#include <vld.h>
#else
#define dbg(x) 
#endif