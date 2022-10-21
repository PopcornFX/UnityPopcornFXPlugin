//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Text;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	[StructLayout(LayoutKind.Sequential)]
	public struct SMeshSamplerBaked
	{
		public IntPtr m_PkmmFileContent;
		public int m_PkmmFileContentSize;
		public int m_SubmeshId;
		public int m_BonesCount;
		public bool m_IsSkinned;
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SFxDesc
	{
		public string m_FxPath;
		public Matrix4x4 m_Transforms;
		public bool m_UsesMeshRenderer;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SCamDesc
	{
		public Matrix4x4 m_ViewMatrix;
		public Matrix4x4 m_ProjectionMatrix;
		public uint m_ViewportWidth;
		public uint m_ViewportHeight;
		public int m_RenderPass;
		public float m_NearClip;
		public float m_FarClip;
	}

	public enum EMeshVertexAttributes : int
	{
		Attrib_Normal = 0,
		Attrib_Tangent = 1,
		Attrib_Color = 2,
		Attrib_Texcoord = 3,
		Attrib_BonesIndicesAndWeights = 4,
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SPopcornFxSettings
	{
		public int m_DeviceType;

		public bool m_EnableFileLog;

		public bool m_EnableRaycastForCollisions;
		public bool m_DisableDynamicEffectBounds;
		public bool m_EnableGPUBillboarding;

		public bool m_EnableLocalizedPages;
		public bool m_EnableLocalizedByDefault;

		public bool m_FreeUnusedBatches;
		public uint m_FrameCountBeforeFreeingUnusedBatches;

		public bool m_IsUnitTesting;

		// Threading
		public bool m_SingleThreadedExecution;
		public bool m_OverrideThreadPool;
		public int m_WorkerCount;
		public IntPtr m_WorkerAffinities;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SMeshDataToFill
	{
		// ptr to the vertex/index buffers
		public int m_IdxCount;
		public IntPtr m_Indices;
		public int m_VtxCount;
		public IntPtr m_Positions;
		public IntPtr m_Normals;
		public IntPtr m_Tangents;
		public IntPtr m_UVs;
		public IntPtr m_Colors;

		// Skinning info
		public IntPtr m_VtxBonesWeights;
		public IntPtr m_VtxBonesIndices;
		public int m_BonesCount;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SCurveSamplerToFill
	{
		public IntPtr m_KeyPoints;
		public int m_CurveDimension;
		public int m_KeyPointsCount;
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct STextureSamplerToFill
	{
		public IntPtr m_TextureData;
		public int m_Width;
		public int m_Height;
		public int m_SizeInBytes;
		public int m_PixelFormat;
		public int m_WrapMode;
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SStats
	{
		public float m_UpdateTime;
		public float m_RenderTime;
		public int m_TotalMemoryFootprint;
		public int m_TotalParticleMemory;
		public int m_UnusedParticleMemory;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SMirrorPackFxSettings
	{
		public string m_PopcornPackFxPath;
		public string m_UnityPackFxPath;
	}

	internal partial class PKFxManagerImpl : object
	{
		public const UInt32 POPCORN_MAGIC_NUMBER = 0x5AFE0000;

		#region plugin name and calling convention

#if (UNITY_IOS || UNITY_SWITCH) && !UNITY_EDITOR
	private const string kPopcornPluginName = "__Internal";
#elif UNITY_XBOXONE && !UNITY_EDITOR
	private const string kPopcornPluginName = "PK-UnityPlugin_XBoxOne";
#elif UNITY_GAMECORE_SCARLETT && !UNITY_EDITOR
	private const string kPopcornPluginName = "PK-UnityPlugin_XBoxSeries";
#else
		private const string kPopcornPluginName = "PK-UnityPlugin";
#endif

#if (UNITY_WINRT || UNITY_SWITCH) && !UNITY_EDITOR
	public const CallingConvention kCallingConvention = CallingConvention.Cdecl;
#else
		public const CallingConvention kCallingConvention = CallingConvention.Winapi;
#endif
		#endregion

#if UNITY_SWITCH && !UNITY_EDITOR
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		protected static extern void RegisterPlugin();
#endif
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr GetRenderEventFunc();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern int PopcornFXDllLoaded();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void PopcornFXStartup(ref SPopcornFxSettings settings);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void PopcornFXShutdown();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetMaxLogStack(int maxLogStack);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern int UnstackLog(StringBuilder dstBuffer, int dstSize, ref int logSeverity);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetMaxCameraCount(int number);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void UpdateCamDesc(int camID, ref SCamDesc desc, bool update);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void UpdateParticles(float DT);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SyncParticlesSimulation();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void TransformAllParticles(Vector3 worldOffset);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void GetStats(ref SStats stats);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void PreloadFxIFN(string path, int usesMeshRenderer);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern int InstantiateFx(ref SFxDesc fxDesc);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern int StartFx(int guid, float dt, float prewarm);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool TerminateFx(int guid);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool StopFx(int guid);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool KillFx(int guid);
		// Exported Event
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool RegisterExportedEvent(int guid, string eventName, int unityKey);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool UnregisterExportedEvent(int guid, string eventName, int unityKey);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void UnregisterEffectAllExportedEvent(int guid);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void UnregisterSceneAllExportedEvent();

		// For the attributes:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr EffectGetAttributesBuffer(int guid);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectUpdateAttributes(int guid);

		// For the samplers shape:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetSamplerShapeTransform(int guid, int samplerId, Matrix4x4 transform);
		// Mesh sampler:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr GetMeshDataToFillFromSampler(int vertexCount, int indexCount, int bonesCount, int samplingInfo);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetMeshSampler(int guid, int samplerId, IntPtr meshSampler, Vector3 size, bool async);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetMeshSamplerBaked(int guid, int samplerId, ref SMeshSamplerBaked meshSamplerBaked, Vector3 size, bool async);
		// Skinning:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr EffectUpdateSamplerSkinningSetMatrices(int guid, int samplerId);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectBeginUpdateSamplerSkinning(int guid, int samplerId, float dt);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectEndUpdateSamplerSkinning(int guid, int samplerId);
		// Other sampler shapes:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetSamplerShape(int guid, int samplerId, Sampler.EShapeType shapeType, Vector3 size);
		// Sampler curve:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr GetCurveSamplerData(int keyPointsCount, int curveDimension);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetCurveSampler(int guid, int samplerId, IntPtr curveSampler);
		// Sampler texture:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr GetTextureSamplerData(int byteSize);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetTextureSampler(int guid, int samplerId, IntPtr textureSampler);
		// Sampler text:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetTextSampler(int guid, int samplerId, string text);
		// Any sampler:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectResetDefaultSampler(int guid, int samplerId);
		// ----------------------------------

		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool EffectSetTransforms(int guid, Matrix4x4 tranforms);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool LoadPkmmAsSceneMesh(string pkmmVirtualPath);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SceneMeshClear();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void WriteProfileReport(string path);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void ProfilerSetEnable(bool enable);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void Reset();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void DeepReset();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void UnloadFx(string path);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void ClearAllCallbacks();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr GetRuntimeVersion();

#if UNITY_EDITOR
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool PopcornFXEditorStartup();
		// Browse an effect content to create the Unity asset:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool BrowseEffectContent(IntPtr pkfxContentPtr, int contentByteSize, string path);

		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]

		public static extern bool SetPackSettings(ref SMirrorPackFxSettings settings);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool StartPackWatcher();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void PausePackWatcher();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void RestartPackWatcher();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool UnstackPackWatcherChanges(out int assetRemaining);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void CancelPackWatcherChanges();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void LockPackWatcher();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void UnlockPackWatcher();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void GetAllAssetPath();
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetForceDeterminismOnBakeNative(bool enable);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void ReimportAssets(int size, string[] ar);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern IntPtr GetMeshDataToFillFromAttribs(int vtxCount, int idxCount, int meshVertexAttributes);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool NextSubmeshToBake(string submeshName, int meshVertexAttributes);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool AppendMeshToBake(IntPtr mesh, Matrix4x4 transform);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern bool BakeMeshes(string outputPkmmVirtualPath);
#endif
		//----------------------------------------------------------------------------

		private const string m_UnityVersion = "Unity 2019.4 and up";
		public const string m_PluginVersion = "2.14.1 for " + m_UnityVersion;
		public static string m_CurrentVersionString = "";
		public static bool		m_IsStarted = false;
		public static string	m_DistortionLayer = "PopcornFX_Disto";

		public static GameObject m_RenderersRoot = null;
		public static int m_CurrentRenderersGUID = 0;
		public static List<SMeshDesc> m_Renderers = new List<SMeshDesc>();
		public static GameObject m_RuntimeAssetsRoot = null;
		public static List<GameObject> m_RuntimeAssets = new List<GameObject>();

		public static float[] m_Samples;
		public static GCHandle m_SamplesHandle;

		//----------------------------------------------------------------------------

		public static string[] s_CustomFileTypes = { ".pkat", ".pkfx", ".pkmm", ".pkfm", ".pksc", ".pkan", ".pkvf" };
		public static string[] s_TexFileTypes = { ".dds", ".png", ".jpg", ".jpeg", ".tga", ".exr", ".hdr", ".tif", ".tiff", ".pkm", ".pvr" };
		public static string[] s_MeshFileTypes = { ".fbx" };
		public static string[] s_SoundFileTypes = { ".mp3", ".wav", ".ogg" };
		public static string[] s_SimcacheFileTypes = { ".pksc" };

		//----------------------------------------------------------------------------

		// Resource dependencies:

		public struct STextureDependency
		{
			public string m_Path;
			public Texture2D m_Texture;

			public STextureDependency(string path, Texture2D tex)
			{
				m_Path = path;
				m_Texture = tex;
			}
			public override bool Equals(object oth)
			{
				if (!(oth is STextureDependency))
					return false;
				STextureDependency sTextureDependency = (STextureDependency)oth;
				return m_Path == sTextureDependency.m_Path && m_Texture == sTextureDependency.m_Texture;
			}

			public override int GetHashCode()
			{
				return m_Path.GetHashCode() ^ m_Texture.GetHashCode();
			}
		}

		public static List<PKFxAsset> m_Dependencies = new List<PKFxAsset>();
		public static List<STextureDependency> m_TexDependencies = new List<STextureDependency>();
		public static List<PinnedData> m_NeedsFreeing;

		public static PKFxEffectAsset m_CurrentlyImportedAsset = null;
		public static PKFxEffectAsset m_CurrentlyBuildAsset = null;

		public static PKFxDelegateHandler DelegateHandlerInstance { get; internal set; }

		//----------------------------------------------------------------------------

		public static bool FillUnityMeshData(IntPtr unityMeshDataPtr, Mesh mesh)
		{
			int[] indices = mesh.triangles;
			int vertexCount = mesh.vertexCount;

			unsafe
			{
				SMeshDataToFill* meshToFill = (SMeshDataToFill*)unityMeshDataPtr.ToPointer();

				if (meshToFill == null)
				{
					Debug.LogError("[PopcornFX] Could not create the mesh data");
					return false;
				}
				else if (meshToFill->m_IdxCount != mesh.triangles.Length || meshToFill->m_VtxCount != vertexCount)
				{
					Debug.LogError("[PopcornFX] Index count and vertex count does not match with the allocated mesh");
					return false;
				}

				Vector3* positions = (Vector3*)meshToFill->m_Positions.ToPointer();
				Vector3* normals = (Vector3*)meshToFill->m_Normals.ToPointer();
				Vector4* tangents = (Vector4*)meshToFill->m_Tangents.ToPointer();
				Vector2* uvs = (Vector2*)meshToFill->m_UVs.ToPointer();
				Vector4* colors = (Vector4*)meshToFill->m_Colors.ToPointer();
				float* bonesWeights = (float*)meshToFill->m_VtxBonesWeights.ToPointer();
				int* bonesIndices = (int*)meshToFill->m_VtxBonesIndices.ToPointer();

				// We use Marshal.Copy as often as possible
				// Indices:
				Marshal.Copy(indices, 0, meshToFill->m_Indices, indices.Length);
				// Positions:
				if (positions != null)
				{
					Vector3[] srcPositions = mesh.vertices;
					for (int i = 0; i < vertexCount; ++i)
					{
						positions[i] = srcPositions[i];
					}
				}
				else
				{
					Debug.LogError("[PopcornFX] Could not copy the mesh positions");
					return false;
				}
				// Normals:
				if (normals != null)
				{
					Vector3[] srcNormals = mesh.normals;
					for (int i = 0; i < vertexCount; ++i)
					{
						normals[i] = srcNormals[i];
					}
				}
				// Tangents (could be copied using Marshal.Copy but does not handle Vector4[]):
				if (tangents != null)
				{
					Vector4[] srcTangents = mesh.tangents;
					for (int i = 0; i < vertexCount; ++i)
					{
						tangents[i] = srcTangents[i];
					}
				}
				// UVs (could be copied using Marshal.Copy but does not handle Vector2[]):
				if (uvs != null)
				{
					Vector2[] srcUvs = mesh.uv;
					for (int i = 0; i < vertexCount; ++i)
					{
						uvs[i] = srcUvs[i];
					}
				}
				// Colors (could be copied using Marshal.Copy but does not handle Vector4[]):
				if (colors != null)
				{
					Color[] srcColors = mesh.colors;
					for (int i = 0; i < vertexCount; ++i)
					{
						colors[i] = srcColors[i];
					}
				}

				// Bones
				if (bonesWeights != null && bonesIndices != null)
				{
					BoneWeight[] boneWeightsSrc = mesh.boneWeights;

					// Test native code:
					for (int i = 0; i < vertexCount; ++i)
					{
						BoneWeight boneWeight = boneWeightsSrc[i];

						bonesWeights[i * 4 + 0] = boneWeight.weight0;
						bonesWeights[i * 4 + 1] = boneWeight.weight1;
						bonesWeights[i * 4 + 2] = boneWeight.weight2;
						bonesWeights[i * 4 + 3] = boneWeight.weight3;

						bonesIndices[i * 4 + 0] = boneWeight.boneIndex0;
						bonesIndices[i * 4 + 1] = boneWeight.boneIndex1;
						bonesIndices[i * 4 + 2] = boneWeight.boneIndex2;
						bonesIndices[i * 4 + 3] = boneWeight.boneIndex3;
					}
				}
			}
			return true;
		}

		//----------------------------------------------------------------------------

		public static void _Render(short cameraID)
		{
			if (cameraID >= 0)
			{
				UInt32 eventID = ((UInt16)cameraID | POPCORN_MAGIC_NUMBER);
				GL.IssuePluginEvent(GetRenderEventFunc(), (int)eventID);
			}
			else
				Debug.LogError("[PopcornFX] PKFxManager: invalid cameraID for rendering " + cameraID);
		}

		//----------------------------------------------------------------------------

		public static bool _IsDllLoaded()
		{
			try
			{
				PopcornFXDllLoaded();
			}
			catch (Exception)
			{
				return false;
			}
			return true;
		}

		public static void Startup()
		{
			PKFxDelegateHandler delegateHandler = PKFxDelegateHandler.Instance;

#if UNITY_SWITCH && !UNITY_EDITOR
			RegisterPlugin();
#endif
			delegateHandler.DiscardAllDelegatesRefs();

			SetDelegateOnResourceLoad(delegateHandler.DelegateToFunctionPointer(new ResourceLoadCallback(OnResourceLoad)));
			SetDelegateOnResourceWrite(delegateHandler.DelegateToFunctionPointer(new ResourceWriteCallback(OnResourceWrite)));
			SetDelegateOnRaycastPack(delegateHandler.DelegateToFunctionPointer(new PKFxRaycasts.RaycastPackCallback(PKFxRaycasts.OnRaycastPack)));
			SetDelegateOnFxStopped(delegateHandler.DelegateToFunctionPointer(new FxCallback(OnFxStopped)));
			SetDelegateOnRaiseEvent(delegateHandler.DelegateToFunctionPointer(new RaiseEventCallback(OnRaiseEvent)));
			SetDelegateOnAudioWaveformData(delegateHandler.DelegateToFunctionPointer(new AudioCallback(OnAudioWaveformData)));
			SetDelegateOnAudioSpectrumData(delegateHandler.DelegateToFunctionPointer(new AudioCallback(OnAudioSpectrumData)));
			SetDelegateOnSetupNewBillboardRenderer(delegateHandler.DelegateToFunctionPointer(new RendererSetupCallback(OnNewBillboardRendererSetup)));
			SetDelegateOnSetupNewRibbonRenderer(delegateHandler.DelegateToFunctionPointer(new RendererSetupCallback(OnNewRibbonRendererSetup)));
			SetDelegateOnSetupNewMeshRenderer(delegateHandler.DelegateToFunctionPointer(new RendererSetupCallback(OnNewMeshRendererSetup)));
			SetDelegateOnSetupNewTriangleRenderer(delegateHandler.DelegateToFunctionPointer(new RendererSetupCallback(OnNewTriangleRendererSetup)));
			SetDelegateOnResizeRenderer(delegateHandler.DelegateToFunctionPointer(new RendererResizeCallback(OnRendererResize)));
			SetDelegateOnSetParticleCount(delegateHandler.DelegateToFunctionPointer(new SetParticleCountCallback(OnSetParticleCount)));
			SetDelegateOnSetRendererActive(delegateHandler.DelegateToFunctionPointer(new SetRendererActiveCallback(OnSetRendererActive)));
			SetDelegateOnSetMeshInstancesCount(delegateHandler.DelegateToFunctionPointer(new SetMeshInstancesCountCallback(OnSetMeshInstancesCount)));
			SetDelegateOnSetMeshInstancesBuffer(delegateHandler.DelegateToFunctionPointer(new SetMeshInstancesBufferCallback(OnSetMeshInstancesBuffer)));
			SetDelegateOnRetrieveCustomMaterialInfo(delegateHandler.DelegateToFunctionPointer(new RetrieveCustomMaterialInfoCallback(OnRetrieveCustomMaterialInfo)));
			SetDelegateOnRetrieveRendererBufferInfo(delegateHandler.DelegateToFunctionPointer(new RetrieveRendererBufferInfoCallback(OnRetrieveRendererBufferInfo)));
			SetDelegateOnUpdateRendererBounds(delegateHandler.DelegateToFunctionPointer(new RendererBoundsUpdateCallback(OnRendererBoundsUpdate)));
			SetDelegateOnGetMeshCount(delegateHandler.DelegateToFunctionPointer(new GetMeshCountCallback(OnGetMeshCount)));
			SetDelegateOnGetMeshLODsCount(delegateHandler.DelegateToFunctionPointer(new GetMeshLODsCountCallback(OnGetMeshLODsCount)));
			SetDelegateOnGetMeshBounds(delegateHandler.DelegateToFunctionPointer(new GetMeshBoundsCallback(OnGetMeshBounds)));
#if UNITY_EDITOR
			SetDelegateOnEffectAboutToBake(delegateHandler.DelegateToFunctionPointer(new EffectAboutToBakeCallback(OnEffectAboutToBake)));
			SetDelegateOnAssetChange(delegateHandler.DelegateToFunctionPointer(new AssetChangeCallback(OnAssetChange)));
			SetDelegateOnEffectDependencyFound(delegateHandler.DelegateToFunctionPointer(new EffectDependencyFoundCallback(OnEffectDependencyFound)));
			SetDelegateOnEffectAttributeFound(delegateHandler.DelegateToFunctionPointer(new EffectAttributeFoundCallback(OnEffectAttributeFound)));
			SetDelegateOnEffectSamplerFound(delegateHandler.DelegateToFunctionPointer(new EffectSamplerFoundCallback(OnEffectSamplerFound)));
			SetDelegateOnEffectRendererFound(delegateHandler.DelegateToFunctionPointer(new EffectRendererFoundCallback(OnEffectRendererFound)));
			SetDelegateOnEffectEventFound(delegateHandler.DelegateToFunctionPointer(new EffectEventFoundCallback(OnEffectEventFound)));
			SetDelegateOnGetEffectInfo(delegateHandler.DelegateToFunctionPointer(new GetEffectInfoCallback(OnGetEffectInfo)));
			SetDelegateOnGetAllAssetPath(delegateHandler.DelegateToFunctionPointer(new GetAllAssetPathCallback(OnGetAllAssetPath)));
			// Startup the editor manager:
			PopcornFXEditorStartup();
#endif
			m_Samples = new float[1024];
			m_SamplesHandle = GCHandle.Alloc(m_Samples, GCHandleType.Pinned);
			m_CurrentVersionString = Marshal.PtrToStringAnsi(GetRuntimeVersion());
			m_IsStarted = true;
		}

#if UNITY_EDITOR
		public static void StartupPopcornFileWatcher(bool enableEffectHotReload)
		{
			SMirrorPackFxSettings packFxSettings;

			if (string.IsNullOrEmpty(PKFxSettings.PopcornPackFxPath))
				packFxSettings.m_PopcornPackFxPath = null;
			else
				packFxSettings.m_PopcornPackFxPath = Path.GetFullPath(PKFxSettings.PopcornPackFxPath);
			if (string.IsNullOrEmpty(PKFxSettings.PopcornPackFxPath))
				packFxSettings.m_UnityPackFxPath = null;
			else
				packFxSettings.m_UnityPackFxPath = PKFxSettings.UnityPackFxPath;

			SetPackSettings(ref packFxSettings);

			if (enableEffectHotReload)
				StartPackWatcher();
		}

		public static void LockPackWatcherChanges()
		{
			LockPackWatcher();
		}

		public static void UnlockPackWatcherChanges()
		{
			UnlockPackWatcher();
		}

		public static void SetForceDetermismOnBake(bool enable)
		{
			SetForceDeterminismOnBakeNative(enable);
		}
#endif

		//----------------------------------------------------------------------------

		private static void WalkDependencies(PKFxEffectAsset fxAsset)
		{
			foreach (PKFxEffectAsset.DependencyDesc depDesc in fxAsset.m_Dependencies)
			{
				if (depDesc.m_Object == null)
				{
					Debug.LogWarning("Dependency: \"" + depDesc.m_Path + "\" of Effect: \"" + fxAsset.AssetVirtualPath + "\" is null, Reimport your effect");
					return;
				}
				PKFxAsset depAsset = depDesc.m_Object as PKFxAsset;

				PKFxEffectAsset depFx = depDesc.m_Object as PKFxEffectAsset;
				Texture2D depTex = depDesc.m_Object as Texture2D;
				AudioClip depAudio = depDesc.m_Object as AudioClip;

				if (depFx != null && !m_Dependencies.Contains(depFx))
				{
					m_Dependencies.Add(depFx);
					WalkDependencies(depFx);
				}
				else if (depTex != null && !m_TexDependencies.Contains(new STextureDependency(depDesc.m_Path, depTex)))
					m_TexDependencies.Add(new STextureDependency(depDesc.m_Path, depTex));
				else if (depAudio != null)
					PKFxSoundManager.AddSound(depDesc);
				else if (depAsset != null && !m_Dependencies.Contains(depAsset))
					m_Dependencies.Add(depAsset);
			}
		}

		//----------------------------------------------------------------------------

		public static bool _PreloadFxDependencies(PKFxEffectAsset fxToPreload)
		{
			if (fxToPreload != null)
			{
				var loading = fxToPreload as PKFxAsset;
				Debug.Assert(fxToPreload.m_Data.Length > 0);
				Debug.Assert(loading.m_Data.Length > 0);
				if (!m_Dependencies.Contains(loading))
				{
					m_Dependencies.Add(loading);
					WalkDependencies(fxToPreload);
				}
				return true;
			}
			else
				Debug.LogError("[PopcornFX] Attempting to load null asset dependency.");
			return false;
		}

		//----------------------------------------------------------------------------

		public static bool PreloadSceneMesh(PKFxMeshAsset mesh)
		{
			if (mesh != null)
			{
				var loading = mesh as PKFxAsset;
				Debug.Assert(mesh.m_Data.Length > 0, "scene mesh " + mesh.AssetVirtualPath + " len " + mesh.m_Data.Length);
				if (!m_Dependencies.Contains(loading))
				{
					m_Dependencies.Add(loading);
				}
				return true;
			}
			else
				Debug.LogError("[PopcornFX] Attempting to load null scene mesh.");
			return false;
		}

		//----------------------------------------------------------------------------

		public static void UnloadAllFxDependencies()
		{
			// May be null when recompiling C#
			if (m_NeedsFreeing != null)
				m_NeedsFreeing.Clear();
			if (m_Dependencies != null)
				m_Dependencies.Clear();
			if (m_TexDependencies != null)
				m_TexDependencies.Clear();
		}

		//----------------------------------------------------------------------------

		public static void UnloadFxInDependencies(string path)
		{
			// May be null when recompiling C#
			if (m_Dependencies != null)
				m_Dependencies.RemoveAll(x => x.AssetVirtualPath == path);
		}

		//----------------------------------------------------------------------------

		public static void ClearRenderers()
		{
			m_CurrentRenderersGUID = 0;
			m_Renderers.Clear();
			if (m_RenderersRoot != null)
			{
				foreach (Transform t in m_RenderersRoot.transform)
				{
					Transform.DestroyImmediate(t.gameObject);
				}
				Transform.DestroyImmediate(m_RenderersRoot);
				m_RenderersRoot = null;
			}
		}


		//----------------------------------------------------------------------------

		public static GameObject GetNewRenderingObject(string name)
		{
			if (m_RenderersRoot == null)
			{
				m_RenderersRoot = new GameObject("PopcornFX Renderers");
				m_RenderersRoot.transform.position = Vector3.zero;
				UnityEngine.Object.DontDestroyOnLoad(m_RenderersRoot);
			}
			GameObject gameObject = new GameObject(name);
			gameObject.transform.parent = m_RenderersRoot.transform;
			return gameObject;
		}

		//----------------------------------------------------------------------------

		public static void SetupSliceInRenderingObject(SMeshDesc desc)
		{
			GameObject gameObject = desc.m_RenderingObject;

			gameObject.SetActive(false);

			desc.m_Slice = desc.m_RenderingObject.AddComponent<MeshFilter>();
			Mesh mesh = desc.m_Slice.mesh;

			mesh.MarkDynamic();
			mesh.Clear();

			// Load the default size configuration
			string particleMeshGeneratedName = desc.m_BatchDesc.m_GeneratedName;
			PKFxSettings.SParticleMeshDefaultSize meshSizeToUpdate = GetParticleMeshDefaultSizeSettings(particleMeshGeneratedName);

			if (meshSizeToUpdate != null)
			{
				uint vertexCount = (uint)meshSizeToUpdate.m_DefaultVertexBufferSize;
				uint indexCount = (uint)meshSizeToUpdate.m_DefaultIndexBufferSize;
				bool useLargeIndices = vertexCount > UInt16.MaxValue;

				ResizeParticleMeshBuffer(mesh, desc, vertexCount, indexCount, vertexCount, indexCount, useLargeIndices);
			}

			var renderer = gameObject.AddComponent<MeshRenderer>();
			renderer.material = desc.m_Material;

			PKFxSettings.MaterialFactory.SetupRenderer(desc.m_BatchDesc, gameObject, renderer);
		}

		//----------------------------------------------------------------------------

		public static void SetupProceduralInRenderingObject(SMeshDesc desc)
		{
			GameObject gameObject = desc.m_RenderingObject;

			gameObject.SetActive(false);

			desc.m_Procedural = desc.m_RenderingObject.AddComponent<PKFxProceduralRenderer>();
			desc.m_Procedural.Init(desc.m_BatchDesc, desc.m_Material);

			// Load the default size configuration
			string particleMeshGeneratedName = desc.m_BatchDesc.m_GeneratedName;
			PKFxSettings.SParticleMeshDefaultSize meshSizeToUpdate = GetParticleMeshDefaultSizeSettings(particleMeshGeneratedName);

			if (meshSizeToUpdate != null)
			{
				uint vertexCount = (uint)meshSizeToUpdate.m_DefaultVertexBufferSize;
				uint indexCount = (uint)meshSizeToUpdate.m_DefaultIndexBufferSize;

				desc.m_Procedural.ResizeParticleComputeBuffer(0, 0, 0, vertexCount, indexCount);
			}

			PKFxSettings.MaterialFactory.SetupRenderer(desc.m_BatchDesc, gameObject, null);
		}

		//----------------------------------------------------------------------------
		// Sets up the root gameobject for the different mesh "slices" with the same material.
		public static int SetupRenderingObject(GameObject renderingObject,
												SBatchDesc batchDesc,
												Material mat)
		{
			int newId = m_CurrentRenderersGUID++;

			renderingObject.name += " " + newId;

			var meshDesc = new SMeshDesc(mat, batchDesc, renderingObject);

			if (!PKFxSettings.UseGPUBillboarding || 
				batchDesc.m_Type == ERendererType.Ribbon || 
				batchDesc.m_Type == ERendererType.Triangle) // to improve
				SetupSliceInRenderingObject(meshDesc);
			else
				SetupProceduralInRenderingObject(meshDesc);

			m_Renderers.Add(meshDesc);

			return newId;
		}

		//----------------------------------------------------------------------------

		public static int SetupMeshRenderingObject(GameObject renderingObject, SBatchDesc batchDesc, Material mat)
		{
			int newId = m_CurrentRenderersGUID++;

			renderingObject.name += " " + newId;
			var renderer = renderingObject.AddComponent<PKFxMeshInstancesRenderer>();
			renderer.m_Material = mat;

			PKFxSettings.MaterialFactory.SetupMeshRenderer(batchDesc, renderingObject, renderer);

			if (renderer.Meshes.Length == 0)
				return -1;

			var filter = renderingObject.AddComponent<MeshFilter>();
			filter.mesh = renderer.Meshes[0].m_Mesh;

			m_Renderers.Add(new SMeshDesc(filter, mat, batchDesc, renderer, renderingObject));
			Debug.Assert(m_Renderers[newId].m_Slice.mesh == filter.mesh);

			return newId;
		}

		//----------------------------------------------------------------------------

		private static PKFxSettings.SParticleMeshDefaultSize GetParticleMeshDefaultSizeSettings(string meshName)
		{
			foreach (PKFxSettings.SParticleMeshDefaultSize meshConf in PKFxSettings.MeshesDefaultSize)
			{
				if (meshConf.m_GeneratedName == meshName)
					return meshConf; // The configuration already exists
			}
			return null;
		}

		public static bool SetMeshSamplerBaked(int effectGUID, int samplerId, SMeshSamplerBaked samplerBaked, PKFxMeshAsset pkmmFile, Vector3 size, bool async = false)
		{
			bool success;
			unsafe
			{
				fixed (byte* p = pkmmFile.m_Data)
				{
					samplerBaked.m_PkmmFileContent = (IntPtr)p;
					samplerBaked.m_PkmmFileContentSize = pkmmFile.m_Data.Length;
					// /!\ Need to call the C++ callback in the fixed scoped
					success = EffectSetMeshSamplerBaked(effectGUID, samplerId, ref samplerBaked, size, async);
				}
			}
			return success;
		}
	}
}
