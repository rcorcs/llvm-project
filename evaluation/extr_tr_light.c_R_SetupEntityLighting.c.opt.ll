; ModuleID = 'extr_tr_light.c_R_SetupEntityLighting.c.ll'
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
  %16 = load i32*, i32** undef, align 8, !tbaa !21
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
  %21 = load i32*, i32** undef, align 8, !tbaa !22
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

for.body:                                         ; preds = %for.body, %for.body.lr.ph
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
  %29 = load i32*, i32** undef, align 8, !tbaa !22
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
  br label %rolled.reg.pre

if.end117:                                        ; preds = %rolled.reg.latch
  %33 = load %struct.TYPE_19__*, %struct.TYPE_19__** @r_debugLight, align 8, !tbaa !13
  %integer = getelementptr inbounds %struct.TYPE_19__, %struct.TYPE_19__* %33, i64 0, i32 0
  %34 = load i64, i64* %integer, align 8, !tbaa !33
  %tobool118.not = icmp eq i64 %34, 0
  br i1 %tobool118.not, label %if.end121, label %if.then119

if.then119:                                       ; preds = %if.end117
  %call120 = tail call i32 @LogLight(%struct.TYPE_15__* nonnull %ent) #2
  br label %if.end121

if.end121:                                        ; preds = %if.then119, %if.end117
  %35 = load i32 (float)*, i32 (float)** getelementptr inbounds (%struct.TYPE_18__, %struct.TYPE_18__* @ri, i64 0, i32 0), align 8, !tbaa !35
  %36 = load i32*, i32** undef, align 8, !tbaa !21
  %37 = load i32, i32* %36, align 4, !tbaa !12
  %conv124 = sitofp i32 %37 to float
  %call125 = tail call i32 %35(float %conv124) #2
  %ambientLightInt = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 5
  store i32 %call125, i32* %ambientLightInt, align 8, !tbaa !12
  %38 = load i32 (float)*, i32 (float)** getelementptr inbounds (%struct.TYPE_18__, %struct.TYPE_18__* @ri, i64 0, i32 0), align 8, !tbaa !35
  %39 = load i32*, i32** undef, align 8, !tbaa !21
  %arrayidx128 = getelementptr inbounds i32, i32* %39, i64 1
  %40 = load i32, i32* %arrayidx128, align 4, !tbaa !12
  %conv129 = sitofp i32 %40 to float
  %call130 = tail call i32 %38(float %conv129) #2
  %arrayidx132 = getelementptr inbounds i32, i32* %ambientLightInt, i64 1
  store i32 %call130, i32* %arrayidx132, align 4, !tbaa !12
  %41 = load i32 (float)*, i32 (float)** getelementptr inbounds (%struct.TYPE_18__, %struct.TYPE_18__* @ri, i64 0, i32 0), align 8, !tbaa !35
  %42 = load i32*, i32** undef, align 8, !tbaa !21
  %arrayidx134 = getelementptr inbounds i32, i32* %42, i64 2
  %43 = load i32, i32* %arrayidx134, align 4, !tbaa !12
  %conv135 = sitofp i32 %43 to float
  %call136 = tail call i32 %41(float %conv135) #2
  %arrayidx138 = getelementptr inbounds i32, i32* %ambientLightInt, i64 2
  store i32 %call136, i32* %arrayidx138, align 8, !tbaa !12
  %arrayidx140 = getelementptr inbounds i32, i32* %ambientLightInt, i64 3
  store i32 255, i32* %arrayidx140, align 4, !tbaa !12
  %call141 = tail call float @VectorNormalize(i32* undef) #2
  %axis = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 3, i32 3
  %44 = load i32*, i32** %axis, align 8, !tbaa !37
  %45 = load i32, i32* %44, align 4, !tbaa !12
  %call144 = tail call i8* @DotProduct(i32* undef, i32 %45) #2
  %modelLightDir = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 4
  %46 = load i8**, i8*** %modelLightDir, align 8, !tbaa !38
  store i8* %call144, i8** %46, align 8, !tbaa !13
  %47 = load i32*, i32** %axis, align 8, !tbaa !37
  %arrayidx148 = getelementptr inbounds i32, i32* %47, i64 1
  %48 = load i32, i32* %arrayidx148, align 4, !tbaa !12
  %call149 = tail call i8* @DotProduct(i32* undef, i32 %48) #2
  %49 = load i8**, i8*** %modelLightDir, align 8, !tbaa !38
  %arrayidx151 = getelementptr inbounds i8*, i8** %49, i64 1
  store i8* %call149, i8** %arrayidx151, align 8, !tbaa !13
  %50 = load i32*, i32** %axis, align 8, !tbaa !37
  %arrayidx154 = getelementptr inbounds i32, i32* %50, i64 2
  %51 = load i32, i32* %arrayidx154, align 4, !tbaa !12
  %call155 = tail call i8* @DotProduct(i32* undef, i32 %51) #2
  %52 = load i8**, i8*** %modelLightDir, align 8, !tbaa !38
  %arrayidx157 = getelementptr inbounds i8*, i8** %52, i64 2
  store i8* %call155, i8** %arrayidx157, align 8, !tbaa !13
  %53 = load i32*, i32** %lightDir40, align 8, !tbaa !24
  %call159 = tail call i32 @VectorCopy(i32* undef, i32* %53) #2
  br label %cleanup

cleanup:                                          ; preds = %if.end121, %entry
  ret void

rolled.reg.pre:                                   ; preds = %for.end
  %54 = getelementptr inbounds %struct.TYPE_15__, %struct.TYPE_15__* %ent, i64 0, i32 0
  br label %rolled.reg.loop

rolled.reg.loop:                                  ; preds = %rolled.reg.pre, %rolled.reg.latch
  %55 = phi i8 [ 0, %rolled.reg.pre ], [ %86, %rolled.reg.latch ]
  br label %rolled.reg.bb

rolled.reg.bb:                                    ; preds = %rolled.reg.loop
  %56 = zext i8 %55 to i32
  %57 = getelementptr i32*, i32** %54, i32 %56
  %58 = load i32*, i32** %57, align 8
  %59 = load i32, i32* %58, align 4
  %60 = sitofp i32 %59 to float
  %61 = getelementptr inbounds i32, i32* %58, i64 1
  %62 = load i32, i32* %61, align 4
  %63 = sitofp i32 %62 to float
  %64 = getelementptr inbounds i32, i32* %58, i64 2
  %65 = load i32, i32* %64, align 4
  %66 = sitofp i32 %65 to float
  %67 = tail call float @MAX(float %60, float %63) #2
  %68 = tail call float @MAX(float %67, float %66) #2
  %69 = fcmp ogt float %68, 2.550000e+02
  br i1 %69, label %rolled.reg.bb1, label %rolled.reg.bb2

rolled.reg.bb1:                                   ; preds = %rolled.reg.bb
  %70 = fdiv float 2.550000e+02, %68
  %71 = load i32*, i32** %57, align 8
  %72 = load i32, i32* %71, align 4
  %73 = sitofp i32 %72 to float
  %74 = fmul float %70, %73
  %75 = fptosi float %74 to i32
  store i32 %75, i32* %71, align 4
  %76 = getelementptr inbounds i32, i32* %71, i64 1
  %77 = load i32, i32* %76, align 4
  %78 = sitofp i32 %77 to float
  %79 = fmul float %70, %78
  %80 = fptosi float %79 to i32
  store i32 %80, i32* %76, align 4
  %81 = getelementptr inbounds i32, i32* %71, i64 2
  %82 = load i32, i32* %81, align 4
  %83 = sitofp i32 %82 to float
  %84 = fmul float %70, %83
  %85 = fptosi float %84 to i32
  store i32 %85, i32* %81, align 4
  br label %rolled.reg.bb2

rolled.reg.bb2:                                   ; preds = %rolled.reg.bb1, %rolled.reg.bb
  br label %rolled.reg.latch

rolled.reg.latch:                                 ; preds = %rolled.reg.bb2
  %86 = add i8 %55, 1
  %87 = icmp ne i8 %86, 4
  br i1 %87, label %rolled.reg.loop, label %if.end117
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
