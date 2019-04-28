#pragma once

// HSV‚ğRGB‚É•ÏŠ·(int F‘Š(0~359), int Ê“x(0~255), int –¾“x(0~255))
int HsvChangeRgb(int H, int S, int V)
{
	float H_f = static_cast<float>(H);
	float S_f = static_cast<float>(S);
	float V_f = static_cast<float>(V);

	float r;
	float g;
	float b;

	int Hi;
	float min = V_f - ((S_f / 255.0f) * V_f);
	float a = (V - min);

	Hi = static_cast<int> (H_f / 60.0f);

	switch (Hi)
	{
	case 0:
		r = V_f;
		g = (H_f / 60.0f) * a + min;
		b = min;
		break;

	case 1:
		r = ((120.0f - H_f) / 60.0f) * a + min;
		g = V_f;
		b = min;
		break;

	case 2:
		r = min;
		g = V_f;
		b = ((H_f - 120.0f) / 60.0f) * a + min;
		break;

	case 3:
		r = min;
		g = ((240.0f - H_f) / 60.0f) * a + min;
		b = V_f;
		break;

	case 4:
		r = ((H_f - 240.0f) / 60.0f) * a + min;
		g = min;
		b = V_f;
		break;

	case 5:
		r = V_f;
		g = min;
		b = ((360.0f - H_f) / 60.0f) * a + min;
		break;

	default:
		r = 255;
		g = 255;
		b = 255;
		break;
	}

	return ( (static_cast<int>(r)<<16) + (static_cast<int>(g)<<8) + (static_cast<int>(b)) );
}