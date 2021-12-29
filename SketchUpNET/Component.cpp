/*

SketchUpNET - a C++ Wrapper for the Trimble(R) SketchUp(R) C API
Copyright(C) 2015, Autor: Maximilian Thumfart

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <SketchUpAPI/slapi.h>
#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/initialize.h>
#include <SketchUpAPI/unicodestring.h>
#include <SketchUpAPI/model/model.h>
#include <SketchUpAPI/model/entities.h>
#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/component_definition.h>
#include <msclr/marshal.h>
#include <vector>
#include "surface.h"
#include "edge.h"
#include "group.h"
#include "curve.h"
#include "utilities.h"
#include "Transform.h"
#include "Instance.h"
#include <SketchUpAPI/model/axes.h>
#include "Axis.cpp"
#include <SketchUpAPI/model/entity.h>
#include <SketchUpAPI/model/attribute_dictionary.h>

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace SketchUpNET
{
	public ref class Component
	{
	public:
		System::String^ Name;
		System::String^ Description;
		List<Surface^>^ Surfaces;
		List<Instance^>^ Instances;
		System::String^ Guid;
		List<Curve^>^ Curves;
		List<Edge^>^ Edges;
		List<Group^>^ Groups;
		Axis^ ComponetAxis;
		System::Collections::Generic::Dictionary<String^, String^>^ Atts;

		Component(System::String^ name, System::String^ guid, List<Surface^>^ surfaces, List<Curve^>^ curves,
			List<Edge^>^ edges, List<Instance^>^ instances, System::String^ desc, List<Group^>^ groups, Axis^ axis,
			System::Collections::Generic::Dictionary<String^, String^>^ atts)
		{
			this->Name = name;
			this->Surfaces = surfaces;
			this->Guid = guid;
			this->Curves = curves;
			this->Edges = edges;
			this->Description = desc;
			this->Instances = instances;
			this->Groups = groups;
			this->ComponetAxis = axis;
			this->Atts = atts;
		};

		Component() {};
	internal:
		static Component^ FromSU(SUComponentDefinitionRef comp, bool includeMeshes, System::Collections::Generic::Dictionary<String^, Material^>^ materials)
		{
			SUStringRef name = SU_INVALID;
			SUStringCreate(&name);
			SUComponentDefinitionGetName(comp, &name);

			SUModelRef model = SU_INVALID;
			SUAxesRef axes;
			SUEntityRef entity = SUComponentDefinitionToEntity(comp);
			SUEntityGetModel(entity, &model);
			SUModelGetAxes(model, &axes);




			SUStringRef desc = SU_INVALID;
			SUStringCreate(&desc);
			SUComponentDefinitionGetDescription(comp, &desc);

			SUEntitiesRef entities = SU_INVALID;
			SUComponentDefinitionGetEntities(comp, &entities);

			size_t faceCount = 0;
			SUEntitiesGetNumFaces(entities, &faceCount);

			SUStringRef guid = SU_INVALID;
			SUStringCreate(&guid);
			SUComponentDefinitionGetGuid(comp, &guid);

			List<Surface^>^ surfaces = Surface::GetEntitySurfaces(entities, includeMeshes, materials);
			List<Curve^>^ curves = Curve::GetEntityCurves(entities);
			List<Edge^>^ edges = Edge::GetEntityEdges(entities);
			List<Instance^>^ instances = Instance::GetEntityInstances(entities);
			List<Group^>^ grps = Group::GetEntityGroups(entities, includeMeshes, materials);
			Axis^ axis = Axis::FromSU(axes);




			System::Collections::Generic::Dictionary<String^, String^>^ atts = gcnew System::Collections::Generic::Dictionary<String^, String^>();
			size_t attcount;
			SUEntityGetNumAttributeDictionaries(entity, &attcount);

			if (attcount > 0)
			{
				SUAttributeDictionaryRef dictionaries[100];

				SUEntityGetAttributeDictionaries(entity, attcount, dictionaries, &attcount);
				for (size_t i = 0; i < attcount; i++)
				{
					//size_t sattcount;
					//SUStringRef keys[100];
					//for (int SAttributeKeNo = 0; SAttributeKeNo < 100; SAttributeKeNo++)
					//{
					//	SUStringCreate(&keys[SAttributeKeNo]); // we can ignore the returned SU_RESULT
					//}
					//SUAttributeDictionaryGetKeys(dictionaries[i], 100, keys, &sattcount);

					SUStringRef aname = SU_INVALID;
					SUStringCreate(&aname);
					SUAttributeDictionaryGetName(dictionaries[i], &aname);
					atts->Add(i.ToString(), Utilities::GetString(aname));
					SUStringRelease(&aname);
				}
			}


			Component^ v = gcnew Component(Utilities::GetString(name), Utilities::GetString(guid), surfaces, curves, edges, instances, Utilities::GetString(desc), grps, axis, atts);

			return v;
		};

		//static void FDatasmithSketchUpMetadata(SUEntityRef InSEntityRef)
		//{
		//	// Get the number of attribute dictionaries in the SketchUp entity.
		//	size_t SAttributeDictionaryCount = 0;
		//	SUEntityGetNumAttributeDictionaries(InSEntityRef, &SAttributeDictionaryCount); // we can ignore the returned SU_RESULT

		//	if (SAttributeDictionaryCount > 0)
		//	{
		//		// Retrieve the attribute dictionaries in the SketchUp entity.
		//		SUAttributeDictionaryRef SAttributeDictionaries[100];
		//		SUEntityGetAttributeDictionaries(InSEntityRef, SAttributeDictionaryCount, SAttributeDictionaries, &SAttributeDictionaryCount); // we can ignore the returned SU_RESULT
		//		// Retrieve the key-value pairs of the SketchUp attribute dictionaries.
		//		for (SUAttributeDictionaryRef SAttributeDictionaryRef : SAttributeDictionaries)
		//		{
		//			ScanAttributeDictionary(SAttributeDictionaryRef);
		//		}
		//	}

		//	if (SUEntityGetType(InSEntityRef) == SURefType::SURefType_ComponentInstance)
		//	{
		//		// Retrieve the classification info from the SketckUp component instance.
		//		SUClassificationInfoRef SClassificationInfoRef = SU_INVALID;
		//		SUResult SResult = SUComponentInstanceCreateClassificationInfo(SUComponentInstanceFromEntity(InSEntityRef), &SClassificationInfoRef);
		//		// Make sure the SketckUp component instance is classified (no SU_ERROR_NO_DATA).
		//		if (SResult == SU_ERROR_NONE)
		//		{
		//			// Get the number of schemas in the SketchUp classification info.
		//			size_t SSchemaCount = 0;
		//			SUClassificationInfoGetNumSchemas(SClassificationInfoRef, &SSchemaCount);

		//			for (int32 SSchemaNo = 0; SSchemaNo < SSchemaCount; SSchemaNo++)
		//			{
		//				// Retrieve the SketchUp classification schema attribute.
		//				SUClassificationAttributeRef SSchemaAttributeRef = SU_INVALID;
		//				SUClassificationInfoGetSchemaAttribute(SClassificationInfoRef, SSchemaNo, &SSchemaAttributeRef); // we can ignore the returned SU_RESULT

		//				// Retrieve the key-value pairs of the SketchUp classification schema.
		//				ScanClassificationSchema(SSchemaAttributeRef);
		//			}

		//			// Release the SketchUp component classification info.
		//			SUClassificationInfoRelease(&SClassificationInfoRef); // we can ignore the returned SU_RESULT
		//		}
		//	}
		//}


	};

	static void ScanAttributeDictionary(SUAttributeDictionaryRef InSAttributeDictionaryRef)
	{
		//// Retrieve the SketchUp attribute dictionary name.
		//SUStringRef SAttributeDictionaryName;
		//SUAttributeDictionaryGetName(InSAttributeDictionaryRef, &SAttributeDictionaryName);

		//// Skip uninteresting SketchUp attribute dictionaries.
		////if (SAttributeDictionaryName.IsEmpty() || !InterestingAttributeDictionarySet.Contains(SAttributeDictionaryName))
		////{
		////	return;
		////}

		//// Get the number of keys in the SketchUp attribute dictionary.
		//size_t SAttributeKeyCount = 0;
		//SUAttributeDictionaryGetNumKeys(InSAttributeDictionaryRef, &SAttributeKeyCount); // we can ignore the returned SU_RESULT

		//if (SAttributeKeyCount > 0)
		//{
		//	// Retrieve the keys in the SketchUp attribute dictionary.
		//	SUStringRef SAttributeKeys[200];

		//	for (int SAttributeKeNo = 0; SAttributeKeNo < SAttributeKeyCount; SAttributeKeNo++)
		//	{
		//		SUStringCreate(&SAttributeKeys[SAttributeKeNo]); // we can ignore the returned SU_RESULT
		//	}

		//	SUAttributeDictionaryGetKeys(InSAttributeDictionaryRef, SAttributeKeyCount, SAttributeKeys, &SAttributeKeyCount); // we can ignore the returned SU_RESULT

		//	for (SUStringRef SAttributeKeyRef : SAttributeKeys)
		//	{
		//		SUStringRef SAttributeKey;
		//		SU_USE_STRING(SAttributeKeyRef, SAttributeKey);

		//		// Retrieve the value associated with the key from the SketchUp attribute dictionary.
		//		SUTypedValueRef STypedValueRef = SU_INVALID;
		//		SUTypedValueCreate(&STypedValueRef); // we can ignore the returned SU_RESULT
		//		SUResult SResult = SUAttributeDictionaryGetValue(InSAttributeDictionaryRef, TCHAR_TO_UTF8(*SAttributeKey), &STypedValueRef);
		//		// Make sure there is a value associated with the given key (no SU_ERROR_NO_DATA).
		//		if (SResult == SU_ERROR_NONE)
		//		{
		//			SUStringRef SAttributeValue = GetAttributeValue(STypedValueRef);

		//			if (!SAttributeValue.IsEmpty())
		//			{
		//				// Add the SketchUp attribute key-value pair to our metadata dictionary.
		//				MetadataKeyValueMap.Add(FString::Printf(TEXT("%ls:%ls"), *SAttributeDictionaryName, *SAttributeKey), SAttributeValue);
		//			}
		//		}

		//		// Release the SketchUp attribute key and value.
		//		SUStringRelease(&SAttributeKeyRef); // we can ignore the returned SU_RESULT
		//		SUTypedValueRelease(&STypedValueRef); // we can ignore the returned SU_RESULT
		//	}
		//}
	}


}