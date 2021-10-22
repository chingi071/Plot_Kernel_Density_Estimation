#include "kde.h"
#include "iostream"
#include<cmath>
#include <algorithm>
#include <string>
#include <numeric>

using namespace std;
#define pi atan(1.0) * 4.0

KDE::KDE()
{

}

vector<float> KDE::readData(string fname)
{
    ifstream inFile(fname, ios::in);
    if (!inFile)
    {
        cout << "open file fail!" << endl;
        exit(1);
    }

    string line;
    vector<float> f_DataList;
    while (getline(inFile, line))
    {
        f_DataList.push_back(::atof(line.c_str()));
    }
    inFile.close();

    return f_DataList;
}

KDEResult KDE::get_kde_result(vector<float> f_DataList)
{
    vector<float> f_process_DataList = preprocessing(f_DataList);
    size_t DataLength = f_process_DataList.size();

    float minValue = *min_element(f_process_DataList.begin(), f_process_DataList.end());
    float maxValue = *max_element(f_process_DataList.begin(), f_process_DataList.end());
    vector<float> f_xList = linspace(minValue, maxValue, DataLength);

    init_bandwidth(f_process_DataList);

    vector<float> f_yList;
    for (size_t i=0; i < DataLength; i++)
    {
        float f_KDEValue = cal_kde(f_xList[i], f_process_DataList);
        f_yList.push_back(f_KDEValue);
    }

    std::vector<float>::iterator y_iter = max_element(f_yList.begin(), f_yList.end());
    int find_MaxValue_index = distance(f_yList.begin(), y_iter);

    KDEResult kde_result;
    kde_result.f_xList = f_xList;
    kde_result.f_yList = f_yList;
    kde_result.DataLength = DataLength;
    kde_result.DataMaxValue = f_xList[find_MaxValue_index];
    kde_result.i_xMin = (int) f_xList[0];
    kde_result.i_xMax = (int) f_xList[DataLength-1];
    kde_result.i_yMax = *y_iter;

    return kde_result;
}

float KDE::cal_kde(float x, vector<float> f_DataList)
{
    int DataLength = f_DataList.size();
    float f_KDEValue = 0.0;

    for (auto i:f_DataList)
    {
       float x_value = (x-i)/bandwidth;
       float f_GaussValue = cal_gauss(x_value);
       f_KDEValue += f_GaussValue;
    }

    f_KDEValue /= DataLength * bandwidth;

    return f_KDEValue;
}

float KDE::cal_gauss(float x)
{
    float f_GaussValue = 1/pow(2 * pi, 0.5) * exp((-0.5) *pow(x, 2.0));

    return f_GaussValue;
}

vector<float> KDE::preprocessing(vector<float> f_DataList)
{
    float iqr = cal_percentile(f_DataList, 0.75)-cal_percentile(f_DataList, 0.25);

    vector<float> f_process_DataList;
    if (iqr > 0){
        float max_outlier = cal_percentile(f_DataList, 0.75) + 1.5*iqr;
        float min_outlier = cal_percentile(f_DataList, 0.25) - 1.5*iqr;

        for (size_t i=0; i < f_DataList.size(); i++){
            if (f_DataList[i] < max_outlier && f_DataList[i] > min_outlier){
                f_process_DataList.push_back(f_DataList[i]);
            }
        }
    }else{
        f_process_DataList = f_DataList;
    }

    return f_process_DataList;
}

float KDE::cal_percentile(vector<float> f_DataList, float percent)
{
    sort(f_DataList.begin(), f_DataList.end());
    int DataLength = f_DataList.size();
    float m = (DataLength-1)*percent;
    int m_floor = floor(m);
    int m_ceil = ceil(m);

    float rate = m - m_floor;

    float PercentileValue = (1.0-rate)*f_DataList[m_floor] + rate*f_DataList[m_ceil];

    return PercentileValue;
}

vector<float> KDE::linspace(float start_in, float end_in, int num_in)
{
    std::vector<float> linspaced;

      float start = static_cast<float>(start_in);
      float end = static_cast<float>(end_in);
      int num = static_cast<int>(num_in);

      if (num == 0) { return linspaced; }
      if (num == 1)
      {
          linspaced.push_back(start);

          return linspaced;
      }

      float delta = (end - start) / (num - 1);

      for(int i=0; i < num-1; ++i)
        {
          linspaced.push_back(start + delta * i);
        }
      linspaced.push_back(end);

      return linspaced;
}

void KDE::init_bandwidth(vector<float> f_DataList)
{
    bandwidth = 1.05*std_dev(f_DataList) * pow(f_DataList.size(), -1.0/5.0);
}

float KDE::std_dev(std::vector<float> const & data)
{
    const float init = 0.0;
    float sum = accumulate(data.begin(), data.end(), init);
    float mean = sum / data.size();

    float res = 0.0;
    for_each(data.begin(), data.end(), [&](float x) {
        res += pow(x - mean, 2.0);
    });

    float variance = res / (data.size());

    return sqrt(variance);
}

KDE::~KDE()
{

}
