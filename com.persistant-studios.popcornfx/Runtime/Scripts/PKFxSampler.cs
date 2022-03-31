//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.Serialization;

namespace PopcornFX
{
	public enum ESamplerType : int
	{
		SamplerShape = 0,
		SamplerCurve,
		SamplerImage,
		SamplerText,
		SamplerUnsupported
	};


	[Serializable]
	public class ShapeTransform
	{
		public Vector3 m_Position;
		public Quaternion m_Rotation;
		public Vector3 m_Scale;

		public ShapeTransform()
		{
			m_Position = Vector3.zero;
			m_Rotation = Quaternion.identity;
			m_Scale = Vector3.one;
		}

		public ShapeTransform(Transform transform)
		{
			m_Position = transform.position;
			m_Rotation = transform.rotation;
			m_Scale = transform.lossyScale;
		}

		public ShapeTransform(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			m_Position = position;
			m_Rotation = rotation;
			m_Scale = scale;
		}

		public Matrix4x4 transform
		{
			get
			{
				Matrix4x4 mat = new Matrix4x4();
				mat.SetTRS(m_Position, m_Rotation, m_Scale);
				return mat;
			}
		}

		public static ShapeTransform identity
		{
			get
			{
				return new ShapeTransform();
			}
		}
	}

	[Serializable]
	public class CurveDefaultValue
	{
		public AnimationCurve[] m_Curves;

		public CurveDefaultValue(int dimension, int keyCount, IntPtr curveTimes, IntPtr floatValues, IntPtr floatTangents)
		{
			if (keyCount != 0 && (curveTimes == IntPtr.Zero || floatValues == IntPtr.Zero || floatTangents == IntPtr.Zero))
				return;
			unsafe
			{
				float* curveTimesPtr = (float*)curveTimes.ToPointer();
				float* floatValuesPtr = (float*)floatValues.ToPointer();
				float* floatTangentsPtr = (float*)floatTangents.ToPointer();

				m_Curves = new AnimationCurve[dimension];

				for (int c = 0; c < dimension; ++c)
				{
					if (keyCount == 0)
					{
						m_Curves[c] = AnimationCurve.Linear(0, 0, 1, 0);
					}
					else
					{
						m_Curves[c] = new AnimationCurve();
						for (int k = 0; k < keyCount; ++k)
						{
							Keyframe keyframe = new Keyframe(curveTimesPtr[k], *floatValuesPtr, floatTangentsPtr[0], floatTangentsPtr[1]);
							m_Curves[c].AddKey(keyframe);

							floatValuesPtr += 1;
							floatTangentsPtr += 2;
						}
					}
				}
			}
		}
	}

	[Serializable]
	public class SamplerDesc
	{
		[FormerlySerializedAs("Type")] public ESamplerType m_Type;
		[FormerlySerializedAs("Name")] public string m_Name;
		[FormerlySerializedAs("Description")] public string m_Description;

		public enum ESamplerUsageFlags : int
		{
			UsageFlags_Custom = 1 << 0,
			UsageFlags_Mesh_Position = (UsageFlags_Custom) << 0,
			UsageFlags_Mesh_Velocity = (UsageFlags_Custom) << 1,
			UsageFlags_Mesh_Normal = (UsageFlags_Custom) << 2,
			UsageFlags_Mesh_Tangent = (UsageFlags_Custom) << 3,
			UsageFlags_Mesh_UV0 = (UsageFlags_Custom) << 4,
			UsageFlags_Mesh_UVN = (UsageFlags_Custom) << 5,
			UsageFlags_Mesh_Color0 = (UsageFlags_Custom) << 6,
			UsageFlags_Mesh_ColorN = (UsageFlags_Custom) << 7,
		}

		public static string UsageFlagsToString(int usageFlags)
		{
			string ret = "";
			int curUsage = (int)ESamplerUsageFlags.UsageFlags_Custom;

			while (curUsage <= (int)ESamplerUsageFlags.UsageFlags_Mesh_ColorN)
			{
				if ((usageFlags & curUsage) != 0)
				{
					if (ret.Length != 0)
						ret += ", ";
					ret += ((ESamplerUsageFlags)curUsage).ToString();
				}
				curUsage <<= 1;
			}
			return ret;
		}

		public int m_UsageFlags;

		// For shape samplers:
		public ShapeTransform m_ShapeDefaultTransform;
		public CurveDefaultValue m_CurveDefaultValue;

		// Copy constructor:
		public SamplerDesc(SamplerDesc desc)
		{
			m_Type = desc.m_Type;
			m_Name = desc.m_Name;
			m_Description = desc.m_Description;
			m_UsageFlags = desc.m_UsageFlags;
			m_ShapeDefaultTransform = desc.m_ShapeDefaultTransform;
		}

		// Build from native structure:
		public SamplerDesc(SNativeSamplerDesc desc)
		{
			m_Type = (ESamplerType)desc.m_SamplerType;
			m_Name = Marshal.PtrToStringAnsi(desc.m_SamplerName);
			m_Description = Marshal.PtrToStringAnsi(desc.m_Description);
			m_UsageFlags = desc.m_SamplerUsageFlags;
			m_ShapeDefaultTransform = new ShapeTransform(desc.m_ShapePosition, desc.m_ShapeRotation, Vector3.one);
			m_CurveDefaultValue = new CurveDefaultValue(desc.m_CurveDimension,
														desc.m_CurveKeyCount,
														desc.m_CurveTimes,
														desc.m_CurveFloatValues,
														desc.m_CurveFloatTangents);
		}

		public SamplerDesc(string name, ESamplerType type)
		{
			m_Type = type;
			m_Name = name;
			m_UsageFlags = 0;
		}

		public override string ToString()
		{
			// Just the type and the name are taken into account when mix and matching the samplers:
			return m_Type.ToString() + ";" + m_Name + ";" + m_UsageFlags + ";";
		}

		public override int GetHashCode()
		{
			return ToString().GetHashCode();
		}

		public override bool Equals(object other)
		{
			SamplerDesc desc = other as SamplerDesc;

			if (desc == null)
				return false;
			return this == desc;
		}

		public static bool operator ==(SamplerDesc first, SamplerDesc second)
		{
			return first.m_Type == second.m_Type && first.m_Name == second.m_Name;
		}

		public static bool operator !=(SamplerDesc first, SamplerDesc second)
		{
			return !(first == second);
		}
	}

	public class SamplerDescShapeBox
	{
		public ShapeTransform m_Transform;
		public Vector3 m_Dimensions;

		public SamplerDescShapeBox()
		{
			m_Transform = ShapeTransform.identity;
			m_Dimensions = Vector3.one;
		}

		public SamplerDescShapeBox(Vector3 dimension, ShapeTransform transform)
		{
			m_Transform = transform;
			m_Dimensions = dimension;
		}
	}

	public class SamplerDescShapeSphere
	{
		public ShapeTransform m_Transform;
		public float m_InnerRadius;
		public float m_Radius;

		public SamplerDescShapeSphere()
		{
			m_Transform = ShapeTransform.identity;
			m_InnerRadius = 0.0f;
			m_Radius = 1.0f;
		}

		public SamplerDescShapeSphere(float radius, float innerRadius, ShapeTransform transform)
		{
			m_Transform = transform;
			m_InnerRadius = innerRadius;
			m_Radius = radius;
		}
	}

	public class SamplerDescShapeCylinder
	{
		public ShapeTransform m_Transform;
		public float m_InnerRadius;
		public float m_Radius;
		public float m_Height;

		public SamplerDescShapeCylinder()
		{
			m_Transform = ShapeTransform.identity;
			m_InnerRadius = 1.0f;
			m_Radius = 1.0f;
			m_Height = 1.0f;
		}

		public SamplerDescShapeCylinder(float radius, float innerRadius, float height, ShapeTransform transform)
		{
			m_Transform = transform;
			m_InnerRadius = innerRadius;
			m_Radius = radius;
			m_Height = height;
		}
	}

	public class SamplerDescShapeCapsule
	{
		public ShapeTransform m_Transform;
		public float m_InnerRadius;
		public float m_Radius;
		public float m_Height;

		public SamplerDescShapeCapsule()
		{
			m_Transform = ShapeTransform.identity;
			m_InnerRadius = 1.0f;
			m_Radius = 1.0f;
			m_Height = 1.0f;
		}

		public SamplerDescShapeCapsule(float radius, float innerRadius, float height, ShapeTransform transform)
		{
			m_Transform = transform;
			m_InnerRadius = innerRadius;
			m_Radius = radius;
			m_Height = height;
		}
	}

	public class SamplerDescShapeBakedMesh
	{
		public ShapeTransform m_Transform;
		public PKFxMeshAsset m_BakedMesh;
		public int m_BakedMeshSubMeshId;
		public SkinnedMeshRenderer m_SkinnedMeshRenderer;
		public Vector3 m_Dimensions;
		public bool m_AsynchronousSet;

		public SamplerDescShapeBakedMesh()
		{
			m_Transform = ShapeTransform.identity;
			m_Dimensions = Vector3.one;
			m_BakedMesh = null;
			m_BakedMeshSubMeshId = 0;
			m_AsynchronousSet = false;
		}

		public SamplerDescShapeBakedMesh(Vector3 dimension,
											ShapeTransform transform,
											PKFxMeshAsset mesh,
											int subMeshId,
											SkinnedMeshRenderer skinnedMeshRenderer,
											bool asynchronousSet = false)
		{
			m_Dimensions = dimension;
			m_Transform = transform;
			m_BakedMesh = mesh;
			m_BakedMeshSubMeshId = subMeshId;
			m_SkinnedMeshRenderer = skinnedMeshRenderer;
			m_AsynchronousSet = asynchronousSet;
		}
	}

	public class SamplerDescShapeMesh
	{
		public ShapeTransform m_Transform;
		public Vector3 m_Dimensions;
		public Mesh m_Mesh;
		public bool m_AsynchronousSet;

		public SamplerDescShapeMesh()
		{
			m_Transform = ShapeTransform.identity;
			m_Dimensions = Vector3.one;
			m_Mesh = null;
			m_AsynchronousSet = false;
		}

		public SamplerDescShapeMesh(Vector3 dimension,
									ShapeTransform transform,
									Mesh mesh,
									bool asynchronousSet = false)
		{
			m_Transform = transform;
			m_Dimensions = dimension;
			m_Mesh = mesh;
			m_AsynchronousSet = asynchronousSet;
		}
	}

	public class SamplerDescShapeMeshFilter
	{
		public ShapeTransform m_Transform;
		public Vector3 m_Dimensions;
		public MeshFilter m_MeshFilter;
		public bool m_AsynchronousSet;

		public SamplerDescShapeMeshFilter()
		{
			m_Transform = ShapeTransform.identity;
			m_Dimensions = Vector3.one;
			m_MeshFilter = null;
			m_AsynchronousSet = false;
		}

		public SamplerDescShapeMeshFilter(Vector3 dimension,
											ShapeTransform transform,
											MeshFilter mesh,
											bool asynchronousSet = false)
		{
			m_Transform = transform;
			m_Dimensions = dimension;
			m_MeshFilter = mesh;
			m_AsynchronousSet = asynchronousSet;
		}
	}

	public class SamplerDescShapeSkinnedMesh
	{
		public ShapeTransform m_Transform;
		public Vector3 m_Dimensions;
		public SkinnedMeshRenderer m_SkinnedMesh;
		public bool m_AsynchronousSet;

		public SamplerDescShapeSkinnedMesh()
		{
			m_Transform = ShapeTransform.identity;
			m_Dimensions = Vector3.one;
			m_SkinnedMesh = null;
			m_AsynchronousSet = false;
		}

		public SamplerDescShapeSkinnedMesh(Vector3 dimension,
											ShapeTransform transform,
											SkinnedMeshRenderer skinnedMesh,
											bool asynchronousSet = false)
		{
			m_Dimensions = dimension;
			m_Transform = transform;
			m_SkinnedMesh = skinnedMesh;
			m_AsynchronousSet = asynchronousSet;
		}
	}

	[System.Serializable]
	public class Sampler
	{
		public enum ETexcoordMode : int
		{
			Clamp = 0,
			Wrap
		}

		public enum EShapeType : int
		{
			BoxShape = 0,
			SphereShape,
			CylinderShape,
			CapsuleShape,
			MeshShape,
			MeshFilterShape,
			SkinnedMeshShape,
			BakedMeshShape,
			ShapeUnsupported
		}

		public SamplerDesc m_Descriptor = null;
		// For sampler shape:
		public EShapeType m_ShapeType = EShapeType.ShapeUnsupported;

		// Shape transform:
		public Transform m_ShapeTransformReference = null;
		public ShapeTransform m_ShapeTransform = ShapeTransform.identity;

		public Vector3 m_Dimensions = Vector3.one;
		public Vector3 Dimensions
		{
			get { return m_Dimensions; }
			set
			{
				if (m_Dimensions != value)
				{
					m_Dimensions = value;
					m_WasModified = true;
				}
			}
		}

		// For shape mesh:
		public bool m_AsynchronousSet = false;  // if true, the sampler will be set over multiple frames
		public PKFxMeshAsset m_BakedMesh;
		public int m_BakedMeshSubMeshId = 0;
		public Mesh m_Mesh;
		public MeshFilter m_MeshFilter;
		public SkinnedMeshRenderer m_SkinnedMeshRenderer;

		// For sampler image:
		public Texture2D m_Texture;
		public ETexcoordMode m_TextureTexcoordMode;

		// For sampler curve:
		public bool m_CurveIsOverride = false; // We have the m_CurveIsOverride because the curve is always filled
		public AnimationCurve[] m_CurvesArray;
		public float[] m_CurvesTimeKeys;

		// For sampler text:
		public string m_Text = "";

		// Was modified:
		public bool m_WasModified = true;

		// Just used to display the shape in the editor:
		public bool m_WorldSpaceSampling = false;

		public Sampler(SamplerDesc dsc)
		{
			m_Descriptor = dsc;
			m_ShapeType = EShapeType.ShapeUnsupported;
			m_ShapeTransform = dsc.m_ShapeDefaultTransform;
			// Set the curve default values:
			m_CurveIsOverride = false;
			UpdateDefaultCurveValueIFN(dsc);
		}

		public Sampler(string name, SamplerDescShapeBox dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_Dimensions = dsc.m_Dimensions;
			m_ShapeTransform = dsc.m_Transform;
			m_ShapeType = (int)EShapeType.BoxShape;
		}

		public Sampler(string name, SamplerDescShapeSphere dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_Dimensions = new Vector3(dsc.m_Radius, dsc.m_InnerRadius);
			m_Dimensions.y = Mathf.Min(m_Dimensions.x, m_Dimensions.y);
			m_Dimensions.x = Mathf.Max(m_Dimensions.x, m_Dimensions.y);
			m_ShapeTransform = dsc.m_Transform;
			m_ShapeType = EShapeType.SphereShape;
		}

		public Sampler(string name, SamplerDescShapeCylinder dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_ShapeTransform = dsc.m_Transform;
			m_Dimensions = new Vector3(dsc.m_Radius, dsc.m_InnerRadius, dsc.m_Height);
			m_Dimensions.y = Mathf.Min(m_Dimensions.x, m_Dimensions.y);
			m_Dimensions.x = Mathf.Max(m_Dimensions.x, m_Dimensions.y);
			m_ShapeType = EShapeType.CylinderShape;
		}

		public Sampler(string name, SamplerDescShapeCapsule dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_ShapeTransform = dsc.m_Transform;
			m_Dimensions = new Vector3(dsc.m_Radius, dsc.m_InnerRadius, dsc.m_Height);
			m_Dimensions.y = Mathf.Min(m_Dimensions.x, m_Dimensions.y);
			m_Dimensions.x = Mathf.Max(m_Dimensions.x, m_Dimensions.y);
			m_ShapeType = EShapeType.CapsuleShape;
		}

		public Sampler(string name, SamplerDescShapeBakedMesh dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_Dimensions = dsc.m_Dimensions;
			m_AsynchronousSet = dsc.m_AsynchronousSet;
			m_BakedMesh = dsc.m_BakedMesh;
			m_BakedMeshSubMeshId = dsc.m_BakedMeshSubMeshId;
			m_Mesh = null;
			m_MeshFilter = null;
			m_SkinnedMeshRenderer = dsc.m_SkinnedMeshRenderer;
			m_ShapeTransform = dsc.m_Transform;
			m_ShapeType = EShapeType.BakedMeshShape;
		}

		public Sampler(string name, SamplerDescShapeMesh dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_Dimensions = dsc.m_Dimensions;
			m_AsynchronousSet = dsc.m_AsynchronousSet;
			m_BakedMesh = null;
			m_BakedMeshSubMeshId = 0;
			m_Mesh = dsc.m_Mesh;
			m_MeshFilter = null;
			m_SkinnedMeshRenderer = null;
			m_ShapeTransform = dsc.m_Transform;
			m_ShapeType = EShapeType.MeshShape;
		}

		public Sampler(string name, SamplerDescShapeMeshFilter dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_Dimensions = dsc.m_Dimensions;
			m_AsynchronousSet = dsc.m_AsynchronousSet;
			m_BakedMesh = null;
			m_BakedMeshSubMeshId = 0;
			m_MeshFilter = dsc.m_MeshFilter;
			m_Mesh = m_MeshFilter.sharedMesh;
			m_SkinnedMeshRenderer = null;
			m_ShapeTransform = dsc.m_Transform;
			m_ShapeType = EShapeType.MeshFilterShape;
		}

		public Sampler(string name, SamplerDescShapeSkinnedMesh dsc)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerShape);
			m_Dimensions = dsc.m_Dimensions;
			m_AsynchronousSet = dsc.m_AsynchronousSet;
			m_SkinnedMeshRenderer = dsc.m_SkinnedMesh;
			m_BakedMesh = null;
			m_BakedMeshSubMeshId = 0;
			m_Mesh = dsc.m_SkinnedMesh.sharedMesh;
			m_MeshFilter = null;
			m_ShapeTransform = dsc.m_Transform;
			m_ShapeType = EShapeType.SkinnedMeshShape;
		}

		public Sampler(string name, AnimationCurve[] curvesArray)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerCurve);
			m_CurvesArray = curvesArray;
			if (m_CurvesArray.Length != 0)
			{
				int iKey = 0;
				//To simplify, cpp code can handle adding keys, no need to keep C# curves sync in key number
				m_CurvesTimeKeys = new float[m_CurvesArray[0].keys.Length];
				foreach (var key in m_CurvesArray[0].keys)
				{
					m_CurvesTimeKeys[iKey++] = key.time;
				}
			}
			m_ShapeType = EShapeType.ShapeUnsupported;
			m_CurveIsOverride = true;
		}

		public Sampler(string name, Texture2D texture, ETexcoordMode texcoordMode)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerImage);
			m_Texture = texture;
			m_TextureTexcoordMode = texcoordMode;
			m_ShapeType = EShapeType.ShapeUnsupported;
		}

		public Sampler(string name, string text)
		{
			m_Descriptor = new SamplerDesc(name, ESamplerType.SamplerText);
			m_Text = text;
			m_ShapeType = EShapeType.ShapeUnsupported;
		}

		public void UpdateDefaultCurveValueIFN(SamplerDesc dsc)
		{
			if (!m_CurveIsOverride)
			{
				if (dsc.m_CurveDefaultValue == null || dsc.m_CurveDefaultValue.m_Curves == null)
				{
					Debug.LogWarning("[PopcornFX] Curve asset has no default value: you should re-import the effect asset that was not created correctly");
					m_CurveIsOverride = false;
					return;
				}
				int curveDimension = dsc.m_CurveDefaultValue.m_Curves.Length;
				if (curveDimension != 0)
				{
					m_CurvesArray = new AnimationCurve[curveDimension];
					for (int i = 0; i < m_CurvesArray.Length; ++i)
					{
						m_CurvesArray[i] = new AnimationCurve(dsc.m_CurveDefaultValue.m_Curves[i].keys);
					}
					m_CurvesTimeKeys = new float[m_CurvesArray[0].keys.Length];
					for (int i = 0; i < m_CurvesTimeKeys.Length; ++i)
					{
						m_CurvesTimeKeys[i] = m_CurvesArray[0].keys[i].time;
					}
					m_CurveIsOverride = false;
				}
			}
		}
	}
}
