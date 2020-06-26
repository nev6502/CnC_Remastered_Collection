// IMGUTIL.H
//

#ifdef _DEBUG
#pragma optimize( "", on )
#endif

// SRGB -> Linear color space conversion.
inline float ByteToLinear(byte p) {
	float v;
	v = ((float)p) / 255.0f;
	v = (double)v >= 0.0404499992728233 ? (float)pow((200.0 * (double)v + 11.0) / 211.0, 2.40000009536743) : (float)((double)v * 25.0 / 323.0);
	return v;
}

inline float ToSRGB(float v)
{
	return (double)v >= 0.0031308 ? (float)((pow((double)v, 0.416666656732559) * 211.0 - 11.0) / 200.0) : (float)((double)v * 323.0 / 25.0);
}

inline float PixelToHue(float R, float G, float B) {
	if (R == G && G == B)
		return 0; // 0 makes as good an UNDEFINED value as any

	float r = (float)R / 255.0f;
	float g = (float)G / 255.0f;
	float b = (float)B / 255.0f;

	float max, min;
	float delta;
	float hue = 0.0f;

	max = r; min = r;

	if (g > max) max = g;
	if (b > max) max = b;

	if (g < min) min = g;
	if (b < min) min = b;

	delta = max - min;

	if (r == max) {
		hue = (g - b) / delta;
	}
	else if (g == max) {
		hue = 2 + (b - r) / delta;
	}
	else if (b == max) {
		hue = 4 + (r - g) / delta;
	}
	hue *= 60;

	if (hue < 0.0f) {
		hue += 360.0f;
	}
	return hue;
}

struct Turple {
	Turple() {
		memset(this, 0, sizeof(Turple));
	}
	Turple(float Item1, float Item2, float Item3) {
		this->Item1 = Item1;
		this->Item2 = Item2;
		this->Item3 = Item3;
		this->Item4 = -1;
	}
	Turple(float Item1, float Item2, float Item3, float Item4) {
		this->Item1 = Item1;
		this->Item2 = Item2;
		this->Item3 = Item3;
		this->Item4 = Item4;
	}
	float Item1;
	float Item2;
	float Item3;
	float Item4;
};


float step(float a, float x)
{
	return x >= a;
}

inline float4 lerp(float4 a, float4 b, float w)
{
	float4 l;
	l.x = a.x + w * (b.x - a.x);
	l.y = a.y + w * (b.y - a.y);
	l.z = a.z + w * (b.z - a.z);
	l.w = a.w + w * (b.w - a.w);
	return l;
}

inline float3 lerp(float3 a, float3 b, float w)
{
	float3 l;
	l.x = a.x + w * (b.x - a.x);
	l.y = a.y + w * (b.y - a.y);
	l.z = a.z + w * (b.z - a.z);
	return l;
}

inline float3 frac(float3 v)
{
	float3 f;
	f.x = v.x - floor(v.x);
	f.y = v.y - floor(v.y);
	f.z = v.z - floor(v.z);
	return f;
}

inline float frac(float v)
{
	return v - floor(v);
}

inline float lerp(float a, float b, float w)
{
	return a + w * (b - a);
}


inline float3 clamp(float3 x, float a, float b)
{
	float3 r;
	r.x = max(a, min(b, x.x));
	r.y = max(a, min(b, x.y));
	r.z = max(a, min(b, x.z));
	return r;
}

inline float saturate(float x)
{
	return max(0, min(1, x));
}

inline void RenderTeamPalette(byte* pixel, HouseColorHD &teamColor) {
	float linear1 = ByteToLinear(pixel[2]);
	float linear2 = ByteToLinear(pixel[1]);
	float linear3 = ByteToLinear(pixel[0]);
	float num1 = linear1;
	float num2 = linear2;
	float num3 = (float)linear3;
	Turple valueTuple1 = Turple(0.0f, -0.3333333, 0.6666667, -1 );
	Turple valueTuple2;
	
	if ((double)num2 >= (double)num3)
		valueTuple2 = Turple( num2, num3, valueTuple1.Item1, valueTuple1.Item2 );
	else
		valueTuple2 = Turple( num3, num2, valueTuple1.Item4, valueTuple1.Item3 );

	Turple valueTuple3;
	if ((double)num1 >= (double)valueTuple2.Item1)
		valueTuple3 = Turple(num1, valueTuple2.Item2, valueTuple2.Item3, valueTuple2.Item1);
	else
		valueTuple3 = Turple(valueTuple2.Item1, valueTuple2.Item2, valueTuple2.Item4, num1);
	double num4 = (double)valueTuple3.Item1 - (double)min(valueTuple3.Item4, valueTuple3.Item2);
	float num5 = 1E-10f;
	float num6 = (float)num4;
	float num7 = num5;
	float num8 = abs(valueTuple3.Item3 + (float)(((double)valueTuple3.Item4 - (double)valueTuple3.Item2) / (6.0 * (double)num6 + (double)num7)));
	float num9 = num6 / (valueTuple3.Item1 + num7);
	double num10 = (double)valueTuple3.Item1;
	float num11 = num8;
	float num12 = num9;
	float num13 = (float)num10;
	float num14 = PixelToHue(teamColor.LowerBounds.x, teamColor.LowerBounds.y, teamColor.LowerBounds.z) / 360;
	float num15 = PixelToHue(teamColor.UpperBounds.x, teamColor.UpperBounds.y, teamColor.UpperBounds.z) / 360;

	if ((double)num11 >= (double)num14 && (double)num15 >= (double)num11)
	{
		float num16 = (float)((double)num11 / ((double)num15 - (double)num14) * ((double)num15 + (double)teamColor.Fudge - ((double)num14 - (double)teamColor.Fudge))) + teamColor.HSVShift.x;
		float t = num12 + teamColor.HSVShift.y;
		float num17 = num13 + teamColor.HSVShift.z;
		Turple valueTuple4 = Turple(1.0f, 0.6666667, 0.3333333, 3);
		Turple valueTuple5 = Turple(abs(frac(num16 + valueTuple4.Item1) * 6 - valueTuple4.Item4), abs(frac(num16 + valueTuple4.Item2) * 6 - valueTuple4.Item4), abs(frac(num16 + valueTuple4.Item3) * 6 - valueTuple4.Item4));
		float num18 = num17 * lerp(valueTuple4.Item1, saturate(valueTuple5.Item1 - valueTuple4.Item1), t);
		float num19 = num17 * lerp(valueTuple4.Item1, saturate(valueTuple5.Item2 - valueTuple4.Item1), t);
		float num20 = num17 * lerp(valueTuple4.Item1, saturate(valueTuple5.Item3 - valueTuple4.Item1), t);
		Turple valueTuple6 = Turple(min(1, max(0.0f, num18 - teamColor.InputLevels.x) / (teamColor.InputLevels.z - teamColor.InputLevels.x)), min(1, max(0.0f, num19 - teamColor.InputLevels.x) / (teamColor.InputLevels.z - teamColor.InputLevels.x)), min(1, max(0.0f, num20 - teamColor.InputLevels.x) / (teamColor.InputLevels.z - teamColor.InputLevels.x)));
		valueTuple6.Item1 = (float)pow((double)valueTuple6.Item1, (double)teamColor.InputLevels.y);
		valueTuple6.Item2 = (float)pow((double)valueTuple6.Item2, (double)teamColor.InputLevels.y);
		valueTuple6.Item3 = (float)pow((double)valueTuple6.Item3, (double)teamColor.InputLevels.y);
		num1 = lerp(teamColor.OutputLevels.x, teamColor.OutputLevels.y, valueTuple6.Item1);
		num2 = lerp(teamColor.OutputLevels.x, teamColor.OutputLevels.y, valueTuple6.Item2);
		num3 = lerp(teamColor.OutputLevels.x, teamColor.OutputLevels.y, valueTuple6.Item3);
	}

	Turple valueTuple7 = Turple(min(1, max(0.0f, num1 - teamColor.OverallInputLevels.x) / (teamColor.OverallInputLevels.z - teamColor.OverallInputLevels.x)), min(1, max(0.0f, num2 - teamColor.OverallInputLevels.x) / (teamColor.OverallInputLevels.z - teamColor.OverallInputLevels.x)), min(1, max(0.0f, num3 - teamColor.OverallInputLevels.x) / (teamColor.OverallInputLevels.z - teamColor.OverallInputLevels.x)));
	valueTuple7.Item1 = (float)pow((double)valueTuple7.Item1, (double)teamColor.OverallInputLevels.y);
	valueTuple7.Item2 = (float)pow((double)valueTuple7.Item2, (double)teamColor.OverallInputLevels.y);
	valueTuple7.Item3 = (float)pow((double)valueTuple7.Item3, (double)teamColor.OverallInputLevels.y);
	float v1 = lerp(teamColor.OverallOutputLevels.x, teamColor.OverallOutputLevels.y, valueTuple7.Item1);
	float v2 = lerp(teamColor.OverallOutputLevels.x, teamColor.OverallOutputLevels.y, valueTuple7.Item2);
	float v3 = lerp(teamColor.OverallOutputLevels.x, teamColor.OverallOutputLevels.y, valueTuple7.Item3);
	pixel[2] = (byte)((double)ToSRGB(v1) * (double)255.0f);
	pixel[1] = (byte)((double)ToSRGB(v2) * (double)255.0f);
	pixel[0] = (byte)((double)ToSRGB(v3) * (double)255.0f);
}
#ifdef _DEBUG
#pragma optimize( "", off )
#endif