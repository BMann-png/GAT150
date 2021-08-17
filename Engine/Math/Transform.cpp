#include "Transform.h"

namespace pbls
{
	void pbls::Transform::Update()
	{
		Matrix33 mxs;
		mxs.Scale(scale);
		
		Matrix33 mxr;
		mxr.Rotate(rotation);

		Matrix33 mxt;
		mxt.Translate(position);

		matrix = (mxs * mxr)* mxt;
	}
	void Transform::Update(const Matrix33& mx)
	{
		Matrix33 mxs;
		mxs.Scale(localScale);

		Matrix33 mxr;
		mxr.Rotate(localRotation);

		Matrix33 mxt;
		mxt.Translate(localPosition);

		matrix = (mxs * mxr) * mxt * mx;

		position = matrix.GetTranslate();
		rotation = matrix.GetRotation();
		scale = matrix.GetScale();
	}
}