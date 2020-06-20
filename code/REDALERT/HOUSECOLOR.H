// HOUSECOLOR.H
//

#include "vectormath.h"

struct HouseColorHD {
	HouseColorHD(float3 lowerbounds,
		float3 upperbounds,
		float fudge,
		float3 hsvshift,
		float3 inputlevels,
		float2 outputlevels,
		float3 overallinputlevels,
		float2 overalloutputlevels,
		float3 radarmapcolor)
	{
		this->LowerBounds = lowerbounds;
		this->UpperBounds = upperbounds;
		this->Fudge = fudge;
		this->HSVShift = hsvshift;
		this->InputLevels = inputlevels;
		this->OutputLevels = outputlevels;
		this->OverallInputLevels = overallinputlevels;
		this->OverallOutputLevels = overalloutputlevels;
		this->RadarMapColor = radarmapcolor;
	}

	float3 LowerBounds;
	float3 UpperBounds;
	float Fudge;
	float3 HSVShift;
	float3 InputLevels;
	float2 OutputLevels;
	float3 OverallInputLevels;
	float2 OverallOutputLevels;
	float3 RadarMapColor;
};

// This data has been sourced from the TD Original Skirmish Color mod by Pokey
// 
static HouseColorHD *houseColors[8] = {
	// Spain
	new HouseColorHD(float3(0.26, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.713,-0.621, 0.000000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.6421568, 0.4784698, 0.4784698)),	
	// Greece
	new HouseColorHD(float3(0.264706f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.310000f,-0.211000, 0.005000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0980f, 0.3922, 0.9216)),
	// USSR
	new HouseColorHD(float3(0.264706f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.689000f,0.382000, 0.000000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.756863f, 0.0, 0.015686)),
	// England
	new HouseColorHD(float3(0.26f, 1.0f, 1.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.777f,-0.196, -0.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.737255, 0.627451, 0.239216)),	
	// Ukraine
	new HouseColorHD(float3(0.26, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.726,0.119, 0.000000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.772549, 0.380392, 0.07451)),
	// Germany
	new HouseColorHD(float3(0.26, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.911f,-0.381, 0.000000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.184314, 0.67451, 0.141176)),
	// France
	new HouseColorHD(float3(0.26, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.119,-0.233, 0.000000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.1151961, 0.6911765, 0.4031861)),
	// Turkey
	new HouseColorHD(float3(0.26, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.22f), 0, float3(0.685,-0.151, 0.000000), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.0f, 1.0f, 1.0f), float2(0.0f, 1.0f), float3(0.5784314, 0.1155861, 0.05387352)),
};