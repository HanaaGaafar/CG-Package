#pragma once
#ifndef CH_EXTREME_POINTS_HANAA
#define CH_EXTREME_POINTS_HANAA


#include "Algorithm.h"


class CH_ExtremePoints_Hanaa : public Algorithm
{
private:




public:
	CH_ExtremePoints_Hanaa(){}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		vector<Point> input = inputPoints;
		sort(input.begin(),input.end());
		input.resize(unique(input.begin(),input.end())-input.begin());
		if(input.size()<3)return;

		
		outputPoints.clear(), outputLines.clear();
		for (int i = 0 ; i < input.size() ; i++)
		{
			bool inTriangle = false;

			for (int j = 0 ; j < input.size() ; j++)
			{
				if(j==i) continue;

				for (int k = 0 ; k < input.size() ; k++)
				{
					if (i == k || j ==k) continue;

					for (int l = 0 ; l < input.size() ; l++)
					{
						if (i==l || j==l || k==l) continue;

						inTriangle = Utilities::PointInTriangle(input[i],input[j],input[k],input[l]);
						if (inTriangle) break;

					}
					if (inTriangle) break;

				}
				if (inTriangle) break;

			}
			if (!inTriangle)
			{
				outputPoints.push_back(input[i]);
			}
		}

		
		Utilities::SortPointForDrawing(outputPoints,outputLines);
	}
	
};
#endif