#ifndef KDE_H
#define KDE_H
#pragma once

#include<cmath>
#include<iostream>
#include<fstream>
#include <vector>

using namespace std;

struct KDEResult{
    vector<float> f_xList;
    vector<float> f_yList;
    size_t DataLength;
    float DataMaxValue;
    int i_xMin;
    int i_xMax;
    float i_yMax;
};

class KDE
{
public:
    KDE();
    ~KDE();
    vector<float> readData(string fname);
    KDEResult get_kde_result(vector<float> f_DataList);
    float cal_kde(float x, vector<float> f_DataList);
    float cal_gauss(float x);
    vector<float> preprocessing(vector<float> f_DataList);
    float cal_percentile(vector<float> f_DataList, float p);
    vector<float> linspace(float start_in, float end_in, int num_in);
    void init_bandwidth(vector<float> f_DataList);
    float std_dev(std::vector<float> const & data);

private:
    float bandwidth;

};

#endif // KDE_H
