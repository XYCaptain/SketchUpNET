#include <SketchUpAPI/slapi.h>
#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/initialize.h>
#include <SketchUpAPI/unicodestring.h>
#include <SketchUpAPI/model/model.h>
#include <SketchUpAPI/model/entities.h>
#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/curve.h>
#include <SketchUpAPI/model/drawing_element.h>
#include <msclr/marshal.h>
#include <vector>
#include "vertex.h"
#include "utilities.h"
#include "transform.h"
#include <SketchUpAPI/model/axes.h>

#pragma once

namespace SketchUpNET
{
	public ref class Axis
	{
	public:

		Vertex^ Origin;
		Transform^ AxisTransform;

		Axis(Vertex^ origin, Transform^ transform)
		{
			this->Origin = origin;
			this->AxisTransform = transform;
		};

		Axis() {};
	internal:
		static Axis^ FromSU(SUAxesRef axes)
		{
			SUPoint3D originVertex = SU_INVALID;
			SUTransformation transformation = SU_INVALID;
			SUAxesGetOrigin(axes, &originVertex);
			SUAxesGetTransform(axes, &transformation);

			Axis^ v = gcnew Axis(Vertex::FromSU(originVertex), Transform::FromSU(transformation));
			return v;
		};
	};
}