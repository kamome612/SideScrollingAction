#pragma once

//float GetEase(float start, float end, float rate) {
//	return (end - start) * rate + start;
//}


//テンプレートで書く
template<typename T>
T GetEase(T start, T end, float rate) {
	return (end - start) * rate + start;
}