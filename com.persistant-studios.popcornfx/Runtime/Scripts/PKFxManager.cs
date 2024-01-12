//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Text;
using UnityEngine;
using Unity.Collections.LowLevel.Unsafe;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	public class PKFxManager
	{
		public static string	ImportedAssetName { get; set; }
		public static string	ImportedAssetPath { get; set; }
		public static string 	ExtractedPkkg { get; set; }
		public static string	DistortionLayer { get { return PKFxManagerImpl.m_DistortionLayer; } }
		public static int		DistortionLayerID { get { return LayerMask.NameToLayer(PKFxManagerImpl.m_DistortionLayer); } }

		public static float					TimeMultiplier = 1.0f;
		public static int					MaxCameraSupport = 1;
		public static bool					UseFixedDT = false;
		public static bool					IsUnitTesting = false;
		public static PKFxRenderingPlugin	RenderingPlugin = null;

		public static PKFxLightPool			LightPool = null;

		private static Dictionary<string, AudioClip> m_SoundDependencies = new Dictionary<string, AudioClip>();
		public static PKFxAudioPool					 SoundPool = null;

		public static string				StoredQualityLevel = "Medium";
		public static string[]				QualitiesLevelDescription;
		


		//----------------------------------------------------------------------------
		// PopcornFX General State With Native Interop
		//----------------------------------------------------------------------------

#if UNITY_EDITOR
		public static void PublicSetDelegateOnAssetChange(IntPtr delegatePtr)
		{
			PKFxManagerImpl.SetDelegateOnAssetChange(delegatePtr);
		}

		//Should be called only when overriding PublicSetDelegateOnAssetChange
		public static void PublicOnAssetChange(ref SAssetChangesDesc desc)
		{
			PKFxManagerImpl.OnAssetChange(ref desc);
		}
#endif

		public enum EPopcornLogLevels
		{
			Level_Debug = 0,
			Level_Info,
			Level_Warning,
			Level_Error,
			Level_ErrorCritical,
			Level_None,
		};

		public static string PopcornFXVersion()
		{
			StartupPopcorn(false);
			return PKFxManagerImpl.m_CurrentVersionString;
		}

		public static bool IsDllLoaded()
		{
			return PKFxManagerImpl._IsDllLoaded();
		}

		public static void StartupPopcorn(bool forceRestart)
		{
			if (!PKFxManagerImpl.m_IsStarted || forceRestart)
			{
				SetupPopcornFxSettings(IsUnitTesting);
				PKFxManagerImpl.Startup();
			}
		}

		public static void ShutdownPopcorn()
		{
			PKFxManagerImpl.PopcornFXShutdown();
		}

		public static void SetMaxLogsPerFrame(int maxLogs)
		{
			PKFxManagerImpl.SetMaxLogStack(maxLogs);
		}

		public static int GetPopcornFXLogNumber()
		{
			if (!PKFxManagerImpl._IsDllLoaded())
				return 0;

			return PKFxManagerImpl.GetStackLogNumber();
		}

		public static int GetPopcornFXLog(ref string log, ref EPopcornLogLevels level)
		{
			if (!PKFxManagerImpl._IsDllLoaded())
				return 0;
			const int bufferSize = 0x1000;
			StringBuilder sb = new StringBuilder(bufferSize);

			int ilevel = 0;
			int logRemaining = PKFxManagerImpl.UnstackLog(sb, bufferSize, ref ilevel);

			log = sb.ToString();
			level = (EPopcornLogLevels)ilevel;

			return logRemaining;
		}

		public static void ResetAllEffects()
		{
			PKFxManagerImpl.Reset();
		}

		public static void ResetAndUnloadAllEffects()
		{
			PKFxManagerImpl.UnloadAllFxDependencies();
			PKFxManagerImpl.DeepReset();
		}

		public static bool IsSupportedTextureExtension(string extension)
		{
			return PKFxUtils.ArrayContains(PKFxManagerImpl.s_TexFileTypes, extension.ToLower());
		}
		public static bool IsSupportedMeshExtension(string extension)
		{
			return PKFxUtils.ArrayContains(PKFxManagerImpl.s_MeshFileTypes, extension.ToLower());
		}

		public static void ClearAllCallbacks()
		{
			PKFxManagerImpl.ClearAllCallbacks();
		}

		public static void ClearRenderers()
		{
			PKFxManagerImpl.ClearRenderers();
		}

		public static void SetSceneMesh(PKFxMeshAsset sceneMesh)
		{
			if (sceneMesh == null)
				PKFxManagerImpl.SceneMeshClear();
			else
			{
				PKFxManagerImpl.PreloadSceneMesh(sceneMesh);
				if (PKFxManagerImpl.LoadPkmmAsSceneMesh(sceneMesh.AssetVirtualPath))
					Debug.Log("[PopcornFX]  Scene Mesh loaded");
				else
					Debug.LogError("[PopcornFX]  Failed to load mesh " + sceneMesh + " as scene mesh");
				PKFxManagerImpl.UnloadAllFxDependencies();
			}
		}

#if UNITY_EDITOR
		public static bool AppendMeshToBakeList(Mesh mesh, Transform transform, string submeshName = "Default", int vertexAttribMask = ~0, bool newSubmesh = true)
		{
			return AppendMeshToBakeList(mesh, transform.localToWorldMatrix, submeshName, vertexAttribMask, newSubmesh);
		}

		public static bool AppendMeshToBakeList(Mesh mesh, Matrix4x4 localToWorldMatrix, string submeshName = "Default", int vertexAttribMask = ~0, bool newSubmesh = true)
		{
			int subMeshCount = mesh.subMeshCount;
			if (subMeshCount <= 0)
			{
				Debug.LogError("[PopcornFX] Mesh doesn't have sub meshes");
				return false;
			}

			int vertexCount = mesh.vertexCount;
			int indexCount = mesh.triangles.Length;

			// Get the vertex attributes
			int vertexAttribs = 0;
			vertexAttribs |= (mesh.normals.Length == vertexCount ? (1 << (int)EMeshVertexAttributes.Attrib_Normal) : 0);
			vertexAttribs |= (mesh.tangents.Length == vertexCount ? (1 << (int)EMeshVertexAttributes.Attrib_Tangent) : 0);
			vertexAttribs |= (mesh.colors.Length == vertexCount ? (1 << (int)EMeshVertexAttributes.Attrib_Color) : 0);
			vertexAttribs |= (mesh.uv.Length == vertexCount ? (1 << (int)EMeshVertexAttributes.Attrib_Texcoord) : 0);
			vertexAttribs |= (mesh.boneWeights.Length == vertexCount ? (1 << (int)EMeshVertexAttributes.Attrib_BonesIndicesAndWeights) : 0);

			vertexAttribs &= vertexAttribMask;

			if (newSubmesh)
				PKFxManagerImpl.NextSubmeshToBake(submeshName, vertexAttribs);

			IntPtr toFill = PKFxManagerImpl.GetMeshDataToFillFromAttribs(vertexCount, indexCount, vertexAttribs);
			if (!PKFxManagerImpl.FillUnityMeshData(toFill, mesh))
				return false;
			return PKFxManagerImpl.AppendMeshToBake(toFill, localToWorldMatrix);
		}

		public static bool BakeMeshList(string outputPath)
		{
			return PKFxManagerImpl.BakeMeshes(outputPath);
		}
#endif

		public static void SetupPopcornFxSettings(bool isUnitTesting)
		{
			SPopcornFxSettings settings = new SPopcornFxSettings();

			settings.m_EnableFileLog = PKFxSettings.EnableFileLogs;
			settings.m_EnableRaycastForCollisions = PKFxSettings.EnableRaycastForCollisions;
			settings.m_DisableDynamicEffectBounds = PKFxSettings.DisableDynamicEffectBounds;
			settings.m_EnableGPUBillboarding = PKFxSettings.UseGPUBillboarding;
			settings.m_IsUnitTesting = isUnitTesting;
			settings.m_UseApplicationAudioLoopback = PKFxSettings.UseApplicationAudioLoopback;

			settings.m_EnableLocalizedPages = PKFxSettings.EnableLocalizedPages;
			settings.m_EnableLocalizedByDefault = PKFxSettings.EnableLocalizedByDefault;
			settings.m_LightRendererEnabled = PKFxSettings.EnablePopcornFXLight;

			settings.m_SoundRendererEnabled = PKFxSettings.EnablePopcornFXSound;

			settings.m_FreeUnusedBatches = PKFxSettings.FreeUnusedBatches;
			settings.m_FrameCountBeforeFreeingUnusedBatches = PKFxSettings.FrameCountBeforeFreeingUnusedBatches;

			if (PKFxSettings.EnablePopcornFXLogs)
				SetMaxLogsPerFrame(0x100);
			else
				SetMaxLogsPerFrame(0);
			if (!Application.isPlaying)
			{
				settings.m_SingleThreadedExecution = false;
				settings.m_OverrideThreadPool = false;
				settings.m_WorkerCount = 0;
				settings.m_WorkerAffinities = IntPtr.Zero;
			}
			else
			{
				settings.m_SingleThreadedExecution = PKFxSettings.SingleThreadedExecution;
				if (!PKFxSettings.SingleThreadedExecution && PKFxSettings.OverrideThreadPoolConfig)
				{
					// Create pool with correct affinities:
					int[] affinities = new int[PKFxSettings.ThreadsAffinity.Count];
					IntPtr workersAffinities = Marshal.AllocHGlobal(PKFxSettings.ThreadsAffinity.Count * sizeof(int));

					PKFxSettings.ThreadsAffinity.CopyTo(affinities);
					Marshal.Copy(affinities, 0, workersAffinities, PKFxSettings.ThreadsAffinity.Count);

					settings.m_OverrideThreadPool = true;
					settings.m_WorkerCount = PKFxSettings.ThreadsAffinity.Count;
					settings.m_WorkerAffinities = workersAffinities;
				}
				else
				{
					settings.m_OverrideThreadPool = false;
					settings.m_WorkerCount = 0;
					settings.m_WorkerAffinities = IntPtr.Zero;
				}
			}

			settings.m_RaycastHitSize = UnsafeUtility.SizeOf<RaycastHit>();
			settings.m_RaycastCommandSize = UnsafeUtility.SizeOf<RaycastCommand>();

			settings.m_CPPMarkerMaxDepth = PKFxSettings.CPPMarkersMaxDepth;

			PKFxManagerImpl.PopcornFXStartup(ref settings);
			if (settings.m_WorkerAffinities != IntPtr.Zero)
			{
				Marshal.FreeHGlobal(settings.m_WorkerAffinities);
			}
		}

		public static void SetMaxCameraCount(int count)
		{
			MaxCameraSupport = count;
			PKFxManagerImpl.SetMaxCameraCount(count);
		}

		//----------------------------------------------------------------------------
		// Editor Only
		//----------------------------------------------------------------------------
		#region editor
#if UNITY_EDITOR

		public static void StartupPopcornFileWatcher(bool enableEffectHotreload)
		{
			PKFxManagerImpl.StartupPopcornFileWatcher(enableEffectHotreload);
		}

		public static void LockPackWatcherChanges()
		{
			PKFxManagerImpl.LockPackWatcherChanges();
		}

		public static void UnlockPackWatcherChanges()
		{
			PKFxManagerImpl.UnlockPackWatcherChanges();
		}

		public static bool PullPackWatcherChanges(out int remainingChanges)
		{
			return PKFxManagerImpl.UnstackPackWatcherChanges(out remainingChanges);
		}

		public static void CancelPackWatcherChanges()
		{
			PKFxManagerImpl.CancelPackWatcherChanges();
		}

		public static void RestartPackWatcher()
		{
			PKFxManagerImpl.RestartPackWatcher();
		}

		public static void PausePackWatcher()
		{
			PKFxManagerImpl.PausePackWatcher();
		}

		public static bool BrowseEffectContent(IntPtr pkfxContentPtr, int contentByteSize, string path)
		{
			return PKFxManagerImpl.BrowseEffectContent(pkfxContentPtr, contentByteSize, path);
		}

		public static void GetAllAssetPath()
		{
			PKFxManagerImpl.GetAllAssetPath();
		}

		public static void ReimportAssets(List<string> assets, string platform)
		{
			PKFxManagerImpl.ReimportAssets(assets.Count, assets.ToArray(), platform, QualitySettings.names.Length);
		}

		public static void SetForceDetermismOnBake(bool enable)
		{
			PKFxManagerImpl.SetForceDetermismOnBake(enable);
		}

		public static bool ExtractPkkg(string path, bool runUpgrades)
		{
			if (!PKFxManagerImpl.ExtractPkkg(path, runUpgrades))
			{
				Debug.Log("[PopcornFX] PKFxManagerImpl.ExtractPkkg failed");
				return false;
			}

			if (!string.IsNullOrEmpty(PKFxManager.ExtractedPkkg)) // Pkkg extraction/upgrade success
			{
				return true;
			}
			return false;
		}

		public static bool DeleteExtractedPkkg(string path)
		{
			return PKFxManagerImpl.DeleteExtractedPkkg(path);
		}

		public static List<string> PullEffectChangeIFN()
		{
			LockPackWatcherChanges();

			int totalChanges = 0;
			int remainingChanges = 0;
			bool unstackChangesSuccess = true;
			List<string> updatedAssets = null;

			try // Need try catch to avoid dead lock!
			{
				unstackChangesSuccess = PKFxManager.PullPackWatcherChanges(out totalChanges);

				if (totalChanges > 0)
					updatedAssets = new List<string>(totalChanges);
				remainingChanges = totalChanges;
				while (remainingChanges > 0 && unstackChangesSuccess)
				{
					unstackChangesSuccess = PKFxManager.PullPackWatcherChanges(out remainingChanges);

					if (!updatedAssets.Contains(PKFxManager.GetImportedAssetPath()))
						updatedAssets.Add(PKFxManager.GetImportedAssetPath());
					if (EditorUtility.DisplayCancelableProgressBar("Baking and importing PopcornFX effects",
																	"Importing \'" + PKFxManager.GetImportedAssetName() + "\' and its dependencies.",
																	(float)(totalChanges - remainingChanges) / (float)totalChanges))
					{
						remainingChanges = 0;
						PKFxManager.CancelPackWatcherChanges();
					}
				}

				EditorUtility.ClearProgressBar();

				if (unstackChangesSuccess == false)
					Debug.LogWarning("[PopcornFX] PackWatcher Unstack issue");

			}
			catch (Exception e)
			{
				Debug.LogError("Error in effect importer: " + e.Message);
			}

			PKFxManager.UnlockPackWatcherChanges();

			return updatedAssets;
		}

		public static void SetTargetPlatformForAssets(string targetPlatform)
		{
			if (!PKFxSettings.EnableAssetPlatformVersion)
				return;

			PKFxSettings.GetProjetAssetPath();
			PKFxSettings.ReimportAssets(PKFxSettings.AssetPathList, targetPlatform);
			PullEffectChangeIFN();
		}

#endif
#endregion editor

		//----------------------------------------------------------------------------
		// Frame Update
		//----------------------------------------------------------------------------
		#region FrameUpdateApi

		public static void UpdateCamera(int GUID, ref SCamDesc desc)
		{
			PKFxManagerImpl.UpdateCamDesc(GUID, ref desc, false);
		}

		public static void UpdateParticles()
		{
			// Sync previous frame IFN:
			PKFxManagerImpl.SyncParticlesSimulation();

			// Update the FX transforms and attributes:
			foreach (PKFxEmitter fx in PKFxEmitter.g_PlayingEffectsToUpdate)
			{
				fx.UpdateEffectTransforms();
				fx.UpdateAttributes();
				fx.UpdateSamplers(false);
			}
			float frameDt = Time.smoothDeltaTime * TimeMultiplier;
			if (UseFixedDT)
				frameDt = Time.fixedDeltaTime * TimeMultiplier;

			// Update the particles:
			PKFxManagerImpl.UpdateParticles(frameDt);
		}

		public static void TransformAllParticles(Vector3 worldOffset)
		{
			PKFxManagerImpl.TransformAllParticles(worldOffset);
		}

		public static void Render(short cameraID)
		{
			PKFxManagerImpl._Render(cameraID);
		}
		#endregion FrameUpdateApi

		//----------------------------------------------------------------------------
		// Effect Related API With Native Interop
		//----------------------------------------------------------------------------
		#region effectsApi
		public static int InstantiateEffect(ref SFxDesc effectDescription)
		{
			if (effectDescription.m_FxPath == null || effectDescription.m_FxPath.Length == 0)
				return -1;
			return PKFxManagerImpl.InstantiateFx(ref effectDescription);
		}

		public static int StartEffect(int effectGUID, float dt = 0.0f, float prewarm = 0.0f)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.StartFx(effectGUID, dt, prewarm);
			return -1;
		}

		public static bool StopEffect(int effectGUID, float dt = 0.0f)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.StopFx(effectGUID);
			return false;
		}

		public static bool TerminateEffect(int effectGUID, float dt = 0.0f)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.TerminateFx(effectGUID);
			return false;
		}

		public static bool KillEffect(int effectGUID, float dt = 0.0f)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.KillFx(effectGUID);
			return false;
		}

		public static bool TeleportEffect(int effectGUID)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.TeleportFx(effectGUID);
			return false;
		}

		public static bool SetFxVisibility(int effectGUID, bool enabled)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.SetFxVisibility(effectGUID, enabled);
			return false;
		}

		public static bool SetFxTimescale(int effectGUID, float timescale)
		{
			if (effectGUID >= 0)
				return PKFxManagerImpl.SetFxTimescale(effectGUID, timescale);
			return false;
		}

		public static bool PreloadEffectDependencies(PKFxEffectAsset fxAsset)
		{
			return PKFxManagerImpl._PreloadFxDependencies(fxAsset);
		}

		public static void PreloadEffectIFN(string path, bool requiresGameThreadCollect)
		{
			PKFxManagerImpl.PreloadFxIFN(path, requiresGameThreadCollect ? 1 : 0);
		}

		public static bool SetEffectTransform(int effectGUID, Transform transform)
		{
			Matrix4x4 m = transform.localToWorldMatrix;
			return PKFxManagerImpl.EffectSetTransforms(effectGUID, m);
		}

		public static IntPtr GetTextureSamplerToFill(int byteSize)
		{
			return PKFxManagerImpl.GetTextureSamplerData(byteSize);
		}

		public static void UnloadEffect(string path)
		{
			PKFxManagerImpl.UnloadFxInDependencies(path);
			PKFxManagerImpl.UnloadFx(path);
		}
		#endregion effectsApi

		//----------------------------------------------------------------------------
		//Audio Callback
		//----------------------------------------------------------------------------

		public delegate IntPtr AudioCallback(IntPtr channelName, IntPtr nbSamples);

		public static void RegisterAudioSpectrumData(AudioCallback callback)
		{
			PKFxDelegateHandler delegateHandler = PKFxDelegateHandler.Instance;

			PKFxManagerImpl.SetDelegateOnAudioSpectrumData(delegateHandler.DelegateToFunctionPointer(callback));
		}
		public static void RegisterAudioWaveformData(AudioCallback callback)
		{
			PKFxDelegateHandler delegateHandler = PKFxDelegateHandler.Instance;

			PKFxManagerImpl.SetDelegateOnAudioWaveformData(delegateHandler.DelegateToFunctionPointer(callback));
		}
		public static void SetApplicationLoopbackAudioVolume(float volume)
		{
			PKFxManagerImpl.SetApplicationLoopbackAudioVolume(volume);
		}

		//----------------------------------------------------------------------------
		// Effect Event Callback
		//----------------------------------------------------------------------------
		#region Event Callback

		public static bool EffectRegisterEvent(int effectGUID, string eventName, int unityKey)
		{
			return PKFxManagerImpl.RegisterExportedEvent(effectGUID, eventName, unityKey);
		}

		public static bool EffectUnregisterEvent(int effectGUID, string eventName, int unityKey)
		{
			return PKFxManagerImpl.UnregisterExportedEvent(effectGUID, eventName, unityKey);
		}

		public static void EffectUnregisterAllEvents(int effectGUID)
		{
			PKFxManagerImpl.UnregisterEffectAllExportedEvent(effectGUID);
		}

		public static void UnregisterSceneAllEvents()
		{
			PKFxManagerImpl.UnregisterSceneAllExportedEvent();
		}
		#endregion

		//----------------------------------------------------------------------------
		// Attributes Buffers
		//----------------------------------------------------------------------------
		#region Attributes Buffers
		public static IntPtr GetAttributesBuffer(int effectGUID)
		{
			return PKFxManagerImpl.EffectGetAttributesBuffer(effectGUID);
		}
		#endregion Attributes Buffers


		//----------------------------------------------------------------------------
		// Effect Samplers
		//----------------------------------------------------------------------------
		#region Samplers

		public static bool SetDefaultSampler(int effectGUID, int samplerId)
		{
			return PKFxManagerImpl.EffectResetDefaultSampler(effectGUID, samplerId);
		}

		public static bool SetMeshSampler(int effectGUID, int samplerId, IntPtr meshSampler, Vector3 size, bool async = false)
		{
			return PKFxManagerImpl.EffectSetMeshSampler(effectGUID, samplerId, meshSampler, size, async);
		}

		public static IntPtr GetMeshSamplerToFill(int effectGUID, int samplerId, int bonesCount, int samplingInfo)
		{
			return PKFxManagerImpl.GetMeshDataToFillFromSampler(effectGUID, samplerId, bonesCount, samplingInfo);
		}

		public static bool BeginUpdateSamplerSkinning(int effectGUID, int samplerId, float dt)
		{
			return PKFxManagerImpl.EffectBeginUpdateSamplerSkinning(effectGUID, samplerId, dt);
		}

		public static bool EndUpdateSamplerSkinning(int effectGUID, int samplerId)
		{
			return PKFxManagerImpl.EffectEndUpdateSamplerSkinning(effectGUID, samplerId);
		}

		public static IntPtr UpdateSamplerSkinningSetMatrices(int effectGUID, int samplerId)
		{
			return PKFxManagerImpl.EffectUpdateSamplerSkinningSetMatrices(effectGUID, samplerId);
		}

		public static bool SetSamplerShape(int effectGUID, int samplerId, Sampler.EShapeType shapeType, Vector3 size)
		{
			return PKFxManagerImpl.EffectSetSamplerShape(effectGUID, samplerId, shapeType, size);
		}

		public static bool SetSamplerShapeTransform(int effectGUID, int samplerId, Matrix4x4 transform)
		{
			return PKFxManagerImpl.EffectSetSamplerShapeTransform(effectGUID, samplerId, transform);
		}
		public static bool SetCurveSampler(int effectGUID, int samplerId, IntPtr curveSampler)
		{
			return PKFxManagerImpl.EffectSetCurveSampler(effectGUID, samplerId, curveSampler);
		}

		public static IntPtr GetCurveSamplerToFill(int keyPointsCount, int curveDimension)
		{
			return PKFxManagerImpl.GetCurveSamplerData(keyPointsCount, curveDimension);
		}

		public static bool SetTextureSampler(int effectGUID, int samplerId, IntPtr textureSampler)
		{
			return PKFxManagerImpl.EffectSetTextureSampler(effectGUID, samplerId, textureSampler);
		}

		public static bool SetTextSampler(int effectGUID, int samplerId, string text)
		{
			return PKFxManagerImpl.EffectSetTextSampler(effectGUID, samplerId, text);
		}
		#endregion Samplers

		#region Get/Set
		public static PKFxEffectAsset GetBuiltAsset()
		{
			return PKFxManagerImpl.m_CurrentlyBuildAsset;
		}

		public static void SetBuiltAsset(PKFxEffectAsset value)
		{
			PKFxManagerImpl.m_CurrentlyBuildAsset = value;
		}

		public static PKFxEffectAsset GetImportedAsset()
		{
			return PKFxManagerImpl.m_CurrentlyImportedAsset;
		}

		public static void SetImportedAsset(PKFxEffectAsset value)
		{
			PKFxManagerImpl.m_CurrentlyImportedAsset = value;
		}

		public static string GetImportedAssetName()
		{
			return ImportedAssetName;
		}

		public static string GetImportedAssetPath()
		{
			return ImportedAssetPath;
		}
		#endregion Get/Set

		//----------------------------------------------------------------------------
		// Profiler Management
		//----------------------------------------------------------------------------

		public static void ProfilerEnable(bool onOff)
		{
			PKFxManagerImpl.ProfilerSetEnable(onOff);
		}

		public static void ProfilerWriteReport(string path)
		{
			PKFxManagerImpl.WriteProfileReport(path);
		}

		public static bool InteractiveProfilerPullData(string reportName, ref SStatsToFill dataToFill)
		{
			return PKFxManagerImpl.StatsPullFrameData(reportName, ref dataToFill);
		}

		public static void InteractiveProfilerEnable(bool onOff)
		{
			PKFxManagerImpl.StatsEnableFrameStats(onOff);
			PKFxManagerImpl.StatsEnableEffectsStats(onOff);
		}

		// Sounds
		//----------------------------------------------------------------------------

		internal static void AddSound(PKFxEffectAsset.DependencyDesc depDesc)
		{
			m_SoundDependencies[depDesc.m_Path] = depDesc.m_Object as AudioClip;
		}

		public static AudioClip GetSoundFromPath(string path)
		{
			if (m_SoundDependencies.ContainsKey(path))
				return m_SoundDependencies[path];
			return null;
		}

		// Misc
		//----------------------------------------------------------------------------

		internal static void SetAttributesDirty(int fxGuid)
		{
			PKFxManagerImpl.EffectUpdateAttributes(fxGuid);
		}

		public static void DrawMeshRenderers()
		{
			foreach (var renderer in PKFxManagerImpl.m_Renderers)
			{
				if (renderer.m_InstancesRenderer != null)
					renderer.m_InstancesRenderer.DrawMeshes();
			}
		}

		public static void UpdateQualityLevels()
		{
			string[] qualitiesLevelDescriptions = QualitySettings.names;
			for (int i = 0; i < qualitiesLevelDescriptions.Length; ++i)
				qualitiesLevelDescriptions[i] = qualitiesLevelDescriptions[i].Replace(" ", ""); //No space in quality level from PopcornFX
			QualitiesLevelDescription = qualitiesLevelDescriptions;

			StoredQualityLevel = QualitySettings.names[QualitySettings.GetQualityLevel()].Replace(" ", ""); //No space in quality level from PopcornFX

			PKFxManagerImpl.SetCurrentQualityLevel(StoredQualityLevel);

			List<PKFxEmitter>				killedEffects = new List<PKFxEmitter>();
			Dictionary<int, PKFxEmitter>	fxs = new Dictionary<int, PKFxEmitter>(PKFxEmitter.g_ListEffects);
			// Start by clearing the FXs in the C# so they don't get updated
			foreach (KeyValuePair<int, PKFxEmitter> effect in fxs)
			{
				if (effect.Value.Alive)
				{
					killedEffects.Add(effect.Value);
					effect.Value.KillEffect();
				}
			}

			ResetAndUnloadAllEffects();
			ClearRenderers();
			SetMaxCameraCount(MaxCameraSupport);

			foreach (PKFxEmitter effect in killedEffects)
			{
				effect.StartEffect();
			}
		}

		public static void SetQualityLevelSettings()
		{
			int			currentQualityLevel = QualitySettings.GetQualityLevel();
			string[]	qualitiesLevelDescriptions = QualitySettings.names;

			for (int i = 0; i < qualitiesLevelDescriptions.Length; ++i)
				qualitiesLevelDescriptions[i] = qualitiesLevelDescriptions[i].Replace(" ", ""); //No space in quality level from PopcornFX

			QualitiesLevelDescription = qualitiesLevelDescriptions;
			StoredQualityLevel = qualitiesLevelDescriptions[currentQualityLevel];

			//Store into Settings:
			PKFxSettings.Instance.StoreQualityLevelSettings(qualitiesLevelDescriptions, currentQualityLevel);
			//Broadcast it to Native:
#if UNITY_EDITOR
			PKFxManagerImpl.SetQualityLevelSettings(qualitiesLevelDescriptions, qualitiesLevelDescriptions.Length, currentQualityLevel, true);
#else
			PKFxManagerImpl.SetQualityLevelSettings(qualitiesLevelDescriptions, qualitiesLevelDescriptions.Length, currentQualityLevel, false);
#endif

		}

		public static bool CanSkipUpdate()
		{
			return PKFxManagerImpl.CanSkipUpdate();
		}

	}
}
