; ModuleID = 'extr_tr_light.c_R_SetupEntityLighting.c'
source_filename = "extr_tr_light.c_R_SetupEntityLighting.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.TYPE_17__ = type { i32, i32*, %struct.TYPE_20__* }
%struct.TYPE_20__ = type { i64 }
%struct.TYPE_19__ = type { i64 }
%struct.TYPE_18__ = type { i32 (float)* }
%struct.TYPE_14__ = type { i32, i32, %struct.TYPE_16__* }
%struct.TYPE_16__ = type { float, i32*, i32 }
%struct.TYPE_15__ = type { i32*, i32*, i32*, %struct.TYPE_13__, i8**, i32, i64 }
%struct.TYPE_13__ = type { i32, i32*, i32*, i32* }

@qtrue = dso_local local_unnamed_addr global i64 0, align 8
@RF_LIGHTING_ORIGIN = dso_local local_unnamed_addr global i32 0, align 4
@RDF_NOWORLDMODEL = dso_local local_unnamed_addr global i32 0, align 4
@tr = dso_local local_unnamed_addr global %struct.TYPE_17__ zeroinitializer, align 8
@DLIGHT_AT_RADIUS = dso_local local_unnamed_addr global float 0.000000e+00, align 4
@DLIGHT_MINIMUM_RADIUS = dso_local local_unnamed_addr global float 0.000000e+00, align 4
@r_debugLight = dso_local local_unnamed_addr global %struct.TYPE_19__* null, align 8
@ri = dso_local local_unnamed_addr global %struct.TYPE_18__ zeroinitializer, align 8

; Function Attrs: nounwind optsize uwtable
define dso_local void @R_SetupEntityLighting(%struct.TYPE_14__* nocapture readonly %refdef, %struct.TYPE_15__* %ent) local_unnamed_addr #0 {
entry:
  %lightingCalculated = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 6
  %0 = load i64, i64* %lightingCalculated, align 8, !tbaa !2
  %tobool.not = icmp eq i64 %0, 0
  br i1 %tobool.not, label %if.end, label %cleanup

if.end:                                           ; preds = %entry
  %1 = load i64, i64* @qtrue, align 8, !tbaa !10
  store i64 %1, i64* %lightingCalculated, align 8, !tbaa !2
  %renderfx = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 3, i32 0
  %2 = load i32, i32* %renderfx, align 8, !tbaa !11
  %3 = load i32, i32* @RF_LIGHTING_ORIGIN, align 4, !tbaa !12
  %and = and i32 %3, %2
  %tobool2.not = icmp eq i32 %and, 0
  %origin = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 3, i32 2
  %lightingOrigin = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 3, i32 1
  %origin.sink = select i1 %tobool2.not, i32** %origin, i32** %lightingOrigin
  %4 = load i32*, i32** %origin.sink, align 8, !tbaa !13
  %call6 = tail call i32 @VectorCopy(i32* %4, i32* undef) #2
  %rdflags = getelementptr inbounds %struct.TYPE_14__, %struct.TYPE_14__* %refdef, i64 0, i32 0
  %5 = load i32, i32* %rdflags, align 8, !tbaa !14
  %6 = load i32, i32* @RDF_NOWORLDMODEL, align 4, !tbaa !12
  %and8 = and i32 %6, %5
  %tobool9.not = icmp eq i32 %and8, 0
  br i1 %tobool9.not, label %land.lhs.true, label %if.else13

land.lhs.true:                                    ; preds = %if.end
  %7 = load %struct.TYPE_20__*, %struct.TYPE_20__** getelementptr inbounds (%struct.TYPE_17__, %struct.TYPE_17__* @tr, i64 0, i32 2), align 8, !tbaa !16
  %lightGridData = getelementptr inbounds %struct.TYPE_20__, %struct.TYPE_20__* %7, i64 0, i32 0
  %8 = load i64, i64* %lightGridData, align 8, !tbaa !18
  %tobool10.not = icmp eq i64 %8, 0
  br i1 %tobool10.not, label %if.else13, label %if.then11

if.then11:                                        ; preds = %land.lhs.true
  %call12 = tail call i32 @R_SetupEntityLightingGrid(%struct.TYPE_15__* nonnull %ent, %struct.TYPE_20__* nonnull %7) #2
  br label %if.end26

if.else13:                                        ; preds = %land.lhs.true, %if.end
  %9 = load i32, i32* getelementptr inbounds (%struct.TYPE_17__, %struct.TYPE_17__* @tr, i64 0, i32 0), align 8, !tbaa !20
  %mul = mul nsw i32 %9, 150
  %ambientLight = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 0
  %10 = load i32*, i32** %ambientLight, align 8, !tbaa !21
  %arrayidx = getelementptr inbounds i32, i32* %10, i64 2
  store i32 %mul, i32* %arrayidx, align 4, !tbaa !12
  %arrayidx15 = getelementptr inbounds i32, i32* %10, i64 1
  store i32 %mul, i32* %arrayidx15, align 4, !tbaa !12
  store i32 %mul, i32* %10, align 4, !tbaa !12
  %11 = load i32, i32* getelementptr inbounds (%struct.TYPE_17__, %struct.TYPE_17__* @tr, i64 0, i32 0), align 8, !tbaa !20
  %mul18 = mul nsw i32 %11, 150
  %directedLight = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 1
  %12 = load i32*, i32** %directedLight, align 8, !tbaa !22
  %arrayidx19 = getelementptr inbounds i32, i32* %12, i64 2
  store i32 %mul18, i32* %arrayidx19, align 4, !tbaa !12
  %arrayidx21 = getelementptr inbounds i32, i32* %12, i64 1
  store i32 %mul18, i32* %arrayidx21, align 4, !tbaa !12
  store i32 %mul18, i32* %12, align 4, !tbaa !12
  %13 = load i32*, i32** getelementptr inbounds (%struct.TYPE_17__, %struct.TYPE_17__* @tr, i64 0, i32 1), align 8, !tbaa !23
  %lightDir24 = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 2
  %14 = load i32*, i32** %lightDir24, align 8, !tbaa !24
  %call25 = tail call i32 @VectorCopy(i32* %13, i32* %14) #2
  br label %if.end26

if.end26:                                         ; preds = %if.else13, %if.then11
  %15 = load i32, i32* getelementptr inbounds (%struct.TYPE_17__, %struct.TYPE_17__* @tr, i64 0, i32 0), align 8, !tbaa !20
  %mul27 = shl nsw i32 %15, 5
  %ambientLight28 = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 0
  %16 = load i32*, i32** %ambientLight28, align 8, !tbaa !21
  %17 = load i32, i32* %16, align 4, !tbaa !12
  %add = add nsw i32 %17, %mul27
  store i32 %add, i32* %16, align 4, !tbaa !12
  %18 = load i32, i32* getelementptr inbounds (%struct.TYPE_17__, %struct.TYPE_17__* @tr, i64 0, i32 0), align 8, !tbaa !20
  %mul30 = shl nsw i32 %18, 5
  %arrayidx32 = getelementptr inbounds i32, i32* %16, i64 1
  %19 = load i32, i32* %arrayidx32, align 4, !tbaa !12
  %add33 = add nsw i32 %19, %mul30
  store i32 %add33, i32* %arrayidx32, align 4, !tbaa !12
  %arrayidx36 = getelementptr inbounds i32, i32* %16, i64 2
  %20 = load i32, i32* %arrayidx36, align 4, !tbaa !12
  %add37 = add nsw i32 %20, %mul30
  store i32 %add37, i32* %arrayidx36, align 4, !tbaa !12
  %directedLight38 = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 1
  %21 = load i32*, i32** %directedLight38, align 8, !tbaa !22
  %call39 = tail call float @VectorLength(i32* %21) #2
  %lightDir40 = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 2
  %22 = load i32*, i32** %lightDir40, align 8, !tbaa !24
  %call41 = tail call i32 @VectorScale(i32* %22, float %call39, i32* undef) #2
  %num_dlights = getelementptr inbounds %struct.TYPE_14__, %struct.TYPE_14__* %refdef, i64 0, i32 1
  %23 = load i32, i32* %num_dlights, align 4, !tbaa !25
  %cmp258 = icmp sgt i32 %23, 0
  br i1 %cmp258, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %if.end26
  %dlights = getelementptr inbounds %struct.TYPE_14__, %struct.TYPE_14__* %refdef, i64 0, i32 2
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.body
  %indvars.iv = phi i64 [ 0, %for.body.lr.ph ], [ %indvars.iv.next, %for.body ]
  %24 = load %struct.TYPE_16__*, %struct.TYPE_16__** %dlights, align 8, !tbaa !26
  %origin43 = getelementptr inbounds %struct.TYPE_16__, %struct.TYPE_16__* %24, i64 %indvars.iv, i32 2
  %25 = load i32, i32* %origin43, align 8, !tbaa !27
  %call44 = tail call i32 @VectorSubtract(i32 %25, i32* undef, i32* undef) #2
  %call45 = tail call float @VectorNormalize(i32* undef) #2
  %26 = load float, float* @DLIGHT_AT_RADIUS, align 4, !tbaa !30
  %radius = getelementptr inbounds %struct.TYPE_16__, %struct.TYPE_16__* %24, i64 %indvars.iv, i32 0
  %27 = load float, float* %radius, align 8, !tbaa !31
  %mul47 = fmul float %27, %27
  %mul48 = fmul float %26, %mul47
  %28 = load float, float* @DLIGHT_MINIMUM_RADIUS, align 4, !tbaa !30
  %cmp49 = fcmp olt float %call45, %28
  %d.0 = select i1 %cmp49, float %28, float %call45
  %mul52 = fmul float %d.0, %d.0
  %div = fdiv float %mul48, %mul52
  %29 = load i32*, i32** %directedLight38, align 8, !tbaa !22
  %color = getelementptr inbounds %struct.TYPE_16__, %struct.TYPE_16__* %24, i64 %indvars.iv, i32 1
  %30 = load i32*, i32** %color, align 8, !tbaa !32
  %call55 = tail call i32 @VectorMA(i32* %29, float %div, i32* %30, i32* %29) #2
  %call56 = tail call i32 @VectorMA(i32* undef, float %div, i32* undef, i32* undef) #2
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %31 = load i32, i32* %num_dlights, align 4, !tbaa !25
  %32 = sext i32 %31 to i64
  %cmp = icmp slt i64 %indvars.iv.next, %32
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body, %if.end26
  %33 = load i32*, i32** %ambientLight28, align 8, !tbaa !21
  %34 = load i32, i32* %33, align 4, !tbaa !12
  %conv = sitofp i32 %34 to float
  %arrayidx60 = getelementptr inbounds i32, i32* %33, i64 1
  %35 = load i32, i32* %arrayidx60, align 4, !tbaa !12
  %conv61 = sitofp i32 %35 to float
  %arrayidx63 = getelementptr inbounds i32, i32* %33, i64 2
  %36 = load i32, i32* %arrayidx63, align 4, !tbaa !12
  %conv64 = sitofp i32 %36 to float
  %call65 = tail call float @MAX(float %conv, float %conv61) #2
  %call66 = tail call float @MAX(float %call65, float %conv64) #2
  %cmp67 = fcmp ogt float %call66, 2.550000e+02
  br i1 %cmp67, label %if.then69, label %if.end86

if.then69:                                        ; preds = %for.end
  %div70 = fdiv float 2.550000e+02, %call66
  %37 = load i32*, i32** %ambientLight28, align 8, !tbaa !21
  %38 = load i32, i32* %37, align 4, !tbaa !12
  %conv73 = sitofp i32 %38 to float
  %mul74 = fmul float %div70, %conv73
  %conv75 = fptosi float %mul74 to i32
  store i32 %conv75, i32* %37, align 4, !tbaa !12
  %arrayidx77 = getelementptr inbounds i32, i32* %37, i64 1
  %39 = load i32, i32* %arrayidx77, align 4, !tbaa !12
  %conv78 = sitofp i32 %39 to float
  %mul79 = fmul float %div70, %conv78
  %conv80 = fptosi float %mul79 to i32
  store i32 %conv80, i32* %arrayidx77, align 4, !tbaa !12
  %arrayidx82 = getelementptr inbounds i32, i32* %37, i64 2
  %40 = load i32, i32* %arrayidx82, align 4, !tbaa !12
  %conv83 = sitofp i32 %40 to float
  %mul84 = fmul float %div70, %conv83
  %conv85 = fptosi float %mul84 to i32
  store i32 %conv85, i32* %arrayidx82, align 4, !tbaa !12
  br label %if.end86

if.end86:                                         ; preds = %if.then69, %for.end
  %41 = load i32*, i32** %directedLight38, align 8, !tbaa !22
  %42 = load i32, i32* %41, align 4, !tbaa !12
  %conv89 = sitofp i32 %42 to float
  %arrayidx91 = getelementptr inbounds i32, i32* %41, i64 1
  %43 = load i32, i32* %arrayidx91, align 4, !tbaa !12
  %conv92 = sitofp i32 %43 to float
  %arrayidx94 = getelementptr inbounds i32, i32* %41, i64 2
  %44 = load i32, i32* %arrayidx94, align 4, !tbaa !12
  %conv95 = sitofp i32 %44 to float
  %call96 = tail call float @MAX(float %conv89, float %conv92) #2
  %call97 = tail call float @MAX(float %call96, float %conv95) #2
  %cmp98 = fcmp ogt float %call97, 2.550000e+02
  br i1 %cmp98, label %if.then100, label %if.end117

if.then100:                                       ; preds = %if.end86
  %div101 = fdiv float 2.550000e+02, %call97
  %45 = load i32*, i32** %directedLight38, align 8, !tbaa !22
  %46 = load i32, i32* %45, align 4, !tbaa !12
  %conv104 = sitofp i32 %46 to float
  %mul105 = fmul float %div101, %conv104
  %conv106 = fptosi float %mul105 to i32
  store i32 %conv106, i32* %45, align 4, !tbaa !12
  %arrayidx108 = getelementptr inbounds i32, i32* %45, i64 1
  %47 = load i32, i32* %arrayidx108, align 4, !tbaa !12
  %conv109 = sitofp i32 %47 to float
  %mul110 = fmul float %div101, %conv109
  %conv111 = fptosi float %mul110 to i32
  store i32 %conv111, i32* %arrayidx108, align 4, !tbaa !12
  %arrayidx113 = getelementptr inbounds i32, i32* %45, i64 2
  %48 = load i32, i32* %arrayidx113, align 4, !tbaa !12
  %conv114 = sitofp i32 %48 to float
  %mul115 = fmul float %div101, %conv114
  %conv116 = fptosi float %mul115 to i32
  store i32 %conv116, i32* %arrayidx113, align 4, !tbaa !12
  br label %if.end117

if.end117:                                        ; preds = %if.then100, %if.end86
  %49 = load %struct.TYPE_19__*, %struct.TYPE_19__** @r_debugLight, align 8, !tbaa !13
  %integer = getelementptr inbounds %struct.TYPE_19__, %struct.TYPE_19__* %49, i64 0, i32 0
  %50 = load i64, i64* %integer, align 8, !tbaa !33
  %tobool118.not = icmp eq i64 %50, 0
  br i1 %tobool118.not, label %if.end121, label %if.then119

if.then119:                                       ; preds = %if.end117
  %call120 = tail call i32 @LogLight(%struct.TYPE_15__* nonnull %ent) #2
  br label %if.end121

if.end121:                                        ; preds = %if.then119, %if.end117
  %51 = load i32 (float)*, i32 (float)** getelementptr inbounds (%struct.TYPE_18__, %struct.TYPE_18__* @ri, i64 0, i32 0), align 8, !tbaa !35
  %52 = load i32*, i32** %ambientLight28, align 8, !tbaa !21
  %53 = load i32, i32* %52, align 4, !tbaa !12
  %conv124 = sitofp i32 %53 to float
  %call125 = tail call i32 %51(float %conv124) #2
  %ambientLightInt = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 5
  store i32 %call125, i32* %ambientLightInt, align 8, !tbaa !12
  %54 = load i32 (float)*, i32 (float)** getelementptr inbounds (%struct.TYPE_18__, %struct.TYPE_18__* @ri, i64 0, i32 0), align 8, !tbaa !35
  %55 = load i32*, i32** %ambientLight28, align 8, !tbaa !21
  %arrayidx128 = getelementptr inbounds i32, i32* %55, i64 1
  %56 = load i32, i32* %arrayidx128, align 4, !tbaa !12
  %conv129 = sitofp i32 %56 to float
  %call130 = tail call i32 %54(float %conv129) #2
  %arrayidx132 = getelementptr inbounds i32, i32* %ambientLightInt, i64 1
  store i32 %call130, i32* %arrayidx132, align 4, !tbaa !12
  %57 = load i32 (float)*, i32 (float)** getelementptr inbounds (%struct.TYPE_18__, %struct.TYPE_18__* @ri, i64 0, i32 0), align 8, !tbaa !35
  %58 = load i32*, i32** %ambientLight28, align 8, !tbaa !21
  %arrayidx134 = getelementptr inbounds i32, i32* %58, i64 2
  %59 = load i32, i32* %arrayidx134, align 4, !tbaa !12
  %conv135 = sitofp i32 %59 to float
  %call136 = tail call i32 %57(float %conv135) #2
  %arrayidx138 = getelementptr inbounds i32, i32* %ambientLightInt, i64 2
  store i32 %call136, i32* %arrayidx138, align 8, !tbaa !12
  %arrayidx140 = getelementptr inbounds i32, i32* %ambientLightInt, i64 3
  store i32 255, i32* %arrayidx140, align 4, !tbaa !12
  %call141 = tail call float @VectorNormalize(i32* undef) #2
  %axis = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 3, i32 3
  %60 = load i32*, i32** %axis, align 8, !tbaa !37
  %61 = load i32, i32* %60, align 4, !tbaa !12
  %call144 = tail call i8* @DotProduct(i32* undef, i32 %61) #2
  %modelLightDir = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 4
  %62 = load i8**, i8*** %modelLightDir, align 8, !tbaa !38
  store i8* %call144, i8** %62, align 8, !tbaa !13
  %63 = load i32*, i32** %axis, align 8, !tbaa !37
  %arrayidx148 = getelementptr inbounds i32, i32* %63, i64 1
  %64 = load i32, i32* %arrayidx148, align 4, !tbaa !12
  %call149 = tail call i8* @DotProduct(i32* undef, i32 %64) #2
  %65 = load i8**, i8*** %modelLightDir, align 8, !tbaa !38
  %arrayidx151 = getelementptr inbounds i8*, i8** %65, i64 1
  store i8* %call149, i8** %arrayidx151, align 8, !tbaa !13
  %66 = load i32*, i32** %axis, align 8, !tbaa !37
  %arrayidx154 = getelementptr inbounds i32, i32* %66, i64 2
  %67 = load i32, i32* %arrayidx154, align 4, !tbaa !12
  %call155 = tail call i8* @DotProduct(i32* undef, i32 %67) #2
  %68 = load i8**, i8*** %modelLightDir, align 8, !tbaa !38
  %arrayidx157 = getelementptr inbounds i8*, i8** %68, i64 2
  store i8* %call155, i8** %arrayidx157, align 8, !tbaa !13
  %69 = load i32*, i32** %lightDir40, align 8, !tbaa !24
  %call159 = tail call i32 @VectorCopy(i32* undef, i32* %69) #2
  br label %cleanup

cleanup:                                          ; preds = %entry, %if.end121
  ret void
}

; Function Attrs: optsize
declare dso_local i32 @VectorCopy(i32*, i32*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local i32 @R_SetupEntityLightingGrid(%struct.TYPE_15__*, %struct.TYPE_20__*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local float @VectorLength(i32*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local i32 @VectorScale(i32*, float, i32*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local i32 @VectorSubtract(i32, i32*, i32*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local float @VectorNormalize(i32*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local i32 @VectorMA(i32*, float, i32*, i32*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local float @MAX(float, float) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local i32 @LogLight(%struct.TYPE_15__*) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local i8* @DotProduct(i32*, i32) local_unnamed_addr #1

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
!2 = !{!3, !9, i64 72}
!3 = !{!"TYPE_15__", !4, i64 0, !4, i64 8, !4, i64 16, !7, i64 24, !4, i64 56, !8, i64 64, !9, i64 72}
!4 = !{!"any pointer", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!"TYPE_13__", !8, i64 0, !4, i64 8, !4, i64 16, !4, i64 24}
!8 = !{!"int", !5, i64 0}
!9 = !{!"long", !5, i64 0}
!10 = !{!9, !9, i64 0}
!11 = !{!3, !8, i64 24}
!12 = !{!8, !8, i64 0}
!13 = !{!4, !4, i64 0}
!14 = !{!15, !8, i64 0}
!15 = !{!"TYPE_14__", !8, i64 0, !8, i64 4, !4, i64 8}
!16 = !{!17, !4, i64 16}
!17 = !{!"TYPE_17__", !8, i64 0, !4, i64 8, !4, i64 16}
!18 = !{!19, !9, i64 0}
!19 = !{!"TYPE_20__", !9, i64 0}
!20 = !{!17, !8, i64 0}
!21 = !{!3, !4, i64 0}
!22 = !{!3, !4, i64 8}
!23 = !{!17, !4, i64 8}
!24 = !{!3, !4, i64 16}
!25 = !{!15, !8, i64 4}
!26 = !{!15, !4, i64 8}
!27 = !{!28, !8, i64 16}
!28 = !{!"TYPE_16__", !29, i64 0, !4, i64 8, !8, i64 16}
!29 = !{!"float", !5, i64 0}
!30 = !{!29, !29, i64 0}
!31 = !{!28, !29, i64 0}
!32 = !{!28, !4, i64 8}
!33 = !{!34, !9, i64 0}
!34 = !{!"TYPE_19__", !9, i64 0}
!35 = !{!36, !4, i64 0}
!36 = !{!"TYPE_18__", !4, i64 0}
!37 = !{!3, !4, i64 48}
!38 = !{!3, !4, i64 56}
