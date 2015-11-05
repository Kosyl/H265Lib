template <typename T> inline T clipToRange(T minVal, T maxVal, T a)
{
	return std::min<T>(std::max<T>(minVal, a), maxVal);
}