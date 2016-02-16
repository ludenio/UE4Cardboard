// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ISimpleHMDPlugin.h"
#include "HeadMountedDisplay.h"
#include "IHeadMountedDisplay.h"
#include "SceneViewExtension.h"

struct FDistortionVertex;

/**
 * Simple Head Mounted Display
 */
class FSimpleHMD : public IHeadMountedDisplay, public ISceneViewExtension, public TSharedFromThis<FSimpleHMD, ESPMode::ThreadSafe>
{
public:
	/** IHeadMountedDisplay interface */
	virtual bool IsHMDConnected() override { return true; }
	virtual bool IsHMDEnabled() const override;
	virtual void EnableHMD(bool allow = true) override;
	virtual EHMDDeviceType::Type GetHMDDeviceType() const override;
	virtual bool GetHMDMonitorInfo(MonitorInfo&) override;

	virtual void GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const override;

	virtual bool DoesSupportPositionalTracking() const override;
	virtual bool HasValidTrackingPosition() override;
	virtual void GetPositionalTrackingCameraProperties(FVector& OutOrigin, FQuat& OutOrientation, float& OutHFOV, float& OutVFOV, float& OutCameraDistance, float& OutNearPlane, float& OutFarPlane) const override;
	virtual void RebaseObjectOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation) const override;

	virtual void SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
	virtual float GetInterpupillaryDistance() const override;

	virtual void GetCurrentOrientationAndPosition(FQuat& CurrentOrientation, FVector& CurrentPosition) override;
	virtual TSharedPtr<class ISceneViewExtension, ESPMode::ThreadSafe> GetViewExtension() override;
	virtual void ApplyHmdRotation(APlayerController* PC, FRotator& ViewRotation) override;
	virtual void UpdatePlayerCameraRotation(class APlayerCameraManager* Camera, struct FMinimalViewInfo& POV) override;

	virtual bool IsChromaAbCorrectionEnabled() const override;

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void OnScreenModeChange(EWindowMode::Type WindowMode) override;

	virtual bool IsPositionalTrackingEnabled() const override;
	virtual bool EnablePositionalTracking(bool enable) override;

	virtual bool IsHeadTrackingAllowed() const override;

	virtual bool IsInLowPersistenceMode() const override;
	virtual void EnableLowPersistenceMode(bool Enable = true) override;

	virtual void ResetOrientationAndPosition(float yaw = 0.f) override;
	virtual void ResetOrientation(float Yaw = 0.f) override;
	virtual void ResetPosition() override;

	virtual void SetClippingPlanes(float NCP, float FCP) override;

	virtual void SetBaseRotation(const FRotator& BaseRot) override;
	virtual FRotator GetBaseRotation() const override;

	virtual void SetBaseOrientation(const FQuat& BaseOrient) override;
	virtual FQuat GetBaseOrientation() const override;

	virtual void DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) override;

	/** IStereoRendering interface */
	virtual bool IsStereoEnabled() const override;
	virtual bool EnableStereo(bool stereo = true) override;
	virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;
	virtual void CalculateStereoViewOffset(const EStereoscopicPass StereoPassType, const FRotator& ViewRotation,
		const float MetersToWorld, FVector& ViewLocation) override;
	virtual FMatrix GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType, const float FOV) const override;
	virtual void InitCanvasFromView(FSceneView* InView, UCanvas* Canvas) override;
	virtual void GetEyeRenderParams_RenderThread(const struct FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const override;

	/** ISceneViewExtension interface */
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) {}
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;

public:
	/** Constructor */
	FSimpleHMD();

	/** Destructor */
	virtual ~FSimpleHMD();

	/** @return	True if the HMD was initialized OK */
	bool IsInitialized() const;

    // Added
    void SetDistortedLensType(bool state);
    void XformDistortionMesh(float xs, float ys);

private:

	FQuat					CurHmdOrientation;
	FQuat					LastHmdOrientation;

	FRotator				DeltaControlRotation;    // same as DeltaControlOrientation but as rotator
	FQuat					DeltaControlOrientation; // same as DeltaControlRotation but as quat

	double					LastSensorTime;

    uint32                  LensNumVerts;
    uint32                  LensNumTris;
    FDistortionVertex*      LensVerts;
    uint16*                 LensIndices;
    
    static const uint32 LensNodNumVerts = 8;
    static const uint32 LensNodNumTris = 4;
    static FDistortionVertex LensNodVerts[LensNodNumVerts];
    static uint16 LensNodIndices[LensNodNumTris * 3];    
    
    static const uint32 LensDistortNumVerts = 3200;
    static const uint32 LensDistortNumTris = 18252 / 3;
    static FDistortionVertex LensDistortVerts[LensDistortNumVerts];
    static uint16 LensDistortIndices[LensDistortNumTris * 3];

	void GetCurrentPose(FQuat& CurrentOrientation);
};


DEFINE_LOG_CATEGORY_STATIC(LogHMD, Log, All);



#if 0

class ENGINE_API FSceneView
{
public:
	const FSceneViewFamily* Family;
	/** can be 0 (thumbnail rendering) */
	FSceneViewStateInterface* State;

	/** The uniform buffer for the view's parameters.  This is only initialized in the rendering thread's copies of the FSceneView. */
	TUniformBufferRef<FViewUniformShaderParameters> UniformBuffer;

	/** uniform buffer with the lights for forward lighting/shading */
	TUniformBufferRef<FForwardLightData> ForwardLightData;

	/** The actor which is being viewed from. */
	const AActor* ViewActor;

	/** An interaction which draws the view's interaction elements. */
	FViewElementDrawer* Drawer;

	/* Final position of the view in the final render target (in pixels), potentially scaled by ScreenPercentage */
	FIntRect ViewRect;

	/* Final position of the view in the final render target (in pixels), potentially constrained by an aspect ratio requirement (black bars) */
	const FIntRect UnscaledViewRect;

	/* Raw view size (in pixels), used for screen space calculations */
	const FIntRect UnconstrainedViewRect;

	/** Maximum number of shadow cascades to render with. */
	int32 MaxShadowCascades;

	FViewMatrices ViewMatrices;

	/** Variables used to determine the view matrix */
	FVector		ViewLocation;
	FRotator	ViewRotation;
	FQuat		BaseHmdOrientation;
	FVector		BaseHmdLocation;
	float		WorldToMetersScale;

	// normally the same as ViewMatrices unless "r.Shadow.FreezeCamera" is activated
	FViewMatrices ShadowViewMatrices;

	FMatrix ProjectionMatrixUnadjustedForRHI;

	FLinearColor BackgroundColor;
	FLinearColor OverlayColor;

	/** Color scale multiplier used during post processing */
	FLinearColor ColorScale;

	/** For stereoscopic rendering, whether or not this is a full pass, or a left / right eye pass */
	EStereoscopicPass StereoPass;

	/** Whether this view should render the first instance only of any meshes using instancing. */
	bool bRenderFirstInstanceOnly;

	// Whether to use FOV when computing mesh LOD.
	bool bUseFieldOfViewForLOD;

	/** Current buffer visualization mode */
	FName CurrentBufferVisualizationMode;

	/**
	* These can be used to override material parameters across the scene without recompiling shaders.
	* The last component is how much to include of the material's value for that parameter, so 0 will completely remove the material's value.
	*/
	FVector4 DiffuseOverrideParameter;
	FVector4 SpecularOverrideParameter;
	FVector4 NormalOverrideParameter;
	FVector2D RoughnessOverrideParameter;

	/** The primitives which are hidden for this view. */
	TSet<FPrimitiveComponentId> HiddenPrimitives;

	// Derived members.

	/** redundant, ViewMatrices.GetViewProjMatrix() */
	/* UE4 projection matrix projects such that clip space Z=1 is the near plane, and Z=0 is the infinite far plane. */
	FMatrix ViewProjectionMatrix;
	/** redundant, ViewMatrices.GetInvViewMatrix() */
	FMatrix InvViewMatrix;				
	/** redundant, ViewMatrices.GetInvViewProjMatrix() */
	FMatrix InvViewProjectionMatrix;	

	float TemporalJitterPixelsX;
	float TemporalJitterPixelsY;

	FConvexVolume ViewFrustum;

	bool bHasNearClippingPlane;

	FPlane NearClippingPlane;

	float NearClippingDistance;

	/** true if ViewMatrix.Determinant() is negative. */
	bool bReverseCulling;

	/* Vector used by shaders to convert depth buffer samples into z coordinates in world space */
	FVector4 InvDeviceZToWorldZTransform;

	/** FOV based multiplier for cull distance on objects */
	float LODDistanceFactor;
	/** Square of the FOV based multiplier for cull distance on objects */
	float LODDistanceFactorSquared;

	/** Whether we did a camera cut for this view this frame. */
	bool bCameraCut;
	
	/** Whether world origin was rebased this frame. */
	bool bOriginOffsetThisFrame;

	// -1,-1 if not setup
	FIntPoint CursorPos;

	/** True if this scene was created from a game world. */
	bool bIsGameView;

	/** For sanity checking casts that are assumed to be safe. */
	bool bIsViewInfo;

	/** Whether this view is being used to render a scene capture. */
	bool bIsSceneCapture;

	/** Whether this view is being used to render a reflection capture. */
	bool bIsReflectionCapture;
	
	/** Whether this view was created from a locked viewpoint. */
	bool bIsLocked;

	/** 
	 * Whether to only render static lights and objects.  
	 * This is used when capturing the scene for reflection captures, which aren't updated at runtime. 
	 */
	bool bStaticSceneOnly;

	/** Aspect ratio constrained view rect. In the editor, when attached to a camera actor and the camera black bar showflag is enabled, the normal viewrect 
	  * remains as the full viewport, and the black bars are just simulated by drawing black bars. This member stores the effective constrained area within the
	  * bars.
	 **/
	FIntRect CameraConstrainedViewRect;

	/** Sort axis for when TranslucentSortPolicy is SortAlongAxis */
	FVector TranslucentSortAxis;

	/** Translucent sort mode */
	TEnumAsByte<ETranslucentSortPolicy::Type> TranslucentSortPolicy;
	
#if WITH_EDITOR
	/** The set of (the first 64) groups' visibility info for this view */
	uint64 EditorViewBitflag;

	/** For ortho views, this can control how to determine LOD parenting (ortho has no "distance-to-camera") */
	FVector OverrideLODViewOrigin;

	/** True if we should draw translucent objects when rendering hit proxies */
	bool bAllowTranslucentPrimitivesInHitProxy;

	/** BitArray representing the visibility state of the various sprite categories in the editor for this view */
	TBitArray<> SpriteCategoryVisibility;
	/** Selection color for the editor (used by post processing) */
	FLinearColor SelectionOutlineColor;
	/** Selection color for use in the editor with inactive primitives */
	FLinearColor SubduedSelectionOutlineColor;
	/** True if any components are selected in isolation (independent of actor selection) */
	bool bHasSelectedComponents;
#endif

	/**
	 * The final settings for the current viewer position (blended together from many volumes).
	 * Setup by the main thread, passed to the render thread and never touched again by the main thread.
	 */
	FFinalPostProcessSettings FinalPostProcessSettings;

	/** Parameters for atmospheric fog. */
	FTextureRHIRef AtmosphereTransmittanceTexture;
	FTextureRHIRef AtmosphereIrradianceTexture;
	FTextureRHIRef AtmosphereInscatterTexture;

	/** Feature level for this scene */
	ERHIFeatureLevel::Type FeatureLevel;

	/** Initialization constructor. */
	FSceneView(const FSceneViewInitOptions& InitOptions);

	/** used by ScreenPercentage */
	void SetScaledViewRect(FIntRect InScaledViewRect);

	/** Transforms a point from world-space to the view's screen-space. */
	FVector4 WorldToScreen(const FVector& WorldPoint) const;

	/** Transforms a point from the view's screen-space to world-space. */
	FVector ScreenToWorld(const FVector4& ScreenPoint) const;

	/** Transforms a point from the view's screen-space into pixel coordinates relative to the view's X,Y. */
	bool ScreenToPixel(const FVector4& ScreenPoint,FVector2D& OutPixelLocation) const;

	/** Transforms a point from pixel coordinates relative to the view's X,Y (left, top) into the view's screen-space. */
	FVector4 PixelToScreen(float X,float Y,float Z) const;

	/** Transforms a point from the view's world-space into pixel coordinates relative to the view's X,Y (left, top). */
	bool WorldToPixel(const FVector& WorldPoint,FVector2D& OutPixelLocation) const;

	/** Transforms a point from pixel coordinates relative to the view's X,Y (left, top) into the view's world-space. */
	FVector4 PixelToWorld(float X,float Y,float Z) const;

	/** 
	 * Transforms a point from the view's world-space into the view's screen-space. 
	 * Divides the resulting X, Y, Z by W before returning. 
	 */
	FPlane Project(const FVector& WorldPoint) const;

	/** 
	 * Transforms a point from the view's screen-space into world coordinates
	 * multiplies X, Y, Z by W before transforming. 
	 */
	FVector Deproject(const FPlane& ScreenPoint) const;

	/** 
	 * Transforms 2D screen coordinates into a 3D world-space origin and direction 
	 * @param ScreenPos - screen coordinates in pixels
	 * @param out_WorldOrigin (out) - world-space origin vector
	 * @param out_WorldDirection (out) - world-space direction vector
	 */
	void DeprojectFVector2D(const FVector2D& ScreenPos, FVector& out_WorldOrigin, FVector& out_WorldDirection) const;

	/** 
	 * Transforms 2D screen coordinates into a 3D world-space origin and direction 
	 * @param ScreenPos - screen coordinates in pixels
	 * @param ViewRect - view rectangle
	 * @param InvViewMatrix - inverse view matrix
	 * @param InvProjMatrix - inverse projection matrix
	 * @param out_WorldOrigin (out) - world-space origin vector
	 * @param out_WorldDirection (out) - world-space direction vector
	 */
	static void DeprojectScreenToWorld(const FVector2D& ScreenPos, const FIntRect& ViewRect, const FMatrix& InvViewMatrix, const FMatrix& InvProjMatrix, FVector& out_WorldOrigin, FVector& out_WorldDirection);

	/** Overload to take a single combined view projection matrix. */
	static void DeprojectScreenToWorld(const FVector2D& ScreenPos, const FIntRect& ViewRect, const FMatrix& InvViewProjMatrix, FVector& out_WorldOrigin, FVector& out_WorldDirection);

	/** 
	 * Transforms 3D world-space origin into 2D screen coordinates
	 * @param WorldPosition - the 3d world point to transform
	 * @param ViewRect - view rectangle
	 * @param ViewProjectionMatrix - combined view projection matrix
	 * @param out_ScreenPos (out) - screen coordinates in pixels
	 */
	static bool ProjectWorldToScreen(const FVector& WorldPosition, const FIntRect& ViewRect, const FMatrix& ViewProjectionMatrix, FVector2D& out_ScreenPos);

	inline FVector GetViewRight() const { return ViewMatrices.ViewMatrix.GetColumn(0); }
	inline FVector GetViewUp() const { return ViewMatrices.ViewMatrix.GetColumn(1); }
	inline FVector GetViewDirection() const { return ViewMatrices.ViewMatrix.GetColumn(2); }

	/** @return true:perspective, false:orthographic */
	inline bool IsPerspectiveProjection() const { return ViewMatrices.IsPerspectiveProjection(); }

	/** Returns the location used as the origin for LOD computations
	 * @param Index, 0 or 1, which LOD origin to return
	 * @return LOD origin
	 */
	FVector GetTemporalLODOrigin(int32 Index, bool bUseLaggedLODTransition = true) const;

	/** Get LOD distance factor: Sqrt(GetLODDistanceFactor()*SphereRadius*SphereRadius / ScreenPercentage) = distance to this LOD transition
	 * @return distance factor
	 */
	float GetLODDistanceFactor() const;

	/** Get LOD distance factor for temporal LOD: Sqrt(GetTemporalLODDistanceFactor(?)*SphereRadius*SphereRadius / ScreenPercentage) = distance to this LOD transition
	 * @param Index, 0 or 1, which temporal sample to return
	 * @return distance factor
	 */
	float GetTemporalLODDistanceFactor(int32 Index, bool bUseLaggedLODTransition = true) const;

	/** 
	 * Returns the blend factor between the last two LOD samples
	 */
	float GetTemporalLODTransition() const;

	/** 
	 * returns a unique key for the view state if one exists, otherwise returns zero
	 */
	uint32 GetViewKey() const;

	/** Allow things like HMD displays to update the view matrix at the last minute, to minimize perceived latency */
	void UpdateViewMatrix();

	/** Setup defaults and depending on view position (postprocess volumes) */
	void StartFinalPostprocessSettings(FVector InViewLocation);

	/**
	 * custom layers can be combined with the existing settings
	 * @param Weight usually 0..1 but outside range is clamped
	 */
	void OverridePostProcessSettings(const FPostProcessSettings& Src, float Weight);

	/** applied global restrictions from show flags */
	void EndFinalPostprocessSettings(const FSceneViewInitOptions& ViewInitOptions);

	/** Configure post process settings for the buffer visualization system */
	void ConfigureBufferVisualizationSettings();

	/** Get the feature level for this view (cached from the scene so this is not different per view) **/
	ERHIFeatureLevel::Type GetFeatureLevel() const { return FeatureLevel; }

	/** Get the feature level for this view **/
	EShaderPlatform GetShaderPlatform() const;
};

class FViewInfo : public FSceneView
{
public:

	/** 
	 * The view's state, or NULL if no state exists.
	 * This should be used internally to the renderer module to avoid having to cast View.State to an FSceneViewState*
	 */
	FSceneViewState* ViewState;

	/** A map from primitive ID to a boolean visibility value. */
	FSceneBitArray PrimitiveVisibilityMap;

	/** Bit set when a primitive is known to be unoccluded. */
	FSceneBitArray PrimitiveDefinitelyUnoccludedMap;

	/** A map from primitive ID to a boolean is fading value. */
	FSceneBitArray PotentiallyFadingPrimitiveMap;

	/** Primitive fade uniform buffers, indexed by packed primitive index. */
	TArray<FUniformBufferRHIParamRef,SceneRenderingAllocator> PrimitiveFadeUniformBuffers;

	/** A map from primitive ID to the primitive's view relevance. */
	TArray<FPrimitiveViewRelevance,SceneRenderingAllocator> PrimitiveViewRelevanceMap;

	/** A map from static mesh ID to a boolean visibility value. */
	FSceneBitArray StaticMeshVisibilityMap;

	/** A map from static mesh ID to a boolean occluder value. */
	FSceneBitArray StaticMeshOccluderMap;

	/** A map from static mesh ID to a boolean velocity visibility value. */
	FSceneBitArray StaticMeshVelocityMap;

	/** A map from static mesh ID to a boolean shadow depth visibility value. */
	FSceneBitArray StaticMeshShadowDepthMap;

	/** A map from static mesh ID to a boolean dithered LOD fade out value. */
	FSceneBitArray StaticMeshFadeOutDitheredLODMap;

	/** A map from static mesh ID to a boolean dithered LOD fade in value. */
	FSceneBitArray StaticMeshFadeInDitheredLODMap;

	/** An array of batch element visibility masks, valid only for meshes
	 set visible in either StaticMeshVisibilityMap or StaticMeshShadowDepthMap. */
	TArray<uint64,SceneRenderingAllocator> StaticMeshBatchVisibility;

	/** The dynamic primitives visible in this view. */
	TArray<const FPrimitiveSceneInfo*,SceneRenderingAllocator> VisibleDynamicPrimitives;

	/** The dynamic editor primitives visible in this view. */
	TArray<const FPrimitiveSceneInfo*,SceneRenderingAllocator> VisibleEditorPrimitives;

	/** View dependent global distance field clipmap info. */
	FGlobalDistanceFieldInfo GlobalDistanceFieldInfo;

	/** Set of translucent prims for this view */
	FTranslucentPrimSet TranslucentPrimSet;

	/** Set of distortion prims for this view */
	FDistortionPrimSet DistortionPrimSet;
	
	/** Set of CustomDepth prims for this view */
	FCustomDepthPrimSet CustomDepthSet;

	/** A map from light ID to a boolean visibility value. */
	TArray<FVisibleLightViewInfo,SceneRenderingAllocator> VisibleLightInfos;

	/** The view's batched elements. */
	FBatchedElements BatchedViewElements;

	/** The view's batched elements, above all other elements, for gizmos that should never be occluded. */
	FBatchedElements TopBatchedViewElements;

	/** The view's mesh elements. */
	TIndirectArray<FMeshBatch> ViewMeshElements;

	/** The view's mesh elements for the foreground (editor gizmos and primitives )*/
	TIndirectArray<FMeshBatch> TopViewMeshElements;

	/** The dynamic resources used by the view elements. */
	TArray<FDynamicPrimitiveResource*> DynamicResources;

	/** Gathered in initviews from all the primitives with dynamic view relevance, used in each mesh pass. */
	TArray<FMeshBatchAndRelevance,SceneRenderingAllocator> DynamicMeshElements;

	TArray<FMeshBatchAndRelevance,SceneRenderingAllocator> DynamicEditorMeshElements;

	FSimpleElementCollector SimpleElementCollector;

	FSimpleElementCollector EditorSimpleElementCollector;

	/** Parameters for exponential height fog. */
	FVector4 ExponentialFogParameters;
	FVector ExponentialFogColor;
	float FogMaxOpacity;

	/** Parameters for directional inscattering of exponential height fog. */
	bool bUseDirectionalInscattering;
	float DirectionalInscatteringExponent;
	float DirectionalInscatteringStartDistance;
	FVector InscatteringLightDirection;
	FLinearColor DirectionalInscatteringColor;

	/** Translucency lighting volume properties. */
	FVector TranslucencyLightingVolumeMin[TVC_MAX];
	float TranslucencyVolumeVoxelSize[TVC_MAX];
	FVector TranslucencyLightingVolumeSize[TVC_MAX];

	/** true if the view has at least one mesh with a translucent material. */
	uint32 bHasTranslucentViewMeshElements : 1;
	/** Indicates whether previous frame transforms were reset this frame for any reason. */
	uint32 bPrevTransformsReset : 1;
	/** Whether we should ignore queries from last frame (useful to ignoring occlusions on the first frame after a large camera movement). */
	uint32 bIgnoreExistingQueries : 1;
	/** Whether we should submit new queries this frame. (used to disable occlusion queries completely. */
	uint32 bDisableQuerySubmissions : 1;
	/** Whether we should disable distance-based fade transitions for this frame (usually after a large camera movement.) */
	uint32 bDisableDistanceBasedFadeTransitions : 1;
	/** Whether the view has any materials that use the global distance field. */
	uint32 bUsesGlobalDistanceField : 1;
	/** Bitmask of all shading models used by primitives in this view */
	uint16 ShadingModelMaskInView;

	FViewMatrices PrevViewMatrices;

	/** Last frame's view and projection matrices */
	FMatrix	PrevViewProjMatrix;

	/** Last frame's view rotation and projection matrices */
	FMatrix	PrevViewRotationProjMatrix;

	/** An intermediate number of visible static meshes.  Doesn't account for occlusion until after FinishOcclusionQueries is called. */
	int32 NumVisibleStaticMeshElements;

	/** Precomputed visibility data, the bits are indexed by VisibilityId of a primitive component. */
	const uint8* PrecomputedVisibilityData;

	FOcclusionQueryBatcher IndividualOcclusionQueries;
	FOcclusionQueryBatcher GroupedOcclusionQueries;

	// Hierarchical Z Buffer
	TRefCountPtr<IPooledRenderTarget> HZB;

	// Size of the HZB's mipmap 0
	// NOTE: the mipmap 0 is downsampled version of the depth buffer
	FIntPoint HZBMipmap0Size;

	/** Used by occlusion for percent unoccluded calculations. */
	float OneOverNumPossiblePixels;

	// Mobile gets one light-shaft, this light-shaft.
	FVector4 LightShaftCenter; 
	FLinearColor LightShaftColorMask;
	FLinearColor LightShaftColorApply;
	bool bLightShaftUse;

	FHeightfieldLightingViewInfo HeightfieldLightingViewInfo;

	TShaderMap<FGlobalShaderType>* ShaderMap;

	bool bIsSnapshot;

	/** Custom visibility query for view */
	ICustomVisibilityQuery* CustomVisibilityQuery;

	/** 
	 * Initialization constructor. Passes all parameters to FSceneView constructor
	 */
	FViewInfo(const FSceneViewInitOptions& InitOptions);

	/** 
	* Initialization constructor. 
	* @param InView - copy to init with
	*/
	explicit FViewInfo(const FSceneView* InView);

	/** 
	* Destructor. 
	*/
	~FViewInfo();

	/** Creates the view's uniform buffer given a set of view transforms. */
	TUniformBufferRef<FViewUniformShaderParameters> CreateUniformBuffer(
		FRHICommandList& RHICmdList,
		const TArray<FProjectedShadowInfo*, SceneRenderingAllocator>* DirectionalLightShadowInfo,
		const FMatrix& EffectiveTranslatedViewMatrix, 
		const FMatrix& EffectiveViewToTranslatedWorld, 
		FBox* OutTranslucentCascadeBoundsArray, 
		int32 NumTranslucentCascades) const;

	/** Initializes the RHI resources used by this view. */
	void InitRHIResources(const TArray<FProjectedShadowInfo*, SceneRenderingAllocator>* DirectionalLightShadowInfo);

	/** Determines distance culling and fades if the state changes */
	bool IsDistanceCulled(float DistanceSquared, float MaxDrawDistance, float MinDrawDistance, const FPrimitiveSceneInfo* PrimitiveSceneInfo);

	/** Gets the eye adaptation render target for this view. */
	IPooledRenderTarget* GetEyeAdaptation() const;

	/** Tells if the eyeadaptation texture exists without attempting to allocate it. */
	bool HasValidEyeAdaptation() const;

	/** Informs sceneinfo that eyedaptation has queued commands to compute it at least once */
	void SetValidEyeAdaptation();

	/** Create acceleration data structure and information to do forward lighting with dynamic branching. */
	void CreateLightGrid();

	FORCEINLINE_DEBUGGABLE float GetDitheredLODTransitionValue(const FStaticMesh& Mesh) const
	{
		float DitherValue = 0.0f;
		if (Mesh.bDitheredLODTransition)
		{
			if (StaticMeshFadeOutDitheredLODMap[Mesh.Id])
			{
				DitherValue = GetTemporalLODTransition();
			}
			else if (StaticMeshFadeInDitheredLODMap[Mesh.Id])
			{
				DitherValue = GetTemporalLODTransition() - 1.0f;
			}
		}
		return DitherValue;
	}

	/** Create a snapshot of this view info on the scene allocator. */
	FViewInfo* CreateSnapshot() const;

	/** Destroy all snapshots before we wipe the scene allocator. */
	static void DestroyAllSnapshots();

private:

	FSceneViewState* GetEffectiveViewState() const;

	/** Initialization that is common to the constructors. */
	void Init();

	/** Calculates bounding boxes for the translucency lighting volume cascades. */
	void CalcTranslucencyLightingVolumeBounds(FBox* InOutCascadeBoundsArray, int32 NumCascades) const;

	/** Sets the sky SH irradiance map coefficients. */
	void SetupSkyIrradianceEnvironmentMapConstants(FVector4* OutSkyIrradianceEnvironmentMap) const;

	/** All light sources available for forward shading. Can be indexed in the shader.*/
	void CreateForwardLightDataUniformBuffer(FForwardLightData& Out) const;
};
*/
#endif
