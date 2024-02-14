#include "Vector4.h"

Vector4 Esing(const Vector4& st, const Vector4& ed, float t)
{
	return{
		st.x * (1.0f - t) + ed.x * t,
		st.y * (1.0f - t) + ed.y * t,
st.z * (1.0f - t) + ed.z * t,
st.w * (1.0f - t) + ed.w * t,

	};
}
