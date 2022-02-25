CHANGELOG

    v2.8.4
        - Feature: Added PKFxRenderFeaturesBinding to factory. Allows user to configure and customize the render of particle on batch levels
    v2.8.3
        - Fixed #3552: Cannot override attribute sampler texture density
        - Fixed #6495: Add Information to failed gamma space change when importing dds for distortion
        - Fixed #4307: Ramp texture attribute returns the wrong colour at u = 1
        - Fixed #4034: Import panel doesn't close on its own
        - Fixed #2694: Legacy rendering: Distortion doesn't render if there's more than one Camera in the scene
    v2.8.2
        - Changed Naming convention for versions. It now follows the official PopcornFX version for more clarity.

        - Splitted the plugin into several package in order to simplify setup and different SRP
        - Added PopcornFX namespace to all PopcornFX files
        - Reorganized script distribution. Script inside `Internal` folder should not be used directly
        - Fixed #6498: Baked pkmm is not outputed to the correct directory
        - Fixed #6496: Implement Prewarm
        - Fixed #6507: Remove Broadcasted Event description from PKFxEmitter
        - Fixed #6503: Add Stopped()
        - Fixed #6502: PKFxEmitter: Add getter to attributes properties
        - Fixed #6501: Add an easier GetAttribute()
        - Fixed #6499: PKFxEmitter should expose an easier way to set it's asset
        - Fixed #6493: error when importing pkmm dependencies: Unsupported PKMM file version

----- Old Changelog -----

    v3.Beta2
        - Heavy optimizations on the dynamic physics
        - Added filter ID for collisions
        - Fix glitch when particles die
        - Added a "trigger and forget" option that do not update the FX transforms
        - SetSampler now does not rebuild the acceleration structures by default, we added two flags "" and "" to build the necessary acceleration structures during the SetSampler
        - The meshes for the particles are now created during the pre-loading and not the first frame
        - Added a single threaded implem of PopcornFX to avoid a visual studio hang when debugging the C#

    v3.Beta1
        - Material factory to create the materials from a renderer description, removed the references to HDRP from everywhere else
        - Transparent mesh renderers are now correctly handled

    v3.Beta0
        - Huge code refactor, pretty much all the integration code has changed
        - The FXs and their dependencies (textures, atlas coordinates, meshes) are now in the resource folder and not anymore in StreamingAssets
        - All the rendering is now done by Unity, PopcornFX just fills Unity meshes
        - You can specify Unity custom shader names for your effects using the UserData in the renderers
        - Due to Unity's limitations, all the vertex and index buffers need to be allocated during preload,
        we added an options in the PKFX settings to change those values


	v2.9p9
		- Fix calls to obsolete and deprecated functions on Unity 2017.2.

	v2.9p8
		- Sync. PopcornFX runtime v1.12.2

	v2.9p7
		- Sync. PopcornFX runtime v1.12.1

	v2.9p6
		- Fixed a crash if StartEffect() and TerminateEffect() were called before Start()
		- Sync. PopcornFX runtime v1.12.0

	v2.9p5
		- Remove support for Unity versions lower than 5.2
		- Add support for Unity 2017
		- Memory optimizations on the Attribute Sampler mesh.
		- Fixed a crash with the DepthRT.
		- Reset IsPlaying in Awake.
		- Fixed attributes updates after an hot-reload.
		- Sync. PopcornFX runtime v1.11.6

	v2.9p4
		- Fixed VR rendering.
	v2.9p3
		- Build scene mesh at runtime on all platforms.
		- Can register multiple camera.
		- Fixed 1 frame delay on registered camera.
		- Do not load all the HBOs at application start.
		- StopEffect() became TerminateEffect().
		- StopEffect() stop the emission but continue to update the transform and attributes until all particles died.
		- Load Attribute Samplers when the FX start.
		- Fixed a crash if an Attribute Sampler Skinned Mesh is not in the first position of the samplers list.
		- Add Support for Unity 5.6
		- Sync. PopcornFX runtime v1.11.3

	v2.9p2
		- Attribute samplers : handle ETC2 texture format.
		- Fixed Sample scene custom shaders.
		- Sync. PopcornFX runtime v1.11.0

	v2.9p1
		- Attribute samplers : Fixed an error when unsupported samplers are above the others.
		- Support the reversed depth buffer in Unity 5.5
		- Sync. PopcornFX runtime v1.10.8

	v2.9
		- Attribute samplers : added getter/setters for easier access via scripts
        - Attribute sampler image : Logs an error when texture read is disabled.
        - Attribute sampler image : Added PVRTC support.
        - Attribute sampler shape : allow to drag an drop a MeshFilter on a
        sampler mesh.
		- Attribute samplers : Fixed broken list with unsupported samplers.
        - Attributes : fixed leak with attributes names when getting descriptors.
        - Attributes : now display their descriptions as tooltips.
        - Scene mesh builder : now creates path if it doesn't already exist.
        - Fixed a #define preventing the plugin to load on OSX when targetting
        iOS.
        - Added DeepReset : like Reset but also unloads FXs and clean the render
        mediums.
        - Now attempts to load the pack from relative path to prevent issues with
        non-ascii paths.
        - Added a "reset attributes to default values" button on PKFxFX.
        - Fixed checks for attribute samplers OnDestroy.
        - Fixed attribute sampler mesh : don't pin non-blittable types (bool).
        - Fixed sampler shape mesh : not feed with the mesh after StopEffect
        StartEffect.
        - Implemented native OSX assert catcher.
        - Sync. PopcornFX runtime v1.10.4

	v2.8
		- Attribute samplers : shapes, textures, curves...
		- Fixed orthographic cameras support (see PopcornFX Preferences...)
		- Fixed glitchy billboarding on Android.
		- Added multi-object editing for FX components.
		- Fixed conf file loading when loading from a read-only location.
		- Fixed moving pkfx files breaking components association.
		- Fixed scene reset when destroying the first camera.
		- Moved from C# assemblies to sources (see updater).
		- Added audio sampling named audiogroup.
		- Stability improvements.
		- Renamed native libraries from HH prefix to PK prefix, reflecting a
		name change that happened even before the plugin was released.
		- Added OpenGL Core support.

    v2.7
        - New rendering pipeline, fully integrated with command buffers (Unity
        5.2 and up). Includes retrieving of the scene's depth without resorting
        to an offline camera (Unity 5.0 and up).
        - Added support for distortion/soft particles in VR.
        - Added support for Android multi-threaded rendering (Unity 5.3+).
        - Depth texture now public.
        - Custom shaders (DX11, DX9, OpenGL/ES).
        - Sync with PopcornFX 1.9.0.
        - Fixed PKFxManager's Debug class that was hiding Unity's in some
        versions of the plugin.
		- Fixed KillEffect's behaviour on trails.
		- Added TransformAllParticles() to apply a global transform on all
		particles (useful for floating origin setups).

    v2.6
        - Added PKFxFX.GetAttr(string) and integer overloads for the
        PKFxManager.Attribute class.
        - Added PKFxFX.IsPlayable().
        - Added possibility to overload the OnAudioSpectrum and OnAudioWaveform
        callbacks.
        - Added possibility to use Application.PersistentDataPath instead of the
        default Application.StreamingAssetsPath (useful for adding effects post-
        install).
        - Added support for Windows XP.
        - Moved rendering to the new command buffer interface (Unity >= 5.2).
        This fixes image effects bugs and lens flare layers bugs.
        - Changed the PopcornFX Settings menu. It's now a window exposing the
        individual effect killing, logging, PackFX location and rendering event
        settings.
        - Destroying an effect now calls StopFX().
        - Added a control button on PKFxFX components to force-reload attributes.
        - Fixed a crash when destroying the PKFxRenderingPlugin component of a
        scene and calling PKFxFX.KillEffect in the same frame.
        - Fixed native VR detection.
        - Fixed PNG loading on iOS (bug or crash depending on the version).

    v2.5
        - Fixed a crash when deleting hot-reloaded effects.
        - Fixed a crash when calling StopEffect in OnDisable.
        - Fixed a crash when getting in and out of a scene with effects using
        spacial layers.
        - Fixed race-condition issues.
        - Fixed a bug where localspace effects and attributes were 1 frame behind
        - Added an Alive() method to the PKFxFX component that returns true as
        long as an effect is spawning particles.
        - Added the possibility to enable the killing of individual effects.
        - Added a main conf file for settings (holds the log in file and effects
        killing settings so far).
        - Added support of PopcornFX's sound layers (see online documentation).
        - Added support for audio waveform sampling.
        - Added support for audio sampling (waveform and spectrum) for iOS.
        - Added PKFxManager.UnloadEffect(string path);
        - Fixed Unity 5.1's native VR double update bug (soft particles and
        distortions still not available) (2.5p1).
        - Fixed empty log file bug (2.5p1).
        - Added global time multiplier for slow-mo/fast forwarding particles
        (2.5p1).
        - Sync with PopcornFX Editor 1.8.2 (fixes issues with mesh sampling)
        (2.5p1).
        - Fixed OpenGL2 mesh renderer texturing issues (2.5p2).

    v2.4
        - Synchronized with PopcornFX Editor v1.8.X (make sure to upgrade your
          packs).
        - New fat library for ios64 support.
        - Fixed a bug where PopcornFX components would interfere with other
          plugins' components.
        - Effects hot-reload in Windows and MacOSX editor mode.
        - Int/Int2/3/4 attributes support.
        - Float attributes now support min/max values.

    v2.3
        - Fixed a bug where fx wouldn't load in specific pack architectures.
        - Added PKFxManager.GetStats() to retrieve stats from the PopcornFX run-
          time.
        - Added a link to the online documentation in the Help menu ("PopcornFX
          Wiki").
        - Added a version identifier in the PKFxRenderingPlugin component's
          inspector view.
        - Added "PopcornFX Settings" in the Edit menu with options to enable or
          disable the runtime log to a file (requires a restart to take effect).
        - Added "Create PopcornFX" in the GameObject menu to create empty
          effects or PopcornFX-enabled cameras.
        - Fixed UI refresh after drag'n'drop of a pkfx file in the FX field of
          a PKFxFX component.
        - Added a warning when in editor if the color space is not set to linear

    v2.2
        - Fixed audio sampling on MacosX
        - Fixed DX11 LOD bias
        - OSX binaries now Universal (x86 and x86_64)
        - Soft animation blending now supported in GL/DX9/DX11
        - Fixed DX11 depth texture update bug (soft particles and distortion)
        - Fixed component.camera api deprecation for Unity 5
        - Drag'n'drop .pkfx files on FX components instead of the unconvenient
          list

    v2.1
        - Proper color space detection (sRGB/gamma correction)
        - Distortions blur pass (Blue channel)
        - Fixed distortions bug
        - Massive renames to comply with naming convention
          /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\
          /!\/!\/!\ THIS WILL BREAK MANY PREEXISTING FX COMPONENT!    /!\/!\/!\
          /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\
        - DX11 bugfixes, proper distortions and soft particles
        - Fixed mobile crash on some sampling functions
        - Masked event ids to prevent interfering with other native plugins
        - Fixed bug where additive meshes were never culled
	    - Fixed lost devices in DX9
	    - Fixed depth/distortion FOV bug
	    - Fixed distortion viewport bug
	    - Windows x32/x64 support.

    v2.0
        - Distortions!
        - Mobile depth-related rendering features (soft particles, distortion).

        - PackFX hierarchy : effects are now accessible at any pack subdirectory
        - Packs must now be baked in the StreamingAssets directory.
          /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\
          /!\/!\/!\ THIS WILL BREAK ANY PREEXISTING FX COMPONENT!     /!\/!\/!\
          /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\   /!\

    v1.08
        - Refactoring of the PKFxManager static class to account for iOS' static
        libraries limitation.

=================================================================================

Known bugs.

    - VelocityCapsuleAligned billboarders produce visual glitches on Android.
